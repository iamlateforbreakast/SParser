#include "ObjectMgr.h"
#include "Object.h"
#include "String2.h"

int step1()
{
  String * testString = 0;

  testString = String_new("Hello World!");

  String_delete(testString);

  return 0;
}

int step2()
{
  String * testString = 0;
  String * testPattern = 0;

  
  testString = String_new("Hello World!");
  testPattern = String_new("World");

  if (String_isContained(testString, testPattern))
  {
    printf("Passed\n");
  }

  String_delete(testString);

  return 0;
}

int step3()
{
  String * testString = 0;
  const char * testPattern = "*.c";

  testString = String_new("test.c");
  
  if (String_matchWildcard(testString, testPattern))
  {
    printf("Passed\n");
  }
  
  String_delete(testString);
  
  return 0;
}

int main()
{
  step1();
  step2();
  step3();

  return 0;
}
