/* UT_Map_01.c */

#include "Map.h"
#include "String2.h"
#include "TestObject.h"
#include "ObjectMgr.h"
#include "Handle.h"
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

extern char words1000[];


List * UT_Map_01_keys;
TestObject ** UT_Map_01_testObjects;
int UT_Map_01_nbTokens = 0;
PRIVATE FILE* UT_Map_01_logChannel = 0;

int UT_Map_01_init_keys()
{
  String * fullText = String_newByRef(words1000);

  UT_Map_01_keys = String_splitToken(fullText, " ");
  UT_Map_01_nbTokens = List_getNbNodes(UT_Map_01_keys);

  UT_Map_01_testObjects = (TestObject**)Memory_alloc(UT_Map_01_nbTokens * sizeof(TestObject*));

  for (int i = 0; i < UT_Map_01_nbTokens; i++)
  {
    UT_Map_01_testObjects[i] = TestObject_new();
  }

  String_delete(fullText);

  return 1;
}

int UT_Map_01_delete_keys(int isOwner)
{
  if (isOwner)
  {
    for (int i = 0; i < UT_Map_01_nbTokens; i++)
    {
      TestObject_delete(UT_Map_01_testObjects[i]);
    }
  }
  List_delete(UT_Map_01_keys);
  Memory_free(UT_Map_01_testObjects, sizeof(UT_Map_01_testObjects));

  return 1;
}

int UT_Map_01_step1()
{
  int isPassed = 1;
  Map* testMap = 0;
  String * key = 0;

  PRINT(("Step 1: Test 1 - Create an instance of class Map: "));

  testMap = Map_new();

  isPassed = isPassed && OBJECT_IS_VALID(testMap);

  UT_ASSERT((isPassed));

  //PRINT(("Step 1: Test 2 - Insert an object: "));
  //key = (String*)List_getNext(UT_Map_01_keys);
  //Handle * hKey = Handle_new(key, 0);
  //Handle * hItem = Handle_new(UT_Map_01_testObjects[0], 0);
  //Map_insert(testMap, hKey, hItem);

  //UT_ASSERT((1));

  PRINT(("Step 1: Test 2 - Delete the Map: "));
  Map_delete(testMap);

  isPassed = isPassed && OBJECT_IS_INVALID(testMap);

  UT_ASSERT((isPassed));

  //Memory_report();

  return isPassed;
}

int UT_Map_01_step2()
{
  int isPassed = 1;
  Map * testMap = Map_new();

  PRINT(("Step 2: Test 1 - Insert one object: "));
  String * key = (String*)List_getNext(UT_Map_01_keys);
  Handle * hKey = Handle_new(key, 0);
  Handle * hItem = Handle_new(UT_Map_01_testObjects[0], 0);
  int isOk = Map_insert(testMap, hKey, hItem);
  isPassed = isPassed && isOk;

  UT_ASSERT((isPassed));

  PRINT(("Step 2: Test 2 - Find inserted object: "));
  void * foundItem = 0;
  int isFound = Map_find(testMap, key, &foundItem);
  isPassed = isPassed && isFound;

  UT_ASSERT((isPassed));

  PRINT(("Step 2: Test 3 - Reject same object insertion: "));
  int isOK = !Map_insert(testMap, hKey, hItem);
  isPassed = isPassed && isOK;

  UT_ASSERT((isPassed));

  Map_delete(testMap);

  return isPassed;
}

