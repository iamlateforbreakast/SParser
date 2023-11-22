/* TestObject.c */

#include "TestObject.h"
#include "Types.h"
#include "Object.h"
#include "ObjectStore.h"

struct TestObject
{
  Object object;
  int testValue;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class testObjectClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&TestObject_delete,
  .f_copy = (Copy_Operator)&TestObject_copy,
  .f_comp = (Comp_Operator)&TestObject_compare,
  .f_print = (Printer)&TestObject_print,
  .f_size = (Sizer)&TestObject_getSize
};

PUBLIC TestObject * TestObject_new()
{
  return 0;
}

PUBLIC TestObject * TestObject_newFromAllocator(Allocator* allocator)
{
  TestObject * this = 0;

  this = (TestObject*)Object_newFromAllocator(&testObjectClass, allocator);
  // TODO: Check if allocation failed
  this->testValue = 12345678;

  return this;
}

PUBLIC void TestObject_delete(TestObject * this)
{
  ObjectStore * objectStore = ObjectStore_getRef();
  
  ObjectStore_deleteObject(objectStore, (Object*)this);

  ObjectStore_delete(objectStore);
}

PUBLIC int TestObject_compare(TestObject* this, TestObject* compare)
{
  return 0;
}

PUBLIC TestObject* TestObject_copy()
{
  return 0;
}

PUBLIC void TestObject_print(TestObject* this)
{

}

PUBLIC unsigned int TestObject_getSize(TestObject* this)
{
  if (this != 0)
    return sizeof(TestObject);
  else
    return sizeof(TestObject);
}