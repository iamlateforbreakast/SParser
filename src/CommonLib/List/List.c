/**********************************************//**
  @file List.c
     
  @brief This file contains the implementation of the class List.
				        
  The class List implement the List operations:
  - init
  - add
**************************************************/
#include "List.h"
#include "Object.h"

/**********************************************//**
  @private
**************************************************/
typedef struct ListNode ListNode;

/**********************************************//**
  @private
**************************************************/
struct ListNode
{
  void* item;
  ListNode* next;
  ListNode* prev;
};

/**********************************************//**
  @class List
**************************************************/
struct List
{
  Object object;
  ListNode * head;
  ListNode * tail;
  ListNode * current;
  unsigned int nbNodes;
};

/**********************************************//** 
  @brief Create a new instance of the class List.
  @public
  @memberof List
  @return New instance.
**************************************************/
PUBLIC List* List_new()
{
  List * this = 0;
  
  this = (List*)Object_new(sizeof(List),(Destructor)&List_delete, (Copy_operator)&List_copy);
  this->head = 0;
  this->tail = 0;
  this->current = this->head;
  this->nbNodes = 0;
  
  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class List.
  @public
  @memberof List
**************************************************/
PUBLIC void List_delete(List* this)
{
  ListNode * node = 0;
  
  if (this!=0)
  {
    node = this->head;
    while (node!=0)
    {
      this->head = node->next;
      if (((Object*)node->item)->delete!=0)
      {
        ((Object*)node->item)->delete(node->item);
      }
      /* Memory_free(n, sizeof(ListNode)) */
      node = this->head;
    }
    Object_delete(&this->object);
  }
}

/**********************************************//** 
  @brief Copy an instance of the class List.
  @public
  @memberof List
  @return Copy of the given instance.
**************************************************/
PUBLIC List* List_copy(List* this)
{
  List * copy = 0;
  
  return copy;
}

PUBLIC void List_insertHead(List* this, void* item)
{
}

PUBLIC void List_insertTail(List* this, void* item)
{
}

PUBLIC void List_merge(List* this, List* l1)
{
}

PUBLIC void List_iterator(List* this, void (*f)(void* t))
{
}