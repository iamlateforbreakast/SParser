/**********************************************//**
  @file SkipList.c
  @brief This file contains the implementation of the class SkipList.
  The class List implement the SkipList operations
  - Add
  - Remove
  - Get
**************************************************/

#include "SkipList.h"

#include "Pool.h"
#include "Class.h"
#include "Object.h"
#include "Debug.h"
#include <stdlib.h>

#include "SkipNode.h"

/**********************************************//**
  @class SkipList
**************************************************/
typedef struct SkipList
{
    Object object;
    unsigned int level;
    unsigned int nbObjects;
    unsigned int pack;
    void * headerPtr;
    void * endPtr;
} SkipList;

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class skipListClass = 
{
  .f_new = 0,
  .f_delete = (Destructor)&SkipList_delete,
  .f_copy = (Copy_Operator)&SkipList_copy,
  .f_comp = (Comp_Operator)&SkipList_compare,
  .f_print = (Printer)&SkipList_print,
  .f_size = (Sizer)&SkipList_getSize
};

/**********************************************//**
  @private
**************************************************/
PRIVATE unsigned int SkipList_randLevel(SkipList* this);

/**********************************************//**
  @brief Create a new instance of the class SkipList.
  @public
  @memberof SkipList
  @param[in] none
  @return New instance of class SkipList.
**************************************************/
PUBLIC SkipList * SkipList_new()
{
    SkipList* newSkipList = 0;
    
    newSkipList = (SkipList*)Object_new(sizeof(SkipList),&skipListClass);
    
    if (newSkipList==0) return 0;

    newSkipList->level = 1;
    newSkipList->nbObjects = 0;

    /* Create the end node */
    SkipNode * skipNode = SkipNode_new();
    
    newSkipList->headerPtr = skipNode;
    newSkipList->endPtr = skipNode;

    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++)
    {
        skipNode->forward[i] = newSkipList->headerPtr;
    }

    skipNode->key = END_NODE;
    skipNode->item = 0;
    skipNode->level = SKIPLIST_MAX_LEVEL;

    return newSkipList;
}

/**********************************************//**
  @brief Create a new instance of the clss SkipLIst from
  an specified allocator.
  @public
  @memberof SkipList
  @param[in] none
  @return New instance of class SkipList.
**************************************************/
PUBLIC SkipList* SkipList_newFromAllocator(Allocator * allocator)
{
  SkipList* newSkipList = 0;

  newSkipList = (SkipList*)Object_newFromAllocator(&skipListClass, allocator);

  if (newSkipList==0) return 0;

  newSkipList->level = 1;
  newSkipList->nbObjects = 0;

  /* Create the end node */
  SkipNode * skipNode = SkipNode_newFromAllocator(allocator);

  newSkipList->headerPtr = skipNode;
  newSkipList->endPtr = skipNode;

  for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++)
  {
    skipNode->forward[i] = newSkipList->headerPtr;
  }

  skipNode->key = END_NODE;
  skipNode->item = 0;
  skipNode->level = SKIPLIST_MAX_LEVEL;

  return newSkipList;
}

/**********************************************//**
  @brief SkipList_delete
  @public
  @memberof SkipList
  @param[in] Instance to destroy
  @return None
**************************************************/
PUBLIC void SkipList_delete(SkipList* this)
{
  SkipNode * nextPtr = ((SkipNode*)this->headerPtr);

  if (this!=0)
  {
    while (nextPtr->key != 0)
    {
      nextPtr = ((SkipNode*)this->headerPtr)->forward[0];
      SkipNode_delete(this->headerPtr);
      this->headerPtr = nextPtr;
    }
    SkipNode_delete(this->headerPtr);
    //Removed: Pool_free(this->pool);
    Object_deallocate(&this->object);
  }
}

/**********************************************//**
  @brief SkipList_copy
  @public
  @memberof SkipList
  @param[in] Instance to copy
  @return A copy of the SkipList instance.
**************************************************/
PUBLIC SkipList * SkipList_copy(SkipList * this)
{
    return 0;
}

