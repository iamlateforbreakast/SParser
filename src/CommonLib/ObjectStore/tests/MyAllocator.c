#include "MyAllocator.h"
#include "Malloc.h"

#include "Debug.h"

#include <stdlib.h>

struct MyAllocator
{
  Allocator allocator;
  unsigned int nbAllocatedObjects;
  char * memory;
};

MyAllocator * MyAllocator_new()
{
  MyAllocator * myAllocator = (MyAllocator*)Malloc_allocate((Allocator*)Malloc_getRef(),sizeof(MyAllocator));;

  return myAllocator;
}

void MyAllocator_delete(MyAllocator * this)
{
  Malloc_deallocate((Allocator*)Malloc_getRef(), (char*)this);
}

char * MyAllocator_allocate(Allocator * allocator, unsigned int size)
{
    return 0;
}

void MyAllocator_deallocate(Allocator * allocator, char * ptr)
{
  free(ptr);
}

PUBLIC unsigned int MyAllocator_report(Allocator * this)
{
  return this->nbAllocatedObjects;
  //PRINT(("Nb allocated objects: %d\n", ((MyAllocator*)this)->nbAllocatedObjects));
  //PRINT(("Max nb allocated objects: %d\n", this->maxNbObjectAllocated));
  //PRINT(("Nb alloc request: %d\n", this->allocRequestId));
  //PRINT(("Nb free requests: %d\n", this->freeRequestId));
}