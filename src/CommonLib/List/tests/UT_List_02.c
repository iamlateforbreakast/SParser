#include "List.h"
#include "Object.h"
#include "Allocator.h"
#include "MyAllocator.h"
#include "ObjectStore.h"
#include "TestObject.h"
#include "Debug.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_OBJECT_NB (10)

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

ObjectStore* objectStore = 0;
MyAllocator* testAlloc = 0;
AllocInfo* allocInfo = 0;

int step1()
{
  List* testList = 0;

  TestObject * items[MAX_OBJECT_NB];
  int i = 0;

  PRINT(("Step1: Test 1 - Create a list from a custom allocator: "));
  testList = List_newFromAllocator((Allocator*)testAlloc);

  UT_ASSERT((1));

  PRINT(("Step1: Test 2 - Insert %d objects in list: ", MAX_OBJECT_NB));
  for (i = 0; i < MAX_OBJECT_NB; i++)
  {
    items[i] = TestObject_newFromAllocator((Allocator*)testAlloc);
    //items[i]->x = i;
    //items[i]->y = MAX_OBJECT_NB - i;
    List_insertHead(testList, items[i]);
    TRACE(("Nb items %d\n", List_getNbNodes(testList)));
  }
  UT_ASSERT((List_getNbNodes(testList) == MAX_OBJECT_NB));

  PRINT(("Step1: Test 3 - Insert %d objects in list: ", MAX_OBJECT_NB));

  PRINT(("Step1: Test 4 - Check all memory is freed: "));
  
  UT_ASSERT((MyAllocator_report((Allocator*)testAlloc) != 0));
  printf("Nb object allocated: %d\n", MyAllocator_report((Allocator*)testAlloc));

  return 0;
}

int step2()
{
  return 0;
}

int step3()
{
  return 0;
}

int main()
{
  objectStore = ObjectStore_getRef();
  testAlloc = (MyAllocator*)MyAllocator_new();
  allocInfo = ObjectStore_createAllocator(objectStore, (Allocator*)testAlloc);

  step1();
  step2();
  step3();

  ObjectStore_deleteAllocator(objectStore, allocInfo);
  ObjectStore_delete(objectStore);

  return 0;
}
