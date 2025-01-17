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
#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("\x1b[32mPassed\x1b[0m\n"));} \
                          else { PRINT(("\x1b[31mFailed\x1b[0m\n")); return 0;}
#endif

ObjectStore* objectStore = 0;
MyAllocator* testAlloc = 0;
AllocInfo* UT_List_02_allocInfo = 0;

TestObject * items[MAX_OBJECT_NB];
TestObject * removed[MAX_OBJECT_NB];
FILE* channelLog = 0;
int init_testobjects()
{
  for (int i = 0; i < MAX_OBJECT_NB; i++)
  {
    items[i] = TestObject_newFromAllocator((Allocator*)testAlloc);
  }

  return 1;
}

int delete_testobjects()
{
  for (int i = 0; i < MAX_OBJECT_NB; i++)
  {
    TestObject_delete(items[i]);
  }
  return 1;
}

PRIVATE int UT_List_02_step1()
{
  List* testList = 0;

  int i = 0;

  PRINT(("Step 1: Test 1 - Create a list from a custom allocator: "));
  testList = List_newFromAllocator((Allocator*)testAlloc);

  UT_ASSERT((testList!=0));

  PRINT(("Step 1: Test 2 - Insert %d objects in list: ", MAX_OBJECT_NB));
  for (i = 0; i < MAX_OBJECT_NB; i++)
  {
    
    List_insertHead(testList, items[i], 1);
    TRACE(("  Nb items %d\n", List_getNbNodes(testList)));
    TRACE(("  Allocated %d bytes at %x\n", ((Object*)items[i])->class->f_size(0), items[i]));
  }
  UT_ASSERT((List_getNbNodes(testList) == MAX_OBJECT_NB));

  PRINT(("Step 1: Test 3 - Remove %d objects from list: ", MAX_OBJECT_NB));
  for (i = 0; i< MAX_OBJECT_NB; i++)
  {
    removed[i] = List_removeHead(testList);
    TRACE(("  Removed %d bytes at %x\n", ((Object*)removed[i])->class->f_size(0), removed[i]));
  }
  UT_ASSERT((1));
  
  PRINT(("Nb objects left allocated in custom allocator: %d\n", MyAllocator_report((Allocator*)testAlloc)));
  PRINT(("Step 1: test 4 - Delete List from allocator: "));
  List_delete(testList);
  UT_ASSERT((1));
  
  PRINT(("Step 1: Test 5 - Check all memory is freed: "));
  UT_ASSERT((MyAllocator_report((Allocator*)testAlloc) != 1));
  PRINT(("Nb objects left allocated in custom allocator: %d\n", MyAllocator_report((Allocator*)testAlloc)));

  return 0;
}

PRIVATE int UT_List_02_step2()
{
  List* testList = 0;

  int i = 0;

  PRINT(("Step 2: Test 1 - Create a list from a custom allocator: "));
  testList = List_newFromAllocator((Allocator*)testAlloc);

  UT_ASSERT((1));

  PRINT(("Step 2: Test 2 - Insert %d objects in list: ", MAX_OBJECT_NB));
  for (i = 0; i < MAX_OBJECT_NB; i++)
  {
    List_insertTail(testList, items[i], 1);
    TRACE(("  Nb items %d\n", List_getNbNodes(testList)));
    TRACE(("  Allocated %d bytes at %x\n", ((Object*)items[i])->class->f_size(0), items[i]));
  }
  UT_ASSERT((List_getNbNodes(testList) == MAX_OBJECT_NB));

  PRINT(("Step 2: Test 3 - Remove %d objects from list: ", MAX_OBJECT_NB));
  for (i = 0; i< MAX_OBJECT_NB; i++)
  {
    removed[i] = List_removeTail(testList);
    TRACE(("  Removed %d bytes at %x\n", ((Object*)removed[i])->class->f_size(0), removed[i]));
    TestObject_delete(removed[i]);
  }
  UT_ASSERT((1));
  
  PRINT(("Step 2: test 4 - Delete List from allocator: "));
  List_delete(testList);
  UT_ASSERT((1));

  PRINT(("Step 2: Test 5 - Check all memory is freed: "));
  UT_ASSERT((MyAllocator_report((Allocator*)testAlloc) != 1));
  PRINT(("Nb objects left allocated in custom allocator: %d\n", MyAllocator_report((Allocator*)testAlloc)));

  return 0;
}

PRIVATE int UT_List_02_step3()
{
  return 0;
}

PUBLIC int run_UT_List_02()
{
  int isPassed = 1;

  channelLog = Debug_openChannel("UT_List_01.log");
  Debug_setStdoutChannel(channelLog);
  objectStore = ObjectStore_getRef();
  testAlloc = (MyAllocator*)MyAllocator_new(5000);
  UT_List_02_allocInfo = ObjectStore_createAllocator(objectStore, (Allocator*)testAlloc);

  isPassed = isPassed && UT_List_02_step1();
  isPassed = isPassed && UT_List_02_step2();
  isPassed = isPassed && UT_List_02_step3();

  ObjectStore_deleteAllocator(objectStore, UT_List_02_allocInfo);
  ObjectStore_delete(objectStore);

  Debug_closeChannel(channelLog);
  return isPassed;
}
