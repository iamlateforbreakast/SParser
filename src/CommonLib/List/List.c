/**********************************************//**
  @file List.c
     
  @brief This file contains the implementation of the class List.
				        
  The class List implement the List operations:
  - init
  - add
**************************************************/
#include "List.h"
#include "Object.h"
#include "Memory.h"

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
  ListNode * iterator;
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
  // TODO: Check if allocation failed
  this->head = 0;
  this->tail = 0;
  this->iterator = this->head;
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
    //node = this->tail;
    while ((node = this->tail)!=0)
    {
      this->tail = node->next;
      if (((Object*)node->item)->delete!=0)
      {
        ((Object*)node->item)->delete(node->item);
      }
      Memory_free(node, sizeof(ListNode));
      //node = this->tail;
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
  ListNode * iterator = 0;
  ListNode * node = 0;
  
  if (this!=0)
  {
    copy = List_new();
    iterator = this->tail;
    while (iterator!=0)
    {
      if (((Object*)iterator->item)->copy!=0)
      {
        node = (ListNode *)((Object*)iterator->item)->copy(iterator->item);
        List_insertHead(copy, node);
      }
      iterator = iterator->next;
    }
  }
  
  return copy;
}

/**********************************************//** 
  @brief Insert an item at the head of a list instance.
  @public
  @memberof List
  @param[in] item Reference to item.
**************************************************/
PUBLIC void List_insertHead(List* this, void* item)
{
  ListNode* newNode = 0;
  
  newNode = Memory_alloc(sizeof(ListNode));
  newNode->item = item;
  newNode->next = 0;
  newNode->prev = this->head;
  /* The following test case for empty list. */
  if (this->nbNodes == 0)
  {
    this->tail = newNode;
    this->iterator = this->tail;
  }
  else
  {
      this->head->next = newNode;
      if (this->iterator == 0) this->iterator = newNode;
  }
  this->head = newNode;

  this->nbNodes ++;
}

/**********************************************//** 
  @brief Insert an item at the tail of a List instance.
  @public
  @memberof List
  @param[in] item Reference to item.
**************************************************/
PUBLIC void List_insertTail(List* this, void* item)
{
  ListNode* newNode = 0;
  
  newNode = Memory_alloc(sizeof(ListNode));
  newNode->item = item;
  newNode->next = this->tail;
  newNode->prev = 0;
  /* The following test case for empty list. */
  if (this->nbNodes == 0)
  {
    this->head = newNode;
    this->iterator = this->tail;
  }
  else
  {
    this->tail->prev = newNode;
  }
  this->tail = newNode;
  this->nbNodes ++;
}

/**********************************************//** 
  @brief Merge a list into a List instance.
  @public
  @memberof List
  @param[in] l1 Reference to list to merge.
**************************************************/
PUBLIC void List_merge(List* this, List* l1)
{ 
  if ((l1!=0) && (l1->head!=0))
  {
    this->tail->prev = l1->head;
    l1->head->next = this->tail;
    this->tail = l1->tail;
    this->nbNodes += l1->nbNodes;
    this->iterator = this->tail;
  }
  l1->nbNodes = 0;
  l1->iterator = 0;
  l1->head = 0;
  l1->tail = 0;
  l1->iterator = 0;
  List_delete(l1);
}

/**********************************************//** 
  @brief Execute a given function for each item in an instance of List..
  @public
  @memberof List
  @param[in] f Pointer to function.
**************************************************/
PUBLIC void List_forEach(List* this, void (*f)(void* t))
{
  ListNode* iterator = this->tail;
  
  while (iterator!=0)
  {
    f(iterator->item);
    iterator = iterator->next;
  }
}

/**********************************************//** 
  @brief Get the number of item in List instance.
  @public
  @memberof List
  @return Number of items.
**************************************************/
PUBLIC unsigned int List_getSize(List * this)
{
  return this->nbNodes;
}

PUBLIC void * List_getNext(List * this)
{
  void * result = 0;
  
  if (this->iterator==0) 
  {
    this->iterator = this->tail;
    result = 0;
  }
  else
  {
    result = this->iterator->item;
    this->iterator = this->iterator->next;
  }
  
  
  return result;
}