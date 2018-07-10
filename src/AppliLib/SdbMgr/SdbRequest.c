#include "SdbRequest.h"
#include "Object.h"
#include "SdbMgr.h"
#include "Memory.h"

#include <stdarg.h>

struct SdbRequest
{
  Object object;
  char * buffer;
  unsigned int size;
  const char * fmt;
  String ** result;
};

PUBLIC SdbRequest * SdbRequest_new(const char * fmt)
{
  SdbRequest * this = 0;

  this = (SdbRequest*)Object_new(sizeof(SdbRequest),(Destructor)&SdbRequest_delete, (Copy_operator)&SdbRequest_copy);
  this->object.size = sizeof(SdbRequest);
  
  this->fmt = fmt;
  this->size = 0;
  this->buffer = 0;
  this->result = 0;
  
  return this;
}

PUBLIC void SdbRequest_delete(SdbRequest * this)
{
  if (this->buffer)
  {
    Memory_free(this->buffer, this->size);
    this->buffer = 0;
    this->size = 0;
  }
  Object_delete(&this->object);
}

PUBLIC SdbRequest * SdbRequest_copy(SdbRequest * this)
{
  SdbRequest * result = 0;
  
  return result;
}

PUBLIC void SdbRequest_execute(SdbRequest * this, ...)
{
  unsigned int size = 0;
  SdbMgr * sdbMgr = SdbMgr_getRef();
  va_list args;
  
  va_start(args, this);
  size = vsnprintf(0, 0, this->fmt, args);
  this->buffer = Memory_realloc(this->buffer, this->size + 1, size + 1);
  this->buffer[size] = 0;
  
  vsnprintf(this->buffer, size+1, this->fmt, args);
  this->size =size;

  va_end(args);
  
  SdbMgr_execute(sdbMgr, 
                 this->buffer, 
                 &this->result);
  
  SdbMgr_delete(sdbMgr);
}
