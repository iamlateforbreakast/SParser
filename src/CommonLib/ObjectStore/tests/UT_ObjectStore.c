#include "ObjectStore.h"
#include "Class.h"

#include "MyAllocator.h"
#include "Malloc.h"
#include "Debug.h"
#include "Error.h"

#include <stdio.h>

#define DEBUG (0)


#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

typedef struct TestClass
{
  Object object;
} TestClass;

Object* TestClass_new()
{
  return 0;
}

void TestClass_delete(TestClass* this)
{

}

Object* TestClass_copy(TestClass* this)
{
  return 0;
}

int TestClass_compare(TestClass* this, TestClass* compared)
{
  return 0;
}

void TestClass_print(TestClass* this)
{

}

unsigned int TestClass_size()
{
  return sizeof(TestClass);
}

Class testClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&TestClass_delete,
  .f_copy = (Copy_Operator)&TestClass_copy,
  .f_comp = (Comp_Operator)&TestClass_compare,
  .f_print = (Printer)&TestClass_print,
  .f_size = (Sizer)&TestClass_size
};


int step1()
{
  ObjectStore* objectStore1 = 0;
  ObjectStore* objectStore2 = 0;

  /* Test 1 */
  PRINT(("Step 1: Test 1 - Check the ojectStore is created: "));
  objectStore1 = ObjectStore_getRef();
  UT_ASSERT((ObjectStore_getNbAllocatedObjects(objectStore1) == 0));

  /* Test 2 */
  PRINT(("Step 1: Test 2 - Check there is only one ObjectStore: "));

  objectStore2 = ObjectStore_getRef();

  UT_ASSERT((objectStore1 == objectStore2))

  ObjectStore_delete(objectStore1);
  ObjectStore_delete(objectStore2);

  /* Test 3 */
  PRINT(("Step 1: Test 3 - Check all memory is freed: "));

  UT_ASSERT((Malloc_report((Allocator*)Malloc_getRef()) == 0));

  return 1;
}

int step2()
{
  MyAllocator* testAlloc = 0;
  AllocInfo* testAllocInfo = 0;
  ObjectStore* objectStore1 = 0;

  /* Test 1 */
  PRINT(("Step 2: Test 1 - Create a custom allocator: "));
  objectStore1 = ObjectStore_getRef();
  testAlloc = MyAllocator_new();
  testAllocInfo = ObjectStore_createAllocator(objectStore1, (Allocator*)testAlloc);
  UT_ASSERT((1));

  /* Test 2 */
  PRINT(("Step 2: test 2 - Delete a custom allocator: "));
  ObjectStore_deleteAllocator(objectStore1, testAllocInfo);
  UT_ASSERT((1));
  ObjectStore_delete(objectStore1); 

  /* Test 3 */
  PRINT(("Step 2: Test 3 - Check all memory is freed: "));
  UT_ASSERT((Malloc_report((Allocator*)Malloc_getRef()) == 0));

  return 1;
}

int step3()
{
  MyAllocator* testAlloc = 0;
  AllocInfo* testAllocInfo = 0;
  ObjectStore* objectStore = 0;
  Object* object[10] = { 0 };

  objectStore = ObjectStore_getRef();
  testAlloc = MyAllocator_new();
  testAllocInfo = ObjectStore_createAllocator(objectStore, (Allocator*)testAlloc);

  /* Test 1 */
  PRINT(("Step 3: Test 1 - Create an object using custom allocator: "));
  object[0] = ObjectStore_createObject(objectStore, &testClass, (Allocator*)testAlloc);
  UT_ASSERT((MyAllocator_report((Allocator*)testAlloc) == 1));

  /* Test 2 */
  PRINT(("Step 3: Test 2 - Delete an object using custom allocator: "));
  ObjectStore_deleteObject(objectStore, object[0]);
  UT_ASSERT((MyAllocator_report((Allocator*)testAlloc) == 0));


  /* Test 3 */
  PRINT(("Step 3: Test 3 - Create and delete 10 objects using custom allocator: "));
  for (int i = 0; i < 10; i++)
  {
    object[i] = ObjectStore_createObject(objectStore, &testClass, (Allocator*)testAlloc);

    TRACE(("=> %d: Object created at 0x%x with Id:%d AllocatedObjects:%d\n",
      i,
      object[i],
      object[i]->id,
      MyAllocator_report((Allocator*)testAlloc)));
  }
  for (int i = 0; i < 10; i++)
  {
    ObjectStore_deleteObject(objectStore, object[0]);
    TRACE(("=> %d: Object at 0x%x deleted with Id:%d AllocatedObjects:%d\n",
      i,
      object[i],
      object[i]->id,
      MyAllocator_report((Allocator*)testAlloc)));
  }
  UT_ASSERT((MyAllocator_report((Allocator*)testAlloc) == 0));

  ObjectStore_deleteAllocator(objectStore, testAllocInfo);
  ObjectStore_delete(objectStore);

  /* Test 4 */
  PRINT(("Step 3: Test 4 - Check all memory is freed: "));
  UT_ASSERT((Malloc_report((Allocator*)Malloc_getRef()) == 0));

  return 1;
}

int main()
{
  step1();
  step2();
  step3();
}
