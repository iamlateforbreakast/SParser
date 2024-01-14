#include "MyAllocator.h"
#include "Malloc.h"
#include "Types.h"

#include "Debug.h"
#include "Error.h"

struct MyAllocator
{
  Allocator allocator;
  unsigned int size;
  void * memory;
  void * pointer;
};

MyAllocator * MyAllocator_new(unsigned int size)
{
  Malloc* malloc = Malloc_getRef();
  MyAllocator * myAllocator = (MyAllocator*)Malloc_allocate((Allocator*)malloc,sizeof(MyAllocator));

  myAllocator->allocator.new = (NewFunction)MyAllocator_new;
  myAllocator->allocator.delete = (DeleteFunction)MyAllocator_delete;
  myAllocator->allocator.allocate = (AllocateFunction)MyAllocator_allocate;
  myAllocator->allocator.deallocate = (DeAllocateFunction)MyAllocator_deallocate;
  myAllocator->allocator.report = (ReportFunction)MyAllocator_report;
  myAllocator->allocator.nbAllocatedObjects = 0;
  myAllocator->size = size;
  myAllocator->memory = Malloc_allocate((Allocator*)Malloc_getRef(), myAllocator->size);
  myAllocator->pointer = myAllocator->memory;

  return myAllocator;
}

void MyAllocator_delete(MyAllocator * this)
{
  Malloc_deallocate((Allocator*)Malloc_getRef(), (char*)this->memory);
  Malloc_deallocate((Allocator*)Malloc_getRef(), (char*)this);
}

void MyAllocator_reset(Allocator * this)
{

}

void * MyAllocator_allocate(Allocator * this, unsigned int size)
{
  if ((((MyAllocator*)this)->pointer+size)<(((MyAllocator*)this)->memory + ((MyAllocator*)this)->size))
  {
    void * allocatedPtr = ((MyAllocator*)this)->pointer;
    ((MyAllocator*)this)->pointer += ((size / MEM_ALIGN) + 1)*MEM_ALIGN;
    this->nbAllocatedObjects++;
    return allocatedPtr;
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
