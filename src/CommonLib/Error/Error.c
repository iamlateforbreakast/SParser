/**********************************************//** 
  \file Error.c
     
  \brief Reports errors
        
  This file contains error reporting functions.
**************************************************/
#include "Error.h"
#include "Debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG (1)

/**********************************************//** 
  \brief Reports errors
  \param severity Enum
  \param msg Variable list of parameters
  \public

  This function reports errors using different
  formatting according to severity.
**************************************************/
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
