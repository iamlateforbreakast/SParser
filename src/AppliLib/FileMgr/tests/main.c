#include "FileMgr.h"
#include "Object.h"
#include "List.h"
#include "Memory.h"

#include <stdio.h>

#define FILEMGR_MAX_PATH (1024)

typedef struct TestFileMgr
{
  Object object;
  List * files;
  List * directories;
  char rootLocation[FILEMGR_MAX_PATH];
} TestFileMgr;

int step1()
{
  FileMgr * testFileMgr = 0;

  testFileMgr = FileMgr_getRef();
  printf("Root location: %s\n", (((TestFileMgr*)testFileMgr)->rootLocation));

  //FileMgr_addDirectory(testFileMgr, "../../../../../../Solo/AUK40803_tr_solo_dev/vobs/solo/fsw");
  FileMgr_addDirectory(testFileMgr, "..");
  //FileMgr_addDirectory(testFileMgr, "../../..");

  FileMgr_delete(testFileMgr);

  Memory_report();

  return 0;
}

int step2()
{
  FileMgr * testFileMgr = 0;
  String * testFileContent = 0;

  testFileMgr = FileMgr_getRef();

  FileMgr_addDirectory(testFileMgr, ".");

  testFileContent = FileMgr_load(testFileMgr, "main.c");

  FileMgr_delete(testFileMgr);

  String_delete(testFileContent);

  Memory_report();

  return 0;
}

int step3()
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

int step4()
{
// PUBLIC unsigned int FileMgr_setRootLocation(FileMgr* this, const char * location);
// PUBLIC char * FileMgr_getRootLocation(FileMgr* this);
}

int step5()
{
  //PUBLIC unsigned int FileMgr_addDirectory(FileMgr * this, const char * directoryName);
  //PUBLIC List * FileMgr_filterFiles(FileMgr * this, const char * pattern);
  //PUBLIC String * FileMgr_searchFile(FileMgr * this, String * name, List * preferredDir);
}
int main()
{
  step1();
  step2();
  //step3();
  //step4();
  //step5();
  return 0;
}