int UT_Map_01_step3()
{
  int isPassed = 1;
  Map * testMap = Map_new();
  int nbObjects = 30;

  PRINT(("Step 3: Test 1 - Insert %d objects: ", nbObjects));
  List_resetIterator(UT_Map_01_keys);
  for (int i = 0; i < nbObjects; i++)
  {
    String * key = (String*)List_getNext(UT_Map_01_keys);
    Handle * hKey = Handle_new(key, 0);
    Handle * hItem = Handle_new(UT_Map_01_testObjects[0], 0);
    isPassed = isPassed && Map_insert(testMap, hKey, hItem);
    void * p;
    isPassed = isPassed && Map_find(testMap, key, &p);
  }
  
  UT_ASSERT((isPassed));

  nbObjects = 1;

  PRINT(("Step 3: Test 2 - Insert %d objects: ", nbObjects));
  List_resetIterator(UT_Map_01_keys);
  for (int i = 0; i < nbObjects; i++)
  {
    String * key = (String*)List_getNext(UT_Map_01_keys);
    Handle * hKey = Handle_new(key, 0);
    Handle * hItem = Handle_new(UT_Map_01_testObjects[0], 0);
    isPassed = isPassed && Map_insert(testMap, hKey, hItem);
    void * p;
    isPassed = isPassed && Map_find(testMap, key, &p);
  }
  
  UT_ASSERT((isPassed));

  nbObjects = nbObjects * 2;
  
  PRINT(("Step 3: Test 3 - Insert %d objects: ", nbObjects));
  List_resetIterator(UT_Map_01_keys);
  for (int i = 0; i < nbObjects; i++)
  {
    String * key = (String*)List_getNext(UT_Map_01_keys);
    Handle * hKey = Handle_new(key, 0);
    Handle * hItem = Handle_new(UT_Map_01_testObjects[0], 0);
    isPassed = isPassed && Map_insert(testMap, hKey, hItem);
    void * p;
    isPassed = isPassed && Map_find(testMap, key, &p);
  }

  UT_ASSERT((isPassed));

  Map_delete(testMap);

  //Memory_report();

  return isPassed;
}

int UT_Map_01_step4()
{
  int isPassed = 1;
  Map* testMap = Map_new();

  String * s = String_new("Hello world");
  String * item = 0;
  
  PRINT(("Step 3: Test 1 - Insert %d object: ", UT_Map_01_nbTokens));
  Map_find(testMap, s, (void**)&item);
 
  TRACE(("Value : %s\n", String_getBuffer(item)));
  UT_ASSERT((item==0));

  String_delete(s);
  
  Map_delete(testMap);

  return isPassed;
}


int UT_Map_01_step5()
{
  int isPassed = 1;
  Map* testMap = Map_new();

  String * key = String_new("Hello world");
  String * item = String_new("The new value");
  String * newItem = 0;
  
  Handle* hKey = Handle_new(key, 1);
  Handle* hItem = Handle_new(item, 1);

  PRINT(("Step 4: Test 1 - Insert %d object: ", UT_Map_01_nbTokens));

  Map_insert(testMap, hKey, hItem);
  Map_find(testMap, key, (void**)&newItem);
  
  PRINT(("New value : %s\n",
          String_getBuffer(newItem)));
  
  //String_delete(s);
  //String_delete(item);
  UT_ASSERT((1));

  Map_delete(testMap);

  return isPassed;
}

int UT_Map_01_step6()
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
    Handle * hKey = Handle_new(String_newByRef(testNames[i]), 1);
    Handle * hItem = Handle_new(String_newByRef(testColor[i]), 1);
    Map_insert(testMap, hKey, hItem);
    //String_delete(s);
    //String_delete(c);
  }

  PRINT(("Step 5: Test 1 - Insert %d object: ", UT_Map_01_nbTokens));

  l = Map_getAll(testMap);

  UT_ASSERT((1));

  List_delete(l);

  Map_delete(testMap);

  return 0;
}

#if 0
int UT_Map_01_step7()
{
  
  return 0;
}
#endif

#ifdef MAIN
int main()
#else
int run_UT_Map_01()
#endif
{
  int isPassed = 1;
  UT_Map_01_logChannel = Debug_openChannel("UT_Map_01.log");
  Debug_setStdoutChannel(UT_Map_01_logChannel);
  ObjectMgr* objMgr = ObjectMgr_getRef();
  UT_Map_01_init_keys();

  isPassed = UT_Map_01_step1() && isPassed;
  isPassed = UT_Map_01_step2() && isPassed;
  isPassed = UT_Map_01_step3() && isPassed;
  //isPassed = UT_Map_01_step4() && isPassed;
  //isPassed = UT_Map_01_step5() && isPassed;
  //isPassed = UT_Map_01_step6() && isPassed;

  UT_Map_01_delete_keys(1);

  ObjectMgr_report(objMgr);
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  Debug_closeChannel(UT_Map_01_logChannel);
  
  return isPassed;
}
