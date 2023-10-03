/**********************************************//**
  @file Array.c
     
  @brief This file contains the implementation of the class Array.
				        
  The class Array implement the Array operations:
  - init
  - put
  - get
**************************************************/
#include "Array.h"
#include "Class.h"
#include "Object.h"

/**********************************************//**
  @class Array
**************************************************/
struct Array
{
  Object object;
  unsigned int nbElements;
  Object ** tab;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class arrayClass = 
{
  .f_new = 0,
  .f_delete = (Destructor)&Array_delete,
  .f_copy = (Copy_Operator)&Array_copy,
  .f_comp = (Comp_Operator)&Array_compare,
  .f_print = (Printer)&Array_print
};

/**********************************************//** 
  @brief Create a new instance of the class Array.
  @public
  @memberof Array
  @return New instance.
**************************************************/
PUBLIC Array * Array_new(ArrayParam * param)
{
  Array * this = 0;
  this = (Array*)Object_new(sizeof(Array), &arrayClass);

  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class Array.
  @public
  @memberof Array
**************************************************/
PUBLIC void Array_delete(Array * this)
{
  if (this!=0)
  {
    if (this->object.refCount==1)
    {
      Object_delete(&this->object);
      this = 0;
    }
    else if (this->object.refCount>1)
    {
      this->object.refCount--;
    }
  }
}

/**********************************************//** 
  @brief Copy an instance of the class Array.
  @public
  @memberof Array
  @return Copy of the given instance.
**************************************************/
PUBLIC Array * Array_copy(Array * this)
{
  Array * copy = 0;

  return copy;
}

/**********************************************//** 
  @brief Compare 2 instances of the class Array.
  @public
  @memberof Array
  @return 0 if different, 1 if equal.
**************************************************/
PUBLIC int Array_compare(Array * this, Array * compared)
{
  unsigned int result = 0;

  return result;
}

/**********************************************//** 
  @brief Print an instance of the class Array.
  @public
  @memberof Array
**************************************************/
PUBLIC void Array_print(Array * this)
{
}
