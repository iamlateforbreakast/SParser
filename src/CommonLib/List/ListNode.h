/* ListNode.h */

#ifndef _LISTNODE_H_
#define _LISTNODE_H_

#include "Types.h"
#include "Allocator.h"
#include "Class.h"
#include "Object.h"
#include "Memory.h"
#include "ObjectStore.h"

typedef struct ListNode ListNode;

PRIVATE ListNode * ListNode_new();
PRIVATE ListNode * ListNode_newFromAllocator(Allocator * allocator, Object * obejct);
PRIVATE void ListNode_delete(ListNode* this);
PRIVATE ListNode * ListNode_copy(ListNode* this);
PRIVATE int ListNode_compare(ListNode * this, ListNode * compared);
PRIVATE void ListNode_print(ListNode * this);
PRIVATE unsigned int ListNode_getSize(ListNode * this);

/**********************************************//**
  @private
**************************************************/
struct ListNode
{
  Object object;
  void* item;
  int isOwned;
  ListNode* next;
  ListNode* prev;
};

/**********************************************//**
  @class ListNode
**************************************************/
PRIVATE Class listNodeClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&ListNode_delete,
  .f_copy = (Copy_Operator)&ListNode_copy,
  .f_comp = (Comp_Operator)&ListNode_compare,
  .f_print = (Printer)&ListNode_print,
  .f_size = (Sizer)&ListNode_getSize
};

/**********************************************//**
  @brief Create a new instance of the class ListNode.
  @public
  @memberof List
  @return New instance.
**************************************************/
PRIVATE ListNode* ListNode_new(Object* object)
{
  ListNode* this = 0;

  this = (ListNode*)Object_new(sizeof(ListNode), &listNodeClass);
  // TODO: Check if allocation failed
  this->item = Object_getRef(object);
  this->isOwned = 0;
  this->next = 0;
  this->prev = 0;

  return this;
}

/**********************************************//**
  @brief Create a new instance of the class List using a custom allocator
  @public
  @memberof List
  @return New instance.
**************************************************/
PRIVATE ListNode* ListNode_newFromAllocator(Allocator* allocator, Object* object)
{
  ListNode* this = 0;

  this = (ListNode*)Object_newFromAllocator(&listNodeClass, allocator);
  // TODO: Check if allocation failed
  this->item = object;
  this->isOwned = 0;
  this->next = 0;
  this->prev = 0;

  return this;
}

/**********************************************//**
  @brief Delete an instance of the class List.
  @public
  @memberof List
**************************************************/
PRIVATE void ListNode_delete(ListNode* this)
{

  if (this != 0)
  {
    if ((this->item) && ((Object*)this->item)->delete != 0)
    {
      ((Object*)this->item)->delete(this->item);
    }
    if (this->object.allocator)
    {
      ObjectStore* objectStore = ObjectStore_getRef();
      ObjectStore_deleteObject(objectStore, (Object*)this);
      ObjectStore_delete(objectStore);
    }
    else
      Memory_free(this, sizeof(ListNode));
    //Object_delete(&this->object);
  }
}

/**********************************************//**
  @brief Copy an instance of the class List.
  @public
  @memberof List
  @return Copy of the given instance.
**************************************************/
PRIVATE ListNode* ListNode_copy(ListNode* this)
{
  ListNode* copy = 0;
  //Object* item = 0;

  if (this != 0)
  {
    copy = ListNode_new(((Object*)this->item));
  }

  return copy;
}

/**********************************************//**
  @brief Compare 2 instances of the class List.
  @public
  @memberof List
  @return 0 if different, 1 if equal.
**************************************************/
PRIVATE int ListNode_compare(ListNode* this, ListNode* compared)
{
  unsigned int result = 0;

  return result;
}

/**********************************************//**
  @brief Print an instance of the class List.
  @public
  @memberof List
**************************************************/
PRIVATE void ListNode_print(ListNode* this)
{
}

PRIVATE unsigned int ListNode_getSize(ListNode* this)
{
  return sizeof(ListNode);
}


#endif /* _LISTNODE_H_ */