#include "SdbRequest.h"
#include "Object.h"
#include "SdbMgr.h"

#include <stdarg.h>

struct SdbRequest
{
  Object object;
  char * buffer;
  char * fmt;
  char ** result;
};

PRIVATE SdbMgr * sdbMgr = 0;

PUBLIC SdbRequest * SdbRequest_new()
{
  SdbRequest * this = 0;
  
  this = (SdbRequest*)Object_new(sizeof(SdbRequest),(Destructor)&SdbRequest_delete, (Copy_operator)&SdbRequest_copy);
  this->object.size = sizeof(SdbRequest);
  
  return this;
}

PUBLIC void SdbRequest_delete(SdbRequest * this)
{
}

PUBLIC SdbRequest * SdbRequest_copy(SdbRequest * this)
{
  SdbRequest * result = 0;
  
  return result;
}

PUBLIC void SdbRequest_execute(SdbRequest * this, ...)
{
  unsigned int size = 0;
  
  /* get SdbMgr */
  va_list args;
  va_start(args, this->fmt);
  size = vsnprintf(0, 0, this->fmt, args);
  this->buffer = malloc(size+ 1);
  vsnprintf(this->buffer, size, this->fmt, args);
  va_end(args);
  SdbMgr_execute(sdbMgr, this->buffer);
}