/**********************************************//**
  @brief SkipList_add
  @public
  @memberof SkipList
  @param[in] Key to index object
  @param[in] Object to add to SkipList object.
  @return None
**************************************************/
PUBLIC void SkipList_add(SkipList* this, Object * key, Object * item)
{
  //Removed: unsigned int update[SKIPLIST_MAX_LEVEL];
  void* updatePtr[SKIPLIST_MAX_LEVEL];
  //Removed: unsigned int currentNodeIdx = this->headerIdx;
  //void * currentNodePtr = this->headerPtr;

  //printf("[Cache usage] Add start %d\n", SkipList_reportCache(this));
  for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) updatePtr[i] = 0;

  void * nextNodePtr = this->headerPtr;
  //SkipNode* nextNode = Pool_read(this->pool, this->headerIdx);
  //SkipNode * nextNode = Allocator_read(this->allocator, this->header->Ptr);
  for (int i = this->level - 1; (i >= 0) && (nextNodePtr!=0); i--)
  {
    //while ((*this->isGreater)(((SkipNode*)nextNodePtr)->key,key))
    while ((((SkipNode*)nextNodePtr)->key !=0) && (Object_comp(((SkipNode*)nextNodePtr)->key, key) < 0))
    {
      updatePtr[i] = nextNodePtr;
      nextNodePtr = ((SkipNode*)nextNodePtr)->forward[i];
      //Removed: Pool_discardCache(this->pool, update[i]);
      //Removed: nextNode = Pool_read(this->pool, nextNode->forward[i]);
    }
    //Removed: Pool_discardCache(this->pool, nextNode->forward[i]);
    //Removed: Pool_discardCache(this->pool, nextNodeIdx);
    //Removed: nextNode = Pool_read(this->pool, update[i]);
    //Removed: nextNodeIdx = update[i];
    nextNodePtr = updatePtr[i];
  }

  //Removed: SkipNode * currentNode = Pool_read(this->pool, update[0]);
  void * currentNodePtr = updatePtr[0];
  //Removed: unsigned int currentKey = ((SkipNode*)currentNodePtr)->key;
  //Removed: Pool_discardCache(this->pool, update[0]);

  //printf("[Pool Usage]: %d\n", Pool_reportCacheUsed(this->pool));
  //if ((currentNodePtr!=0) && (((SkipNode*)currentNodePtr)->key == key))
  if ((currentNodePtr != 0) && (Object_comp(((SkipNode*)currentNodePtr)->key, key) == 0))
  {
    ((SkipNode*)currentNodePtr)->item = item;
    return;
  }
  else
  {
    unsigned int level = this->level;
    if (updatePtr[0] != 0) level = SkipList_randLevel(this);
    if (level > this->level)
    {
      this->level = level;
    }
    //Removed: unsigned int insertIdx;
    //Removed: SkipNode* insert = Pool_alloc(this->pool, &insertIdx);
    SkipNode * insertPtr = SkipNode_new();
    insertPtr->key = key;
    insertPtr->item = item;
    insertPtr->level = level;
    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) insertPtr->forward[i] = this->endPtr;

    //Removed: SkipNode* header = Pool_read(this->pool, this->headerIdx);
    for (int i = 0; i < (int)level; i++)
    {
      /* Inserting after the front node */
      if (updatePtr[0] != 0)
      {
        if (updatePtr[i] != 0)
        {
          //Removed: SkipNode* nodeBeforeInsert = Pool_read(this->pool, updatePtr[i]);
          insertPtr->forward[i] = ((SkipNode*)updatePtr[i])->forward[i];
          ((SkipNode*)updatePtr[i])->forward[i] = insertPtr;
          //Removed: Pool_write(this->pool, update[i], nodeBeforeInsert);
          if (((SkipNode*)this->headerPtr)->forward[i] == 0) ((SkipNode*)this->headerPtr)->forward[i] = insertPtr;
        }
        else
        {
          ((SkipNode*)this->headerPtr)->forward[i] = insertPtr;
          ((SkipNode*)this->headerPtr)->level = level;
        }
      }
      else
      { 
        /* Inserting in front */
        insertPtr->forward[i] = this->headerPtr;
      }
    }
    //Removed: Pool_write(this->pool, this->headerIdx, header);
    /* case of front insert */
    if (updatePtr[0] == 0) this->headerPtr = insertPtr;
    //Removed: Pool_write(this->pool, insertIdx, insert);
    this->nbObjects++;
    //Object_getRef(item);

    return;
  }
}

