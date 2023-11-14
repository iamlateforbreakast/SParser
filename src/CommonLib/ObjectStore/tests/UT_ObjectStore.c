#include "ObjectStore.h"
#include "Class.h"

#include "MyAllocator.h"
#include "Malloc.h"
#include "Debug.h"

#include <stdio.h>

#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

typedef struct TestClass
{
  Object object;
} TestClass;

Object * TestClass_new()
{
  return 0;
}

void TestClass_delete(TestClass * this)
{

}

Object * TestClass_copy(TestClass * this)
{
  return 0;
}

int TestClass_compare(TestClass * this, TestClass * compared)
{
  return 0;
}

void TestClass_print(TestClass * this)
{

}

Class testClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&TestClass_delete,
  .f_copy = (Copy_Operator)&TestClass_copy,
  .f_comp = (Comp_Operator)&TestClass_compare,
  .f_print = (Printer)&TestClass_print
};


int step1()
{
  ObjectStore * objectStore1 = 0;
  ObjectStore * objectStore2 = 0;

  /* Test 1 */
  PRINT(("Step 1: Test 1 - Check there is only one ObjectStore: "));
  
  objectStore1 = ObjectStore_getRef();
  objectStore2 = ObjectStore_getRef();

  UT_ASSERT((objectStore1 == objectStore2))

  ObjectStore_delete(objectStore1);
  ObjectStore_delete(objectStore2);

  PRINT(("Step 1: Test 2 - Check all memory is freed: "));
  
  UT_ASSERT((Malloc_report((Allocator*)Malloc_getRef())==0));

  return 1;
}

int step2()
{
  Allocator * testAlloc = 0;
  ObjectStore * objectStore = 0;

  /* Test1 */
  printf("Step 2: Test 1 - TBC: ");

  objectStore = ObjectStore_getRef();
  //testAlloc = ObjectStore_addAllocator(objectStore);

  //ObjectStore_deleteAllocator(objectStore, testAlloc);
  ObjectStore_delete(objectStore);

  return 1;
}

int step3()
{
  ObjectStore * objectStore = 0;
  MyAllocator * testAlloc = 0;
  Object * object = 0;

  objectStore = ObjectStore_getRef();
  //testAlloc = (MyAllocator*)ObjectStore_createAllocator(objectStore);

  object = ObjectStore_createObject(objectStore, &testClass, (Allocator*)testAlloc);
  //object = (Object *)testAlloc->allocate(testAlloc, class->f_size());

  //ObjectStore_deleteAllocator(objectStore, (Allocator*)testAlloc);
  ObjectStore_delete(objectStore);

  return 1;
}

int main()
{
    step1();
}
