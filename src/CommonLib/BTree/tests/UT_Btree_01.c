/* UT_BTree */
#include "BTree.h"
#include "String2.h"
#include "TestObject.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Debug.h"

#include "Words1000.h"

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

#define ORDER (3)

List * keys;
TestObject ** UT_BTree_01_testObjects;
TestObject ** removedObjects;
int nbTokens = 0;

int init_keys()
{
  String * fullText = String_newByRef(words1000);
  String * key = 0;

  keys = String_splitToken(fullText, " ");
  nbTokens = List_getNbNodes(keys);

  UT_BTree_01_testObjects = (TestObject**)Memory_alloc(nbTokens * sizeof(TestObject*));
  removedObjects = (TestObject**)Memory_alloc(nbTokens * sizeof(TestObject*));

  for (int i = 0; i < nbTokens; i++)
  {
	key = (String*)List_getNext(keys);
    UT_BTree_01_testObjects[i] = TestObject_new();
  }

  String_delete(fullText);

  return 1;
}

int delete_keys()
{
  for (int i = 0; i < nbTokens; i++)
  {
    TestObject_delete(UT_BTree_01_testObjects[i]);
  }
  List_delete(keys);
  Memory_free(UT_BTree_01_testObjects,sizeof(UT_BTree_01_testObjects));
  Memory_free(removedObjects, sizeof(removedObjects));
  
  return 1;
}

int step1()
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
  BTree_add(testTree, (Object*)key, (Object*)UT_BTree_01_testObjects[0], 0);
  UT_ASSERT((1));

  PRINT(("Step 1: Test 3 - Remove the object: "));
  removedObject = (TestObject*)BTree_remove(testTree, (Object*)key);
  UT_ASSERT((1));
	
  PRINT(("Step 1: Test 4 - Delete BTree: "));
  //String_delete(key);
  BTree_delete(testTree);
  UT_ASSERT((1));

  return isPassed;
}

int step2()
{
  int isPassed = 1;

  BTree* testTree;
  String * key = 0;

  int n = 6; /*ORDER * 2;*/

  List_resetIterator(keys);

  PRINT(("Step 2: Test 1 - Create an instance of class BTree: "));
  testTree = BTree_new(ORDER);
  UT_ASSERT((1));

  PRINT(("Step 2: Test 2 - Insert %d object: ", n));
  for (int i=0; i< n; i++)
  {
	key = List_getNext(keys);
	Object_print((Object*)key);
    BTree_add(testTree, (Object*)key, (Object*)UT_BTree_01_testObjects[i], 0);
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
  String_delete(key);
  BTree_delete(testTree);
  UT_ASSERT((1));

  return isPassed;
}

int step3()
{
  int isPassed = 1;

  BTree* testTree;
  String * key = 0;

  int n = 800; /*ORDER * 2;*/

  List_resetIterator(keys);

  PRINT(("Step 3: Test 1 - Create an instance of class BTree: "));
  testTree = BTree_new(ORDER);
  UT_ASSERT((1));

  PRINT(("Step 3: Test 2 - Insert %d object: ", n));
  for (int i=0; i< n; i++)
  {
	key = List_getNext(keys);
	Object_print((Object*)key);
    BTree_add(testTree, (Object*)key, (Object*)UT_BTree_01_testObjects[i], 0);
  }
  UT_ASSERT((1));

  BTree_print(testTree);

  List_resetIterator(keys);
  PRINT(("Step 3: test 3 - Remove %d objects: ", n));
  for (int i=0; i< n; i++)
  {
	key = List_getNext(keys);
	PRINT(("Remove %d ", i));
	Object_print(key);
	removedObjects[i] = (TestObject*)BTree_remove(testTree, (Object*)key);
	  BTree_print(testTree);

  }

  
  PRINT(("Step 3: Test 4 - Delete BTree: "));
  String_delete(key);
  BTree_delete(testTree);
  UT_ASSERT((1));

  return isPassed;
}

int main(void)
{
  ObjectMgr* objMgr = ObjectMgr_getRef();

  init_keys();

  //step1();
  //step2();
  step3();
  delete_keys();

  ObjectMgr_report(objMgr);
  ObjectMgr_reportUnallocated(objMgr);
  Memory_report();

  //printf("Btree size of pool in   bytes: %d\n", BTree_reportSizeInBytes(testTree));
  //printf("Btree size of pool in Kibytes: %d\n", BTree_reportSizeInBytes(testTree) / 1024);
  //printf("Btree size of pool in Mibytes: %d\n", BTree_reportSizeInBytes(testTree) / (1024 * 1024));
  //BTree_print(testTree);
	
  //double cpu_time1 = get_cpu_time();
  //double wall_time1 = get_wall_time();
	
  //printf("Insert CPU time %f\n", cpu_time1 - cpu_time0);
  //printf("Insert Wall time %f\n", wall_time1 - wall_time0);

  //cpu_time0 = get_cpu_time();
  //wall_time0 = get_wall_time();

  //for (int i = 0; i< NB_ITEMS; i++)
  //{
    //BTree_get(testTree, keys[i], &pTestItem);
    //printf("test item = %x %x\n", pTestItem, &items[i]);
  //}

  //cpu_time1 = get_cpu_time();
  //wall_time1 = get_wall_time();

  //printf("Search CPU time %f\n", cpu_time1 - cpu_time0);
  //printf("Search Wall time %f\n", wall_time1 - wall_time0);

  return 0;
}
