#include "ObjectMgr.h"
#include "Object.h"
#include "String2.h"
#include "Memory.h"
#include "Debug.h"
#include <stdio.h>

#define DEBUG (0)
#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("\x1b[32mPassed\x1b[0m\n"));} \
                          else { PRINT(("\x1b[31mFailed\x1b[0m\n")); return 0;}
#endif
PRIVATE int UT_String_01_step1()
{
  int isPassed = 1;
  String * testString = 0;

  PRINT(("Step 1: Test 1 - Create an instance of class String: "));
  testString = String_new("Hello World!");

  isPassed = isPassed && (testString!=0);
  UT_ASSERT((isPassed));
  PRINT(("Step 1: Test 2 - Delete instance of String: "));
  String_delete(testString);

  UT_ASSERT((1));
  PRINT(("Step 1: Test 3 - Check all memory is freed: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  UT_ASSERT((isPassed));
  TRACE(("Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));
  ObjectMgr_delete(objectMgr);
  return isPassed;
}

PRIVATE int UT_String_01_step2()
{
  int isPassed = 1;
  String * testString = 0;
  String * testPattern = 0;

  
  testString = String_new("Hello World!");
  testPattern = String_new("World");

  PRINT(("Step 2: Test 1 - Check string is contained in another one: "));
  isPassed = isPassed && (String_isContained(testString, testPattern));

  UT_ASSERT((isPassed));

  String_delete(testString);
  String_delete(testPattern);

  PRINT(("Step 2: Test 2 - Check all memory is freed: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  TRACE(("  Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));
  UT_ASSERT((isPassed));
  ObjectMgr_delete(objectMgr);
  return isPassed;
}

PRIVATE int UT_String_01_step3()
{
  int isPassed = 1;
  String * testString = String_new("test.c");
  const char * testPattern = "*.c";

  
  PRINT(("Step 3: Test 1 - Check a string matches a pattern: "));
  isPassed = isPassed && String_matchWildcard(testString, testPattern); 
  UT_ASSERT((isPassed));

  
  String_delete(testString);
  
  PRINT(("Step 3: Test 2 - Check all memory is freed: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  TRACE(("  Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));
  UT_ASSERT((isPassed));
  ObjectMgr_delete(objectMgr);
  return isPassed;
}


PRIVATE int UT_String_01_step4()
{
  int isPassed = 1;
  String * testString = 0;
  const char * testPatterns[] = { "*.txt", "*.[ch]","*.c", "/index.html" , "/test.js", "/"};
  const char * testStrings[] = {  "test.c", "test.h", "/cygdrive/c/Solo/CSW_3.0.3_19wk35/payload/pl/plParam.c", "/", "/test.js", "/"};
  const unsigned int results[] = { 0, 1, 1 , 0, 1, 1};
  unsigned int nbChecksOk = 0;
  int i = 0;
  
  for (i = 0; i<sizeof(results)/sizeof(results[0]); i++)
  {
    testString = String_new(testStrings[i]);

    PRINT((" Check %s matches %s: ", testStrings[i], testPatterns[i]));
    if (String_matchWildcard(testString, testPatterns[i]) == results[i])
    {
      nbChecksOk++;
      PRINT(("OK\n"));
    }
    else
    {
      PRINT(("Failed\n"));
    }
      String_delete(testString);
  }
  
  isPassed = isPassed && (nbChecksOk == sizeof(results)/sizeof(results[0]));
  
  PRINT(("Step 4: Test 2 - Check all memory is freed: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  TRACE(("  Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));
  UT_ASSERT((isPassed));
  ObjectMgr_delete(objectMgr);

  return isPassed;
}

PRIVATE int UT_String_01_step5()
{
  int isPassed = 1;
    String* testString = String_new("TestString");
    String* compareString = 0;

    char* compareText[] = {"AestString", "U", "test", "TestString5", "TestString"};

    int comp = 0;

    for(int i = 0; i < 5; i++)
    {
    compareString = String_newByRef(compareText[i]);
        comp = String_compare(testString, compareString);
        printf("Compare %s with %s result = %d\n", String_getBuffer(testString), String_getBuffer(compareString), comp);
        String_delete(compareString);
    }

  PRINT(("Step 5: Test 2 - Check all memory is freed: "));
  ObjectMgr * objectMgr = ObjectMgr_getRef();
  isPassed = isPassed && (ObjectMgr_report(objectMgr) == 1);
  TRACE(("  Nb objects left allocated: %d\n", ObjectMgr_report(objectMgr)));
  UT_ASSERT((isPassed));
  ObjectMgr_delete(objectMgr);
  return isPassed;
}

PRIVATE int UT_String_01_step6()
{
  int isPassed = 1;
  String * testString = String_new("TestString");

  String_append(testString, "123");

  printf("String append %s\n", String_getBuffer(testString));

  String_delete(testString);
  return isPassed;
}

PUBLIC int run_UT_String_01()
{
  int isPassed = 1;
  
  isPassed = isPassed && UT_String_01_step1();
  isPassed = isPassed && UT_String_01_step2();
  isPassed = isPassed && UT_String_01_step3();
  isPassed = isPassed && UT_String_01_step4();
  isPassed = isPassed && UT_String_01_step5();
  isPassed = isPassed && UT_String_01_step6();

  ObjectMgr* objMgr = ObjectMgr_getRef();;
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  return isPassed;
}
