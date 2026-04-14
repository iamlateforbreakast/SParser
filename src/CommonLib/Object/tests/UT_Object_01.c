/**********************************************//**
  @file UT_Object_01.c

  @brief Unit tests for Object.c

  @details Validates the core lifecycle services provided by the Object
  base class: construction via both allocation paths, validity marking,
  reference counting, copy, compare, print, and deallocation. Also
  verifies that all memory is returned to the ObjectMgr after each step.
**************************************************/

#include "Object.h"
#include "ObjectMgr.h"
#include "ObjectStore.h"
#include "TestObject.h"
#include "Debug.h"
#include "Memory.h"

#include <stdlib.h>
#include <stdio.h>

#define MAX_OBJECT_NB (10)

#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n")); } \
                          else { PRINT(("Failed\n")); return 0; }
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("\x1b[32mPassed\x1b[0m\n")); } \
                          else { PRINT(("\x1b[31mFailed\x1b[0m\n")); return 0; }
#endif

PRIVATE FILE * UT_Object_01_logChannel;

/**********************************************//**
  @brief Step 1 - Construction and validity
  Verifies that Object_new produces a valid, correctly wired object,
  and that Object_deallocate invalidates it and returns memory.
**************************************************/
PRIVATE int UT_Object_01_step1()
{
  int isPassed = 1;
  TestObject * obj = 0;

  PRINT(("Step 1: Test 1 - Create object via Object_new: "));
  obj = TestObject_new();
  isPassed = isPassed && OBJECT_IS_VALID(obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 2 - Validity marker is set: "));
  isPassed = isPassed && Object_isValid((Object*)obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 3 - Class pointer is wired: "));
  isPassed = isPassed && (((Object*)obj)->class != 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 4 - Initial refCount is 1: "));
  isPassed = isPassed && (((Object*)obj)->refCount == 1);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 5 - Delete and deallocate object: "));
  TestObject_delete(obj);
  isPassed = isPassed && OBJECT_IS_INVALID(obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 6 - Object_isValid returns 0 after deallocation: "));
  isPassed = isPassed && !Object_isValid((Object*)obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 7 - All memory returned to ObjectMgr: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  TRACE2((UT_Object_01_logChannel, "Nb objects left: %d\n", ObjectMgr_report(objectMgr)));
  ObjectMgr_delete(objectMgr);
  UT_ASSERT(isPassed);

  return isPassed;
}

/**********************************************//**
  @brief Step 2 - Reference counting
  Verifies that getRef increments refCount, deRef decrements it,
  and that the object remains valid while references are held.
**************************************************/
PRIVATE int UT_Object_01_step2()
{
  int isPassed = 1;
  TestObject * obj = 0;
  Object * ref = 0;

  PRINT(("Step 2: Test 1 - Create object: "));
  obj = TestObject_new();
  isPassed = isPassed && OBJECT_IS_VALID(obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 2 - Object_getRef increments refCount: "));
  ref = Object_getRef((Object*)obj);
  isPassed = isPassed && (((Object*)obj)->refCount == 2);
  isPassed = isPassed && (ref == (Object*)obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 3 - Object remains valid while referenced: "));
  isPassed = isPassed && Object_isValid((Object*)obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 4 - Object_deRef decrements refCount: "));
  Object_deRef((Object*)obj);
  isPassed = isPassed && (((Object*)obj)->refCount == 1);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 5 - Object_getRef on null returns null: "));
  isPassed = isPassed && (Object_getRef(0) == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 6 - Cleanup: "));
  TestObject_delete(obj);
  isPassed = isPassed && OBJECT_IS_INVALID(obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 7 - All memory returned to ObjectMgr: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  ObjectMgr_delete(objectMgr);
  UT_ASSERT(isPassed);

  return isPassed;
}

/**********************************************//**
  @brief Step 3 - Copy and compare
  Verifies that Object_copy produces a distinct but equal object,
  and that Object_comp returns 0 for equal objects.
**************************************************/
PRIVATE int UT_Object_01_step3()
{
  int isPassed = 1;
  TestObject * obj = 0;
  TestObject * copy = 0;

  PRINT(("Step 3: Test 1 - Create object: "));
  obj = TestObject_new();
  isPassed = isPassed && OBJECT_IS_VALID(obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 2 - Object_copy returns a valid distinct object: "));
  copy = (TestObject*)Object_copy((Object*)obj);
  isPassed = isPassed && OBJECT_IS_VALID(copy);
  isPassed = isPassed && (copy != obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 3 - Object_comp returns 0 for equal objects: "));
  isPassed = isPassed && (Object_comp((Object*)obj, (Object*)copy) == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 4 - Object_copy on null returns null: "));
  isPassed = isPassed && (Object_copy(0) == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 5 - Cleanup: "));
  TestObject_delete(obj);
  TestObject_delete(copy);
  isPassed = isPassed && OBJECT_IS_INVALID(obj);
  isPassed = isPassed && OBJECT_IS_INVALID(copy);
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 6 - All memory returned to ObjectMgr: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  ObjectMgr_delete(objectMgr);
  UT_ASSERT(isPassed);

  return isPassed;
}

/**********************************************//**
  @brief Step 4 - Bulk allocation and deallocation
  Allocates MAX_OBJECT_NB objects, verifies all are valid,
  deallocates all, and confirms the ObjectMgr returns to baseline.
**************************************************/
PRIVATE int UT_Object_01_step4()
{
  int isPassed = 1;
  int i = 0;
  TestObject * objects[MAX_OBJECT_NB];

  PRINT(("Step 4: Test 1 - Allocate %d objects: ", MAX_OBJECT_NB));
  for (i = 0; i < MAX_OBJECT_NB; i++)
  {
    objects[i] = TestObject_new();
    TRACE2((UT_Object_01_logChannel, "  Allocated object %d at %p\n", i, objects[i]));
  }
  for (i = 0; i < MAX_OBJECT_NB; i++)
  {
    isPassed = isPassed && OBJECT_IS_VALID(objects[i]);
  }
  UT_ASSERT(isPassed);

  PRINT(("Step 4: Test 2 - ObjectMgr count reflects allocations: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == MAX_OBJECT_NB + 1);
  TRACE2((UT_Object_01_logChannel, "Nb objects allocated: %d\n", ObjectMgr_report(objectMgr)));
  ObjectMgr_delete(objectMgr);
  UT_ASSERT(isPassed);

  PRINT(("Step 4: Test 3 - Deallocate all objects: "));
  for (i = 0; i < MAX_OBJECT_NB; i++)
  {
    TestObject_delete(objects[i]);
    TRACE2((UT_Object_01_logChannel, "  Deallocated object %d\n", i));
  }
  for (i = 0; i < MAX_OBJECT_NB; i++)
  {
    isPassed = isPassed && OBJECT_IS_INVALID(objects[i]);
  }
  UT_ASSERT(isPassed);

  PRINT(("Step 4: Test 4 - All memory returned to ObjectMgr: "));
  objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  ObjectMgr_delete(objectMgr);
  UT_ASSERT(isPassed);

  return isPassed;
}

/**********************************************//**
  @brief Step 5 - Construction via ObjectStore allocator path
  Verifies that Object_newFromAllocator produces a valid object
  with the allocator field correctly set, and that deallocation
  routes back through ObjectStore.
**************************************************/
PRIVATE int UT_Object_01_step5()
{
  int isPassed = 1;
  TestObject * obj = 0;
  ObjectStore * store = 0;
  Allocator * allocator = 0;

  PRINT(("Step 5: Test 1 - Obtain ObjectStore and default allocator: "));
  store = ObjectStore_getRef();
  allocator = ObjectStore_getDefaultAllocator(store);
  isPassed = isPassed && (store != 0) && (allocator != 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 5: Test 2 - Create object via Object_newFromAllocator: "));
  obj = (TestObject*)Object_newFromAllocator(TestObject_getClass(), allocator);
  isPassed = isPassed && OBJECT_IS_VALID(obj);
  UT_ASSERT(isPassed);

  PRINT(("Step 5: Test 3 - Allocator field is set on object: "));
  isPassed = isPassed && (((Object*)obj)->allocator == allocator);
  UT_ASSERT(isPassed);

  PRINT(("Step 5: Test 4 - refCount is 1: "));
  isPassed = isPassed && (((Object*)obj)->refCount == 1);
  UT_ASSERT(isPassed);

  PRINT(("Step 5: Test 5 - Delete and deallocate via ObjectStore path: "));
  TestObject_delete(obj);
  isPassed = isPassed && OBJECT_IS_INVALID(obj);
  UT_ASSERT(isPassed);

  ObjectStore_delete(store);

  return isPassed;
}

#ifdef MAIN
int main()
#else
int run_UT_Object_01()
#endif
{
  int isPassed = 1;

  UT_Object_01_logChannel = Debug_openChannel("UT_Object_01.log");
  Debug_setStdoutChannel(UT_Object_01_logChannel);

  PRINT(("\n=== UT_Object_01: Object lifecycle ===\n"));

  isPassed = UT_Object_01_step1() && isPassed;
  isPassed = UT_Object_01_step2() && isPassed;
  isPassed = UT_Object_01_step3() && isPassed;
  isPassed = UT_Object_01_step4() && isPassed;
  isPassed = UT_Object_01_step5() && isPassed;

  PRINT(("\n=== Final memory check ===\n"));
  ObjectMgr * objMgr = ObjectMgr_getRef();
  ObjectMgr_reportUnallocated(objMgr);
  ObjectMgr_delete(objMgr);
  Memory_report();

  Debug_closeChannel(UT_Object_01_logChannel);

  return isPassed;
}
