/* Buffer.h */
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "Types.h"
#include "Class.h"
#include "Object.h"
#include "Debug.h"

typedef struct Buffer Buffer;

PRIVATE Buffer * Buffer_new();
PRIVATE void Buffer_delete(Buffer * this);
PRIVATE void Buffer_print(Buffer * this);
PRIVATE unsigned int Buffer_getSize(Buffer * this);

struct Buffer
{
  Object object;
  String* string;
  char* currentPtr;
  char* startPtr;
  int nbCharRead;
  char* writePtr;
  int nbCharWritten;
};

PRIVATE Class bufferClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&Buffer_delete,
  .f_copy = (Copy_Operator)0,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)&Buffer_print,
  .f_size = (Sizer)&Buffer_getSize
};

PRIVATE Buffer* Buffer_new()
{
  Buffer * this = 0;

  this = (Buffer*)Object_new(sizeof(Buffer), &bufferClass);

  return this;
}
PRIVATE Buffer * Buffer_newFromString(String * content)
{
  Buffer * this = 0;
  this = (Buffer*)Object_new(sizeof(Buffer), &bufferClass);

  this->string = content;
  this->startPtr = String_getBuffer(this->string);
  this->currentPtr = this->startPtr;
  this->nbCharRead = 0;

  return this;
}

PRIVATE void Buffer_delete(Buffer * this)
{
  if (this == 0) return;

  /* De-allocate the specific members */
  String_delete(this->string);
  this->startPtr = 0;
  this->currentPtr = 0;
  this->nbCharRead = 0;
  /* De-allocate the base object */
  Object_deallocate(&this->object);
}

PRIVATE void Buffer_print(Buffer * this)
{
  /* TODO: Implement */
}

PRIVATE unsigned int Buffer_getSize(Buffer * this)
{
  return sizeof(this);
}
PRIVATE unsigned int Buffer_writeNChar(Buffer* this, char* buf, int nchar)
{
}
#endif /* _BUFFER_H_ */
