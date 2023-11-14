#include "Malloc.h"
#include "Types.h"
#include "Debug.h"

#include <stdlib.h>

struct Malloc
{
  Allocator allocator;
  int nbAllocatedObjects;
};

PRIVATE Malloc mem_alloc;


PUBLIC Malloc * Malloc_getRef()
{
  mem_alloc.allocator.allocate = Malloc_allocate;
  mem_alloc.allocator.deallocate = Malloc_deallocate;
  mem_alloc.allocator.report = Malloc_report;

  return &mem_alloc;
}

PUBLIC void Malloc_delete(Malloc * this)
{

}

PUBLIC char * Malloc_allocate(Allocator * this, unsigned int size)
{
  ((Malloc*)this)->nbAllocatedObjects++;
  return (char*)malloc(size);
}

PUBLIC void Malloc_deallocate(Allocator * this, char * ptr)
{
  free(ptr);
  ((Malloc*)this)->nbAllocatedObjects--;
}

PUBLIC void Malloc_report(Allocator * this)
{
  PRINT(("Nb allocated objects: %d\n", ((Malloc*)this)->nbAllocatedObjects));
  //PRINT(("Max nb allocated objects: %d\n", this->maxNbObjectAllocated));
  //PRINT(("Nb alloc request: %d\n", this->allocRequestId));
  //PRINT(("Nb free requests: %d\n", this->freeRequestId));
}