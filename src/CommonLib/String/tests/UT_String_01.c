#include "ObjectMgr.h"
#include "Object.h"
#include "String2.h"
#include <stdio.h>

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
    printf("Step 2: Passed\n");
  }
  else
  {
    printf("Step 2: Failed\n");
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
    printf("Step 3: Passed\n");
  }
  else
  {
    printf("Step 3: Failed\n");
  }
  
  String_delete(testString);
  
  return 0;
}


int step4()
{
  String * testString = 0;
  const char * testPatterns[] = { "*.txt", "*.[ch]","*.c" };
  const char * testStrings[] = {  "test.c", "test.h", "/cygdrive/c/Solo/CSW_3.0.3_19wk35/payload/pl/plParam.c" };
  const unsigned int results[] = { 0, 1, 1 };
  unsigned int nbChecksOk = 0;
  int i = 0;
  
  for (i = 0; i<sizeof(results)/sizeof(results[0]); i++)
  {
    testString = String_new(testStrings[i]);

    if (String_matchWildcard(testString, testPatterns[i]) == results[i]) nbChecksOk++;
    
    String_delete(testString);
  }
  
  if (nbChecksOk == sizeof(results)/sizeof(results[0]))
  {
    printf("Step 4: Passed\n");
  }
  else
  {
    printf("Step 4: Failed\n");
  }

  return 0;
}

int step5()
{
    String* testString = String_new("TestString");
    String* compareString = 0;

    char* compareText[] = {"AestString", "U", "test", "TestString5", "TestString"};

    int comp = 0;

    for(int i = 0; i < 5; i++)
    {
        compareString = String_new(compareText[i]);
        comp = String_compare(testString, compareString);
        printf("Compare %s with %s result = %d\n", String_getBuffer(testString), String_getBuffer(compareString), comp);
        String_delete(compareString);
    }

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
