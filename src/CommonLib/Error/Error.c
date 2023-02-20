#include "Error.h"
#include "Debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG (1)

PUBLIC void Error_new(ErrorSeverity severity, char * msg, ...)
{
  va_list args;

  switch (severity)
  {
    case ERROR_DBG:
	va_start(args, msg);
	TRACE(("[DBUG] "));
	vprintf(msg, args);
	va_end(args);
	break;
    case ERROR_INFO:
	va_start(args, msg);
	PRINT(("[INFO] "));
	vprintf(msg, args);
	va_end(args);
	break;
    case ERROR_NORMAL:
	va_start(args, msg);
	PRINT(("[NORM] "));
	vprintf(msg, args);
	va_end(args);
	break;
    case ERROR_FATAL:
	va_start(args, msg);
	PRINT(("[EXIT] "));
	vprintf(msg, args);
	va_end(args);
	exit(2);
  }
}
