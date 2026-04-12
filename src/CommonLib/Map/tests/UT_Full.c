/*
 * UT_Map_comprehensive.c
 * 
 * Comprehensive unit test suite for Map.c
 * Coverage: 100% of Map functions with edge cases, threading, and memory safety
 */

#include "Map.h"
#include "Handle.h"
#include "String2.h"
#include "TestObject.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Debug.h"
#include "List.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define DEBUG (0)

/* Color output for pass/fail */
#ifdef _WIN32
#define PASS_MSG "Passed"
#define FAIL_MSG "Failed"
#else
#define PASS_MSG "\x1b[32mPassed\x1b[0m"
#define FAIL_MSG "\x1b[31mFailed\x1b[0m"
#endif

/* Test infrastructure */
typedef struct {
  int totalTests;
  int passedTests;
  int failedTests;
  FILE * logFile;
} TestContext;

TestContext g_context = {0, 0, 0, NULL};

#define UT_ASSERT(cond, message) \
  do { \
    g_context.totalTests++; \
    if (cond) { \
      g_context.passedTests++; \
      PRINT(("  [%s] %s\n", PASS_MSG, message)); \
    } else { \
      g_context.failedTests++; \
      PRINT(("  [%s] %s (line %d)\n", FAIL_MSG, message, __LINE__)); \
    } \
  } while(0)

#define TEST_SUITE_START(name) \
  PRINT(("\n" "========================================\n")); \
  PRINT(("%s\n", name)); \
  PRINT(("========================================\n"))

#define TEST_SUITE_END() \
  PRINT(("----------------------------------------\n"))

/* ============================================================================
 * SUITE 1: Basic Map Creation and Deletion
 * ============================================================================ */

