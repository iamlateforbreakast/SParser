/* Debug.h */

#include <stdarg.h>

#define TRACE(x) do { if (DEBUG) dbg_printf x; } while (0)

#define PRINT(x) do { dbg_printf x; } while (0)

void dbg_printf(const char *fmt, ...);
