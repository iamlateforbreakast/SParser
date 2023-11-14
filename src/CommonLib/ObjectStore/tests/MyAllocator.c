#include "MyAllocator.h"
#include "Malloc.h"

#include "Debug.h"

#include <stdlib.h>

#define MYMEMORY_SIZE (1000)

struct MyAllocator
{
  Allocator allocator;
  char * memory;
  char * pointer;
};

MyAllocator * MyAllocator_new()
{
  MyAllocator * myAllocator = (MyAllocator*)Malloc_allocate((Allocator*)Malloc_getRef(),sizeof(MyAllocator));

  myAllocator->allocator.new = (NewFunction)MyAllocator_new;
  myAllocator->allocator.delete = (DeleteFunction)MyAllocator_delete;
  myAllocator->allocator.allocate = (AllocateFunction)MyAllocator_allocate;
  myAllocator->allocator.deallocate = (DeAllocateFunction)MyAllocator_deallocate;
  myAllocator->allocator.report = (ReportFunction)MyAllocator_report;
  myAllocator->allocator.nbAllocatedObjects = 0;
  myAllocator->memory = Malloc_allocate((Allocator*)Malloc_getRef(), MYMEMORY_SIZE);
  myAllocator->pointer = myAllocator->memory;

  return myAllocator;
}

void MyAllocator_delete(MyAllocator * this)
{
  Malloc_deallocate((Allocator*)Malloc_getRef(), (char*)this->memory);
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