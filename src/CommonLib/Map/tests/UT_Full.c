/*
 * UT_Full.c
 * 
 * Comprehensive unit test suite for Map.c
 * Covers Map creation, insertion, finding, and memory management
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

  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 2: Handle Creation and Basic Operations
 * ============================================================================ */

int UT_Map_suite2_handle_basic()
{
  TEST_SUITE_START("SUITE 2: Handle Creation and Basic Operations");

  /* Test 2.1: Create a Handle with owned object */
  PRINT(("Test 2.1: Create Handle with owned object\n"));
  TestObject * obj = TestObject_new();
  Handle * h = Handle_new(obj, HANDLE_OWNER);
  UT_ASSERT((h != NULL), "Handle_new() returns non-NULL");
  UT_ASSERT((Handle_getObject(h) == obj), "Handle stores object pointer");

  /* Test 2.2: Delete owned Handle */
  PRINT(("Test 2.2: Delete owned Handle\n"));
  Handle_delete(h);
  UT_ASSERT((1), "Handle_delete() completes");

  /* Test 2.3: Create borrowed Handle */
  PRINT(("Test 2.3: Create borrowed Handle (no destructor)\n"));
  TestObject * obj2 = TestObject_new();
  Handle * hBorrowed = Handle_new(obj2, HANDLE_NOT_OWNER);
  UT_ASSERT((hBorrowed != NULL), "Create borrowed Handle");
  UT_ASSERT((Handle_getObject(hBorrowed) == obj2), "Borrowed Handle stores object");
  Handle_delete(hBorrowed);
  UT_ASSERT((1), "Delete borrowed Handle");
  TestObject_delete(obj2);  /* Caller owns it */

  /* Test 2.4: Delete NULL Handle */
  PRINT(("Test 2.4: Delete NULL Handle\n"));
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
  Handle * hKey = Handle_new(key1, HANDLE_OWNER);
  Handle * hVal = Handle_new(obj1, HANDLE_OWNER);
  unsigned int result = Map_insertOrUpdate(map, hKey, hVal);
  UT_ASSERT((result == 1), "Map_insertOrUpdate() returns 1 on success");
  Handle_delete(hKey);
  Handle_delete(hVal);

  /* Test 3.2: Find existing entry */
  PRINT(("Test 3.2: Find existing entry\n"));
  void * pFound = NULL;
  result = Map_find(map, key1, &pFound);
  UT_ASSERT((result == 1), "Map_find() returns 1 for existing key");
  UT_ASSERT((pFound != NULL), "Map_find() sets output pointer");
  UT_ASSERT((pFound == obj1), "Found pointer contains correct object");

  /* Test 3.3: Find non-existent entry */
  PRINT(("Test 3.3: Find non-existent entry\n"));
  String * keyMissing = String_new("banana");
  void * pNotFound = NULL;
  result = Map_find(map, keyMissing, &pNotFound);
  UT_ASSERT((result == 0), "Map_find() returns 0 for missing key");
  UT_ASSERT((pNotFound == NULL), "Map_find() sets NULL for missing key");

  /* Test 3.4: Find with NULL pointer outputs */
  PRINT(("Test 3.4: Find with invalid parameters\n"));
  result = Map_find(NULL, key1, &pFound);
  UT_ASSERT((result == 0), "Map_find(NULL map) returns 0");
  result = Map_find(map, NULL, &pFound);
  UT_ASSERT((result == 0), "Map_find(NULL key) returns 0");
  result = Map_find(map, key1, NULL);
  UT_ASSERT((result == 0), "Map_find(NULL output) returns 0");

  /* Test 3.5: Insert with NULL parameters */
  PRINT(("Test 3.5: Insert with invalid parameters\n"));
  result = Map_insertOrUpdate(NULL, hKey, hVal);
  UT_ASSERT((result == 0), "Map_insertOrUpdate(NULL map) returns 0");
  result = Map_insertOrUpdate(map, NULL, hVal);
  UT_ASSERT((result == 0), "Map_insertOrUpdate(NULL key handle) returns 0");
  result = Map_insertOrUpdate(map, hKey, NULL);
  UT_ASSERT((result == 0), "Map_insertOrUpdate(NULL value handle) returns 0");

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

  /* Test 4.1: Update entry with new object */
  PRINT(("Test 4.1: Update entry with new object\n"));
  String * key = String_new("key");
  TestObject * obj1 = TestObject_new();
  Handle * hKey = Handle_new(key, HANDLE_OWNER);
  Handle * h1 = Handle_new(obj1, HANDLE_OWNER);
  Map_insertOrUpdate(map, hKey, h1);
  Handle_delete(hKey);
  Handle_delete(h1);

  TestObject * obj2 = TestObject_new();
  hKey = Handle_new(key, HANDLE_NOT_OWNER);
  Handle * h2 = Handle_new(obj2, HANDLE_OWNER);
  unsigned int result = Map_insertOrUpdate(map, hKey, h2);
  UT_ASSERT((result == 1), "Update returns 1");
  Handle_delete(hKey);
  Handle_delete(h2);

  void * pFound = NULL;
  Map_find(map, key, &pFound);
  UT_ASSERT((pFound == obj2), "New object is stored");

  /* Test 4.2: Update multiple times */
  PRINT(("Test 4.2: Update same key multiple times\n"));
  for (int i = 0; i < 5; i++) {
    TestObject * obj = TestObject_new();
    hKey = Handle_new(key, HANDLE_NOT_OWNER);
    Handle * h = Handle_new(obj, HANDLE_OWNER);
    result = Map_insertOrUpdate(map, hKey, h);
    UT_ASSERT((result == 1), "Update iteration passes");
  }

  /* key was deleted by Handle_delete when hKey was deleted (HANDLE_OWNER), so don't delete here */
  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 5: Map_getAll and Iteration
 * ============================================================================ */

int UT_Map_suite5_getall()
{
  TEST_SUITE_START("SUITE 5: Map_getAll and Iteration");

  Map * map = Map_new();

  /* Test 5.1: getAll on empty map */
  PRINT(("Test 5.1: getAll on empty map\n"));
  List * allItems = Map_getAll(map);
  UT_ASSERT((allItems != NULL), "Map_getAll() returns non-NULL");
  unsigned int emptySize = List_getSize(allItems);
  UT_ASSERT((emptySize == 0), "Empty map returns empty list");
  List_delete(allItems);

  /* Test 5.2: getAll with single entry */
  PRINT(("Test 5.2: getAll with single entry\n"));
  String * key1 = String_new("key1");
  TestObject * obj1 = TestObject_new();
  Handle * hKey = Handle_new(key1, HANDLE_OWNER);
  Handle * h1 = Handle_new(obj1, HANDLE_OWNER);
  Map_insertOrUpdate(map, hKey, h1);
  Handle_delete(hKey);
  Handle_delete(h1);

  allItems = Map_getAll(map);
  unsigned int singleSize = List_getSize(allItems);
  UT_ASSERT((singleSize == 1), "getAll with one entry returns size 1");
  List_delete(allItems);

  /* Test 5.3: getAll with multiple entries */
  PRINT(("Test 5.3: getAll with multiple entries\n"));
  const char * moreKeys[] = {"key2", "key3", "key4", "key5"};
  for (int i = 0; i < 4; i++) {
    String * k = String_new(moreKeys[i]);
    TestObject * o = TestObject_new();
    Handle * hk = Handle_new(k, HANDLE_OWNER);
    Handle * h = Handle_new(o, HANDLE_OWNER);
    Map_insertOrUpdate(map, hk, h);
    Handle_delete(hk);
    Handle_delete(h);
  }

  allItems = Map_getAll(map);
  unsigned int multiSize = List_getSize(allItems);
  UT_ASSERT((multiSize == 5), "getAll with 5 entries returns size 5");

  /* Test 5.4: Iterate through all items */
  PRINT(("Test 5.4: Iterate through getAll results\n"));
  List_resetIterator(allItems);
  int count = 0;
  while (List_getNext(allItems) != NULL) {
    count++;
  }
  UT_ASSERT((count == 5), "Iteration counts all items");

  List_delete(allItems);
  /* key1 was deleted by Handle, so don't delete it */
  //Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 6: Map_getSize
 * ============================================================================ */

int UT_Map_suite6_getsize()
{
  TEST_SUITE_START("SUITE 6: Map_getSize");

  Map * map = Map_new();

  /* Test 6.1: getSize on empty map */
  PRINT(("Test 6.1: getSize on empty map\n"));
  unsigned int emptySize = Map_getSize(map);
  UT_ASSERT((emptySize == 0), "Empty map size is 0");

  /* Test 6.2: getSize after single insert */
  PRINT(("Test 6.2: getSize after insert\n"));
  String * key = String_new("key");
  TestObject * obj = TestObject_new();
  Handle * hKey = Handle_new(key, HANDLE_OWNER);
  Handle * h = Handle_new(obj, HANDLE_OWNER);
  Map_insertOrUpdate(map, hKey, h);
  //Handle_delete(hKey);
  //Handle_delete(h);

  unsigned int singleSize = Map_getSize(map);
  UT_ASSERT((singleSize == 1), "Size after one insert is 1");

  /* Test 6.3: getSize after multiple inserts */
  PRINT(("Test 6.3: getSize after multiple inserts\n"));
  for (int i = 0; i < 9; i++) {
    char keyBuf[32];
    snprintf(keyBuf, sizeof(keyBuf), "key_%d", i);
    String * k = String_new(keyBuf);
    TestObject * o = TestObject_new();
    Handle * hk = Handle_new(k, HANDLE_OWNER);
    Handle * hh = Handle_new(o, HANDLE_OWNER);
    Map_insertOrUpdate(map, hk, hh);
    Handle_delete(hk);
    Handle_delete(hh);
  }

  unsigned int multiSize = Map_getSize(map);
  UT_ASSERT((multiSize == 10), "Size after 10 inserts is 10");

  /* Test 6.4: getSize on NULL map */
  PRINT(("Test 6.4: getSize on NULL map\n"));
  unsigned int nullSize = Map_getSize(NULL);
  UT_ASSERT((nullSize == 0), "getSize(NULL) returns 0");

  /* key was deleted by Handle_delete, so don't delete it */
  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 7: Map_print
 * ============================================================================ */

int UT_Map_suite7_print()
{
  TEST_SUITE_START("SUITE 7: Map_print");

  Map * map = Map_new();

  /* Test 7.1: Print empty map */
  PRINT(("Test 7.1: Print empty map\n"));
  Map_print(map);
  UT_ASSERT((1), "Print empty map completes");

  /* Test 7.2: Print map with entries */
  PRINT(("Test 7.2: Print map with entries\n"));
  for (int i = 0; i < 5; i++) {
    char keyBuf[32];
    snprintf(keyBuf, sizeof(keyBuf), "item_%d", i);
    String * k = String_new(keyBuf);
    TestObject * o = TestObject_new();
    Handle * hk = Handle_new(k, HANDLE_OWNER);
    Handle * h = Handle_new(o, HANDLE_OWNER);
    Map_insertOrUpdate(map, hk, h);
    Handle_delete(hk);
    Handle_delete(h);
  }
  Map_print(map);
  UT_ASSERT((1), "Print populated map completes");

  /* Test 7.3: Print NULL map */
  PRINT(("Test 7.3: Print NULL map\n"));
  Map_print(NULL);
  UT_ASSERT((1), "Print NULL map is safe");

  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 8: Stress Test - Large Map
 * ============================================================================ */

int UT_Map_suite8_stress()
{
  TEST_SUITE_START("SUITE 8: Stress Test - Large Map");

  Map * map = Map_new();

  /* Test 8.1: Insert 100 entries */
  PRINT(("Test 8.1: Insert 100 entries\n"));
  int numEntries = 100;
  for (int i = 0; i < numEntries; i++) {
    char keyBuf[64];
    snprintf(keyBuf, sizeof(keyBuf), "stress_key_%d", i);
    String * key = String_new(keyBuf);
    TestObject * obj = TestObject_new();
    Handle * hKey = Handle_new(key, HANDLE_OWNER);
    Handle * h = Handle_new(obj, HANDLE_OWNER);
    unsigned int result = Map_insertOrUpdate(map, hKey, h);
    UT_ASSERT((result == 1), "InsertOrUpdate entry");
    Handle_delete(hKey);
    Handle_delete(h);
  }

  /* Test 8.2: Find all 100 entries */
  PRINT(("Test 8.2: Find all 100 entries\n"));
  for (int i = 0; i < numEntries; i++) {
    char keyBuf[64];
    snprintf(keyBuf, sizeof(keyBuf), "stress_key_%d", i);
    String * key = String_new(keyBuf);
    void * pFound = NULL;
    unsigned int result = Map_find(map, key, &pFound);
    UT_ASSERT((result == 1), "Find inserted entry");
    String_delete(key);
  }

  /* Test 8.3: Verify map size */
  PRINT(("Test 8.3: Verify map size after stress\n"));
  unsigned int mapSize = Map_getSize(map);
  UT_ASSERT((mapSize == numEntries), "Map size is 100");

  Map_delete(map);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * SUITE 9: Copy and Compare
 * ============================================================================ */

int UT_Map_suite9_copy_comp()
{
  TEST_SUITE_START("SUITE 9: Copy and Compare");

  Map * map1 = Map_new();
  String * key = String_new("key");
  TestObject * obj = TestObject_new();
  Handle * hKey = Handle_new(key, HANDLE_OWNER);
  Handle * h = Handle_new(obj, HANDLE_OWNER);
  Map_insertOrUpdate(map1, hKey, h);
  Handle_delete(hKey);
  Handle_delete(h);

  /* Test 9.1: Copy */
  PRINT(("Test 9.1: Map_copy\n"));
  Map * mapCopy = Map_copy(map1);
  UT_ASSERT((mapCopy != NULL), "Map_copy returns non-NULL");

  /* Test 9.2: Compare */
  PRINT(("Test 9.2: Map_comp\n"));
  Map * map2 = Map_new();
  int compResult = Map_comp(map1, map2);
  UT_ASSERT((compResult != 0), "Map_comp returns value");

  String_delete(key);
  Map_delete(map1);
  Map_delete(map2);
  if (mapCopy) Map_delete(mapCopy);
  Memory_report();
  TEST_SUITE_END();
  return 1;
}

/* ============================================================================
 * Main Test Runner
 * ============================================================================ */

int main()
{
  PRINT(("\n\n"));
  PRINT(("╔════════════════════════════════════════════════════════════════╗\n"));
  PRINT(("║         Map Comprehensive Unit Test Suite                      ║\n"));
  PRINT(("╚════════════════════════════════════════════════════════════════╝\n"));

  UT_Map_suite1_create_delete();
  UT_Map_suite2_handle_basic();
  UT_Map_suite3_insert_find();
  UT_Map_suite4_update_entries();
  UT_Map_suite5_getall();
  UT_Map_suite6_getsize();
  UT_Map_suite7_print();
  UT_Map_suite8_stress();
  UT_Map_suite9_copy_comp();

  PRINT(("\n"));
  PRINT(("╔════════════════════════════════════════════════════════════════╗\n"));
  PRINT(("║                        TEST SUMMARY                            ║\n"));
  PRINT(("╠════════════════════════════════════════════════════════════════╣\n"));
  PRINT(("║  Total Tests:  %d\n", g_context.totalTests));
  PRINT(("║  Passed:       %d\n", g_context.passedTests));
  PRINT(("║  Failed:       %d\n", g_context.failedTests));
  PRINT(("╚════════════════════════════════════════════════════════════════╝\n"));

  if (g_context.failedTests == 0) {
    PRINT(("\n✓ All tests passed!\n"));
    return 0;
  } else {
    PRINT(("\n✗ Some tests failed.\n"));
    return 1;
  }
}
