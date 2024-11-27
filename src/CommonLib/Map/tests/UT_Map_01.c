/* UT_Map_01.c */

#include "Map.h"
#include "String2.h"
#include "TestObject.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)
#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("\x1b[32mPassed\x1b[0m\n");} \
                          else { printf("\x1b[31mFailed\x1b[0m\n"); return 0;}
#endif

extern char words1000[];


List * keys;
TestObject ** testObjects;
int nbTokens = 0;
PRIVATE FILE* logChannel = 0;

int UT_Map_01_init_keys()
{
  String * fullText = String_newByRef(words1000);

  keys = String_splitToken(fullText, " ");
  nbTokens = List_getNbNodes(keys);

  testObjects = (TestObject**)Memory_alloc(nbTokens * sizeof(TestObject*));

  for (int i = 0; i < nbTokens; i++)
  {
    testObjects[i] = TestObject_new();
  }

  String_delete(fullText);

  return 1;
}

int UT_Map_01_delete_keys(int isOwner)
{
  if (isOwner)
  {
    for (int i = 0; i < nbTokens; i++)
    {
      TestObject_delete(testObjects[i]);
    }
  }
  List_delete(keys);
  Memory_free(testObjects, sizeof(testObjects));

  return 1;
}

int UT_Map_01_step1()
{
  Map* testMap = 0;
  String * key = 0;

    PRINT(("Step 1: Test 1 - Create an instance of class Map: "));

  testMap = Map_new();

  UT_ASSERT((testMap!=0));

  PRINT(("Step 1: Test 2 - Insert an object: "));
  key = (String*)List_getNext(keys);
  Map_insert(testMap, key, testObjects[0], 0);

  UT_ASSERT((1));

  PRINT(("Step 1: Test 3 - Delete the Map: "));
  Map_delete(testMap);

  UT_ASSERT((1));

  Memory_report();

  return 0;
}

int UT_Map_01_step2()
{
  Map * testMap = Map_new();
  String * key = 0;
  
  PRINT(("Step 2: Test 1 - Insert %d object: ", nbTokens));
  List_resetIterator(keys);
  for (int i = 0; i < nbTokens; i++)
  {
    key = (String*)List_getNext(keys);
    TRACE(("-->%s\n", String_getBuffer(key)));
    Map_insert(testMap, key, testObjects[i], 0);
  }
  
  UT_ASSERT((1));
  Map_delete(testMap);

  Memory_report();

  return 0;
}

int UT_Map_01_step3()
{
  Map* testMap = Map_new();

  String * s = String_new("Hello world");
  String * item = 0;
  
  PRINT(("Step 3: Test 1 - Insert %d object: ", nbTokens));
  Map_find(testMap, s, (void**)&item);
 
  TRACE(("Value : %s\n", String_getBuffer(item)));
  UT_ASSERT((item==0));

  String_delete(s);
  
  Map_delete(testMap);

  return 0;
}

int UT_Map_01_step4()
{
  Map* testMap = Map_new();

  String * s = String_new("Hello world");
  String * item = String_new("The new value");
  String * newItem = 0;
  
  Map_insert(testMap, s, item, 1);
  Map_find(testMap, s, (void**)&newItem);
  
  printf("New value : %s\n",
          String_getBuffer(newItem));
  
  String_delete(s);
  String_delete(item);

  Map_delete(testMap);

  return 0;
}

int UT_Map_01_step5()
{
  int i = 0;
  String * s = 0;
  String * c = 0;
  List * l = 0;
  Map* testMap = Map_new();


  const char * testNames[] =
  {
    "banana", "strawberry", "cherry", "apple", "orange", "pear", "blackberry"
  };

  const char * testColor[] =
  {
    "yellow", "red", "red", "yellow", "orange", "green", "black"
  };

  for (i=0; i< sizeof(testNames)/sizeof(const char *) ; i++)
  {
    s = String_newByRef(testNames[i]);
    c = String_newByRef(testColor[i]);
    Map_insert(testMap, s, c, 1);
    //String_delete(s);
    //String_delete(c);
  }


  l = Map_getAll(testMap);

     List_delete(l);

  Map_delete(testMap);

  return 0;
}

int UT_Map_01_step6()
{
  
  return 0;
}

int run_UT_Map_01()
{
  int isPassed = 1;
  logChannel = Debug_openChannel("UT_Map_01.log");
  Debug_setStdoutChannel(logChannel);
  ObjectMgr* objMgr = ObjectMgr_getRef();
  UT_Map_01_init_keys();

  UT_Map_01_step1();
  UT_Map_01_step2();
  UT_Map_01_step3();
  UT_Map_01_step4();
  UT_Map_01_step5();
  UT_Map_01_step6();

  UT_Map_01_delete_keys(1);

  ObjectMgr_report(objMgr);
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  Debug_closeChannel(logChannel);
  
  return isPassed;
}
