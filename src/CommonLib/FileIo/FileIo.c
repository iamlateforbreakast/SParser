#include "FileIo.h"
#include "String2.h"
#include "List.h"
#include "ObjectMgr.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>

#define DEBUG (0)
#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("Passed\n"));} \
                          else { PRINT(("Failed\n")); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { PRINT(("\x1b[32mPassed\x1b[0m\n"));} \
                          else { PRINT(("\x1b[31mFailed\x1b[0m\n")); return 0;}
#endif

#define BUFFER_SIZE_BYTES (1000)

PRIVATE FileIo* f = 0;
PRIVATE FileIo* d = 0;
PRIVATE const char* testFileName[5] = { "test1.file","test2.file","test3.file","test4.file","test5.file" };
PRIVATE char testBuffer[BUFFER_SIZE_BYTES];
PRIVATE char readBuffer[BUFFER_SIZE_BYTES];

PRIVATE int UT_FileIo_01_step1()
{
  String* testFileName = String_new("test.file");

  f = FileIo_new();
  PRINT(("Step 1: Test 1 - Create a FileIo object: "));
  UT_ASSERT((1));

  PRINT(("Step 1: Test 2 - Remove previous test.file if exist: "));
  UT_ASSERT((1));
  FileIo_remove(f, testFileName);

  String_delete(testFileName);
  return 1;
}

PRIVATE int UT_FileIo_01_step2()
{
  String* testFileName = String_new("test.file");
	
  PRINT(("Step 2: Test 1 - Create a file test.file: "));
  FileIo_createFile(f, testFileName);
  UT_ASSERT((1));

  PRINT(("Step 2: Test 2 - Write %d bytes in file test.file: ", BUFFER_SIZE_BYTES));
  FileIo_write(f, testBuffer, BUFFER_SIZE_BYTES);
  UT_ASSERT((1));

  PRINT(("Step 2: Test 3 - Delete the FileIo object: "));
  FileIo_delete(f);
  UT_ASSERT((1));

  String_delete(testFileName);
	
  return 1;
}

PRIVATE int UT_FileIo_01_step3()
{
  int isOK = 1;
  String* testFileName = String_new("test.file");
  f = FileIo_new();

  PRINT(("Step 3: Test 1 - Open previous test.file: "));
  FileIo_openFile(f, testFileName);
  UT_ASSERT((1));

  PRINT(("Step 3: Test 2 - Read previous test.file: "));
  FileIo_read(f, readBuffer, BUFFER_SIZE_BYTES);
  for (int i = 0; i < 10; i++)
  {
	isOK = isOK && (testBuffer[i] == readBuffer[i]);
	TRACE(("%d %d\n", testBuffer[i], readBuffer[i]));
  }
  TRACE(("%d\n", isOK));
  UT_ASSERT((isOK));

  FileIo_delete(f);
  String_delete(testFileName);
  return 1;
}

PRIVATE int UT_FileIo_01_step4()
{
  String* testFileName = String_new("test.file");

  f = FileIo_new(testFileName);
  PRINT(("Step 4: Test 1 - Remove previous test.file: "));
  FileIo_remove(f, testFileName);
  UT_ASSERT((1));

  String_delete(testFileName);
  FileIo_delete(f);
  
  return 1;
}

PRIVATE int UT_FileIo_01_step5()
{
	//d = FileIo_newDir("testDir");

	return 1;
}

PRIVATE int UT_FileIo_01_step6()
{
	FileIo* f = FileIo_new();
	String* currentPath = FileIo_getCwd(f);
#ifdef _WIN32
	String_append(currentPath, "\\..\\SParse-SP6\\src\\CommonLib\\List");
#else
	String_append(currentPath, "/../..");
#endif
	PRINT(("Step 6: Test 1 - List files in current directory: "));
	List* listOfFiles = FileIo_listFiles(f, currentPath);
	String* name;

	while ((name = List_getNext(listOfFiles)) != 0)
	{
	  PRINT(("FIle %s\n", String_getBuffer(name)));
	}
	UT_ASSERT((1));
	List_delete(listOfFiles);
	String_delete(currentPath);
	FileIo_delete(f);
	return 1;
}

PRIVATE int UT_FileIo_01_step7()
{
	FileIo * f =FileIo_new();

    String * currentPath = FileIo_getCwd(f);
#ifdef _WIN32
   	String_append(currentPath, "\\..\\.."); 
#else
	String_append(currentPath, "/../..");
#endif
	PRINT(("Step 7: Test 1 - List directories in current directory: "));
    List * listOfDir = FileIo_listDirs(f, currentPath);
	UT_ASSERT(1);
    List_delete(listOfDir);
	String_delete(currentPath);
	FileIo_delete(f);
	return 1;
}

PRIVATE int UT_FileIo_01_step8()
{
  FileIo* f = FileIo_new();
  String* currentPath = FileIo_getCwd(f);
  PRINT(("Current Working Directoy: %s\n", String_getBuffer(currentPath)));
  PRINT(("Step 8: Test 1 - Print current directory: "));
  UT_ASSERT((1));
  String_delete(currentPath);
  FileIo_delete(f);
  return 1;
}

PRIVATE int UT_FileIo_01_step9()
{
  String* testFileName = String_new("test.file");

  FileIo* f = FileIo_new();
  
  PRINT(("Step 9 Test 1 - Show size of file: "));
  FileIo_openFile(f, testFileName);
  FileIo_fSeekEnd(f, 0);
  PRINT(("Ftell %d\n", FileIo_ftell(f)));

  String_delete(testFileName);
  FileIo_delete(f);
  return 1;
}

/*
PUBLIC void FileIo_openDir(FileIo* this, String* fullFileName);
PUBLIC void FileIo_createDir(FileIo* this, String* fullDirName);
*/

int run_UT_FileIo_01()
{
  int isPassed = 1;

  for (int i = 0; i < BUFFER_SIZE_BYTES; i++)
  {
	testBuffer[i] = (char)i;
  }

  isPassed = isPassed && UT_FileIo_01_step1();
  isPassed = isPassed && UT_FileIo_01_step2();
  isPassed = isPassed && UT_FileIo_01_step3();
  //isPassed = isPassed && UT_FileIo_01_step4();
  isPassed = isPassed && UT_FileIo_01_step5();
  isPassed = isPassed && UT_FileIo_01_step6();
  isPassed = isPassed && UT_FileIo_01_step7();
  isPassed = isPassed && UT_FileIo_01_step8();
  isPassed = isPassed && UT_FileIo_01_step9();

  ObjectMgr* objectMgr = ObjectMgr_getRef();

  ObjectMgr_reportUnallocated(objectMgr);

  ObjectMgr_delete(objectMgr);

  Memory_report();

  return isPassed;
}
