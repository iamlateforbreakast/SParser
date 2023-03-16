#ifndef _SKIPLIST_
#define _SKIPLIST_
/* SkipList.h */
#include "Types.h"
#include "Pool.h"

typedef struct SkipList SkipList;

PUBLIC SkipList* SkipList_new();
PUBLIC void SkipList_free(SkipList* skipList);
PUBLIC void SkipList_add(SkipList* this, unsigned int key, void* object);
PUBLIC void * SkipList_delete(SkipList* this, unsigned int key);
PUBLIC void * SkipList_get(SkipList* this, unsigned int key);
PUBLIC void SkipList_print(SkipList* this);
#endif  /* _SKIPLIST_ */
