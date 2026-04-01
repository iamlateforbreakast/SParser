/**********************************************//**
  @file Array.c
     
  @brief This file contains the implementation of the class Array.
				        
  The class Array implement the Array operations:
  - init
  - put
  - get
**************************************************/
#include "Array.h"
#include "Memory.h"
#include "Class.h"
#include "Object.h"
#include "Debug.h"

#define DEFAULT_INITIAL_CAPACITY (8)

/**********************************************//**
  @class Array
**************************************************/
struct Array
{
  Object object;       /* Base class */
  Object **items;      /* Array of pointers to Objects */
  unsigned int nbElements;   /* Current number of elements */
  unsigned int capacity; /* Total allocated slots */
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
  .f_print = (Printer)&Array_print,
  .f_size = (Sizer)&Array_getSize,
  .classSize = sizeof(struct Array)
};

/**********************************************//** 
  @brief Create a new instance of the class Array.
  @public
  @memberof Array
  @return New instance.
**************************************************/
PUBLIC Array * Array_new(unsigned int initialCapacity)
{
  Array * newArray = 0;
  newArray = (Array*)Object_new(sizeof(Array), &arrayClass);

  if (OBJECT_IS_INVALID(newArray)) return 0;

  newArray->nbElements = 0;    
  newArray->capacity = (initialCapacity > 0) ? initialCapacity : DEFAULT_INITIAL_CAPACITY;
    
  /* Allocate the pointer table */
  newArray->items = (Object**)Memory_alloc(sizeof(Object*) * newArray->capacity);
	
  return newArray;
}

/**********************************************//** 
  @brief Create a new instance of the class Array from a fileIo stream
  @public
  @memberof Array
  @return New instance.
**************************************************/
PUBLIC Array * Array_newFromFile(FileIo * fileIo, ArrayParam * param)
{
  Array * newArray = 0;
  newArray = (Array*)Object_new(sizeof(Array), &arrayClass);

  //FileIo_read(fileIo, (char*)&newArray->nbElements, sizeof(newArray->nbElements));
  //newArray->pool = Pool_newFromFile(fileIo, NB_ELEMENT_MAX, ELEMENT_SIZE_BYTES);

  return newArray;
}

/**********************************************//** 
  @brief Delete an instance of the class Array.
  @public
  @memberof Array
**************************************************/
PUBLIC void Array_delete(Array * this)
{
  if (OBJECT_IS_INVALID(this)) return;

  for (unsigned int i = 0; i < this->nbElements; i++) 
  {
    Object_deRef(this->items[i]);
  }

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
  PRINT(("NbElements: %d\n", this->nbElements));

  //unsigned int currentNodeIdx = this->headerIdx;
  //SkipNode* skipNode = Pool_read(this->pool, currentNodeIdx);
  for (int i = 0; i < this->nbElements; i++)
  {
    //PRINT(("SkipNode: %d ", currentNodeIdx));
    //PRINT((" Key: %d ", skipNode->key));
    PRINT(("\n"));
    //Pool_discardCache(this->pool, currentNodeIdx);
    //currentNodeIdx = skipNode->forward[0];
    //skipNode = Pool_read(this->pool, skipNode->forward[0]);
    }
    //Pool_discardCache(this->pool, this->headerIdx);
    //Pool_discardCache(this->pool, currentNodeIdx);
}

PUBLIC unsigned int Array_getSize(Array * this)
{
  return 0;
}
