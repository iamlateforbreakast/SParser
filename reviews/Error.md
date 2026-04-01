Implementing a cross-platform stack trace in C89 requires using platform-specific APIs because C89 itself has no built-in mechanism for stack unwinding. [1, 2, 3] 
## 1. The Strategy
You can bridge these platforms by creating a unified Error_printStackTrace() function that uses:

* Linux/POSIX: backtrace() and backtrace_symbols() from <execinfo.h>.
* Windows: CaptureStackBackTrace() and the DbgHelp library (SymFromAddr) to translate addresses into names. [4, 5, 6, 7] 

## 2. Implementation Logic
Add this to your Error.c (or a dedicated StackTrace.c):

```
#include "Error.h"#include <stdio.h>#include <stdlib.h>
#ifdef _WIN32#include <windows.h>#include <dbghelp.h>#pragma comment(lib, "dbghelp.lib")#else#include <execinfo.h>#endif
#define MAX_FRAMES 64
/* Prints stack trace using platform-specific methods */void Error_printStackTrace(void) {
    void* stack[MAX_FRAMES];
    unsigned short i;
#ifdef _WIN32
    // Windows: Use DbgHelp for symbol resolution
    HANDLE process = GetCurrentProcess();
    SYMBOL_INFO* symbol;
    unsigned short frames = CaptureStackBackTrace(0, MAX_FRAMES, stack, NULL);
    SymInitialize(process, NULL, TRUE);
    
    symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    fprintf(stderr, "Windows Stack Trace:\n");
    for (i = 0; i < frames; i++) {
        if (SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol)) {
            fprintf(stderr, "%d: %s - 0x%0llX\n", i, symbol->Name, symbol->Address);
        }
    }
    free(symbol);#else
    // POSIX: Use execinfo.h
    int frames = backtrace(stack, MAX_FRAMES);
    char** symbols = backtrace_symbols(stack, frames);
    if (symbols != NULL) {
        fprintf(stderr, "POSIX Stack Trace:\n");
        for (i = 0; i < (unsigned short)frames; i++) {
            fprintf(stderr, "%s\n", symbols[i]);
        }
        free(symbols);
    }#endif
}
```

## 3. Key Requirements for Symbol Visibility

To resolve function names rather than raw addresses:

* Linux/GCC: Compile with -rdynamic.
* Windows/MSVC: Link DbgHelp.lib and ensure .pdb files are present.
* Frame Pointers: Disable frame pointer omission (-fno-omit-frame-pointer or /Oy-). [3, 8, 9, 10, 11] 

## 4. Integration

Call Error_printStackTrace() within your error handling logic to capture the trace when a fatal error occurs.
