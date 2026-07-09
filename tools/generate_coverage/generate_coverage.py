import os, re

# Regular expression matching your custom code token
marker_regex = re.compile(r'RECORD_COVERAGE\("([^"]+)"\)')
found_keys = set()

# Scan all C files in your project directory
for root, dirs, files in os.walk("."):
    for file in files:
        if file.endswith((".c", ".h")) and file != "coverage_autogen.h":
            with open(os.path.join(root, file), "r", encoding="utf-8") as f:
                for match in marker_regex.finditer(f.read()):
                    found_keys.add(match.group(1))

# Write out a dynamically calculated header file before compilation compiles
with open("coverage_autogen.h", "w") as out:
    out.write(f"#define MAX_POINTS {len(found_keys)}\n\n")
    out.write("extern const char* g_coverage_names[MAX_POINTS];\n")
