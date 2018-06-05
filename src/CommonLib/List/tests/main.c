#include "List.h"
#include "Object.h"
#include <stdlib.h>
#include <stdio.h>

#define LIST_SIZE (1)

typedef struct TestItem
{
  Object object;
  int x;
  int y;
} TestItem;

void TestItem_delete(TestItem * this);
TestItem * TestItem_copy(TestItem * item);

TestItem * TestItem_new()
{
  TestItem * this = 0;

  this = (TestItem*)malloc(sizeof(TestItem));
  this->object.delete = (Destructor)&TestItem_delete;
  this->object.copy = (Copy_operator)&TestItem_copy;
  this->object.size = sizeof(TestItem);

  return this;
}

void TestItem_delete(TestItem * this)
{
  free(this);
}

TestItem * TestItem_copy(TestItem * item)
{
  TestItem * copy = 0;

  copy = (TestItem*)malloc(sizeof(TestItem));
  copy->x = item->x;
  copy->y = item->y;

  return copy;
}

void TestItem_print(TestItem * item)
{
  printf("x=%d y=%d\n", item->x, item->y);
}

int step1()
{
  List * testList1 = 0;
  List * testList2 = 0;
  List * testList3 = 0;
  TestItem * items[LIST_SIZE*2];
  int i = 0;

  testList1 = List_new();
  testList2 = List_new();

  for (i=0; i<LIST_SIZE; i++)
  {
    items[i] = TestItem_new();
    items[i]->x = i;
    items[i]->y = LIST_SIZE - i;
    List_insertHead(testList1, items[i]);
    printf("Nb items %d\n", List_getSize(testList1));
  }
  for (i=LIST_SIZE; i<LIST_SIZE*2; i++)
  {
    items[i] = TestItem_new();
    items[i]->x = i;
    items[i]->y = LIST_SIZE * 2 - i;
    List_insertTail(testList2, items[i]);
    printf("Nb items %d\n", List_getSize(testList2));
  }

  printf("List1 ------------->\n");
  List_forEach(testList1, &TestItem_print);
  printf("List2 ------------->\n");
  List_forEach(testList2, &TestItem_print);

  List_merge(testList1, testList2);
  printf("List1 + List2 ----->\n");
  List_forEach(testList1, &TestItem_print);

  testList3 = List_copy(testList1);
  printf("Copy of List1 ------>\n");
  List_forEach(testList3, &TestItem_print);

  List_delete(testList1);
  List_delete(testList3);

  Memory_report();

  return 0;
}

int step2()
{
  return 0;
}

int main()
{
  step1();
  step2();
  return 0;
}
