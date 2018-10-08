/**********************************************//** 
  @file String2.c
  
  @brief The String class provide a dynamic array of char terminated
  by 0.
  
  The class String is TBD
**************************************************/

#include "String2.h"
#include "Object.h"
#include "Memory.h"
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
    this->length = Memory_len((void*)initString);
    this->buffer = (char*)Memory_alloc(this->length+1);
    Memory_copy(this->buffer, (void*)initString, this->length+1);
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
    if (this->object.refCount == 1)
    {
      // TODO: Check refCOunt
      if (this->buffer!=0) 
      {
        Memory_free(this->buffer, this->length + 1);
      }
      Object_delete(&this->object);
    }
    else if (this->object.refCount>1)
    {
      this->object.refCount--;
    }
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
  @brief Copy an instance of class String.
  @public
  @memberof String
  @return Copy of instance.
**************************************************/
PUBLIC String * String_getRef(String * this)
{
  String * ref = 0;
  
  if (this!=0)
  {
    this->object.refCount++;
    ref = this;
  }
  
  return ref;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC unsigned int String_isEqual(String * this, String * compared)
{
  unsigned int result = 0;
  
  if (this->length!=compared->length) return 0;
  result = Memory_ncmp(this->buffer, compared->buffer, this->length);
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC String * String_subString(String * this, unsigned int idx, unsigned int length)
{
  String * result = 0;
  
  if (this!=0)
  {
    result = String_new(0);
    if (idx+length < this->length)
    {
      result->length = length;
    }
    else
    {
      result->length = this->length - idx;  
    }
    result->buffer = Memory_alloc(result->length+1);
    Memory_copy(result->buffer, this->buffer + idx, result->length);
    result->buffer[length] = 0;
  }
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC int String_toInt(String* this)
{
  int result = 0;
  
  return result;
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
    this->length = Memory_len((void*)this->buffer);
  }
}

PUBLIC unsigned int String_isContained(String * this, String * s2)
{
  unsigned int result = 0;
  char * p = 0;
  
  if ((this==0) || (s2==0)) return 0;
  
  for (p=this->buffer; p<this->buffer + this->length - s2->length + 1;p++)
  {
    if (Memory_ncmp(p, s2->buffer, s2->length))
    {
      result = 1;
      break;
    }
  }
  
  return result;
} 

PUBLIC unsigned int String_prepend(String * this, const char * prefix)
{
  char * buffer;
  unsigned int newSize = String_getLength(this) + Memory_len((void*)prefix);
  
  buffer = Memory_alloc(newSize+1);
  Memory_copy(buffer, (void*)prefix, Memory_len((void*)prefix));
  Memory_copy(buffer+Memory_len((void*)prefix), String_getBuffer(this), String_getLength(this));
  Memory_free(this->buffer, this->length + 1);
  buffer[newSize] = 0;
  this->buffer = buffer;
  this->length = newSize;
  
  return 0;
}


/**************************************************
 @brief String_matchWildcard
 
 This function matches a String with a wildcard
 pattern
 
 @param [in]     filter: String* - wildcard filter
 @return: unsigned int: 0 no match, 1 match
**************************************************/
PUBLIC unsigned int String_matchWildcard(String * this, const char * wildcard)
{
  unsigned int isMatch = 1;
  unsigned int f_idx = 0;
  unsigned int w_idx = 0;
  unsigned int wlen = Memory_len(wildcard);
  
  for (w_idx = 0; (w_idx < wlen) && (isMatch == 1); w_idx++)
  {
    if ((wildcard[w_idx]=='*') && (w_idx + 1 < wlen))
    {  
      while ((f_idx < this->length) && (this->buffer[f_idx]!= wildcard[w_idx+1]))
      {
        f_idx++;
      }
      if (f_idx == this->length) isMatch = 0;
    }
    else if (wildcard[w_idx]=='?')
    {
    }
    else if (wildcard[w_idx]=='[')
    {
    }
    else if (wildcard[w_idx]==']')
    {
    }
    else if (this->buffer[f_idx]!= wildcard[w_idx])
    {
      isMatch = 0; 
    } else
    {
      if (f_idx < this->length) f_idx++;
    }
  }
  // Still some char in fileName not matched by filter
  if (f_idx < this->length) isMatch = 0;
  
  return isMatch;
}

