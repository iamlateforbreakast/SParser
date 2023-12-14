/* UT_Map_01.c */

#include "Map.h"
#include "String2.h"
#include "TestObject.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

#define NB_KEYS (250)

#include "Words1000.h"

String ** keys;
TestObject ** testObjects;
int nbTokens = 0;

int init_keys()
{
  String * fullText = String_new(words1000);
  List * tokens = 0;

  tokens = String_splitToken(fullText, " ");
  nbTokens = List_getNbNodes(tokens);

  keys = (String**)Memory_alloc(nbTokens * sizeof(String*));
  testObjects = (TestObject**)Memory_alloc(nbTokens * sizeof(TestObject*));

  for (int i = 0; i < nbTokens; i++)
  {
    keys[i] = (String*)List_getNext(tokens);
    testObjects[i] = TestObject_new();
  }

  List_delete(tokens);
  String_delete(fullText);

  return 1;
}

int delete_keys()
{
  for (int i = 0; i < nbTokens; i++)
  {
    TestObject_delete(testObjects[i]);
    String_delete(keys[i]);
  }
  Memory_free(keys, sizeof(keys));
  Memory_free(testObjects,sizeof(testObjects));

  return 1;
}

int step1()
{
  Map* testMap = 0;

  Memory_report();

  PRINT(("Step 1: Test 1 - Build a Map: "));

  testMap = Map_new();

  UT_ASSERT((1));

  PRINT(("Step 1: Test 2 - Insert an object: "));

  Map_insert(testMap, keys[0], testObjects[0]);

  UT_ASSERT((1));

  PRINT(("Step 1: Test 3 - Delete the Map: "));
  Map_delete(testMap);

  UT_ASSERT((1));

  Memory_report();

  return 0;
}

int step2()
{
  Map * testMap = Map_new();
  
  for (int i = 0; i < nbTokens; i++)
  {
    //printf("-->%s\n", String_getBuffer(keys[i]));
    Map_insert(testMap, keys[i], testObjects[i]);
  }
  
  Map_delete(testMap);

  Memory_report();

  return 0;
}

int step3()
{
  Map* testMap = Map_new();

  String * s = String_new("Hello world");
  String * item = 0;
  
  Map_find(testMap, s, (void**)&item);
 
  printf("Value : %s\n", String_getBuffer(item));

  String_delete(s);
  
  Map_delete(testMap);

  return 0;
}

int step4()
{
  Map* testMap = Map_new();

  String * s = String_new("Hello world");
  String * item = String_new("The new value");
  String * newItem = 0;
  
  Map_insert(testMap, s, item);
  Map_find(testMap, s, (void**)&newItem);
  
  printf("New value : %s\n",
          String_getBuffer(newItem));
  
  String_delete(s);
  String_delete(item);

  Map_delete(testMap);

  return 0;
}

int step5()
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

  for (i=0; i<sizeof(testNames)/sizeof(const char *); i++)
  {
    s = String_new(testNames[i]);
    c = String_new(testColor[i]);
    Map_insert(testMap, s, c);
    String_delete(s);
    String_delete(c);
  }

  l = Map_getAll(testMap);

  List_delete(l);

  Map_delete(testMap);

  return 0;
}

int step6()
{
  
  return 0;
}

int main()
{
  init_keys();

  step1();
  step2();
  step3();
  step4();
  step5();
  step6();

  delete_keys();

  Memory_report();

  return 0;
}
