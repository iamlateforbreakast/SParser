#include "SdbMgr.h"
#include "SdbRequest.h"
#include "Object.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (1)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}
typedef struct TestSdbMgr
{
  Object object;
} TestSdbMgr;

SdbMgr * testSdbMgr = 0;
String * sdbName = 0;

int step1()
{
  int isPassed = 1;
  PRINT(("Step 1: Test 1 - Create an instance of class SdbMgr: "));
  sdbName = String_new("test.sql");

  testSdbMgr = SdbMgr_new(sdbName);

  UT_ASSERT((testSdbMgr));

  return isPassed;
}

int step2()
{
  int isPassed = 1;
  SdbRequest * createTable = 0;
  SdbRequest * dropTable = 0;

  dropTable = SdbRequest_new(
              "DROP TABLE "
              "IF EXISTS Test_table;"
              );

  createTable = SdbRequest_new(
              "CREATE TABLE Test_table ( "
              "Test_text text NOT NULL "
              ");");
  PRINT(("Step 2: Test 1 - Create an instance of class SdbMgr: "));
              
  SdbRequest_execute(dropTable);
  SdbRequest_execute(createTable);
  
  SdbRequest_delete(createTable);
  SdbRequest_delete(dropTable);

  return isPassed;
}

int step3()
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

int step4()
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
    printf("Found a record!\n");
    result = SdbRequest_getResults(request);
    s = (String*)List_getNext(result);
    printf("Record: %s\n",String_getBuffer(s));
  }
  SdbRequest_delete(request);

  return isPassed;
}

int step5()
{
  int isPassed = 1;
  SdbMgr_delete(testSdbMgr);

  String_delete(sdbName);
  Memory_report();
  
  return isPassed;
}

int main()
{
  step1();
  step2();
  step3();
  step4();
  step5();
  
  return 0;
}
