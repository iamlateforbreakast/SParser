#include "List.h"
#include "Object.h"
#include "Allocator.h"
#include "TestObject.h"
#include "Debug.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_OBJECT_NB (10)

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

int step1()
{
  List* testList1 = 0;

  TestObject * items[MAX_OBJECT_NB];
  int i = 0;

  testList1 = List_new();

  for (i = 0; i < MAX_OBJECT_NB; i++)
  {
    items[i] = TestObject_new();
    //items[i]->x = i;
    //items[i]->y = MAX_OBJECT_NB - i;
    List_insertHead(testList1, items[i]);
    printf("Nb items %d\n", List_getSize(testList1));
  }

  return 0;
}

int step2()
{
}

int main()
{
  step1();
  step2();

  return 0;
}
