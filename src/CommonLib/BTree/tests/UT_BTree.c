/*********************************************************************************
* Main.c
/* UT_BTree */
#include "BTree.h"
#include "String2.h"
#include "TestObject.h"
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

List * keys;
TestObject ** testObjects;
int nbTokens = 0;

int init_keys()
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

int delete_keys()
{
  for (int i = 0; i < nbTokens; i++)
  {
    TestObject_delete(testObjects[i]);
  }
  List_delete(keys);
  Memory_free(testObjects,sizeof(testObjects));

  return 1;
}

int step1()
{
  int isPassed = 1;
  String * key = (String*)List_getHead(keys);

  BTree* testTree;

  PRINT(("Step 1: Test 1:"));
  testTree = BTree_new(3);

  BTree_add(testTree, key, testObjects[0]);

  String_delete(key);
  BTree_free(testTree);

  return isPassed;
}

int main(void)
{
	step1();
	//BTree* testTree;
	//unsigned int keys[NB_ITEMS];
	//unsigned int items[NB_ITEMS];

	//unsigned int * pTestItem = 0;

	//for (int i = 1; i <= NB_ITEMS; i++)
	//{
	//	keys[i-1] = i * 5;
	//	items[i-1] = i * 10;
	//}

	//testTree = BTree_new(3);
	//printf("Btree size of pool in   bytes: %d\n", BTree_reportSizeInBytes(testTree));
	//printf("Btree size of pool in Kibytes: %d\n", BTree_reportSizeInBytes(testTree) / 1024);
	//printf("Btree size of pool in Mibytes: %d\n", BTree_reportSizeInBytes(testTree) / (1024 * 1024));
	//BTree_print(testTree);

	//double cpu_time0 = get_cpu_time();
	//double wall_time0 = get_wall_time();
	
	//for (int i = 0; i < NB_ITEMS; i++)
	//{
	//	BTree_add(testTree, keys[i], &items[i]);
	//	BTree_print(testTree);
	//}
	
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

    //for (int i = 0; i< NB_ITEMS; i++)
	//{
	//	void * ptrRemoved = BTree_remove(testTree, keys[i]);
//		printf("test item = %i %x %x\n", i, ptrRemoved, &items[i]);
//		BTree_print(testTree);
//	}

//	BTree_free(testTree);

	return 0;
}
