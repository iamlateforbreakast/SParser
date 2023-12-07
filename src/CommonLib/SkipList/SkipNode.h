/* SkipNode.h */

#ifndef _SKIPNODE_H_
#define _SKIPNODE_H_

#include "Types.h"
#include "Object.h"
#include "Allocator.h"
#include <limits.h>

#define SKIPLIST_MAX_LEVEL (6)
#define END_NODE (0)

typedef struct SkipNode SkipNode;

PUBLIC SkipNode * SkipNode_new();
PUBLIC SkipNode * SkipNode_newFromAllocator();
PUBLIC void SkipNode_delete(SkipNode * this);
PUBLIC SkipNode * SkipNode_copy(SkipNode * this);
PUBLIC int SkipNode_compare(SkipNode * this, SkipNode * compared);
PUBLIC void SkipNode_print(SkipNode * this);
PUBLIC unsigned int SkipNode_getSize(SkipNode * this);

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class skipNodeClass = 
{
  .f_new = 0,
  .f_delete = (Destructor)&SkipNode_delete,
  .f_copy = (Copy_Operator)&SkipNode_copy,
  .f_comp = (Comp_Operator)&SkipNode_compare,
  .f_print = (Printer)&SkipNode_print,
  .f_size = (Sizer)&SkipNode_getSize
};

typedef struct SkipNode
{
  Object object;
  Object * key;
  Object * item;
  unsigned int level;
  void * forward[SKIPLIST_MAX_LEVEL];
} SkipNode;


PUBLIC SkipNode * SkipNode_new()
{
  SkipNode * this = 0;

  this = (SkipNode*)Object_new(sizeof(SkipNode),&skipNodeClass);
  if (this==0) return 0;

  this->item = 0;
  this->level = 1;
  this->key = END_NODE;

  return this;
}

PUBLIC SkipNode * SkipNode_newFromAllocator(Allocator * allocator)
{
  SkipNode * this = 0;

  this = (SkipNode*)Object_newFromAllocator(&skipNodeClass, allocator);
  if (this == 0) return 0;
  this->item = 0;
  this->level = 1;
  this->key = END_NODE;
  return this;
}

PUBLIC void SkipNode_delete(SkipNode * this)
{
  if (this==0) return;

  Object_deallocate(&this->object);
}

PUBLIC SkipNode * SkipNode_copy(SkipNode * this)
{
  SkipNode * copy = 0;

  return copy;
}

PUBLIC int SkipNode_compare(SkipNode * this, SkipNode * compared)
{
    return 1;
}

PUBLIC void SkipNode_print(SkipNode * this)
{
  if (this==0) return;
  

  
}

PUBLIC unsigned int SkipNode_getSize(SkipNode * this)
{
  return sizeof(SkipNode);  
}
#endif /* _SKIPNODE_H_ */
