#include "FileIo.h"
#include "String2.h"
#include "List.h"

#define BUFFER_SIZE_BYTES (1000)

FileIo* f = 0;
FileIo* d = 0;
const char* testFileName[5] = { "test1.file","test2.file","test3.file","test4.file","test5.file" };
char testBuffer[BUFFER_SIZE_BYTES];
char readBuffer[BUFFER_SIZE_BYTES];

int step1()
{
	f = FileIo_new();
	FileIo_remove(f, testFileName);

	return 1;
}

int step2()
{
	String* testFileName = String_new("test.file");
	FileIo_createFile(f, testFileName);
	FileIo_write(f, &testBuffer, BUFFER_SIZE_BYTES);
	FileIo_delete(f);

	return 1;
}

int step3()
{
	int isOK = 1;
	String* testFileName = String_new("test.file");
	f = FileIo_new();
	FileIo_openFile(f, testFileName);
	FileIo_read(f, &readBuffer, BUFFER_SIZE_BYTES);
	for (int i = 0; i < 10; i++)
	{
		isOK = isOK && (testBuffer[i] == readBuffer[i]);
		printf("%d %d\n", testBuffer[i], readBuffer[i]);
	}
	printf("%d\n", isOK);
	FileIo_delete(f);

	return 1;
}

int step4()
{
	f = FileIo_new(testFileName);
	FileIo_remove(testFileName);

	return 1;
}

int step5()
{
	//d = FileIo_newDir("testDir");

	return 1;
}

int step6()
{
	FileIo* f = 0;
	List* listOfFiles = FileIo_listDir(f);
	
	return 1;
}

void main()
{
	for (int i = 0; i < BUFFER_SIZE_BYTES; i++)
	{
		testBuffer[i] = (char)i;
	}

	step1();
	step2();
	step3();
	step4();
	step5();
	step6();
}
