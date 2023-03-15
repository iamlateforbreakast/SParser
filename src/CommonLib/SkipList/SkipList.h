#ifndef _SKIPLIST_
#define _SKIPLIST_
/* SkipList.h */
#include "Pool.h"

typedef struct SkipNode
{  
   unsigned int level;
   unsigned int nbObjects;
   unsigned int key;
   void * object;
   SkipNode * header;

typedef struct SkipList
{
   Pool * pool;  
} SkipList;

SkipList * SkipList_new();
void SkipList_free(SkipList * skipList):
void SkipList_add(SkipList * this, void * object);
void * SkipList_delete(SkipList * this, unsigned int key);
void * SkipList_get(SkipList * this, unsigned int key);
#end /* _SKIPLIST_ */
