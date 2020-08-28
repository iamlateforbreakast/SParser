#include "SdbRequest.h"
#include "Class.h"
#include "Object.h"
#include "SdbMgr.h"
#include "Memory.h"
#include "List.h"

#include <stdio.h>
#include <stdarg.h>

/**********************************************//**
  @class SdbRequest
**************************************************/
struct SdbRequest
{
  Object object;
  char * buffer;
  unsigned int size;
  const char * fmt;
  List * result;
  unsigned int nbResults;
  unsigned int nbColumns;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class sdbRequestClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&SdbRequest_delete,
  .f_copy = (Copy_Operator)&SdbRequest_copy,
  .f_equal = (Equal_Operator)0,
  .f_print = (Printer)0
};

/**********************************************//** 
  @brief Create a new SdbRequest instance
  @memberof SdbRequest
  @public
  @parameter SQL statement template
  @return Instance of an SdbRequest
**************************************************/
PUBLIC SdbRequest * SdbRequest_new(const char * fmt)
{
  SdbRequest * this = 0;

  this = (SdbRequest*)Object_new(sizeof(SdbRequest), &sdbRequestClass);
  this->object.size = sizeof(SdbRequest);
  
  this->fmt = fmt;
  this->size = 0;
  this->buffer = 0;
  this->result = 0;
  this->nbResults = 0;
  this->nbColumns = 0;
  
  return this;
}

/**********************************************//** 
  @brief Create a new SdbRequest instance
  @memberof SdbRequest
  @public
  @parameter SQL statement template
  @return Instance of an SdbRequest
**************************************************/
PUBLIC void SdbRequest_delete(SdbRequest * this)
{
  if (this->buffer)
  {
    Memory_free(this->buffer, this->size);
    this->buffer = 0;
    this->size = 0;
    List_delete(this->result);
  }
  Object_delete(&this->object);
}

PUBLIC SdbRequest * SdbRequest_copy(SdbRequest * this)
{
  SdbRequest * result = 0;
  
  return result;
}

/**********************************************//** 
  @brief Execute a SdbRequest
  @memberof SdbRequest
  @public
  @parameter Variable list of parameter to use with SQL template
  @return Instance of an SdbRequest
**************************************************/
PUBLIC void SdbRequest_execute(SdbRequest * this, ...)
{
  unsigned int size = 0;
  SdbMgr * sdbMgr = SdbMgr_getRef();
  va_list args;
  
  /* Determine the size needed for the buffer and allocate the buffer */
  va_start(args, this);
  size = vsnprintf(0, 0, this->fmt, args);
  va_end(args);
  this->buffer = Memory_realloc(this->buffer, this->size + 1, size + 1);
  this->buffer[size] = 0;
  /* Print the formatted string in the buffer */
  va_start(args, this);
  vsnprintf(this->buffer, size+1, this->fmt, args);
  this->size =size;
  va_end(args);
  /* Allocate the list of results */
  this->result = List_new();
  
  this->nbResults = SdbMgr_execute(sdbMgr, 
                 this->buffer, 
                 this->result);
  // TODO: nbColumns
  SdbMgr_delete(sdbMgr);
}

PUBLIC unsigned int SdbRequest_getNbResult(SdbRequest * this)
{
  return this->nbResults;
}

PUBLIC List * SdbRequest_getResults(SdbRequest * this)
{
  return this->result;
}