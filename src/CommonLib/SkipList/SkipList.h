/* SkipList.h */

#ifndef _SKIPLIST_
#define _SKIPLIST_

#include "Types.h"

typedef struct SkipList SkipList;

PUBLIC SkipList* SkipList_new();
PUBLIC void SkipList_delete(SkipList* skipList);
PUBLIC SkipList * SkipList_copy(SkipList * this);
PUBLIC void SkipList_add(SkipList* this, unsigned int key, void* object);
PUBLIC void * SkipList_remove(SkipList* this, unsigned int key);
PUBLIC void * SkipList_get(SkipList* this, unsigned int key);
PUBLIC int SkipList_compare(SkipList * this, SkipList * compared);
PUBLIC void SkipList_print(SkipList* this);
PUBLIC unsigned int SkipList_getSize(SkipList* this);

#endif  /* _SKIPLIST_ */
