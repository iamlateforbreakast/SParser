#include "Handle.h"
#include "Object.h"
#include "Memory.h"
#include "Debug.h"
#include "Error.h"
#include "TestObject.h"
#include "String2.h"

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
 * SUITE 1: Basic Handle Creation and Deletion
 * ============================================================================ */

int UT_Handle_suite1_create_delete()
{
  TEST_SUITE_START("SUITE 1: Handle Creation and Deletion");

  /* Test 1.1: Create a Handle with owned object */
  PRINT(("Test 1.1: Create Handle with owned object\n"));
  TestObject * obj = TestObject_new();
  Handle * h = Handle_new(obj, HANDLE_OWNER);
  UT_ASSERT((h != NULL), "Handle_new() returns non-NULL");
  UT_ASSERT((Handle_getObject(h) == obj), "Handle stores object pointer");

  /* Test 1.2: Delete owned Handle */
  PRINT(("Test 1.2: Delete owned Handle\n"));
  Handle_delete(h);
  UT_ASSERT((1), "Handle_delete() completes");

  /* Test 1.3: Create borrowed Handle */
  PRINT(("Test 1.3: Create borrowed Handle (no destructor)\n"));
  TestObject * obj2 = TestObject_new();
  Handle * hBorrowed = Handle_new(obj2, HANDLE_NOT_OWNER);
  UT_ASSERT((hBorrowed != NULL), "Create borrowed Handle");
  UT_ASSERT((Handle_getObject(hBorrowed) == obj2), "Borrowed Handle stores object");
  Handle_delete(hBorrowed);
  UT_ASSERT((1), "Delete borrowed Handle");
  TestObject_delete(obj2);  /* Caller owns it */
}

/* ============================================================================
 * SUITE 2: Basic Copy Operations
 * ============================================================================ */

int UT_Handle_suite2_copy()
{
  TEST_SUITE_START("SUITE 2: Handle Copy");

  /* Test 2.1: Copy Handle with ownership */
  PRINT(("Test 2.1: Copy Handle with ownership\n"));
  TestObject * obj = TestObject_new();
  Handle * hOwner = Handle_new(obj, HANDLE_OWNER);
  Handle * hCopyOwner = Handle_copyWithOwnership(hOwner);
  UT_ASSERT((hCopyOwner != NULL), "Handle_copyWithOwnership() returns non-NULL");
  UT_ASSERT((Handle_getObject(hCopyOwner) == obj), "Copied Handle stores same object pointer");
  Handle_delete(hOwner);
  Handle_delete(hCopyOwner);

  /* Test 2.2: Copy Handle with reference */
  PRINT(("Test 2.2: Copy Handle with reference\n"));
  TestObject * obj2 = TestObject_new();
  Handle * hRef = Handle_new(obj2, HANDLE_NOT_OWNER);
  Handle * hCopyRef = Handle_copyWithReference(hRef);
  UT_ASSERT((hCopyRef != NULL), "Handle_copyWithReference() returns non-NULL");
  UT_ASSERT((Handle_getObject(hCopyRef) == obj2), "Copied reference Handle stores same object pointer");
  Handle_delete(hRef);
  Handle_delete(hCopyRef);
}

void main()
{
  UT_Handle_suite1_create_delete();
  UT_Handle_suite2_copy();
  Memory_report();
}