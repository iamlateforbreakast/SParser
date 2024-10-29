/**********************************************//**
  @file Debug.c

  @brief This file contains debugging functions.

  The debugging function are TBD
**************************************************/

#include "Debug.h"

#include <stdio.h>

FILE * Debug_channelStdOut = 0;
FILE * Debug_channelStdErr = 0;
FILE * Debug_channelLog = 0;

FILE * Debug_openChannel(const char * name)
{
#ifndef WIN32
  Debug_channelLog = fopen(name, "wb+");
#else
  fopen_s(&Debug_channelLog, name,"wb+");
#endif
  if (!Debug_channelLog) Debug_channelLog = stdout;

  return Debug_channelLog;
}

void Debug_setStdoutChannel(FILE * channel)
{
  Debug_channelStdOut = channel;
}

void Debug_setStderrChannel(FILE * channel)
{
  Debug_channelStdErr = channel;
}

void Debug_closeChannel(FILE * channel)
{
  fclose(Debug_channelLog);
}

void Debug_dbgPrintf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(Debug_channelStdOut, fmt, args);
    va_end(args);
}

void Debug_dbgFprintf(FILE * channel, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(channel, fmt, args);
    va_end(args);
}
