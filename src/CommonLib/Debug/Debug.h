/* Debug.h */

#include <stdarg.h>
#include <stdio.h>

#define TRACE(x) do { if (DEBUG) Debug_dbgPrintf x; } while (0)
#define TRACE2(x) do { if (DEBUG) Debug_dbgFprintf x; } while (0)

#define PRINT(x) do { Debug_dbgPrintf x; } while (0)
#define PRINT2(x) do { Debug_dbgFprintf x; } while (0)

FILE * Debug_openChannel(const char * name);
void Debug_closeChannel(FILE * channel);
void Debug_setStdoutChannel(FILE * channel);
void Debug_setStderrChannel(FILE * channel);
void Debug_dbgPrintf(const char *fmt, ...);
void Debug_dbgFprintf(FILE * channel, const char *fmt, ...);
