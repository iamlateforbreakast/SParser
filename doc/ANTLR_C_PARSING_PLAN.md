# Plan: Parsing C Source with ANTLR

## 1. Current state

SParser (`sparse`) already parses `.c` files through a hand-rolled pipeline:

- `src/ParseLib/TransUnit` reads the translation unit buffer-by-buffer (handles
  `#include`, macro storage via `MacroStore.h`/`MacroDefinition.h`).
- `src/ParseLib/Grammar2` (flex `Grammar2.l` + bison `Grammar2.y`, generated
  `.lex.c`/`.parse.c` committed to the repo) and `src/ParseLib/GrammarC99`
  (a fuller C99 flex/bison grammar, 502-line `.y` + 204-line `.l`) both build a
  `Grammar` and eventually populate `SdbMgr`/SQLite via `Ast`/`DataAccess`.
- Dispatch is a static table in `SParse.c` (`SParse_default[]`) mapping file
  extension → `{new, process, delete}` function pointers matching the
  `Grammar` vtable in `Grammar.h`.
- Build is autoconf/automake, currently only requiring `flex`/`bison`/a C
  compiler (`AC_PROG_LEX`, `AC_PROG_YACC`, `AC_PROG_CC`).
- The codebase already has JSON/XML reader-writer modules
  (`src/AppliLib/JSONReader`, `JSONWriter`, `XmlReader`, `XmlWriter`), which is
  relevant to one of the integration options below.

**Constraint that shapes everything else:** ANTLR4 has no official pure-C
runtime target (targets are Java, C++, C#, Python3, JavaScript/TypeScript, Go,
Swift, PHP, Dart). Since SParser is a C, not C++, project, ANTLR can't be
dropped in as a same-language replacement for Grammar2/GrammarC99 — it has to
be bridged in.

## 2. Decision to make first: integration strategy

| Option | How it works | Pros | Cons |
|---|---|---|---|
| **A. Embedded C++ runtime + C bridge (recommended)** | Generate a C++ lexer/parser/visitor from a `.g4` grammar with the `antlr4` tool, write a thin `extern "C"` wrapper module (new `GrammarAntlr` dir) that walks the parse tree and calls into existing C APIs (`Ast`, `SdbMgr`), link it into the existing binary. | Single process, no serialization overhead, fits the existing `Grammar` vtable/dispatch table exactly, reuses `TransUnit`/`SdbMgr` as-is. | Introduces a C++ compiler and the `antlr4-runtime` C++ library as new build deps; need `extern "C"` boundary discipline. |
| **B. External ANTLR tool + JSON/XML import** | Run a separate ANTLR-generated parser (Java or C++, as a helper binary) that dumps the parse tree as JSON/XML per translation unit; SParser's existing `JSONReader`/`XmlReader` ingest that into `Ast`. | No C++ needed inside the core binary; reuses existing reader modules; easy to prototype. | Two-process pipeline, serialization cost, harder to stream large files, an extra runtime dependency (Java, or a separate C++ helper) shipped alongside `sparse`. |

**Recommendation:** Option A. It keeps parsing in-process and matches the
existing `Grammar` interface, so it can be added to `SParse_default[]` as a
drop-in alternative to `GrammarC99` behind a build/config flag, and compared
head-to-head. Option B is worth keeping as a fallback if linking C++ into the
autotools build proves too disruptive.

Open question to confirm with the maintainer before Phase 1: **A vs. B**, and
whether the target grammar is C99 (matching `GrammarC99`) or a newer standard
(C11/C17).

## 3. Phases

### Phase 0 — Spike (throwaway, no build integration)
- Pull a `.g4` C grammar (the community-maintained `antlr/grammars-v4` `C`
  grammar is the standard starting point) and run it standalone with the
  `antlr4` Java tool + C++ runtime against a couple of files from
  `tests/multi_includes`.
- Confirm it parses real SParser test fixtures and produces a usable parse
  tree; note any grammar gaps (compiler extensions, `#pragma`, GNU
  attributes) SParser's current fixtures exercise.
- Time-box this; its only output is a go/no-go and a list of grammar gaps.

### Phase 1 — Toolchain & build integration
- Add `AC_PROG_CXX` to `configure.ac`; decide whether the C++ compiler is
  required unconditionally or gated behind `--enable-antlr` (recommended,
  to avoid forcing a C++ toolchain on everyone building today).
- Add detection for `antlr4-runtime` (C++ headers/lib) — via
  `pkg-config` if the distro package provides a `.pc` file, else manual
  `AC_CHECK_HEADER`/`AC_CHECK_LIB`.
