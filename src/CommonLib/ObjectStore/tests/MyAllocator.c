#include "MyAllocator.h"
#include "Malloc.h"

#include "Debug.h"
#include "Error.h"

#include <stdlib.h>

#define MYMEMORY_SIZE (1000)

struct MyAllocator
{
  Allocator allocator; 
  void * memory;
  void * pointer;
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

void * MyAllocator_allocate(Allocator * this, unsigned int size)
{
  char * ptr = ((MyAllocator*)this)->pointer;
  char * startAddress = ((MyAllocator*)this)->memory;

  if ((ptr+size)<(startAddress + MYMEMORY_SIZE))
  {
    void * allocatedPtr = ((MyAllocator*)this)->pointer;
    ptr += ((size / 8)+1)*8;
    ((MyAllocator*)this)->pointer = (void*)ptr;
    this->nbAllocatedObjects++;
    return (void*)allocatedPtr;
  }
  else
  {
    Error_new(ERROR_NORMAL,"Failed to allocate");
    return 0;
  }
}

void MyAllocator_deallocate(Allocator * this, void * ptr)
{
  this->nbAllocatedObjects--;
}

PUBLIC unsigned int MyAllocator_report(Allocator * this)
{
  return this->nbAllocatedObjects;
  //PRINT(("Nb allocated objects: %d\n", ((MyAllocator*)this)->nbAllocatedObjects));
  //PRINT(("Max nb allocated objects: %d\n", this->maxNbObjectAllocated));
  //PRINT(("Nb alloc request: %d\n", this->allocRequestId));
  //PRINT(("Nb free requests: %d\n", this->freeRequestId));
}
