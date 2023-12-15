#include "OptionMgr.h"
#include "String2.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "FileIo.h"
#include <stdio.h>
#include "Map.h"

#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

typedef struct testOptionMgr
{
  Object object;
  Map* options;
} testOptionMgr;

extern OptionMgr* optionMgr;

int step1()
{
  testOptionMgr* testOptionMgr1 = 0;
  testOptionMgr* testOptionMgr2 = 0;
  /* Test 1 */
  testOptionMgr1 = (testOptionMgr*)OptionMgr_getRef();
  testOptionMgr2 = (testOptionMgr*)OptionMgr_getRef();
  PRINT(("Step 1: Test 1 - Check there is only one OptionMgr: "));
  UT_ASSERT((testOptionMgr1 == testOptionMgr1))

  /* Test 2 */
  OptionMgr_delete(testOptionMgr1);
  PRINT(("Step 1: Test 2 - Check ref is not null: "));
  UT_ASSERT((optionMgr!=0))

  /* Test 3 */
  OptionMgr_delete(testOptionMgr2);
  PRINT(("Step 1: Test 3 - Check ref is null: "));
  UT_ASSERT((optionMgr == 0))

  /* Test 4 */
  OptionMgr_delete(testOptionMgr2);
  PRINT(("Step 1: Test 4 - Check delete can be called again: "));
  UT_ASSERT((optionMgr == 0))

  /* Test 5 */
  PRINT(("Step 1: Test 5 - Check all memory is freed properly: "));
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))

  return 0;
}

int step2()
{
  testOptionMgr* testOptionMgr1 = OptionMgr_getRef();
  String * option = 0;

  option = OptionMgr_getOption(testOptionMgr1,"DB Name");
  TRACE(("  DB Name option = %s\n", String_getBuffer(option)));

  option = OptionMgr_getOption(testOptionMgr1, "Input Directory");
  TRACE(("  Input option = %s\n", String_getBuffer(option)));

  OptionMgr_delete(testOptionMgr1);
  PRINT(("Step 2: Test 3 - Check all memory is freed properly: "));
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))

  return 0;
}

int step3()
{
  OptionMgr* testOptionMgr1 = OptionMgr_getRef();
  String * option = String_new("Yes");

  OptionMgr_setOption(testOptionMgr1, "Reset after initialisation", option);

  option = OptionMgr_getOption(testOptionMgr1, "Reset after initialisation");
  printf("  Reset after init. option = %s\n", String_getBuffer(option));
  OptionMgr_delete(testOptionMgr1);

  /* Test 5 */
  printf("Step 3: Test 1 - Check all memory is freed properly: ");
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))

  return 0;
}

int step4()
  {
  const char *argv[] = { "sparse", "-o", "test2.db" };
  int argc = 3;
  String * option = 0;
  OptionMgr* testOptionMgr1 = OptionMgr_getRef();

  OptionMgr_readFromCmdLine(testOptionMgr1, argc, argv);

  option = OptionMgr_getOption(testOptionMgr1,"DB Name");
  printf("  DB Name option = %s\n", String_getBuffer(option));

  /* Test 5 */
  //String_delete(option);
  OptionMgr_delete(testOptionMgr1);
  printf("Step 4: Test 1 - Check all memory is freed properly: ");
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))

  return 0;
}

int step5()
{
  String * option = 0;
  FileIo * f = FileIo_new();
  String* currentWorkingDir = FileIo_getCwd(f);
  OptionMgr* testOptionMgr1 = OptionMgr_getRef();

  printf("  Working directory: %s\n", String_getBuffer(currentWorkingDir));

  OptionMgr_readFromFile(testOptionMgr1);
  option = OptionMgr_getOption(testOptionMgr1,"DB Name");
  printf("  DB Name option = %s\n", String_getBuffer(option));



  /* Test 5 */
  FileIo_delete(f);
  String_delete(currentWorkingDir);
  OptionMgr_delete(testOptionMgr1);
  printf("Step 5: Test 1 - Check all memory is freed properly: ");
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))

  return 0;
}

int step6()
{

}

int main()
{
  step1();
  step2();
  step3();
  step4();
  step5();
  step6();

  ObjectMgr* objMgr = ObjectMgr_getRef();
  ObjectMgr_report(objMgr);
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();
  return 0;
}
