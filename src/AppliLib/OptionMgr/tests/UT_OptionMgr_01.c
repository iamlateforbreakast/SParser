#include "OptionMgr.h"
#include "String2.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "FileIo.h"
#include "Map.h"
#include "Debug.h"

#include <stdio.h>
#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

typedef struct testOptionMgr
{
  Object object;
  Map* options;
} testOptionMgr;

//extern OptionMgr* optionMgr;

int UT_OptionMgr_01_step1()
{
  testOptionMgr* testOptionMgr1 = 0;
  testOptionMgr* testOptionMgr2 = 0;
  /* Test 1 */
  testOptionMgr1 = (testOptionMgr*)OptionMgr_getRef();
  testOptionMgr2 = (testOptionMgr*)OptionMgr_getRef();
  PRINT(("Step 1: Test 1 - Check there is only one OptionMgr: "));
  UT_ASSERT((testOptionMgr1 == testOptionMgr1))

  /* Test 2 */
  OptionMgr_delete((OptionMgr*)testOptionMgr1);
  PRINT(("Step 1: Test 2 - Check ref is not null: "));
  UT_ASSERT(0)

  /* Test 3 */
  OptionMgr_delete((OptionMgr*)testOptionMgr2);
  PRINT(("Step 1: Test 3 - Check ref is null: "));
  UT_ASSERT(0)

  /* Test 4 */
  OptionMgr_delete((OptionMgr*)testOptionMgr2);
  PRINT(("Step 1: Test 4 - Check delete can be called again: "));
  UT_ASSERT(0)

  /* Test 5 */
  PRINT(("Step 1: Test 5 - Check all memory is freed properly: "));
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))

  return 0;
}

int UT_OptionMgr_01_step2()
{
  testOptionMgr* testOptionMgr1 = (testOptionMgr*)OptionMgr_getRef();
  const char dbName[] = "test.db";
  const char inputDir[] = ".";
  String * option = 0;

  PRINT(("Step 2: Test 1 - Check option DB Name is read correctly: "));
  option = OptionMgr_getOption((OptionMgr*)testOptionMgr1,"DB Name");
  UT_ASSERT((Memory_cmp(String_getBuffer(option), dbName) == 0));
  TRACE(("  DB Name option = %s\n", String_getBuffer(option)));

  option = OptionMgr_getOption((OptionMgr*)testOptionMgr1, "Input Directory");
  UT_ASSERT((Memory_cmp(String_getBuffer(option), inputDir) == 0));
  TRACE(("  Input option = %s\n", String_getBuffer(option)));

  OptionMgr_delete((OptionMgr*)testOptionMgr1);
  PRINT(("Step 2: Test 3 - Check all memory is freed properly: "));
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))

  return 0;
}

int UT_OptionMgr_01_step3()
{
  OptionMgr* testOptionMgr1 = OptionMgr_getRef();
  String * optionIn = String_new("Yes");
  String* optionOut = 0;

  PRINT(("Step 3: Test 1 - Check a new option can be created and read: "));
  OptionMgr_setOption(testOptionMgr1, "Reset after initialisation", optionIn);

  optionOut = OptionMgr_getOption(testOptionMgr1, "Reset after initialisation");
  UT_ASSERT((Memory_cmp(String_getBuffer(optionIn), String_getBuffer(optionOut)) == 0));
  TRACE(("  Reset after init. option = %s\n", String_getBuffer(optionOut)));
  OptionMgr_delete(testOptionMgr1);

  /* Test 5 */
  PRINT(("Step 3: Test 2 - Check all memory is freed properly: "));
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))

  return 0;
}

int UT_OptionMgr_01_step4()
  {
  const char *argv[] = { "sparse", "-o", "test2.db" };
  int argc = 3;
  String * option = 0;
  OptionMgr* testOptionMgr1 = OptionMgr_getRef();

  PRINT(("Step 4: Test 1 - Check options can be set from commandline: "));
  OptionMgr_readFromCmdLine(testOptionMgr1, argc, argv);

  option = OptionMgr_getOption(testOptionMgr1,"DB Name");
  UT_ASSERT((Memory_cmp(String_getBuffer(option), argv[2]) == 0));
  TRACE(("  DB Name option = %s\n", String_getBuffer(option)));

  /* Test 5 */
  //String_delete(option);
  OptionMgr_delete(testOptionMgr1);
  printf("Step 4: Test 2 - Check all memory is freed properly: ");
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))

  return 0;
}

int UT_OptionMgr_01_step5()
{
  String * option = 0;
  FileIo * f = FileIo_new();
  String* currentWorkingDir = FileIo_getCwd(f);
  OptionMgr* testOptionMgr1 = OptionMgr_getRef();

  PRINT(("Step 5: Test 1 - Check options can be read from file: "));
  printf("  Working directory: %s\n", String_getBuffer(currentWorkingDir));

  OptionMgr_readFromFile(testOptionMgr1);
  option = OptionMgr_getOption(testOptionMgr1,"DB Name");
  printf("  DB Name option = %s\n", String_getBuffer(option));



  /* Test 5 */
  FileIo_delete(f);
  String_delete(currentWorkingDir);
  OptionMgr_delete(testOptionMgr1);
  printf("Step 5: Test 2 - Check all memory is freed properly: ");
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))

  return 0;
}

int UT_OptionMgr_01_step6()
{
  return 0;
}

int run_UT_OptionMgr_01()
{
  int isPassed = 1;

  isPassed = UT_OptionMgr_01_step1() && isPassed;
  isPassed = UT_OptionMgr_01_step2() && isPassed;
  isPassed = UT_OptionMgr_01_step3() && isPassed;
  isPassed = UT_OptionMgr_01_step4() && isPassed;
  isPassed = UT_OptionMgr_01_step5() && isPassed;
  isPassed = UT_OptionMgr_01_step6() && isPassed;

  ObjectMgr* objMgr = ObjectMgr_getRef();
  ObjectMgr_report(objMgr);
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();
  return isPassed;
}
