/**********************************************//**
  @file UT_CharBuffer_01.c

  @brief Unit tests for CharBuffer.c

  @details Covers construction via both allocation paths, append
  (char and string), automatic doubling growth, clear, copy,
  compare, print, and full memory recovery verification after
  each step.
**************************************************/

#include "CharBuffer.h"
#include "Object.h"
#include "ObjectMgr.h"
#include "ObjectStore.h"
#include "Debug.h"
#include "Memory.h"

#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n")); } \
                          else { PRINT(("Failed\n")); return 0; }
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("\x1b[32mPassed\x1b[0m\n")); } \
                          else { PRINT(("\x1b[31mFailed\x1b[0m\n")); return 0; }
#endif

PRIVATE FILE * UT_CharBuffer_01_logChannel;

/*-------- Step 1: Construction and basic validity -------------------*/
PRIVATE int UT_CharBuffer_01_step1()
{
  int isPassed = 1;
  CharBuffer * cb = 0;

  PRINT(("Step 1: Test 1 - Create CharBuffer via CharBuffer_new: "));
  cb = CharBuffer_new();
  isPassed = isPassed && OBJECT_IS_VALID(cb);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 2 - Initial length is 0: "));
  isPassed = isPassed && (CharBuffer_getLength(cb) == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 3 - Initial capacity is CHARBUFFER_DEFAULT_SIZE: "));
  isPassed = isPassed && (CharBuffer_getCapacity(cb) == CHARBUFFER_DEFAULT_SIZE);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 4 - Initial data is empty string: "));
  isPassed = isPassed && (strcmp(CharBuffer_getData(cb), "") == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 5 - Delete CharBuffer: "));
  CharBuffer_delete(cb);
  isPassed = isPassed && OBJECT_IS_INVALID(cb);
  UT_ASSERT(isPassed);

  PRINT(("Step 1: Test 6 - All memory returned to ObjectMgr: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  ObjectMgr_delete(objectMgr);
  UT_ASSERT(isPassed);

  return isPassed;
}

/*-------- Step 2: Append char and string ----------------------------*/
PRIVATE int UT_CharBuffer_01_step2()
{
  int isPassed = 1;
  CharBuffer * cb = 0;

  PRINT(("Step 2: Test 1 - Create CharBuffer: "));
  cb = CharBuffer_new();
  isPassed = isPassed && OBJECT_IS_VALID(cb);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 2 - Append single char: "));
  isPassed = isPassed && CharBuffer_appendChar(cb, 'A');
  isPassed = isPassed && (CharBuffer_getLength(cb) == 1);
  isPassed = isPassed && (strcmp(CharBuffer_getData(cb), "A") == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 3 - Append string: "));
  isPassed = isPassed && CharBuffer_appendString(cb, "hello");
  isPassed = isPassed && (CharBuffer_getLength(cb) == 6);
  isPassed = isPassed && (strcmp(CharBuffer_getData(cb), "Ahello") == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 4 - Append null string returns 0: "));
  isPassed = isPassed && (CharBuffer_appendString(cb, 0) == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 5 - Clear resets length to 0: "));
  CharBuffer_clear(cb);
  isPassed = isPassed && (CharBuffer_getLength(cb) == 0);
  isPassed = isPassed && (strcmp(CharBuffer_getData(cb), "") == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 6 - Capacity unchanged after clear: "));
  isPassed = isPassed && (CharBuffer_getCapacity(cb) == CHARBUFFER_DEFAULT_SIZE);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 7 - Cleanup: "));
  CharBuffer_delete(cb);
  isPassed = isPassed && OBJECT_IS_INVALID(cb);
  UT_ASSERT(isPassed);

  PRINT(("Step 2: Test 8 - All memory returned to ObjectMgr: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  ObjectMgr_delete(objectMgr);
  UT_ASSERT(isPassed);

  return isPassed;
}

/*-------- Step 3: Automatic doubling growth -------------------------*/
PRIVATE int UT_CharBuffer_01_step3()
{
  int isPassed = 1;
  CharBuffer * cb = 0;
  unsigned int i = 0;

  PRINT(("Step 3: Test 1 - Create CharBuffer: "));
  cb = CharBuffer_new();
  isPassed = isPassed && OBJECT_IS_VALID(cb);
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 2 - Fill buffer past default capacity: "));
  for (i = 0; i < CHARBUFFER_DEFAULT_SIZE; i++)
  {
    isPassed = isPassed && CharBuffer_appendChar(cb, 'x');
  }
  isPassed = isPassed && (CharBuffer_getLength(cb) == CHARBUFFER_DEFAULT_SIZE);
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 3 - Capacity has doubled: "));
  isPassed = isPassed && (CharBuffer_getCapacity(cb) == CHARBUFFER_DEFAULT_SIZE * 2);
  TRACE2((UT_CharBuffer_01_logChannel, "Capacity after growth: %u\n", CharBuffer_getCapacity(cb)));
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 4 - Contents are intact after growth: "));
  const char * data = CharBuffer_getData(cb);
  int contentsOk = 1;
  for (i = 0; i < CHARBUFFER_DEFAULT_SIZE; i++)
  {
    contentsOk = contentsOk && (data[i] == 'x');
  }
  isPassed = isPassed && contentsOk;
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 5 - Append string triggering second growth: "));
  /* Fill to just short of current capacity then append a string */
  CharBuffer_clear(cb);
  for (i = 0; i < CHARBUFFER_DEFAULT_SIZE * 2 - 1; i++)
  {
    CharBuffer_appendChar(cb, 'y');
  }
  isPassed = isPassed && CharBuffer_appendString(cb, "overflow");
  isPassed = isPassed && (CharBuffer_getCapacity(cb) >= CHARBUFFER_DEFAULT_SIZE * 2);
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 6 - Cleanup: "));
  CharBuffer_delete(cb);
  isPassed = isPassed && OBJECT_IS_INVALID(cb);
  UT_ASSERT(isPassed);

  PRINT(("Step 3: Test 7 - All memory returned to ObjectMgr: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  ObjectMgr_delete(objectMgr);
  UT_ASSERT(isPassed);

  return isPassed;
}

/*-------- Step 4: Copy and compare ----------------------------------*/
PRIVATE int UT_CharBuffer_01_step4()
{
  int isPassed = 1;
  CharBuffer * cb1 = 0;
  CharBuffer * cb2 = 0;
  CharBuffer * copy = 0;

  PRINT(("Step 4: Test 1 - Create two CharBuffers with same content: "));
  cb1 = CharBuffer_new();
  cb2 = CharBuffer_new();
  CharBuffer_appendString(cb1, "hello");
  CharBuffer_appendString(cb2, "hello");
  isPassed = isPassed && OBJECT_IS_VALID(cb1) && OBJECT_IS_VALID(cb2);
  UT_ASSERT(isPassed);

  PRINT(("Step 4: Test 2 - Compare equal buffers returns 0: "));
  isPassed = isPassed && (CharBuffer_compare(cb1, cb2) == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 4: Test 3 - Compare different buffers returns non-zero: "));
  CharBuffer_appendChar(cb2, 'z');
  isPassed = isPassed && (CharBuffer_compare(cb1, cb2) != 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 4: Test 4 - Copy produces valid distinct object: "));
  copy = CharBuffer_copy(cb1);
  isPassed = isPassed && OBJECT_IS_VALID(copy);
  isPassed = isPassed && (copy != cb1);
  UT_ASSERT(isPassed);

  PRINT(("Step 4: Test 5 - Copy contents match original: "));
  isPassed = isPassed && (strcmp(CharBuffer_getData(copy), CharBuffer_getData(cb1)) == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 4: Test 6 - Copy is independent (modifying copy does not affect original): "));
  CharBuffer_appendChar(copy, '!');
  isPassed = isPassed && (strcmp(CharBuffer_getData(cb1), "hello") == 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 4: Test 7 - Cleanup: "));
  CharBuffer_delete(cb1);
  CharBuffer_delete(cb2);
  CharBuffer_delete(copy);
  UT_ASSERT(isPassed);

  PRINT(("Step 4: Test 8 - All memory returned to ObjectMgr: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  ObjectMgr_delete(objectMgr);
  UT_ASSERT(isPassed);

  return isPassed;
}

/*-------- Step 5: ObjectStore allocator path ------------------------*/
PRIVATE int UT_CharBuffer_01_step5()
{
  int isPassed = 1;
  CharBuffer * cb = 0;
  ObjectStore * store = 0;
  Allocator * allocator = 0;

  PRINT(("Step 5: Test 1 - Obtain ObjectStore default allocator: "));
  store = ObjectStore_getRef();
  allocator = ObjectStore_getDefaultAllocator(store);
  isPassed = isPassed && (store != 0) && (allocator != 0);
  UT_ASSERT(isPassed);

  PRINT(("Step 5: Test 2 - Create CharBuffer via ObjectStore path: "));
  cb = CharBuffer_newFromAllocator(allocator);
  isPassed = isPassed && OBJECT_IS_VALID(cb);
  UT_ASSERT(isPassed);

  PRINT(("Step 5: Test 3 - Allocator field is set on object: "));
  isPassed = isPassed && (((Object*)cb)->allocator == allocator);
  UT_ASSERT(isPassed);

  PRINT(("Step 5: Test 4 - Append and grow work on ObjectStore-backed buffer: "));
  isPassed = isPassed && CharBuffer_appendString(cb, "from allocator");
  isPassed = isPassed && (CharBuffer_getLength(cb) == 14);
  UT_ASSERT(isPassed);

  PRINT(("Step 5: Test 5 - Cleanup via ObjectStore path: "));
  CharBuffer_delete(cb);
  isPassed = isPassed && OBJECT_IS_INVALID(cb);
  UT_ASSERT(isPassed);

  ObjectStore_delete(store);

  return isPassed;
}

/*-------- Runner ----------------------------------------------------*/
#ifdef MAIN
int main()
#else
int run_UT_CharBuffer_01()
#endif
{
  int isPassed = 1;

  UT_CharBuffer_01_logChannel = Debug_openChannel("UT_CharBuffer_01.log");
  Debug_setStdoutChannel(UT_CharBuffer_01_logChannel);

  PRINT(("\n=== UT_CharBuffer_01: CharBuffer lifecycle ===\n"));

  isPassed = UT_CharBuffer_01_step1() && isPassed;
  isPassed = UT_CharBuffer_01_step2() && isPassed;
  isPassed = UT_CharBuffer_01_step3() && isPassed;
  isPassed = UT_CharBuffer_01_step4() && isPassed;
  isPassed = UT_CharBuffer_01_step5() && isPassed;

  PRINT(("\n=== Final memory check ===\n"));
  ObjectMgr * objMgr = ObjectMgr_getRef();
  ObjectMgr_reportUnallocated(objMgr);
  ObjectMgr_delete(objMgr);
  Memory_report();

  Debug_closeChannel(UT_CharBuffer_01_logChannel);

  return isPassed;
}