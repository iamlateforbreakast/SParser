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
PUBLIC String * String_new()
{
  String * this = 0;
  
  this = (String*)Object_new(sizeof(String),(Destructor)&String_delete, (Copy_operator)&String_copy);
  this->buffer = 0;
  this->length = 0;
  
  return this;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC void String_delete(String * this)
{
  if (this->buffer!=0) free(this->buffer);
  Object_delete(&this->object);
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC String * String_copy(String * this)
{
  String * copy = 0;
  
  return copy;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof String
**************************************************/
PUBLIC unsigned int String_isEqual(String * this, String * compared)
{
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
