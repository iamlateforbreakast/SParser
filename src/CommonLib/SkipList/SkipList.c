/**********************************************//**
  @file SkipList.c

  @brief This file contains the implementation of the class SkipList.

  The class List implement the SkipList operations
  - Add
  - Remove
  - Get
**************************************************/

#include "SkipList.h"
#include <limits.h>


#define MAX_OBJECTS (10)
#define MAX_LEVEL (6)

typedef struct SkipNode SkipNode;

typedef struct SkipNode
{  
    unsigned int key;
    void * object;
    unsigned int forward[MAX_LEVEL];
} SkipNode;

typedef struct SkipList
{
   unsigned int level;
   unsigned int nbObjects;
   Pool * pool;
   unsigned int headerIdx;
} SkipList;

/**********************************************//**
  @brief SkipList_new
  @param[in] none
  @return New instance of class SkipList.
**************************************************/
SkipList * SkipList_new()
{
   SkipList * newSkipList = 0;
   
   newSkipList = (SkipList*)malloc(sizeof(SkipList));
   newSkipList->pool = Pool_new(MAX_OBJECTS, sizeof(SkipNode));
   newSkipList->level = 1;
   newSkipList->nbObjects = 0;
   SkipNode * skipNode = Pool_alloc(newSkipList->pool, &newSkipList->headerIdx);
   for (int i=0; i<MAX_LEVEL; i++)
   {
      skipNode->forward[i] = newSkipList->headerIdx;
   }
   skipNode->key = INT_MAX;

   return newSkipList;
}

/**********************************************//**
  @brief SkipList_new
  @param[in] Instance to destroy
  @return None
**************************************************/
void SkipList_free(SkipList * this)
{
  Pool_free(this->pool);
  free(this);
}

/**********************************************//**
  @brief SkipList_add
  @param[in] Key to index object
  @param[in] Object to add to SkipList object.
  @return None
**************************************************/
void SkipList_add(SkipList * this, void * object)
{
   SkipNode * snode = Pool_read(this->pool, this->headerIdx);
   for (int i=this->level;i=1;i--)
   {
      snode->forward[i];
   }
}

/**********************************************//**
  @brief SkipList_delete
  @param[in] Key of object to remove
  @return Object removed from SkipList object.
**************************************************/
void * SkipList_delete(SkipList * this, unsigned int key)
{
   return 0;
}
