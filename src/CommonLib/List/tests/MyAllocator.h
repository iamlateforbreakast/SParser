/* MyAllocator.h */
#ifndef _MYALLOCATOR_H_
#define _MYALLOCATOR_H_

#include "Allocator.h"
#include "Types.h"

typedef struct MyAllocator MyAllocator;

PUBLIC MyAllocator * MyAllocator_new(unsigned int size);
PUBLIC void MyAllocator_delete(MyAllocator * this);
void MyAllocator_reset(Allocator * this);
PUBLIC void * MyAllocator_allocate(Allocator * allocator, unsigned int size);
PUBLIC void MyAllocator_deallocate(Allocator * allocator, void * ptr);
PUBLIC unsigned int MyAllocator_report(Allocator * this);

#endif /* _MYALLOCATOR_H_ */