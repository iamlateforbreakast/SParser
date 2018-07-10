#include "Error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

PUBLIC void Error_new(ErrorSeverity severity, char * msg, ...)
{
  va_list args;

  switch (severity)
  {
    case ERROR_DBG:
	va_start(args, msg);
	printf("[DBUG] ");
	vprintf(msg, args);
	va_end(args);
	break;
    case ERROR_INFO:
	va_start(args, msg);
	printf("[INFO] ");
	vprintf(msg, args);
	va_end(args);
	break;
    case ERROR_NORMAL:
	va_start(args, msg);
	printf("[NORM] ");
	vprintf(msg, args);
	va_end(args);
	break;
    case ERROR_FATAL:
	va_start(args, msg);
	printf("[EXIT] ");
	vprintf(msg, args);
	va_end(args);
	exit(2);
  }
}
