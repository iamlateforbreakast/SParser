/**********************************************//**
  @file List.c
     
  @brief This file contains the implementation of the class List.
				        
  The class List implement the List operations:
  - init
  - add
**************************************************/
#include "List.h"
#include "Class.h"
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
  @private Class Description
**************************************************/
PRIVATE Class listClass = 
{
  .f_new = 0,
  .f_delete = (Destructor)&List_delete,
  .f_copy = (Copy_Operator)&List_copy,
  .f_comp = (Comp_Operator)&List_compare,
  .f_print = (Printer)&List_print
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
  
  this = (List*)Object_new(sizeof(List),&listClass);
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
  
  copy->iterator = copy->tail;
  
  return copy;
}

/**********************************************//** 
  @brief Compare 2 instances of the class List.
  @public
  @memberof List
  @return 0 if different, 1 if equal.
**************************************************/
PUBLIC int List_compare(List * this, List * compared)
{
  unsigned int result = 0;

  return result;
}

/**********************************************//** 
  @brief Print an instance of the class List.
  @public
  @memberof List
**************************************************/
PUBLIC void List_print(List * this)
{
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
    this->iterator = this->head;
  }
  else
  {
    this->tail->prev = newNode;
    if (this->iterator == this->tail)   this->iterator = newNode;
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
PUBLIC void List_forEach(List* this, void (*method)(void* o))
{
  ListNode* iterator = this->tail;
  
  while (iterator!=0)
  {
    method(iterator->item);
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

/**********************************************//** 
  @brief Remove the head item in an instance of LIst
  @public
  @memberof List.
**************************************************/
PUBLIC void * List_removeHead(List * this)
{
  ListNode * headNode = 0;
  void * item = 0;

  if ((this!=0) && (this->nbNodes!=0))
  {
    headNode = this->head;
    item = this->head->item;
    if (this->head->prev!=0)
    {
      this->head = this->head->prev;
      this->head->next = 0;
    }
    else
    {
      this->head = 0;
      this->tail = 0;
      this->iterator = 0;
    }
    this->nbNodes--;
    //TODO: this->iterator;
    Memory_free(headNode, sizeof(ListNode));
  }

  return item;
}

/**********************************************//**
  @brief Remove the tail item in an instance of List
  @public
  @memberof List.
**************************************************/
PUBLIC void* List_removeTail(List* this)
{
  ListNode* tailNode = 0;
  void* item = 0;

  if ((this != 0) && (this->nbNodes != 0))
  {
    tailNode = this->tail;
    item = this->tail->item;
    if (this->tail->next != 0)
    {
      this->tail = this->tail->next;
      this->tail->prev = 0;
      this->iterator = this->tail;
    }
    else
    {
      this->head = 0;
      this->tail = 0;
      this->iterator = 0;
    }
    this->nbNodes--;
    //TODO: this->iterator;
    Memory_free(tailNode, sizeof(ListNode));
  }

  return item;
}

/**********************************************//** 
  @brief Get the head item in an insatnce of LIst
  @public
  @memberof List.
**************************************************/
PUBLIC void * List_getHead(List * this)
{
  void * result = 0;
  
  if (this!=0)
  {
    if (this->head!=0)
    {
      result = this->head->item;
    }
  }
  return result;
}

PUBLIC void List_resetIterator(List * this)
{
  if (this!=0)
  {
    this->iterator = this->head;
  }
}
