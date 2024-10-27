/**********************************************//**
  @file Debug.c

  @brief This file contains debugging functions.

  The debugging function are TBD
**************************************************/

#include "Debug.h"

#include <stdio.h>

FILE * open_channel(const char * name)
{
  FILE * channel;

  channel = fopen(name,"wb+");

  if (!channel) channel = stdout;

  return channel;
}

void close_channel(FILE * channel)
{
  fclose(channel);
}

void dbg_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}

void dbg_fprintf(FILE * channel, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(channel, fmt, args);
    va_end(args);
}