//int skiplist_insert(skiplist* list, int key, int value) {
//    snode* update[SKIPLIST_MAX_LEVEL + 1];
//    snode* x = list->header;
//    int i, level;
//    for (i = list->level; i >= 1; i--) {
//        while (x->forward[i]->key < key)
//            x = x->forward[i];
//        update[i] = x;
//    }
//    x = x->forward[1];
//
//    if (key == x->key) {
//        x->value = value;
//        return 0;
//    }
//    else {
//        level = rand_level();
//        if (level > list->level) {
//            for (i = list->level + 1; i <= level; i++) {
//                update[i] = list->header;
//            }
//            list->level = level;
//        }
//
//        x = (snode*)malloc(sizeof(snode));
//        x->key = key;
//        x->value = value;
//        x->forward = (snode**)malloc(sizeof(snode*) * (level + 1));
//        for (i = 1; i <= level; i++) {
//            x->forward[i] = update[i]->forward[i];
//            update[i]->forward[i] = x;
//        }
//    }
//    return 0;
//}
/**********************************************//**
  @brief SkipList_remove
  @public
  @memberof SkipList
  @param[in] Key of object to remove
  @return Object removed from SkipList object.
**************************************************/
PUBLIC Object * SkipList_remove(SkipList* this, Object * key)
{
  void* removedObject = 0;
  //Removed: unsigned int update[SKIPLIST_MAX_LEVEL];
  void* updatePtr[SKIPLIST_MAX_LEVEL];
  //Removed: unsigned int currentNodeIdx = this->headerIdx;

  //printf("[Cache usage] delete start %d\n", SkipList_reportCache(this));

  //Removed: for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) update[i] = 0;
  for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) updatePtr[i] = 0;

  void * nextNodePtr = this->headerPtr;
  //Removed: unsigned int nextNodeIdx = this->headerIdx;
  //Removed: SkipNode* nextNode = Pool_read(this->pool, this->headerIdx);

  for (int i = this->level - 1; (i >= 0) && (nextNodePtr != 0); i--)
  {

    while ((((SkipNode*)nextNodePtr)->key != 0) && (Object_comp(((SkipNode*)nextNodePtr)->key, key) < 0))
    {
      //update[i] = nextNodeIdx;
      updatePtr[i] = nextNodePtr;
      //nextNodeIdx = nextNode->forward[i];
      nextNodePtr = ((SkipNode*)nextNodePtr)->forward[i];
      //Pool_discardCache(this->pool, update[i]);
      //nextNode = Pool_read(this->pool, nextNode->forward[i]);
    }
    //Pool_discardCache(this->pool, nextNode->forward[i]);
    //Pool_discardCache(this->pool, nextNodeIdx);
    //nextNode = Pool_read(this->pool, update[i]);
    //nextNodeIdx = update[i];
    nextNodePtr = updatePtr[i];
  }

  // removed: SkipNode* currentNode = Pool_read(this->pool, update[0]);
  SkipNode * currentNode = updatePtr[0];
  //unsigned int currentKey = currentNode->key;
    
  // Removed: SkipNode* header = Pool_read(this->pool, this->headerIdx);

  if (Object_comp(((SkipNode*)this->headerPtr)->key, key) == 0)
  {
    //unsigned int nodeToRemoveIdx = this->headerIdx;
    //SkipNode* nodeToRemove = Pool_read(this->pool, nodeToRemoveIdx);
    SkipNode* nodeToRemove = this->headerPtr;
    this->headerPtr = nodeToRemove->forward[0];
    //SkipNode* newHeader = Pool_read(this->pool, this->headerIdx);
    SkipNode* newHeader = this->headerPtr;
    for (int i = newHeader->level; i < (int)nodeToRemove->level; i++) newHeader->forward[i] = nodeToRemove->forward[i];
    newHeader->level = this->level;
    // Removed: Pool_write(this->pool, this->headerIdx, newHeader);
    removedObject = nodeToRemove->item;
    // Removed: Pool_dealloc(this->pool, nodeToRemoveIdx);
    SkipNode_delete(nodeToRemove);
    this->nbObjects--;
    return removedObject;
  }
  //SkipNode* nodeToRemove = Pool_read(this->pool, currentNode->forward[0]);
  //unsigned int nodeToRemoveIdx = currentNode->forward[0];
  SkipNode* nodeToRemove = currentNode->forward[0];
  // Removed: Pool_discardCache(this->pool, update[0]);
  if (Object_comp(key, nodeToRemove->key) == 0)
  {
    SkipNode* nodeBeforeRemoval;
    for (int i = 0; i < (int)nodeToRemove->level; i++)
    {
      //nodeBeforeRemoval = Pool_read(this->pool, update[i]);
      nodeBeforeRemoval = updatePtr[i];
      nodeBeforeRemoval->forward[i] = nodeToRemove->forward[i];
      //Pool_write(this->pool, update[i], nodeBeforeRemoval);
    }
    removedObject = nodeToRemove->item;
    //Pool_dealloc(this->pool, nodeToRemoveIdx);
    SkipNode_delete(nodeToRemove);
    this->nbObjects--;
    return removedObject;
  }
  return 0;
  //printf("[Cache usage] delete end %d\n", SkipList_reportCache(this));
}

//int skiplist_delete(skiplist* list, int key) {
//    int i;
//    snode* update[SKIPLIST_MAX_LEVEL + 1];
//    snode* x = list->header;
//    for (i = list->level; i >= 1; i--) {
//        while (x->forward[i]->key < key)
//            x = x->forward[i];
//        update[i] = x;
//    }
//
//    x = x->forward[1];
//    if (x->key == key) {
//        for (i = 1; i <= list->level; i++) {
//            if (update[i]->forward[i] != x)
//                break;
//            update[i]->forward[1] = x->forward[i];
//        }
//        skiplist_node_free(x);
//
//        while (list->level > 1 && list->header->forward[list->level]
//            == list->header)
//            list->level--;
//        return 0;
//    }
//    return 1;
//}

