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
  Configuration* testConfiguration = 0;
  FileMgr * fileMgr = FileMgr_getRef();
  ObjectMgr* objectMgr = ObjectMgr_getRef();
  FileDesc * dataFile = FileMgr_addFile(fileMgr, "testConfigData.txt");

  if (dataFile == 0) Error_new(ERROR_FATAL, "Cannot open test data file\n");

  String* data = FileDesc_load(dataFile);

  if (!data) Error_new(ERROR_FATAL, "Cannot read the data\n");

  PRINT(("Step 1: Test 1 - Create an instance of class Configuration: "));
  testConfiguration = Configuration_new(data);
  UT_ASSERT((testConfiguration != 0));

  PRINT(("Step 1: Test 2 - Delete an instance of class Configuration: "));
  Configuration_delete(testConfiguration);

  FileMgr_delete(fileMgr);
  String_delete(data);

  PRINT(("Step 1: Test 3 - Check all memory is freed: "));
  UT_ASSERT((ObjectMgr_report(objectMgr) == 1));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));

  ObjectMgr_reportUnallocated(objectMgr);
  ObjectMgr_delete(objectMgr);

  return isPassed; 
}
int step2()
{
  int isPassed = 1;
  return isPassed;
}

int main()
{
  int isPassed =1;

  step1();

  Memory_report();
  return isPassed;
}
