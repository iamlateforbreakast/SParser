/* Arean.h */
#ifndef _ARENA_H_
#define _ARENA_H_
#include "Allocator.h"
#include "Types.h"

typedef struct Arena Arena；

PUBLIC Arena * Arena_getRef();
PUBLIC void Arena_delete(Allocator * this);
PUBLIC void * Arena_allocate(Allocator * this, unsigned int size);
PUBLIC void Arena_deallocate(Allocator * this, void * ptr);
PUBLIC unsigned int Arena_report(Allocator * this);
#endif /* _ARENA_H_ */
