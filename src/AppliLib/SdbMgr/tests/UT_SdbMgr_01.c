#include "SdbMgr.h"
#include "SdbRequest.h"
#include "Object.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}

typedef struct TestSdbMgr
{
  Object object;
} TestSdbMgr;

SdbMgr * testSdbMgr = 0;
String * sdbName = 0;
FILE * UT_SdbMgr_01_logChannel = 0;

int UT_SdbMgr_01_step1()
{
  int isPassed = 1;

  PRINT(("Step 1: Test 1 - Create an instance of class SdbMgr: "));
  sdbName = String_new("test.sql");

  testSdbMgr = SdbMgr_new(sdbName);
  
  isPassed = OBJECT_IS_VALID(testSdbMgr) && isPassed;

  UT_ASSERT((isPassed));

  return isPassed;
}

int UT_SdbMgr_01_step2()
{
  int isPassed = 1;

  SdbRequest * createTable = 0;
  SdbRequest * dropTable = 0;
  SdbRequest * checkTable = 0;

  dropTable = SdbRequest_new(
              "DROP TABLE "
              "IF EXISTS Test_table;"
              );

  createTable = SdbRequest_new(
              "CREATE TABLE Test_table ( "
              "Test_text text NOT NULL "
              ");");

  checkTable = SdbRequest_new(
              "SELECT name FROM test.sql "
              "WHERE type='table' AND name='{Test_table}';"
              );

  PRINT(("Step 2: Test 1 - Create an instance of class SdbMgr: "));
              
  SdbRequest_execute(dropTable);
  SdbRequest_execute(createTable);
  SdbRequest_execute(checkTable);

  if (SdbRequest_getNbResult(checkTable)!=0)
  {
    printf("Found a record!\n");
  }

  SdbRequest_delete(createTable);
  SdbRequest_delete(dropTable);
  SdbRequest_delete(checkTable);

  return isPassed;
}

int UT_SdbMgr_01_step3()
{
  int isPassed = 1;
  SdbRequest * request = 0;
  unsigned int i;

  request = SdbRequest_new(
            "INSERT INTO Test_table(Test_text) "
            "VALUES ('Value %d');");
  
  for (i=0; i<20; i++)
  {
              
    SdbRequest_execute(request, i);
  }
  
  PRINT(("Step 3: Test 1 - Create an instance of class SdbMgr: "));
  SdbRequest_delete(request);
  
  return isPassed;
}

int UT_SdbMgr_01_step4()
{
  int isPassed = 1;
  SdbRequest * request = 0;
  List * result = 0;
  String * s = 0;
  
  request = SdbRequest_new(
              "SELECT * "
              "FROM Test_table "
              "WHERE Test_text='%s';");
  
  SdbRequest_execute(request, "Value 1");

  if (SdbRequest_getNbResult(request)!=0)
  {
    PRINT(("Found a record!\n"));
    result = SdbRequest_getResults(request);
    s = (String*)List_getNext(result);
    PRINT(("Record: %s\n",String_getBuffer(s)));
  }
  SdbRequest_delete(request);

  return isPassed;
}

int UT_SdbMgr_01_step5()
{
  int isPassed = 1;
  SdbMgr_delete(testSdbMgr);

  String_delete(sdbName);
  Memory_report();
  
  return isPassed;
}

int run_UT_SdbMgr_01()
{
  int isPassed = 1;

  UT_SdbMgr_01_logChannel = Debug_openChannel("UT_SdbMgr_01.log");
  Debug_setStdoutChannel(UT_SdbMgr_01_logChannel);

  isPassed = UT_SdbMgr_01_step1() && isPassed;
  isPassed = UT_SdbMgr_01_step2() && isPassed;
  isPassed = UT_SdbMgr_01_step3() && isPassed;
  isPassed = UT_SdbMgr_01_step4() && isPassed;
  isPassed = UT_SdbMgr_01_step5() && isPassed;
  
  Debug_closeChannel(UT_SdbMgr_01_logChannel);

  return isPassed;
}
