/* TestObject.h */
#ifndef _TESTOBJECT_H_
#define _TESTOBJECT_H_

#include "Types.h"
#include "Allocator.h"

typedef struct TestObject TestObject;

PUBLIC TestObject * TestObject_new();
PUBLIC TestObject * TestObject_newFromAllocator(Allocator* allocator);
PUBLIC void TestObject_delete(TestObject * this);

#endif /* _TESTOBJECT_H_ */
