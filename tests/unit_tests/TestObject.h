/* TestObject.h */
#ifndef _TESTOBJECT_H_
#define _TESTOBJECT_H_

#include "Types.h"
#include "Allocator.h"

typedef struct TestObject TestObject;

PUBLIC TestObject * TestObject_new();
PUBLIC TestObject * TestObject_newFromAllocator(Allocator* allocator);
PUBLIC void TestObject_delete(TestObject * this);
PUBLIC int TestObject_compare(TestObject* this, TestObject* compare);
PUBLIC TestObject* TestObject_copy();
PUBLIC void TestObject_print(TestObject* this);
PUBLIC unsigned int TestObject_getSize(TestObject* this);
#endif /* _TESTOBJECT_H_ */
