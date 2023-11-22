/* TestObject.c */

#include "TestObject.h"
#include "Types.h"
#include "Object.h"

struct TestObject
{
  Object object;
  int testValue;
};

PUBLIC TestObject * TestObject_new()
{
  return 0;
}

PUBLIC TestObject * TestObject_newFromAllocator(Allocator* allocator)
{
  return 0;
}

PUBLIC void TestObject_delete(TestObject * this)
{

}
