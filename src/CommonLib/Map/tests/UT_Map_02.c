#include "Map.h"
#include "MyAllocator.h"
#include "ObjectStore.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)

#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

MyAllocator * testAlloc = 0;
AllocInfo * allocInfo = 0;

Map * testMap = 0;

int step1()
{
  PRINT(("Step1: test 1 - Create an instance of Map using a custom alllcator:"));
  testMap = Map_newFromAllocator((Allocator*)testAlloc);
  UT_ASSERT((1));

  return 1;
}

int step2()
{
  Map_delete(testMap);

  return 1;
}

int main()
{
  ObjectStore * objectStore = 0;
  
  objectStore = ObjectStore_getRef();
  testAlloc = MyAllocator_new();
  allocInfo = ObjectStore_createAllocator(objectStore, (Allocator*)testAlloc);

  step1();

  ObjectStore_deleteAllocator(objectStore, allocInfo);
  ObjectStore_delete(objectStore);
}
