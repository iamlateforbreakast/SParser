#ifndef _ARRAY_H_
#define _ARRAY_H_
/*********************************************************************************
* Array.h
*
*********************************************************************************/
#include "Types.h"
#include "Object.h"

typedef struct Array Array;

typedef struct ArrayParam
{
  unsigned int defaultSize;
  unsigned int storageMode;
  unsinged int autoresize;
} ArrayParam;

PUBLIC Array * Array_new(ArrayParam * param);
PUBLIC void Array_delete(Array* this);
PUBLIC List * Array_copy(Array* this);
PUBLIC int Array_compare(Array * this, Array * compared);
PUBLIC void Array_print(Array * this);
PUBLIC void Array_put(Array * this, unsigned int index);
PUBLIC Object * Array_get(Array * this, unsigned int index);
#endif /* _ARRAY_H_ */
