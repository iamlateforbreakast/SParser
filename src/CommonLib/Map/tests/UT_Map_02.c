#include "Map.h"
#include "MyAllocator.h"
#include "ObjectStore.h"
/* UT_Map_02.c */

#include "Map.h"
#include "String2.h"
#include "TestObject.h"
#include "ObjectMgr.h"
#include "Handle.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

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

extern char words1000[];

MyAllocator* UT_Map_02_testAlloc = 0;
AllocInfo* UT_Map_02_allocInfo = 0;
List* UT_Map_02_keys;
TestObject** UT_Map_02_testObjects;
int UT_Map_02_nbTokens = 0;
PRIVATE FILE* UT_Map_02_logChannel = 0;

int UT_Map_02_init_keys()
{
  String* fullText = String_newByRef(words1000);

  UT_Map_02_keys = String_splitToken(fullText, " ");
  UT_Map_02_nbTokens = List_getNbNodes(UT_Map_02_keys);

  UT_Map_02_testObjects = (TestObject**)Memory_alloc(UT_Map_02_nbTokens * sizeof(TestObject*));

  for (int i = 0; i < UT_Map_02_nbTokens; i++)
  {
    UT_Map_02_testObjects[i] = TestObject_new();
  }

  String_delete(fullText);

  return 1;
}

int UT_Map_02_delete_keys(int isOwner)
{
  if (isOwner)
  {
    for (int i = 0; i < UT_Map_02_nbTokens; i++)
    {
      TestObject_delete(UT_Map_02_testObjects[i]);
    }
  }
  List_delete(UT_Map_02_keys);
  Memory_free(UT_Map_02_testObjects, UT_Map_02_nbTokens * sizeof(TestObject*));

  return 1;
}

int UT_Map_02_step1()
{
  int isPassed = 1;
  Map* testMap = 0;

  PRINT(("Step 1: Test 1 - Create an instance of Map using a custom allocator: "));
  testMap = Map_newFromAllocator((Allocator*)UT_Map_02_testAlloc);

  isPassed = isPassed && OBJECT_IS_VALID(testMap);

  UT_ASSERT((isPassed));

  PRINT(("Step 1: Test 2 - Delete the Map: "));
  Map_delete(testMap);

  isPassed = isPassed && OBJECT_IS_INVALID(testMap);

  UT_ASSERT((isPassed));

  return isPassed;
}

int UT_Map_02_step2()
{
  int isPassed = 1;
  Map* testMap = Map_newFromAllocator((Allocator*)UT_Map_02_testAlloc);

  PRINT(("Step 2: Test 1 - Insert one object: "));
  String* key = (String*)List_getNext(UT_Map_02_keys);
  Handle* hKey = Handle_new(key, 0);
  Handle* hItem = Handle_new(UT_Map_02_testObjects[0], 0);
  int isOk = Map_insert(testMap, hKey, hItem);
  isPassed = isPassed && isOk;

  UT_ASSERT((isPassed));

  PRINT(("Step 2: Test 2 - Find inserted object: "));
  void* foundItem = 0;
  int isFound = Map_find(testMap, key, &foundItem);
  isPassed = isPassed && isFound;

  UT_ASSERT((isPassed));

  PRINT(("Step 2: Test 3 - Reject same object insertion: "));
  int isOK = !Map_insert(testMap, hKey, hItem);
  isPassed = isPassed && isOK;

  UT_ASSERT((isPassed));

  Map_delete(testMap);

  return isPassed;
}

int UT_Map_02_step3()
{
  Map* testMap = 0;

  PRINT(("Step 3: Test 1 - Delete the map: "));
  Map_delete(testMap);

  UT_ASSERT((1));

  return 1;
}

#ifdef MAIN
int main()
#else
int run_UT_Map_02()
#endif
{
  int isPassed = 1;
  UT_Map_02_logChannel = Debug_openChannel("UT_Map_02.log");
  Debug_setStdoutChannel(UT_Map_02_logChannel);
  ObjectMgr* objMgr = ObjectMgr_getRef();
  UT_Map_02_init_keys();
  ObjectStore* objectStore = 0;

  objectStore = ObjectStore_getRef();
  UT_Map_02_testAlloc = (MyAllocator*)MyAllocator_new(10000);
  UT_Map_02_allocInfo = ObjectStore_createAllocator(objectStore, (Allocator*)UT_Map_02_testAlloc);

  isPassed = UT_Map_02_step1() && isPassed;
  isPassed = UT_Map_02_step2() && isPassed;
  //isPassed = UT_Map_02_step3() && isPassed;

  UT_Map_02_delete_keys(1);

  ObjectStore_deleteAllocator(objectStore, UT_Map_02_allocInfo);
  ObjectStore_delete(objectStore);

  Memory_report();

  return isPassed;
}
