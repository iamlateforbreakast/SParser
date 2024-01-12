/* TestObject.c */

#include "TestObject.h"
#include "Types.h"
#include "Object.h"
#include "ObjectStore.h"
#include "Debug.h"

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
  TestObject * this = 0;
  
  this = (TestObject*)Object_new(sizeof(TestObject), &testObjectClass);
  this->testValue = 12345678;

  return this;
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
  if (this!=0)
  {
    if (this->object.refCount==1)
    {
      Object_deallocate(&this->object);
    }
    else if (this->object.refCount>1)
    {
      this->object.refCount--;
    }
  }
}

PUBLIC int TestObject_compare(TestObject* this, TestObject* compare)
{
  return 0;
}

PUBLIC TestObject* TestObject_copy(TestObject * this)
{
  TestObject * copy = 0;
  if (this!=0)
  {
    if (this->object.allocator)
      copy = (TestObject*)TestObject_newFromAllocator(this->object.allocator);
    else
      copy = (TestObject*)TestObject_new();
    copy->testValue = this->testValue;
  }
  return copy;
}

PUBLIC void TestObject_print(TestObject* this)
{
  PRINT(("  ~ Object: %d\n", this->object.uniqId));
}

PUBLIC unsigned int TestObject_getSize(TestObject* this)
{
  if (this != 0)
    return sizeof(TestObject);
  else
    return sizeof(TestObject);
}
