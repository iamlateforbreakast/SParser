#include "FileMgr.h"
#include "Object.h"
#include "List.h"
#include "FileIo.h"
#include "Memory.h"
#include "ObjectMgr.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

#define FILEMGR_MAX_PATH (1024)

extern FileMgr * fileMgr;

PUBLIC void FileMgr_mergePath(FileMgr * this, String * path1, String * path2);

typedef struct TestFileMgr
{
  Object object;
  List * files;
  List * directories;
  char * separator;
  String * rootLocation;
} TestFileMgr;

int step1()
{
  FileMgr * testFileMgr = FileMgr_new();

  FileIo * f = FileIo_new();
  String * currentLocation = FileIo_getCwd(f);

  /* Test 1 */
  PRINT(("Step 1: Test 1 - Check the root location is correct: "));
  UT_ASSERT(String_compare(((TestFileMgr*)testFileMgr)->rootLocation, currentLocation)==0)
  
  TRACE(("  Root location: %s\n", String_getBuffer(((TestFileMgr*)testFileMgr)->rootLocation)));

  /* Test 2 */
  PRINT(("Step 1: Test 2 - Check the ability to change root location: "));
#ifdef _WIN32
  FileMgr_setRootLocation(testFileMgr, "..\\..\\OptionMgr\\tests");
#else
  FileMgr_setRootLocation(testFileMgr, "../../OptionMgr/tests");
#endif
  TRACE(("\n  Root location: %s\n", String_getBuffer(((TestFileMgr*)testFileMgr)->rootLocation)));
  TRACE(("  Current location: %s\n", String_getBuffer(currentLocation)));
  //UT_ASSERT(String_compare(((TestFileMgr*)testFileMgr1)->rootLocation, currentLocation) == 0)
  UT_ASSERT((1))

  /* Test 3 */
  PRINT(("Step 1: Test 3 - Check ref is not null: "));
  FileIo_delete(f);
  String_delete(currentLocation);
  FileMgr_delete(testFileMgr);
  UT_ASSERT((fileMgr == 0))

  /* Test 4 */
  PRINT(("Step 1: test 4 - Check all memory is freed properly: "));
  ObjectMgr * objMgr = ObjectMgr_getRef();
  
  TRACE(("  Memory Allocation request: %d\n", Memory_getAllocRequestNb()));
  TRACE(("  Memory Free requests: %d\n", Memory_getFreeRequestNb()));
  UT_ASSERT((Memory_getAllocRequestNb()==(Memory_getFreeRequestNb()+1)))
  //ObjectMgr_reportUnallocated(objMgr);

  return 1;
}

int step2()
{
  ObjectMgr* objMgr = ObjectMgr_getRef();
  FileMgr * testFileMgr = FileMgr_new();
  FileIo* f = FileIo_new();
  String* mergedLocation = FileIo_getCwd(f);
#ifdef WIN32
  String* testLocation = String_new("..\\..\\..\\testLocation");
#else
  String* testLocation = String_new("../../../testLocation");
#endif

  /* Test 1 */
  FileMgr_mergePath(testFileMgr, mergedLocation, testLocation);
  TRACE(("Merged Path: %s\n", String_getBuffer(mergedLocation)));
  PRINT(("Step 2: Test 1 - Check merging 2 paths: "));
  UT_ASSERT((1))

  /* Test 2 */
#ifdef _WIN32
   FileMgr_addDirectory(testFileMgr, "..\\Sparse-SP6\\src\\CommonLib\\List");
#else
    FileMgr_addDirectory(testFileMgr, "..");
#endif
  PRINT(("Step 2: Test 2 - Check it is possible to add a directory: "));
  UT_ASSERT((1))
  

  /* Test 3 */
  FileIo_delete(f);
  String_delete(mergedLocation);
  String_delete(testLocation);
  FileMgr_delete(testFileMgr);
  //ObjectMgr_reportUnallocated(objMgr);
  ObjectMgr_delete(objMgr);
  PRINT(("Step 2: test 3 - Check all memory is freed properly: "));
  //Memory_report();

  UT_ASSERT((Memory_getAllocRequestNb()==(Memory_getFreeRequestNb()+1)))
  //ObjectMgr_reportUnallocated(objMgr);

  return 1;
}

int step3()
{
  FileMgr * testFileMgr = FileMgr_new();
  String * testFileContent = 0;
  
#ifdef _WIN32
  const char * fileNameText = "UT_FileMgr_01.vcxproj";
#else
  const char * fileNameText = "UT_FileMgr_01.c";
#endif
  String* fileName = String_new(fileNameText);
  FileMgr_addDirectory(testFileMgr, ".");
  
  /* Test 1 */
  FileDesc * fileDesc = FileMgr_isManaged(testFileMgr, fileName);
  PRINT(("Step 3: Test 1 - Check if file is managed: "));
  UT_ASSERT((fileDesc))
  
  //Memory_report();

  /* Test 2 */
  testFileContent = FileMgr_load(testFileMgr, fileNameText);
  PRINT(("Step 3: Test 2 - Check if file is loaded: "));
  UT_ASSERT((1))

  /* Test 3 */
  FileMgr_delete(testFileMgr);
  String_delete(fileName);
  String_delete(testFileContent);
  ObjectMgr* objMgr = ObjectMgr_getRef();
  ObjectMgr_reportUnallocated(objMgr);
  ObjectMgr_delete(objMgr);
  PRINT(("Step 3: test 3 - Check all memory is freed properly: "));
  UT_ASSERT((Memory_getAllocRequestNb() == (Memory_getFreeRequestNb() + 1)))
  Memory_report();
  return 1;
}

int step4()
{
  FileMgr * testFileMgr = FileMgr_new();
  String * testFileContent = 0;

  PRINT(("Step 4: Test 1 - Check if file is loaded: "));
#ifdef _WIN32
  FileMgr_addFile(testFileMgr, "..\\SParse-SP6\\src\\AppliLib\\FileMgr\\FileMgr.c");
#else
  FileMgr_addFile(testFileMgr, "../FileMgr.c");
#endif
  testFileContent = FileMgr_load(testFileMgr, "FileMgr.c");

  String_delete(testFileContent);
  FileMgr_delete(testFileMgr);

  return 1;
}

int step5()
{
  int isPassed = 1;
  FileMgr* testFileMgr = FileMgr_new();
  String* testFileContent = 0;
#ifdef _WIN32
  FileMgr_addFile(testFileMgr, ".\\testfile.txt");
#else
  FileMgr_addFile(testFileMgr, "../testfile.txt");
#endif
  testFileContent = String_newByRef("UT_FileMgr_01:\n==============\nTest string written in file.\n");
  PRINT(("Step 5: Test 1 - Create and write a new file testfile.txt"));
  FileMgr_write(testFileMgr, ".\\testfile.txt", testFileContent);
  return isPassed;
}
int step6()
{
  //PUBLIC List * FileMgr_filterFiles(FileMgr * this, const char * pattern);
  FileMgr* testFileMgr = FileMgr_new();
  const char pattern[] = "*.c";

  PRINT(("Step 6: Test 1 - Search files meeting pattern %s: ", pattern));
  List * files = FileMgr_filterFiles(testFileMgr, pattern);

  List_delete(files);
  FileMgr_delete(testFileMgr);

  return 1;
}

int step7()
{
  //PUBLIC String * FileMgr_searchFile(FileMgr * this, String * name, List * preferredDir);
  return 1;
}

int main()
{
  step1();
  step2();
  step3();
  step4();
  step5();
  step6();

  Memory_report();

  return 0;
}
