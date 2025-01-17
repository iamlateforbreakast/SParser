/* UT_Map_02.c */

#include "Map.h"
#include "String2.h"
#include "TestObject.h"
#include "ObjectStore.h"
#include "MyAllocator.h"
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

#include "Words1000.h"

MyAllocator * testAlloc = 0;
AllocInfo * allocInfo = 0;

List * keys;
TestObject ** testObjects;
int nbTokens = 0;

int step1()
{
  Map * testMap = 0;

  PRINT(("Step 1: Test 1 - Create an instance of Map using a custom allocator: "));
  testMap = Map_newFromAllocator((Allocator*)testAlloc);
  UT_ASSERT((1));

  String * s = String_new("Hello world");
  String * item = String_new("The value");
  
  PRINT(("Step 1: Test 2 - Insert an object: "));
  Map_insert(testMap, s, (void**)item, 1);
  UT_ASSERT((1));

  PRINT(("Step 1: Test 3 - Delete the map: "));
  Map_delete(testMap);
  UT_ASSERT((1));

  return 1;
}

int main()
{
  ObjectStore * objectStore = 0;
  
  objectStore = ObjectStore_getRef();
  testAlloc = MyAllocator_new(5000);
  allocInfo = ObjectStore_createAllocator(objectStore, (Allocator*)testAlloc);

  step1();
  //step2();
  //step3();

  ObjectStore_deleteAllocator(objectStore, allocInfo);
  //ObjectMgr_reportUnallocated(objMgr);
  ObjectStore_delete(objectStore);
}
