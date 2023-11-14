#ifndef _MALLOC_H_
#define _MALLOC_H_
#include "Allocator.h"
#include "Types.h"

typedef struct Malloc Malloc;

PUBLIC Malloc * Malloc_getRef();
PUBLIC void Malloc_delete(Allocator * this);
PUBLIC char * Malloc_allocate(Allocator * this, unsigned int size);
PUBLIC void Malloc_deallocate(Allocator * this, char * ptr);
PUBLIC unsigned int Malloc_report(Allocator * this);
#endif /* _MALLOC_H_ */