- Decide code-generation policy, matching the existing convention of
  committing generated flex/bison output (`GrammarC99.lex.c`,
  `.parse.c` are checked in): either (a) commit the ANTLR-generated
  C++ sources too, so building `sparse` never requires Java/the ANTLR
  jar, only regenerating them does; or (b) generate at build time,
  requiring Java. **Recommend (a)** for consistency with current practice
  and to keep the default build dependency list (README's "Dependencies")
  unchanged for users who don't touch the grammar.
- Add `tools/antlr/regenerate.sh` (or a `make antlr-regen` target) that
  wraps `java -jar antlr-<ver>-complete.jar -Dlanguage=Cpp ...` for
  contributors who do modify the grammar.
- Update `README.md` dependency lists (Debian/Fedora) once the concrete
  package names are confirmed (Fedora: `antlr4-cpp-runtime-devel` or vendor
  it; Debian: `libantlr4-runtime-dev`, version must match the `antlr4` tool
  version used for codegen — this is the most common source of breakage).

### Phase 2 — Grammar acquisition & adaptation
- Vendor the chosen `.g4` grammar under
  `src/ParseLib/GrammarAntlr/grammar/` with its license file.
- Adapt it against SParser's actual input corpus (`tests/multi_includes`,
  `tests/self`), fixing mismatches surfaced in Phase 0.
- Decide the preprocessing boundary: SParser's `TransUnit`/`MacroStore`
  already handle `#include`/macro expansion before the grammar sees text
  (see `GrammarC99.l` for how the flex grammar currently receives input).
  The ANTLR lexer should consume the *same* already-preprocessed buffer
  stream from `TransUnit_getNextBuffer`, not re-implement preprocessing.

### Phase 3 — C bridge module (`GrammarAntlr`)
- New directory `src/ParseLib/GrammarAntlr/` mirroring `GrammarC99`'s
  layout, implementing the `Grammar` contract:
  `GrammarAntlr_new`, `_delete`, `_print`, `_getSize`, `_process`
  (see `GrammarC99.h` for the exact signatures to match).
- Internals are C++ (`.cpp`), but the header exposed to the rest of the
  (C) codebase is `extern "C"`-only, same pattern as any C/C++ interop
  boundary — no C++ types cross it.
- Implement an ANTLR `Visitor` (preferred over `Listener` for direct
  tree-to-AST construction) that walks the parse tree and builds
  `Ast`/`DataAccess` nodes exactly as `GrammarC99`'s bison actions do
  today, so downstream `SdbMgr`/SQLite population is unchanged.
- Wire error reporting through ANTLR's `ErrorListener` into the existing
  `Error.h` (`Error_new(ERROR_..., ...)`) so diagnostics look consistent
  regardless of which grammar backend is active.

### Phase 4 — Dispatch integration
- Add a new row to `SParse_default[]` in `SParse.c` for `GrammarAntlr`,
  initially guarded by a compile-time `#ifdef WITH_ANTLR` (set by the
  `--enable-antlr` configure flag from Phase 1) or an `OptionMgr` runtime
  option, so it can run side-by-side with `Grammar2`/`GrammarC99` rather
  than replacing them immediately.

### Phase 5 — Testing & validation
- Add `tests/antlr_grammar/` mirroring `tests/multi_includes` fixtures.
- Differential testing: run the same `.c` fixtures through
  `GrammarC99` and `GrammarAntlr`, diff the resulting `Ast`/SQLite output
  (there's already `Ast_print`/`tools/dbdump/dbdump.py` to help compare).
  Treat any divergence as a bug in the new path until proven otherwise.
- Unit tests for the visitor logic itself, following the existing
  `tests/` subdirectory-per-module convention (see e.g.
  `src/ParseLib/Ast/tests`).
- Extend CI/`make check` to build with `--enable-antlr` at least once so
  the new path doesn't silently bitrot.

### Phase 6 — Rollout
- Once differential tests pass on the existing corpus, flip the default
  dispatch table entry for `*.c` from `Grammar2`/`GrammarC99` to
  `GrammarAntlr` (still leaving the old grammars in the tree, unused, until
  confidence is high).
- Update `TODO` and `README.md`; decide (separately, later) whether
  `Grammar2`/`GrammarC99`/their flex+bison inputs get removed once ANTLR
  is trusted in production — don't couple that cleanup to this migration.

## 4. Risks / open questions to resolve before starting Phase 1

1. **A vs. B** (embedded C++ bridge vs. external process + JSON/XML import)
   — needs an explicit decision, it changes almost every later step.
2. **C++ in an otherwise pure-C autotools project** — acceptable, or is a
   subprocess-based approach preferred specifically to avoid a C++ build
   dependency?
3. **Target C standard** — C99 parity with `GrammarC99`, or newer?
4. **Generated-sources policy** — commit generated C++ parser (matches
   current flex/bison convention) vs. generate at build time (adds a Java
   dependency for every build).
5. **Fate of `Grammar2`/`GrammarC99`** — kept indefinitely as alternates,
   or scheduled for removal post-migration?

## 5. Suggested milestone order

1. Phase 0 spike → go/no-go + decisions above.
2. Phase 1 build integration (empty `GrammarAntlr` module compiles).
3. Phase 2+3 grammar + bridge, targeting one `tests/multi_includes` file.
4. Phase 5 differential tests green on the full existing corpus.
5. Phase 4+6 dispatch integration and rollout.
