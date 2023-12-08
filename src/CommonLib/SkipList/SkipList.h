/* SkipList.h */

#ifndef _SKIPLIST_
#define _SKIPLIST_

#include "Types.h"
#include "Object.h"
#include "Allocator.h"

typedef struct SkipList SkipList;

PUBLIC SkipList * SkipList_new();
PUBLIC SkipList * SkipList_newFromAllocator(Allocator * allocator);
PUBLIC void SkipList_delete(SkipList* skipList);
PUBLIC SkipList * SkipList_copy(SkipList * this);
PUBLIC void SkipList_add(SkipList* this, Object * key, Object * item);
PUBLIC Object * SkipList_remove(SkipList* this, Object * key);
PUBLIC Object * SkipList_get(SkipList* this, Object * key);
PUBLIC int SkipList_compare(SkipList * this, SkipList * compared);
PUBLIC void SkipList_print(SkipList* this);
PUBLIC unsigned int SkipList_getSize(SkipList* this);

#endif  /* _SKIPLIST_ */
