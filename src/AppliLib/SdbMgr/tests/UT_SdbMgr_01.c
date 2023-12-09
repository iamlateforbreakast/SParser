#include "SdbMgr.h"
#include "SdbRequest.h"
#include "Object.h"
#include "Memory.h"

#include <stdio.h>

typedef struct TestSdbMgr
{
  Object object;
} TestSdbMgr;

SdbMgr * testSdbMgr = 0;

int step1()
{
  String * sdbName = String_new("test.sql");

  testSdbMgr = SdbMgr_new(sdbName);

  String_delete(sdbName);

  return 0;
}

int step2()
{
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
              
  SdbRequest_execute(dropTable);
  SdbRequest_execute(createTable);
  
  SdbRequest_delete(createTable);
  SdbRequest_delete(dropTable);

  return 0;
}

int step3()
{
  SdbRequest * request = 0;
  unsigned int i;

  request = SdbRequest_new(
            "INSERT INTO Test_table(Test_text) "
            "VALUES ('Value %d');");
  
  for (i=0; i<20; i++)
  {
              
    SdbRequest_execute(request, i);
  }
  
  SdbRequest_delete(request);
  
  return 0;
}

int step4()
{
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

  return 0;
}

int step5()
{
  SdbMgr_delete(testSdbMgr);

  Memory_report();
  
  return 0;
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
