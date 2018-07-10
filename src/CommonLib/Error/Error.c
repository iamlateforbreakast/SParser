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
	printf(args);
	va_end(args);
	break;
    case ERROR_INFO:
	va_start(args, msg);
	printf("[INFO] ");
	printf(args);
	va_end(args);
	break;
    case ERROR_NORMAL:
	va_start(args, msg);
	printf("[NORM] ");
	printf(args);
	va_end(args);
	break;
    case ERROR_FATAL:
	va_start(args, msg);
	printf("[EXIT] ");
	printf(args);
	va_end(args);
	exit(2);
  }
}