PUBLIC Object * SkipList_get(SkipList* this, Object * key)
{
  void* foundItem = 0;
  //unsigned int update[SKIPLIST_MAX_LEVEL];
  //unsigned int currentNodeIdx = this->headerIdx;
  void* updatePtr[SKIPLIST_MAX_LEVEL];

  //printf("[Cache usage] delete start %d\n", SkipList_reportCache(this));

  //for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) update[i] = 0;
  for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) updatePtr[i] = 0;
  // Removed: unsigned int nextNodeIdx = this->headerIdx;
  // SkipNode* nextNode = Pool_read(this->pool, this->headerIdx);

  void* nextNodePtr = this->headerPtr;

  for (int i = this->level - 1; (i >= 0) && (nextNodePtr != 0); i--)
  {
    //while (((SkipNode*)nextNodePtr)->key <= key)
    while ((((SkipNode*)nextNodePtr)->key != 0) && (Object_comp(((SkipNode*)nextNodePtr)->key, key) <= 0))
    {
      updatePtr[i] = nextNodePtr;
      nextNodePtr = ((SkipNode*)nextNodePtr)->forward[i];
      //Pool_discardCache(this->pool, update[i]);
      //nextNode = Pool_read(this->pool, nextNode->forward[i]);
    }
    // Pool_discardCache(this->pool, nextNode->forward[i]);
    // Pool_discardCache(this->pool, nextNodeIdx);
    //nextNode = Pool_read(this->pool, update[i]);
    nextNodePtr = updatePtr[i];
  }

  //SkipNode* currentNode = Pool_read(this->pool, update[0]);
  SkipNode* currentNode = updatePtr[0];
  Object * currentKey = currentNode->key;
  //Pool_discardCache(this->pool, update[0]);
  //if (currentKey == key)
  if (Object_comp(currentKey, key) == 0)
  {
    foundItem = currentNode->item;
  }
    
  return foundItem;
}

/**********************************************//**
  @brief SkipList_compare
  @public
  @memberof SkipList
  @param[in] Instance to be compared to.
  @return 0 if equal, <0 if S1<S2, >0 if S1>S2
**************************************************/
PUBLIC int SkipList_compare(SkipList * this, SkipList * compared)
{
    return 0;
}

/**********************************************//**
  @brief SkipList_print
  @public
  @memberof SkipList
  @param[in] None
  @return None
**************************************************/
PUBLIC void SkipList_print(SkipList* this)
{
    PRINT(("NbObject: %d\n", this->nbObjects));
    PRINT(("Level: %d\n", this->level));
    PRINT(("Header node: %x\n", this->headerPtr));

    SkipNode * currentNodePtr = this->headerPtr;
    //SkipNode* skipNode = Pool_read(this->pool, currentNodeIdx);
    for (int i = 0; i < (int)this->nbObjects+1; i++)
    {
        PRINT(("SkipNode: %x ", currentNodePtr));
        PRINT((" Level: %d ", currentNodePtr->level));
        if (currentNodePtr->key != 0)
    {
      //PRINT((" Key: %d ", currentNodePtr->key));
      PRINT((" Key: ")); Object_print(currentNodePtr->key); PRINT((" "));
    }
    else
      PRINT((" Key: END_KEY "));
        for (int j = 0; j < (int)currentNodePtr->level; j++)
        {
            PRINT((" Forward[%d]: %x", j, currentNodePtr->forward[j]));
        }
        PRINT(("\n"));
        //Pool_discardCache(this->pool, currentNodeIdx);
        currentNodePtr = currentNodePtr->forward[0];
        //skipNode = Pool_read(this->pool, skipNode->forward[0]);
    }
    //Pool_discardCache(this->pool, this->headerIdx);
    //Pool_discardCache(this->pool, currentNodeIdx);*/
}

/**********************************************//**
  @brief SkipList_getSize
  @public
  @memberof SkipList
  @param[in] None
  @return None
**************************************************/ 
PUBLIC unsigned int SkipList_getSize(SkipList* this)
{
  if (this == 0)
    return sizeof(SkipList);
  else
    return sizeof(SkipList) + sizeof(SkipNode) * (this->nbObjects + 1);
}

PRIVATE unsigned int SkipList_randLevel(SkipList* this)
{
    int level = 1;
    while (rand() < RAND_MAX / 2 && level < SKIPLIST_MAX_LEVEL)
        level++;
    return level;
}
