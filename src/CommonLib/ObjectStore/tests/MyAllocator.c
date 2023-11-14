#include "MyAllocator.h"

struct MyAllocator
{
  Allocator allocator;
  char * memory;
};

MyAllocator * MyAllocator_new()
{
  
  Allocator_setAllocateFunction(&MyAllocator_allocate);
  Allocator_setDeAllocateFunction(&MyAllocator_deallocate);
}

void MyAllocator_delete(MyAllocator * this)
{
  
}

char * MyAllocator_allocate(Allocator * allocator, unsigned int size)
{
    return 0;
}

void MyAllocator_deallocate(Allocator * allocator, char * ptr)
{
  free(ptr);
}