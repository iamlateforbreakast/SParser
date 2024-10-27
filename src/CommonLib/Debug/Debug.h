/* Debug.h */

#include <stdarg.h>
#include <stdio.h>

#define TRACE(x) do { if (DEBUG) dbg_printf x; } while (0)
#define TRACE2(x) do { if (DEBUG) dbg_fprintf x; } while (0)

#define PRINT(x) do { dbg_printf x; } while (0)
#define PRINT2(x) do { dbg_fprintf x; } while (0)

FILE * open_channel(const char * name);
void close_channel(FILE * channel);
void dbg_printf(const char *fmt, ...);
void dbg_fprintf(FILE * channel, const char *fmt, ...);