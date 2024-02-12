/* UT_GrammarC99_01.c */
#include "GrammarC99.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)
#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("\x1b[32mPassed\x1b[0m\n");} \
                          else { printf("\x1b[31mFailed\x1b[0m\n"); return 0;}
#endif

int step1()
{
  int isPassed = 1;
  GrammarC99* testC99 = 0;

  PRINT(("Step 1: Test 1 - Create an instance of class GrammarC99: "));
  testC99 = (GrammarC99*)GrammarC99_new();
  UT_ASSERT((testC99 != 0));

  PRINT(("Step 1: Test 2 - Delete an instance of class GrammarC99: "));
  GrammarC99_delete(testC99);
  UT_ASSERT((1));

  PRINT(("Step 1: Test 3 - Check all memory is freed: "));
  ObjectMgr* objectMgr = ObjectMgr_getRef();
  UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));
  ObjectMgr_delete(objectMgr);

  return isPassed;
}

int main()
{
  int isPassed = 1;

  step1();
  
  return isPassed;
}
