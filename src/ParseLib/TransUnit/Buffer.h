/* Buffer.h */
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "Memory.h"
#include "Types.h"
#include "Class.h"
#include "Object.h"
#include "Debug.h"

#define DEFAULT_SIZE (1024)

typedef struct Buffer Buffer;

PRIVATE Buffer * Buffer_new();
PRIVATE void Buffer_delete(Buffer * this);
PRIVATE void Buffer_print(Buffer * this);
PRIVATE unsigned int Buffer_getSize(Buffer * this);
PRIVATE unsigned int Buffer_accept(Buffer* this, char* keyword);

/**********************************************//**
  @class Buffer
**************************************************/
struct Buffer
{
  Object object;
  char *content;
  char* readPtr;
  int nbCharRead;
  char* writePtr;
  int nbCharWritten;
  int size;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class bufferClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&Buffer_delete,
  .f_copy = (Copy_Operator)0,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)&Buffer_print,
  .f_size = (Sizer)&Buffer_getSize
};

/**********************************************//**
  @brief Create a new Buffer object.
  @public
  @memberof TransUnit
  @return Created Buffer instance.
**************************************************/
PRIVATE Buffer* Buffer_new()
{
  Buffer * this = 0;

  this = (Buffer*)Object_new(sizeof(Buffer), &bufferClass);

  this->size = DEFAULT_SIZE;
  this->content = (char*)Memory_alloc(this->size);
  *this->content = 0;

  this->readPtr = this->content;
  this->nbCharRead = 0;

  this->writePtr = this->content;
  this->nbCharWritten = 0;

  return this;
}

/**********************************************//**
  @brief Create a new Buffer object using exisitinng String
  @public
  @memberof TransUnit
  @return Created Buffer instance.
**************************************************/
PRIVATE Buffer * Buffer_newFromString(String * content)
{
  Buffer * this = 0;
  this = (Buffer*)Object_new(sizeof(Buffer), &bufferClass);

  this->content = String_getBuffer(content);
  this->size = String_getSize(content);

  this->readPtr = this->content;
  this->nbCharRead = 0;

  this->writePtr = this->content + this->size;
  this->nbCharWritten = this->size;

  return this;
}

/**********************************************//**
  @brief Delete an instance of a Buffer object.
  @public
  @memberof TransUnit
**************************************************/
PRIVATE void Buffer_delete(Buffer * this)
{
  if (this == 0) return;

  /* De-allocate the specific members */
  Memory_free(this->content, this->size);
  this->content = 0;
  this->readPtr = 0;
  this->writePtr = 0;
  this->nbCharRead = 0;
  this->nbCharWritten = 0;
  
  /* De-allocate the base object */
  Object_deallocate(&this->object);
}

/**********************************************//**
  @brief Print a new Buffer object.
  @public
  @memberof TransUnit
**************************************************/
PRIVATE void Buffer_print(Buffer * this)
{
  /* TODO: Implement */
}

/**********************************************//**
  @brief Returns the size a Buffer object.
  @public
  @memberof TransUnit
  @return Size in bytes.
**************************************************/
PRIVATE unsigned int Buffer_getSize(Buffer * this)
{
  return sizeof(this);
}

PRIVATE unsigned int Buffer_writeNChar(Buffer* this, char* buf, int nchar)
{
}

PRIVATE unsigned int Buffer_accept(Buffer* this, char* keyword)
{
  if (Memory_ncmp(this->readPtr, keyword, sizeof(keyword)))
  {
    this->readPtr += sizeof(keyword);
    this->nbCharRead += sizeof(keyword);
    return 1;
  }
  else
    return 0;
}
#endif /* _BUFFER_H_ */
