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
#include "Pool.h"

#define NB_ELEMENT_MAX (100)
#define ELEMENT_SIZE_BYTES (100)

/**********************************************//**
  @class Array
**************************************************/
struct Array
{
  Object object;
  unsigned int nbElements;
  Pool * pool;
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
  Array * newArray = 0;
  newArray = (Array*)Object_new(sizeof(Array), &arrayClass);

  newArray->pool = Pool_new(NB_ELEMENT_MAX, ELEMENT_SIZE_BYTES);
  newArray->nbElements = 0;

  return newArray;
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
      Pool_delete(this->pool);
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
  PRINT(("NbObject: %d\n", this->nbObjects));

  unsigned int currentNodeIdx = this->headerIdx;
  SkipNode* skipNode = Pool_read(this->pool, currentNodeIdx);
  for (int i = 0; i < this->nbObjects; i++)
  {
    PRINT(("SkipNode: %d ", currentNodeIdx));
    PRINT((" Key: %d ", skipNode->key));
    for (int j = 0; j < skipNode->level; j++)
    {
       PRINT((" Forward[%d]: %d", j, skipNode->forward[j]));
    }
    PRINT(("\n"));
    Pool_discardCache(this->pool, currentNodeIdx);
    currentNodeIdx = skipNode->forward[0];
    skipNode = Pool_read(this->pool, skipNode->forward[0]);
    }
    Pool_discardCache(this->pool, this->headerIdx);
    Pool_discardCache(this->pool, currentNodeIdx);
}
