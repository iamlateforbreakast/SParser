/* MyAllocator.h */
#ifndef _MYALLOCATOR_H_
#define _MYALLOCATOR_H_

#include "Allocator.h"

typedef struct MyAllocator MyAllocator;

MyAllocator * MyAllocator_new();
void MyAllocator_delete(MyAllocator * this);
char * MyAllocator_allocate(Allocator * allocator, unsigned int size);
void MyAllocator_deallocate(Allocator * allocator, char * ptr);

#endif /* _MYALLOCATOR_H_ */