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

  return 0;
}

int step2()
{
  SdbRequest * request = 0;

  request = SdbRequest_new(
              "CREATE TABLE Test_table ( "
              "Test_text text NOT NULL "
              ");");
              
  SdbRequest_execute(request);
  
  SdbRequest_delete(request);
  
  return 0;
}

int step3()
{
  SdbRequest * request = 0;
  unsigned int i;
  
  for (i=0; i<20; i++)
  {
    request = SdbRequest_new(
              "INSERT INTO Test_table(Test_text) "
              "VALUES ('Value %d');");
              
    SdbRequest_execute(request, i);
  }
  
  SdbRequest_delete(request);
  
  return 0;
}

int step4()
{
  SdbRequest * request = 0;
  
  request = SdbRequest_new(
              "SELECT * "
              "FROM Test_table "
              "WHERE Test_text=%s");
  
  SdbRequest_execute(request, "Value 1");
  
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
