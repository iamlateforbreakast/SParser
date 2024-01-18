/* Stub_Malloc_01.c */
#include "Debug.h"
#include <stdio.h>

struct stub_data
{
  void * malloc_result;
  int malloc_nb_calls;
  int free_nb_calls;
};

static struct stub_data data;

void reset_counters()
{
  data.malloc_nb_calls = 0;
  data.free_nb_calls = 0;
}

void dbg_printf(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  va_end(args);
}

void set_data_malloc_result(void * p)
{
  data.malloc_result = p;
}

void * malloc(size_t size)
{
  return data.malloc_result;
}

void free(void * ptr)
{
  data.free_nb_calls++;
}