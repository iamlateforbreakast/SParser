/**********************************************//**
  @file Debug.c

  @brief This file contains debugging functions.

  The debugging function are TBD
**************************************************/

#include "Debug.h"

#include <stdio.h>

void dbg_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}