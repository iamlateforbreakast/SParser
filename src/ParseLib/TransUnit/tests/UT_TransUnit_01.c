/* UT_TransUnit_01.c */
#include "TransUnit.h"
#include "FileMgr.h"
#include "ObjectMgr.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

int step1()
{
  int isPassed = 1;
  TransUnit* testTransUnit = 0;
  FileDesc* testFile = FileDesc_new();

  PRINT(("Step 1: Test 1 - Create an instance of class TransUnit: "));
  testTransUnit = TransUnit_new(0);
  UT_ASSERT((testTransUnit != 0));

  PRINT(("Step 1: Test 2 - Delete an instance of class TransUnit: "));
  TransUnit_delete(testTransUnit);

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