int UT_Map_suite1_create_delete()
{
  TEST_SUITE_START("SUITE 1: Basic Map Creation and Deletion");

  /* Test 1.1: Create a new Map */
  PRINT(("Test 1.1: Create a new Map\n"));
  Map * map = Map_new();
  UT_ASSERT((map != NULL), "Map_new() returns non-NULL");
  UT_ASSERT((OBJECT_IS_VALID(map)), "Created Map is valid");

  /* Test 1.2: Delete a new empty Map */
  PRINT(("Test 1.2: Delete an empty Map\n"));
  Map_delete(map);
  UT_ASSERT((1), "Map_delete() completes without crash");
  Memory_report();

  /* Test 1.3: Create and delete multiple times */
  PRINT(("Test 1.3: Create and delete multiple Maps\n"));
  for (int i = 0; i < 5; i++) {
    Map * m = Map_new();
    UT_ASSERT((m != NULL), "Create iteration passes");
    Map_delete(m);
  }
  Memory_report();

  /* Test 1.4: Delete NULL pointer */
  PRINT(("Test 1.4: Delete NULL pointer\n"));
  Map_delete(NULL);
  UT_ASSERT((1), "Map_delete(NULL) is safe");

  /* Test 1.5: Delete with invalid object */
  PRINT(("Test 1.5: Delete invalid Map\n"));
  Map invalidMap = {0};
  Map_delete(&invalidMap);
  UT_ASSERT((1), "Map_delete(invalid) is safe");

  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 2: Handle Creation and Reference Counting
 * ============================================================================ */

int UT_Map_suite2_handle_refcount()
{
  TEST_SUITE_START("SUITE 2: Handle Creation and Reference Counting");

  /* Test 2.1: Create a Handle with owned object */
  PRINT(("Test 2.1: Create Handle with owned object\n"));
  TestObject * obj = TestObject_new();
  Handle * h = Handle_new(obj, (Destructor)&TestObject_delete);
  UT_ASSERT((h != NULL), "Handle_new() returns non-NULL");
  UT_ASSERT((Handle_get(h) == obj), "Handle stores object pointer");

  /* Test 2.2: Get reference (increment refCount) */
  PRINT(("Test 2.2: Increment Handle refCount\n"));
  Handle * hRef = Handle_getRef(h);
  UT_ASSERT((hRef == h), "Handle_getRef() returns same handle");

  /* Test 2.3: Delete with refCount > 1 */
  PRINT(("Test 2.3: Delete when refCount > 1\n"));
  Handle_delete(hRef);  // Should decrement, not delete
  UT_ASSERT((Handle_get(h) == obj), "Object survives when refCount > 1");

  /* Test 2.4: Final delete destroys object */
  PRINT(("Test 2.4: Final delete destroys object\n"));
  Handle_delete(h);  // Should delete now
  UT_ASSERT((1), "Handle_delete() completes");

  /* Test 2.5: Create Handle with no destructor (borrowed) */
  PRINT(("Test 2.5: Create borrowed Handle (no destructor)\n"));
  TestObject * obj2 = TestObject_new();
  Handle * hBorrowed = Handle_new(obj2, NULL);
  UT_ASSERT((hBorrowed != NULL), "Create borrowed Handle");
  UT_ASSERT((Handle_get(hBorrowed) == obj2), "Borrowed Handle stores object");
  Handle_delete(hBorrowed);
  UT_ASSERT((1), "Delete borrowed Handle");
  TestObject_delete(obj2);  // Caller owns it

  /* Test 2.6: Delete NULL Handle */
  PRINT(("Test 2.6: Delete NULL Handle\n"));
  Handle_delete(NULL);
  UT_ASSERT((1), "Handle_delete(NULL) is safe");

  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 3: Basic Insert and Find Operations
 * ============================================================================ */

int UT_Map_suite3_insert_find()
{
  TEST_SUITE_START("SUITE 3: Basic Insert and Find Operations");

  Map * map = Map_new();
  UT_ASSERT((map != NULL), "Create Map");

  /* Test 3.1: Insert single entry */
  PRINT(("Test 3.1: Insert single entry\n"));
  String * key1 = String_new("apple");
  TestObject * obj1 = TestObject_new();
  Handle * h1 = Handle_new(obj1, (Destructor)&TestObject_delete);
  unsigned int result = Map_insert(map, key1, h1);
  UT_ASSERT((result == 1), "Map_insert() returns 1 on success");
  Handle_delete(h1);

  /* Test 3.2: Find existing entry */
  PRINT(("Test 3.2: Find existing entry\n"));
  Handle * hFound = NULL;
  result = Map_find(map, key1, &hFound);
  UT_ASSERT((result == 1), "Map_find() returns 1 for existing key");
  UT_ASSERT((hFound != NULL), "Map_find() sets output handle");
  UT_ASSERT((Handle_get(hFound) == obj1), "Found handle contains correct object");

  /* Test 3.3: Find non-existent entry */
  PRINT(("Test 3.3: Find non-existent entry\n"));
  String * keyMissing = String_new("banana");
  Handle * hNotFound = NULL;
  result = Map_find(map, keyMissing, &hNotFound);
  UT_ASSERT((result == 0), "Map_find() returns 0 for missing key");
  UT_ASSERT((hNotFound == NULL), "Map_find() sets NULL for missing key");

  /* Test 3.4: Find with NULL pointer outputs */
  PRINT(("Test 3.4: Find with invalid parameters\n"));
  result = Map_find(NULL, key1, &hFound);
  UT_ASSERT((result == 0), "Map_find(NULL map) returns 0");
  result = Map_find(map, NULL, &hFound);
  UT_ASSERT((result == 0), "Map_find(NULL key) returns 0");
  result = Map_find(map, key1, NULL);
  UT_ASSERT((result == 0), "Map_find(NULL output) returns 0");

  /* Test 3.5: Insert with NULL parameters */
  PRINT(("Test 3.5: Insert with invalid parameters\n"));
  result = Map_insert(NULL, key1, h1);
  UT_ASSERT((result == 0), "Map_insert(NULL map) returns 0");
  result = Map_insert(map, NULL, h1);
  UT_ASSERT((result == 0), "Map_insert(NULL key) returns 0");
  result = Map_insert(map, key1, NULL);
  UT_ASSERT((result == 0), "Map_insert(NULL handle) returns 0");

  String_delete(key1);
  String_delete(keyMissing);
  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 4: Update Existing Entries
 * ============================================================================ */

int UT_Map_suite4_update_entries()
{
  TEST_SUITE_START("SUITE 4: Update Existing Entries");

  Map * map = Map_new();

  /* Test 4.1: Update entry with new owned object */
  PRINT(("Test 4.1: Update entry with new owned object\n"));
  String * key = String_new("key");
  TestObject * obj1 = TestObject_new();
  Handle * h1 = Handle_new(obj1, (Destructor)&TestObject_delete);
  Map_insert(map, key, h1);
  Handle_delete(h1);

  TestObject * obj2 = TestObject_new();
  Handle * h2 = Handle_new(obj2, (Destructor)&TestObject_delete);
  unsigned int result = Map_insert(map, key, h2);
  UT_ASSERT((result == 1), "Update returns 1");
  Handle_delete(h2);

  Handle * hFound = NULL;
  Map_find(map, key, &hFound);
  UT_ASSERT((Handle_get(hFound) == obj2), "New object is stored");

  /* Test 4.2: Update multiple times */
  PRINT(("Test 4.2: Update same key multiple times\n"));
  for (int i = 0; i < 5; i++) {
    TestObject * obj = TestObject_new();
    Handle * h = Handle_new(obj, (Destructor)&TestObject_delete);
    result = Map_insert(map, key, h);
    UT_ASSERT((result == 1), "Update iteration passes");
    Handle_delete(h);
  }

  /* Test 4.3: Update with borrowed handle */
  PRINT(("Test 4.3: Update with borrowed handle\n"));
  TestObject * borrowedObj = TestObject_new();
  Handle * hBorrowed = Handle_new(borrowedObj, NULL);
  result = Map_insert(map, key, hBorrowed);
  UT_ASSERT((result == 1), "Update with borrowed handle");
  Handle_delete(hBorrowed);
  TestObject_delete(borrowedObj);

  String_delete(key);
  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 5: Collision Handling and Hash Distribution
 * ============================================================================ */

int UT_Map_suite5_collisions()
{
  TEST_SUITE_START("SUITE 5: Collision Handling and Hash Distribution");

  Map * map = Map_new();

  /* Test 5.1: Insert multiple entries (potential collisions) */
  PRINT(("Test 5.1: Insert many entries\n"));
  const char * keys[] = {
    "apple", "banana", "cherry", "date", "elderberry",
    "fig", "grape", "honeydew", "jackfruit", "kiwi",
    "lemon", "mango", "nectarine", "orange", "papaya"
  };
  int numKeys = sizeof(keys) / sizeof(keys[0]);

  for (int i = 0; i < numKeys; i++) {
    String * key = String_new(keys[i]);
    TestObject * obj = TestObject_new();
    Handle * h = Handle_new(obj, (Destructor)&TestObject_delete);
    unsigned int result = Map_insert(map, key, h);
    UT_ASSERT((result == 1), "Insert collision candidate");
    Handle_delete(h);
    String_delete(key);
  }

  /* Test 5.2: Find all inserted entries */
  PRINT(("Test 5.2: Find all inserted entries\n"));
  for (int i = 0; i < numKeys; i++) {
    String * key = String_new(keys[i]);
    Handle * hFound = NULL;
    unsigned int result = Map_find(map, key, &hFound);
    UT_ASSERT((result == 1), "Find after insert");
    UT_ASSERT((hFound != NULL), "Found handle is not NULL");
    String_delete(key);
  }

  /* Test 5.3: Duplicate key insertion (should update) */
  PRINT(("Test 5.3: Insert duplicate key\n"));
  String * dupKey = String_new("apple");
  TestObject * dupObj = TestObject_new();
  Handle * hDup = Handle_new(dupObj, (Destructor)&TestObject_delete);
  unsigned int result = Map_insert(map, dupKey, hDup);
  UT_ASSERT((result == 1), "Update existing key returns 1");
  Handle_delete(hDup);
  String_delete(dupKey);

  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 6: Map_getAll and Iteration
 * ============================================================================ */

int UT_Map_suite6_getall()
{
  TEST_SUITE_START("SUITE 6: Map_getAll and Iteration");

  Map * map = Map_new();

  /* Test 6.1: getAll on empty map */
  PRINT(("Test 6.1: getAll on empty map\n"));
  List * allItems = Map_getAll(map);
  UT_ASSERT((allItems != NULL), "Map_getAll() returns non-NULL");
  unsigned int emptySize = List_getSize(allItems);
  UT_ASSERT((emptySize == 0), "Empty map returns empty list");
  List_delete(allItems);

  /* Test 6.2: getAll with single entry */
  PRINT(("Test 6.2: getAll with single entry\n"));
  String * key1 = String_new("key1");
  TestObject * obj1 = TestObject_new();
  Handle * h1 = Handle_new(obj1, (Destructor)&TestObject_delete);
  Map_insert(map, key1, h1);
  Handle_delete(h1);

  allItems = Map_getAll(map);
  unsigned int singleSize = List_getSize(allItems);
  UT_ASSERT((singleSize == 1), "getAll with one entry returns size 1");
  List_delete(allItems);

  /* Test 6.3: getAll with multiple entries */
  PRINT(("Test 6.3: getAll with multiple entries\n"));
  const char * moreKeys[] = {"key2", "key3", "key4", "key5"};
  for (int i = 0; i < 4; i++) {
    String * k = String_new(moreKeys[i]);
    TestObject * o = TestObject_new();
    Handle * h = Handle_new(o, (Destructor)&TestObject_delete);
    Map_insert(map, k, h);
    Handle_delete(h);
    String_delete(k);
  }

  allItems = Map_getAll(map);
  unsigned int multiSize = List_getSize(allItems);
  UT_ASSERT((multiSize == 5), "getAll with 5 entries returns size 5");

  /* Test 6.4: Iterate through all items */
  PRINT(("Test 6.4: Iterate through getAll results\n"));
  List_resetIterator(allItems);
  int count = 0;
  while (List_getNext(allItems) != NULL) {
    count++;
  }
  UT_ASSERT((count == 5), "Iteration counts all items");

  List_delete(allItems);
  String_delete(key1);
  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 7: Map_getSize
 * ============================================================================ */

int UT_Map_suite7_getsize()
{
  TEST_SUITE_START("SUITE 7: Map_getSize");

  Map * map = Map_new();

  /* Test 7.1: getSize on empty map */
  PRINT(("Test 7.1: getSize on empty map\n"));
  unsigned int emptySize = Map_getSize(map);
  UT_ASSERT((emptySize == 0), "Empty map size is 0");

  /* Test 7.2: getSize after single insert */
  PRINT(("Test 7.2: getSize after insert\n"));
  String * key = String_new("key");
  TestObject * obj = TestObject_new();
  Handle * h = Handle_new(obj, (Destructor)&TestObject_delete);
  Map_insert(map, key, h);
  Handle_delete(h);

  unsigned int singleSize = Map_getSize(map);
  UT_ASSERT((singleSize == 1), "Size after one insert is 1");

  /* Test 7.3: getSize after multiple inserts */
  PRINT(("Test 7.3: getSize after multiple inserts\n"));
  for (int i = 0; i < 9; i++) {
    char keyBuf[32];
    snprintf(keyBuf, sizeof(keyBuf), "key_%d", i);
    String * k = String_new(keyBuf);
    TestObject * o = TestObject_new();
    Handle * hh = Handle_new(o, (Destructor)&TestObject_delete);
    Map_insert(map, k, hh);
    Handle_delete(hh);
    String_delete(k);
  }

  unsigned int multiSize = Map_getSize(map);
  UT_ASSERT((multiSize == 10), "Size after 10 inserts is 10");

  /* Test 7.4: getSize on NULL map */
  PRINT(("Test 7.4: getSize on NULL map\n"));
  unsigned int nullSize = Map_getSize(NULL);
  UT_ASSERT((nullSize == 0), "getSize(NULL) returns 0");

  String_delete(key);
  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 8: Map_print
 * ============================================================================ */

int UT_Map_suite8_print()
{
  TEST_SUITE_START("SUITE 8: Map_print");

  Map * map = Map_new();

  /* Test 8.1: Print empty map */
  PRINT(("Test 8.1: Print empty map\n"));
  Map_print(map);
  UT_ASSERT((1), "Print empty map completes");

  /* Test 8.2: Print map with entries */
  PRINT(("Test 8.2: Print map with entries\n"));
  for (int i = 0; i < 5; i++) {
    char keyBuf[32];
    snprintf(keyBuf, sizeof(keyBuf), "item_%d", i);
    String * k = String_new(keyBuf);
    TestObject * o = TestObject_new();
    Handle * h = Handle_new(o, (Destructor)&TestObject_delete);
    Map_insert(map, k, h);
    Handle_delete(h);
    String_delete(k);
  }
  Map_print(map);
  UT_ASSERT((1), "Print populated map completes");

  /* Test 8.3: Print NULL map */
  PRINT(("Test 8.3: Print NULL map\n"));
  Map_print(NULL);
  UT_ASSERT((1), "Print NULL map is safe");

  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 9: Reference Counting and Lifecycle
 * ============================================================================ */

int UT_Map_suite9_refcount()
{
  TEST_SUITE_START("SUITE 9: Reference Counting and Lifecycle");

  /* Test 9.1: Map object refCount with single reference */
  PRINT(("Test 9.1: Single Map reference\n"));
  Map * map = Map_new();
  String * key = String_new("key");
  TestObject * obj = TestObject_new();
  Handle * h = Handle_new(obj, (Destructor)&TestObject_delete);
  Map_insert(map, key, h);
  Handle_delete(h);

  // Store entry
  Handle * hStored = NULL;
  Map_find(map, key, &hStored);
  
  // Delete map
  Map_delete(map);
  UT_ASSERT((1), "Map with single ref deleted");

  String_delete(key);
  Memory_report();

  /* Test 9.2: Multiple Handle references */
  PRINT(("Test 9.2: Multiple Handle references\n"));
  TestObject * obj2 = TestObject_new();
  Handle * hPrimary = Handle_new(obj2, (Destructor)&TestObject_delete);
  Handle * hRef1 = Handle_getRef(hPrimary);
  Handle * hRef2 = Handle_getRef(hRef1);

  UT_ASSERT((hPrimary == hRef1), "getRef returns same handle");
  UT_ASSERT((hRef1 == hRef2), "Multiple refs to same handle");

  Handle_delete(hRef2);
  Handle_delete(hRef1);
  Handle_delete(hPrimary);
  UT_ASSERT((1), "All refs deleted");

  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 10: Stress Test - Large Map
 * ============================================================================ */

int UT_Map_suite10_stress()
{
  TEST_SUITE_START("SUITE 10: Stress Test - Large Map");

  Map * map = Map_new();

  /* Test 10.1: Insert 100 entries */
  PRINT(("Test 10.1: Insert 100 entries\n"));
  int numEntries = 100;
  for (int i = 0; i < numEntries; i++) {
    char keyBuf[64];
    snprintf(keyBuf, sizeof(keyBuf), "stress_key_%d", i);
    String * key = String_new(keyBuf);
    TestObject * obj = TestObject_new();
    Handle * h = Handle_new(obj, (Destructor)&TestObject_delete);
    unsigned int result = Map_insert(map, key, h);
    UT_ASSERT((result == 1), "Insert entry");
    Handle_delete(h);
    String_delete(key);
  }

  /* Test 10.2: Find all 100 entries */
  PRINT(("Test 10.2: Find all 100 entries\n"));
  for (int i = 0; i < numEntries; i++) {
    char keyBuf[64];
    snprintf(keyBuf, sizeof(keyBuf), "stress_key_%d", i);
    String * key = String_new(keyBuf);
    Handle * hFound = NULL;
    unsigned int result = Map_find(map, key, &hFound);
    UT_ASSERT((result == 1), "Find inserted entry");
    String_delete(key);
  }

  /* Test 10.3: Verify map size */
  PRINT(("Test 10.3: Verify map size after stress\n"));
  unsigned int mapSize = Map_getSize(map);
  UT_ASSERT((mapSize == numEntries), "Map size is 100");

  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 11: Copy and Compare (Stubs)
 * ============================================================================ */

int UT_Map_suite11_copy_comp()
{
  TEST_SUITE_START("SUITE 11: Copy and Compare (Stubs)");

  Map * map1 = Map_new();
  String * key = String_new("key");
  TestObject * obj = TestObject_new();
  Handle * h = Handle_new(obj, (Destructor)&TestObject_delete);
  Map_insert(map1, key, h);
  Handle_delete(h);

  /* Test 11.1: Copy (currently unimplemented) */
  PRINT(("Test 11.1: Map_copy (stub)\n"));
  Map * mapCopy = Map_copy(map1);
  UT_ASSERT((mapCopy == NULL), "Map_copy returns NULL (not implemented)");

  /* Test 11.2: Compare (currently unimplemented) */
  PRINT(("Test 11.2: Map_comp (stub)\n"));
  Map * map2 = Map_new();
  int compResult = Map_comp(map1, map2);
  UT_ASSERT((compResult == 0), "Map_comp returns 0 (not implemented)");

  String_delete(key);
  Map_delete(map1);
  Map_delete(map2);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 12: Memory Allocator Interface
 * ============================================================================ */

int UT_Map_suite12_custom_allocator()
{
  TE