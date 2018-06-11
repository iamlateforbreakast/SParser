#include"SdbRequest.h"
#include "Object.h"
#include <stdarg.h>

struct SdbRequest
{
  Object object;
  char * buffer;
  char * result[];
};

PRIVATE SdbMgr * sdbMgr = 0;

PUBLIC SdbRequest * SdbRequest_new()
{
  SdbRequest * this = 0;
  
  this = (SdbRequest*)Object_new(sizeof(SdbRequest),(Destructor)&SdbRequest_delete, (Copy_operator)&SdbRequest_copy);
  this->object.size = sizeof(SdbRequest);
  
  return this;
}

PUBLIC SdbRequest_delete(SdbRequest * this)
{
}

PUBLIC SdbRequest * SdbRequest_copy()
{
  SdbRequest * result = 0;
  
  return result;
}

PUBLIC SdbRequest_execute(SdbRequest * this, ...)
{
  unsigned int size = 0;
  
  /* get SdbMgr */
  va_list args;
  va_start(args, fmt);
  size = vsnprintf(0, 0, fmt, args);
  this->buffer = malloc(size+ 1);
  vsnprintf(this->buffer, size, fmt, args);
  va_end(args);
  SdbMgr_execute(sdbMgr, this->buffer);
}