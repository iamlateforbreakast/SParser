#include "FileMgr.h"
#include "Object.h"
#include "List.h"
#include "FileIo.h"
#include "Memory.h"

#include <stdio.h>

#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}
#define FILEMGR_MAX_PATH (1024)

extern FileMgr * fileMgr;

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
  FileMgr * testFileMgr1 = 0;
  FileMgr * testFileMgr2 = 0;
  testFileMgr1 = FileMgr_getRef();
  testFileMgr2 = FileMgr_getRef();
  FileIo * f = FileIo_new();
  String * currentLocation = FileIo_getCwd(f);
  
  /* Test 1 */
  printf("Step 1: Test 1 - Check there is only one FileMgr: ");
  UT_ASSERT((testFileMgr1==testFileMgr2))

  /* Test 2 */
  printf("Step 1: Test 2 - Check the root location is correct: ");
  UT_ASSERT(String_compare(((TestFileMgr*)testFileMgr1)->rootLocation, currentLocation)==0)
  
  printf("Root location: %s\n", String_getBuffer(((TestFileMgr*)testFileMgr1)->rootLocation));

  /* Test 3 */
  printf("Step 1: Test 3 - Check the ability to change root location: ");
  printf("Failed\n");
  // FileMgr_setRootLocation(FileMgr

  /* Test 4 */
  printf("Step 1: Test 4 - Check ref is not null: ");
  FileIo_delete(f);
  String_delete(currentLocation);
  FileMgr_delete(testFileMgr1);
  UT_ASSERT((testFileMgr1!=0))
  
  /* Test 5 */
  printf("Step 1: Test 5 - Check ref is null: ");
  FileMgr_delete(testFileMgr2);
  UT_ASSERT((fileMgr==0))

  /* Test 6 */
  printf("Step 1: Test 6 - Check it is possible to delete a null pointer: ");
  FileMgr_delete(testFileMgr2);
  UT_ASSERT((fileMgr==0))

  /* Test 7 */
  printf("Step 1: test 7 - Check all memory is freed properly: ");
  UT_ASSERT((Memory_getAllocRequestNb()==(Memory_getFreeRequestNb()+1)))

  return 1;
}

int step2()
{
    // PUBLIC unsigned int * this, const char * location);
  //FileMgr_addDirectory(testFileMgr, "../../../../../../Solo/AUK40803_tr_solo_dev/vobs/solo/fsw");
  //FileMgr_addDirectory(testFileMgr, "..");
  //FileMgr_addDirectory(testFileMgr, "../../..");
}
int step3()
{
  FileMgr * testFileMgr = 0;
  String * testFileContent = 0;
  String * fileName = String_new("main.c");

  testFileMgr = FileMgr_getRef();

  FileMgr_addDirectory(testFileMgr, ".");
  
  if (FileMgr_isManaged(testFileMgr, fileName))
  {
    testFileContent = FileMgr_load(testFileMgr, "main.c");
  }
  FileMgr_delete(testFileMgr);

  String_delete(testFileContent);

  Memory_report();

  return 0;
}

int step4()
{
  FileMgr * testFileMgr = FileMgr_getRef();
  String * testFileContent = 0;

  FileMgr_addFile(testFileMgr, "../FileMgr.c");
  testFileContent = FileMgr_load(testFileMgr, "FileMgr.c");

  String_delete(testFileContent);
  FileMgr_delete(testFileMgr);

  Memory_report();

  return 0;
}

int step5()
{

// PUBLIC char * FileMgr_getRootLocation(FileMgr* this);
}

int step6()
{
  //PUBLIC unsigned int FileMgr_addDirectory(FileMgr * this, const char * directoryName);
  //PUBLIC List * FileMgr_filterFiles(FileMgr * this, const char * pattern);
  //PUBLIC String * FileMgr_searchFile(FileMgr * this, String * name, List * preferredDir);
}
int main()
{
  step1();
  //step2();
  //step3();
  //step4();
  //step5();
  return 0;
}
