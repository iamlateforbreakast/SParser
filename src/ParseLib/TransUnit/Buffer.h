/* Buffer.h */
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "String2.h"
#include "Memory.h"
#include "Types.h"
#include "Class.h"
#include "Object.h"
#include "Debug.h"

#define DEFAULT_SIZE (1024)

typedef struct Buffer Buffer;

PRIVATE Buffer * Buffer_new();
PRIVATE Buffer * Buffer_newFromString();
PRIVATE void Buffer_delete(Buffer * this);
PRIVATE void Buffer_print(Buffer * this);
PRIVATE unsigned int Buffer_getSize(Buffer * this);
PRIVATE int Buffer_accept(Buffer* this, const char* keyword);
PRIVATE int Buffer_writeNChar(Buffer* this, char* buf, int nchar);
PRIVATE int Buffer_readOneChar(Buffer* this, char* c);
PRIVATE int Buffer_peekOneChar(Buffer* this, char* c);
PRIVATE int Buffer_readWithDelimiter(Buffer* this, char c, String** extracted);
PRIVATE int Buffer_isEmpty(Buffer* this);
PRIVATE String* Buffer_toString(Buffer* this);

/**********************************************//**
  @class Buffer
**************************************************/
struct Buffer
{
  Object object;
  String* string;
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
  Buffer* this = 0;

  this = (Buffer*)Object_new(sizeof(Buffer), &bufferClass);

  if (OBJECT_IS_INVALID(this)) return 0;

  this->size = DEFAULT_SIZE;
  this->string = 0;
  this->content = (char*)Memory_alloc(this->size);
  *this->content = 0;

  this->readPtr = this->content;
  this->nbCharRead = 0;

  this->writePtr = this->content;
  this->nbCharWritten = 0;

  return this;
}

/**********************************************//**
  @brief Create a new Buffer object from a given String
  @public
  @memberof TransUnit
  @return Created Buffer instance.
**************************************************/
PRIVATE Buffer * Buffer_newFromString(String* content)
{
  Buffer * this = 0;

  this = (Buffer*)Object_new(sizeof(Buffer), &bufferClass);

  if (OBJECT_IS_INVALID(this)) return 0;

  this->string = content;
  this->content = String_getBuffer(content);
  this->size = String_getLength(content);

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
  if (OBJECT_IS_INVALID(this)) return;

  /* De-allocate the specific members */

  if (this->string)
    String_delete(this->string);
  else
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
  @brief Print an instance of a Buffer object.
  @public
  @memberof TransUnit
**************************************************/
PRIVATE void Buffer_print(Buffer * this)
{

}

/**********************************************//**
  @brief Give the size in bytes of an instance of a Buffer object.
  @public
  @memberof TransUnit
  @return size in bytes of instance
**************************************************/
PRIVATE unsigned int Buffer_getSize(Buffer * this)
{
  return sizeof(this);
}

/**********************************************//**
  @brief Write a buffer to the Buffer object.
  @public
  @memberof TransUnit
**************************************************/
PRIVATE int Buffer_writeNChar(Buffer* this, char* buf, int nchar)
{
  if ((this->nbCharWritten + nchar) > this->size)
  {
    PRINT(("Must resize\n"));
    this->content = Memory_realloc(this->content, this->size, this->size * 2);
    this->size *= 2;

    return 0;
  }
  for (int idx = 0; idx < nchar; idx++)
  {
    *this->writePtr = buf[idx];
    this->writePtr++;
  }
  this->nbCharWritten += nchar;

  return 1;
}

/**********************************************//**
  @brief Accept a keyword from the Buffer object.
  @public
  @memberof TransUnit
**************************************************/
PRIVATE int Buffer_accept(Buffer* this, const char* keyword)
{
  long long int len1, len2;

  if ((this->content + this->size) < (this->readPtr + Memory_len(keyword)))
  {
    len1 = this->readPtr + Memory_len(keyword) - this->content - this->size;
    len2 = Memory_len(keyword) - len1;
  }
  else
  {
    len1 = Memory_len(keyword);
    len2 = 0;
  }

  if (Memory_ncmp(this->readPtr, keyword, len1))
  {
    this->readPtr += len1;
    this->nbCharRead += len1;
    if (Memory_ncmp(this->content, keyword + len1, len2))
    {
      this->readPtr += len2;
      this->nbCharRead += len2;
      return 1;
    }
    else
      return 0;
  }
  else
    return 0;
}

/**********************************************//**
  @brief Read one character from the Buffer object.
  @public
  @memberof TransUnit
  @return status: failed = 0, success = 1
**************************************************/
PRIVATE int Buffer_readOneChar(Buffer* this, char* c)
{
  *c = *this->readPtr;
  this->nbCharRead++;
  this->readPtr++;
  if ((this->readPtr) >= (this->content + this->size))
    this->readPtr = this->content;
  if ((this->readPtr == this->writePtr)) return 0;
  /* Success: one character was read */
  return 1;
}
/**********************************************//**
  @brief Read one character from the Buffer object.
  @public
  @memberof TransUnit
  @return status: failed = 0, success = 1
**************************************************/
PRIVATE int Buffer_peekOneChar(Buffer* this, char* c)
{
  *c = *this->readPtr;
  if ((this->readPtr == this->writePtr)) return 0;
  /* Success: one character was read */
  return 1;
}

/**********************************************//**
  @brief Read a string between 2 delimiters from the Buffer object.
  @public
  @memberof TransUnit
  @return status: failed = 0, success = 1
**************************************************/
PRIVATE int Buffer_readWithDelimiter(Buffer* this, char c, String** extracted)
{
  String* result = 0;

  *extracted = result;

  return 0;
}

PRIVATE int Buffer_isEmpty(Buffer* this)
{
  return (this->readPtr == this->writePtr);
}

PRIVATE String* Buffer_toString(Buffer* this)
{
  String* result = 0;
  int size = 0;
  char* buffer ;
  int stringLength = 0;

  if (this->readPtr > this->writePtr)
  {
    stringLength = this->writePtr - this->content + this->size;
    buffer = Memory_alloc(stringLength + 1);
    Memory_copy(buffer, this->readPtr, this->size - stringLength);
    Memory_copy(buffer, this->content, this->writePtr - this->content);
    buffer[stringLength] = 0;
  }
  else
  {
    stringLength = this->writePtr - this->content;
    buffer = Memory_alloc(stringLength + 1);
    Memory_copy(buffer, this->content, stringLength);
    buffer[stringLength] = 0;
  }
  result = String_new(0);
  String_setBuffer(result, buffer, 1);
  return result;
}
#endif /* _BUFFER_H_ */
