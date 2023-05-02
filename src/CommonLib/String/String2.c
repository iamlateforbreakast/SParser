/**********************************************//** 
  /file String2.c
  
  /brief The String class provide a dynamic array of char terminated
  by 0.
  
  The class String is a container for text data.
**************************************************/

#include "String2.h"
#include "Class.h"
#include "Object.h"
#include "Memory.h"
#include "List.h"
#include <stdlib.h>

/**********************************************//** 
  /class String
**************************************************/
struct String
{
  Object object;
  int isOwned;
  char * buffer;
  unsigned int length;
};

/**********************************************//**
  /private Class Description
**************************************************/
PRIVATE Class stringClass = 
{
  .f_new = NULL,
  .f_delete = (Destructor)&String_delete,
  .f_copy = (Copy_Operator)&String_copy,
  .f_comp = (Comp_Operator)&String_compare,
  .f_print = (Printer)NULL
};

/**********************************************//** 
  /brief Create a new String object.
  /public
  /memberof String
  /return Created String object.
**************************************************/
PUBLIC String * String_new(const char* initString)
{
  String * this = 0;
  
  this = (String*)Object_new(sizeof(String),&stringClass);
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
  this->isOwned = 1;
  return this;
}

/**********************************************//** 
  /brief Create a new instance without copying the
  initial text.
  /public
  /memberof String
  /param[in] initString the const text to use for init
  /return created object
**************************************************/
PUBLIC String * String_newByRef(const char * initString)
{
  String * this = 0;
  
  this = (String*)Object_new(sizeof(String), &stringClass);

  if (initString != 0)
  {
    this->length = Memory_len((void*)initString);
    this->buffer = initString;
  }
  else
  {
    this->length = 0;
    this->buffer = 0;
  }
  this->isOwned = 0;
  
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
      if ((this->buffer!=0) && (this->isOwned))
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
  @brief Compare this String with another String
  @public
  @param [in]     compared: String to compare
  @memberof String
  @return 0 if S1=S2, negative if S1<S2, positive if S1>S2
**************************************************/
PUBLIC int String_compare(String * this, String * compared)
{ 
  int i = 0;
  int result = 0;

  while ((i < (int)this->length)&&(i < (int)compared->length))
  {
     result = (int)this->buffer[i] - (int)compared->buffer[i];
     if (result != 0) break;
     i++;
  }
  if (result == 0) result = (int)this->length - (int)compared->length;
  //result = Memory_ncmp(this->buffer, compared->buffer, this->length);
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
  
  result = atoi(String_getBuffer(this));
  
  return result;
}

/**********************************************//**
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC unsigned int String_getLength(String * this)
{
  if (this!=0)
  {
    return this->length;
  }
  return 0;
}

/**********************************************//**
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC char * String_getBuffer(String * this)
{
  if (this!=0)
  {
    return this->buffer;
  }
  return 0;
}

/**********************************************//**
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC void String_setBuffer(String * this, char * buffer)
{
  if (this!=0)
  {
    if (this->buffer!=0)
    {
      Memory_free(this->buffer, this->length+1);
    }
    this->buffer = buffer;
    if (buffer != 0)
      this->length = Memory_len((void*)this->buffer);
    else
      this->length = 0;
  }
}

/**********************************************//**
  @brief TBD
  @public
  @memberof String
**************************************************/
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

/**************************************************
 @brief String_prepend
 
 This function add the prefix at the front of a String object
 
 @param [in]     prefic: const char * - prefix
 @return: unsigned int: 0 successfull
**************************************************/
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
 @brief String_append

 This function add the prefix at the back of a String object

 @param [in]     postfix: const char * - prefix
 @return: unsigned int: 0 successfull
**************************************************/
PUBLIC unsigned int String_append(String* this, const char* postfix)
{
  char* buffer;
  unsigned int newSize = String_getLength(this) + Memory_len((void*)postfix);

  buffer = Memory_alloc(newSize + 1);
  
  Memory_copy(buffer, String_getBuffer(this), String_getLength(this));
  Memory_copy(buffer + String_getLength(this), (void*)postfix, Memory_len((void*)postfix));
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
  unsigned int isRange = 0;
  unsigned int possibleMatch = 0;
  
  while ((w_idx < wlen) && (isMatch == 1))
  {
    if ((wildcard[w_idx]=='*') && (w_idx + 1 < wlen))
    {  
      while ((f_idx < this->length) && (this->buffer[f_idx]!= wildcard[w_idx+1]))
      {
        f_idx++;
      }
      if (f_idx == this->length) isMatch = 0;
      w_idx++;
    }
    else if (wildcard[w_idx]=='?')
    {
    }
    else if (wildcard[w_idx]=='[')
    {
      //f_idx++;
      isRange = 1; 
      w_idx++;
    }
    else if (wildcard[w_idx]==']')
    {
      isRange = 0;
      w_idx++;
    }
    else if (this->buffer[f_idx]!= wildcard[w_idx])
    {
      if (!isRange)
      {
        //isMatch = 0;
        if (possibleMatch>0) w_idx = w_idx - possibleMatch-1;
        possibleMatch = 0;
        f_idx++;
      }
     else
     {
        //f_idx++;
        w_idx++;
     }     
    } 
    else
    {
      if (f_idx < this->length) f_idx++;
      w_idx++;
      possibleMatch++;
    }
    //printf("Possible match=%d\n", possibleMatch);
  }
  // Still some char in fileName not matched by filter
  if (f_idx < this->length) isMatch = 0;
  
  return isMatch;
}

/**************************************************
 @brief String_splitToken

 This function splits a string 

 @param [in]     separator: const char *: separator
 @return: unsigned int: 0 no match, 1 match
**************************************************/
PUBLIC List* String_splitToken(String* this, const char* separator)
{
  List* result = List_new();
  char* str = String_getBuffer(this);
  char* token = 0;
  token = strtok(str, separator);
  while (token != 0)
  {
    List_insertHead(result, String_new(token));
    token = strtok(0, separator);
  }
  return result;
}

/**************************************************
 @brief String_stealBuffer

 This function move the buffer from string s

 @param [in]     s String to steal from
**************************************************/
PUBLIC void String_stealBuffer(String* this, String* s)
{
  String_setBuffer(this, String_getBuffer(s));
  s->buffer = 0;
  s->length = 0;
  s->isOwned = 0;
}
