#include "List.h"
#include "Object.h"
#include "TestObject.h"
#include "Debug.h"
#include <stdlib.h>
#include <stdio.h>

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

#define LIST_SIZE (10)

int step1()
{
  List * testList1 = 0;
  List * testList2 = 0;
  List * testList3 = 0;
  TestObject * items[LIST_SIZE*2];
  int i = 0;

  PRINT(("Step 1: Test 1 - Build 2 Lists: "));
  testList1 = List_new();
  testList2 = List_new();

  UT_ASSERT((1));

  PRINT(("Step 1: test 2 - Insert head and insert tail: \n"));

  for (i=0; i<LIST_SIZE; i++)
  {
    items[i] = TestObject_new();
    //items[i]->x = i;
    //items[i]->y = LIST_SIZE - i;
    List_insertHead(testList1, items[i]);
    PRINT(("Nb items %d\n", List_getNbNodes(testList1)));
  }
  for (i=LIST_SIZE; i<LIST_SIZE*2; i++)
  {
    items[i] = TestObject_new();
    //items[i]->x = i;
    //items[i]->y = LIST_SIZE * 2 - i;
    List_insertTail(testList2, items[i]);
    PRINT(("Nb items %d\n", List_getNbNodes(testList2)));
  }

  PRINT(("Step 1: test 3 - Print both lists: \n"));
  PRINT(("List1 ------------->\n"));
  List_forEach(testList1, (void (*)(void* o))&TestObject_print);
  PRINT(("List2 ------------->\n"));
  List_forEach(testList2, (void (*)(void* o))&TestObject_print);

  PRINT(("Step 1: test 4 - Merge both lists: \n"));
  List_merge(testList1, testList2);
  PRINT(("List1 + List2 ----->\n"));
  List_forEach(testList1, (void (*)(void* o))&TestObject_print);

  testList3 = List_copy(testList1);
  PRINT(("Copy of List1 ------>\n"));
  List_forEach(testList3, (void (*)(void* o))&TestObject_print);

  List_delete(testList1);
  List_delete(testList3);

  //Memory_report();

  return 0;
}

int step2()
{
  List * testList1 = 0;
  List * testList2 = 0;

  TestObject * items[LIST_SIZE];
  TestObject * output1 = 0;
  TestObject * output2 = 0;

  int i;
  
  testList1 = List_new();
  testList2 = List_new();

  for (i=0; i<LIST_SIZE;i++)
  {
    items[i] = TestObject_new();
    //items[i]->x = i;
    //items[i]->y = LIST_SIZE - i;
    List_insertHead(testList1, items[i]);
    List_insertHead(testList2, items[i]);
    printf("Test List1: Nb items %d\n", List_getNbNodes(testList1));
    printf("Test List2: Nb items %d\n", List_getNbNodes(testList2));
  }
  
  output1 = List_getHead(testList1);
  output2 = List_getHead(testList2);

  for (i=0; i<LIST_SIZE; i++)
  {
    output1 = List_removeHead(testList1);
    output2 = List_removeTail(testList2);
    //TestItem_delete(output1);
    printf("Test List 1: Nb items %d\n", List_getNbNodes(testList1));
    printf("Test List 2: Nb items %d\n", List_getNbNodes(testList2));
  }

  output1 = List_removeHead(testList1);
  output2 = List_removeTail(testList2);

  List_delete(testList1);
  List_delete(testList2);

  return 0;
}

int main()
{
  step1();
  step2();
  return 0;
}
