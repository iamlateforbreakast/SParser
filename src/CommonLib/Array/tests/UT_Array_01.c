#include "Array.h"
#include "String2.h"
#include "Memory.h"
#include "Object.h"
#include "FileIo.h"
#include <stdio.h>
#include <stdlib.h>

#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}
#define STEP_PASS (1)
#define STEP_FAIL (0)

typedef struct TestObject TestObject;

struct TestObject
{
  Object object;
  int id;
};

TestObject * TestObject_new()
{
  TestObject * newTestObject = 0;

  newTestObject = (TestObject*)malloc(sizeof(TestObject));
  newTestObject->id = 0;

  return newTestObject;
}

void TestObject_delete(TestObject * this)
{
  free(this);
}

int step1()
{
  Array * refArray = 0;
  Array * testArray = 0;
  ArrayParam testParam = { .defaultSize = 10, .storageMode = 0, .autoresize = 0};

  /* Test 1 */
  printf("Step 1: Test 1 - Check the Array object is well constructed: ");
  testArray = Array_new(&testParam);
  UT_ASSERT((Memory_ncmp(testArray, refArray, sizeof(testArray))==1));


  /* Test 2 */
  printf("Step 1: Test 2 - Check the Array object is destructed: ");
  Array_delete(testArray);

  /* Test 3 */
  printf("Step 1: Test 3 - Check all memory is freed properly: ");
  UT_ASSERT((Memory_getAllocRequestNb()==(Memory_getFreeRequestNb()+1)))
  return 0;
}

int step2()
{
  int nbTestObjects = 5;
  ArrayParam testParam = { .defaultSize = 10, .storageMode = 0, .autoresize = 0};
  Array * testArray = Array_new(&testParam);

  /* Create test objects */
  TestObject ** objects = (TestObject**)malloc(sizeof(TestObject*)*nbTestObjects);

  for (int i=0; i<nbTestObjects;i++)
  {
    objects[i]=TestObject_new();
    objects[i]->id = i;
  }

  /* Test 1 */
  printf("Step 2: Test 1 - appen new data");
  for (int i=0; i<nbTestObjects;i++)
  {
    // Array_append(testArray, objects[i]);
  }
  // Array_getSize(testArray);
  UT_ASSERT(0)

  /* Test 2 */
  printf("Step 2: Test 2 - Print Array");
  //Array_print(testArray);
  UT_ASSERT(0)

  /* Test 3 */
  printf("Step 2: Test 3 - access newly append data");
  for (int i=0; i<nbTestObjects;i++)
  {
    // Array_get(testArray,0);
  }
  UT_ASSERT(0)

  /* Test 4 */
  printf("Step 2: Test 4 - Check all memory is freed properly: ");
  Array_delete(testArray);
  UT_ASSERT(0)

  /* Free test data */
  free(objects);

  return 0;
}

int step3()
{
  Array * testArray = 0;
  ArrayParam testParam = { .defaultSize = 10, .storageMode = 0, .autoresize = 0};

  /* Test 1 */
  String * fileName = String_new("./test.array");
  FileIo * inStream = FileIo_new();
  FileIo_createFile(inStream, fileName);
  testArray = Array_newFromFile(inStream, &testParam);

  /* Test 2 */
  return 0;
}

unsigned int main(void)
{
  step1();
  step2();
  step3();
}
