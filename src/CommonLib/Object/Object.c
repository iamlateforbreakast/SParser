/**********************************************//** 
  @file Object.c
  
  @class Object
  
  @brief This file contains the implementation for the class Object
  
  The class Object is TBD
**************************************************/

#include "Object.h"

PUBLIC Object * Object_new(unsigned int size, void (*f_delete)(Object*), void (*f_copy)(Object*))
{
  Object * this = 0;
  
  /* this->delete = f_delete; */
  /* this->copy = f_copy; */
  /* this->refCount = 0; */
  /* this->size = size; */
  
  return this;
}

PUBLIC void Object_delete(Object * this)
{
}

PUBLIC Object * Object_copy(Object * this)
{
  Object * copy;
  
  return copy;
}

PUBLIC Object* Object_getRef(Object* this)
{
}
