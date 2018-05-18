/**********************************************//** 
  @file String2.c
  
  @brief The String class provide a dynamic array of char terminated
  by 0.
  
  The class String is TBD
**************************************************/

#include "String2.h"
#include "Object.h"
#include <stdlib.h>

/**********************************************//** 
  @class String
**************************************************/
struct String
{
  Object object;
  char * buffer;
  unsigned int length;
};

/**********************************************//** 
  @brief Create a new String object.
  @public
  @memberof String
  @return Created String object.
**************************************************/
PUBLIC String * String_new(const char* initString)
{
  String * this = 0;
  
  this = (String*)Object_new(sizeof(String),(Destructor)&String_delete, (Copy_operator)&String_copy);
  // TODO: Check if this is NULL
  if (initString!=0)
  {
    this->length = strlen(initString);
    this->buffer = Memory_alloc(this->length+1);
    Memory_copy(this->buffer, initString, this->length+1);
    this->buffer[this->length] = 0;
  }
  else
  {
    this->buffer = 0;
    this->length = 0;
  }
  
  return this;
}

/**********************************************//** 
  @brief Delete an instance of class String.
  @public
  @memberof String
**************************************************/
PUBLIC void String_delete(String * this)
{
  if (this!=0)
  {
    // TODO: Check refCOunt
    if (this->buffer!=0) 
    {
      Memory_free(this->buffer, this->length + 1);
    }
    Object_delete(&this->object);
  }
}

/**********************************************//** 
  @brief Copy an instance of class String.
  @public
  @memberof String
  @return Copy of instance.
**************************************************/
PUBLIC String * String_copy(String * this)
{
  String * copy = 0;
  
  if (this!=0)
  {
    copy = String_new(0);
    copy->length = this->length;
    if (this->buffer!=0)
    {
      copy->buffer = Memory_alloc(copy->length+1);
      Memory_copy(copy->buffer, this->buffer, copy->length);
      copy->buffer[this->length] = 0;
    }
  }
  
  return copy;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC unsigned int String_isEqual(String * this, String * compared)
{
  unsigned int result = 0;
  
  result = Memory_ncmp(this, compared, this->length);
  if (this->length==compared->length)
  {
    result = 1;
  }
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC String* String_subString(String* this, unsigned int pos, unsigned int length)
{
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC int String_toInt(String* this)
{
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC String* String_searchAndReplace(String* this, String* search, String* replace)
{
}

PUBLIC unsigned int String_getLength(String * this)
{
  if (this!=0)
  {
    return this->length;
  }
  return 0;
}

PUBLIC char * String_getBuffer(String * this)
{
  if (this!=0)
  {
    return this->buffer;
  }
  return 0;
}

PUBLIC void String_setBuffer(String * this, char * buffer)
{
  if (this!=0)
  {
    if (this->buffer!=0)
    {
      Memory_free(this->buffer, this->length+1);
    }
    this->buffer = buffer;
    // TO DO: check for NULL char
    this->length = strlen(this->buffer);
  }
}