/* UT_BTree */
#include "BTree.h"
#include "String2.h"
#include "TestObject.h"
#include "ObjectMgr.h"
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

#define ORDER (10)

extern char words1000[];

PRIVATE List * keys;
PRIVATE TestObject ** testObjects;
PRIVATE TestObject ** removedObjects;
PRIVATE int nbTokens = 0;
PRIVATE FILE* channelLog = 0;

PRIVATE int UT_BTree_01_init_keys()
{
  String * fullText = String_newByRef(words1000);
  String * key = 0;

  keys = String_splitToken(fullText, " ");
  nbTokens = List_getNbNodes(keys);

  testObjects = (TestObject**)Memory_alloc(nbTokens * sizeof(TestObject*));
  removedObjects = (TestObject**)Memory_alloc(nbTokens * sizeof(TestObject*));

  for (int i = 0; i < nbTokens; i++)
  {
	key = (String*)List_getNext(keys);
    testObjects[i] = TestObject_new();
  }

  String_delete(fullText);

  return 1;
}

PRIVATE int UT_BTree_01_delete_keys()
{
  for (int i = 0; i < nbTokens; i++)
  {
    TestObject_delete(testObjects[i]);
  }
  List_delete(keys);
  Memory_free(testObjects,sizeof(testObjects));
  Memory_free(removedObjects, sizeof(removedObjects));
  
  return 1;
}

PRIVATE int UT_BTree_01_step1()
{
  int isPassed = 1;
  TestObject * removedObject = 0;
  String * key = (String*)List_getHead(keys);

  BTree* testTree;
  List_resetIterator(keys);

  PRINT(("Step 1: Test 1 - Create an instance of class BTree: "));
  testTree = BTree_new(ORDER);
  UT_ASSERT((1));

  PRINT(("Step 1: Test 2 - Insert one object: "));
  BTree_add(testTree, (Object*)key, (Object*)testObjects[0], 0);
  UT_ASSERT((1));

  PRINT(("Step 1: test 3 - Check the number of nodes: "));
  unsigned int nbNodes = BTree_getNbNodes(testTree);
  //PRINT(("  Nb nodes = %d\n", nbNodes));
  UT_ASSERT((nbNodes == 1));

  PRINT(("Step 1: Test 3 - Remove the object: "));
  removedObject = (TestObject*)BTree_remove(testTree, (Object*)key);
  UT_ASSERT((1));
	
  PRINT(("Step 1: Test 4 - Delete BTree: "));
  //String_delete(key);
  BTree_delete(testTree);
  UT_ASSERT((1));

  return isPassed;
}

PRIVATE int UT_BTree_01_step2()
{
  int isPassed = 1;

  BTree* testTree;
  String * key = 0;

  int n = ORDER * 2; /*ORDER * 2;*/

  List_resetIterator(keys);

  PRINT(("Step 2: Test 1 - Create an instance of class BTree: "));
  testTree = BTree_new(ORDER);
  UT_ASSERT((1));

  PRINT(("Step 2: Test 2 - Insert %d object: ", n));
  for (int i=0; i< n; i++)
  {
	key = List_getNext(keys);
	//Object_print((Object*)key);
    BTree_add(testTree, (Object*)key, (Object*)testObjects[i], 0);
  }
  UT_ASSERT((1));

  //BTree_print(testTree);

  List_resetIterator(keys);
  PRINT(("Step 2: test 3 - Remove %d objects: ", n));
  for (int i=0; i< n - 1; i++)
  {
	key = List_getNext(keys);
	removedObjects[i] = (TestObject*)BTree_remove(testTree, (Object*)key);
  }

  //BTree_print(testTree);
  
  PRINT(("Step 2: Test 4 - Delete BTree: "));
  //String_delete(key);
  BTree_delete(testTree);
  UT_ASSERT((1));

  return isPassed;
}

PRIVATE int UT_BTree_01_step3()
{
  int isPassed = 1;

  BTree* testTree;
  String * key = 0;

  int n = ORDER * 10; /*ORDER * 20;*/

  List_resetIterator(keys);

  PRINT(("Step 3: Test 1 - Create an instance of class BTree: "));
  testTree = BTree_new(ORDER);
  UT_ASSERT((1));

  PRINT(("Step 3: Test 2 - Insert %d object: ", n));
  for (int i=0; i< n; i++)
  {
	key = List_getNext(keys);
	Object_print((Object*)key);
    BTree_add(testTree, (Object*)key, (Object*)testObjects[i], 0);
  }
  UT_ASSERT((1));

  //BTree_print(testTree);

  List_resetIterator(keys);
  PRINT(("Step 3: test 3 - Remove %d objects: ", n));
  for (int i=0; i< n; i++)
  {
	key = List_getNext(keys);
	//PRINT(("Remove %d ", i));
	//Object_print(key);
	removedObjects[i] = (TestObject*)BTree_remove(testTree, (Object*)key);
	//BTree_print(testTree);
  }

  
  PRINT(("Step 3: Test 4 - Delete BTree: "));
  //String_delete(key);
  BTree_delete(testTree);
  UT_ASSERT((1));

  return isPassed;
}

PUBLIC int run_UT_BTree_01(void)
{
  int isPassed = 1;

  channelLog = Debug_openChannel("UT_List_01.log");
  Debug_setStdoutChannel(channelLog);

  ObjectMgr* objMgr = ObjectMgr_getRef();

  UT_BTree_01_init_keys();

  isPassed = isPassed && UT_BTree_01_step1();
  isPassed = isPassed && UT_BTree_01_step2();
  isPassed = isPassed && UT_BTree_01_step3();
  
  UT_BTree_01_delete_keys();

  ObjectMgr_report(objMgr);
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  Debug_closeChannel(channelLog);

  return isPassed;
}
