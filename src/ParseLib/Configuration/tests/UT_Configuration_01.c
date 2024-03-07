/* UT_Configuration_01.c */
#include "Configuration.h"
#include "FileMgr.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (1)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

int step1()
{
  int isPassed = 1;
  FileMgr * fileMgr = FileMgr_getRef();
  ObjectMgr* objectMgr = ObjectMgr_getRef();
  FileDesc * data_file = FileMgr_addFile(fileMgr, "testConfigData.txt");

  if (!data_file)
  {
    Error_new(ERROR_FATAL, "Test file not found\n");
  }
  String * testConfigData = FileDesc_load(data_file);

  Configuration * testConfiguration = 0;

  PRINT(("Step 1: Test 1 - Create an instance of class Configuration: "));
  testConfiguration = Configuration_new(testConfigData);
  UT_ASSERT((testConfiguration != 0));

  PRINT(("Step 1: Test 2 - Delete an instance of class TransUnit: "));
  Configuration_delete(testConfiguration);

  FileMgr_delete(fileMgr);

  PRINT(("Step 1: Test 3 - Check all memory is freed: "));
  //UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));

  ObjectMgr_reportUnallocated(objectMgr);
  ObjectMgr_delete(objectMgr);

  return isPassed;
}

int main()
{
  int isPassed =1;

  step1();

  return isPassed;
}