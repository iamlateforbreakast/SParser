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
  PRINT(("Step 1: Test 1 - Create an instance of Map using a custom allocator: "));
  testMap = Map_newFromAllocator((Allocator*)testAlloc);
  UT_ASSERT((1));

  return 1;
}

int step2()
{
  String * s = String_new("Hello world");
  String * item = String_new("The value");
  
  PRINT(("Step 2: Test 1 - Insert a string item: "));
  Map_insert(testMap, s, (void**)item);
  UT_ASSERT((1));

  return 1;
}

int step3()
{
  PRINT(("Step 3: Test 1 - Delete the map: "));
  Map_delete(testMap);
  UT_ASSERT((1));

  return 1;
}

int main()
{
  ObjectStore * objectStore = 0;
  
  objectStore = ObjectStore_getRef();
  testAlloc = MyAllocator_new();
  allocInfo = ObjectStore_createAllocator(objectStore, (Allocator*)testAlloc);

  step1();
  step2();
  step3();

  ObjectStore_deleteAllocator(objectStore, allocInfo);
  ObjectStore_delete(objectStore);
}
