#include "FileIo.h"

#define BUFFER_SIZE (1000)

FileIo * f = 0;
FileIo * d = 0;
const char* testFileName[5] = { "test1.file","test2.file","test3.file","test4.file","test5.file" };
char testBuffer[BUFFER_SIZE];
char* readBuffer;

int step1()
{
  remove(testFileName);
  f = FileIo_new(testFileName[0]);
} 

int step2()
{
  FileIo_write(f, testBuffer, BUFFER_SIZE);
  FileIo_delete(f);
}

int step3()
{
  f = FileIo_new(testFileName);
  FileIo_read(f, readBuffer, BUFFER_SIZE);
  FileIo_delete(f);
}

int step4()
{
  f	= FileIo_new(testFileName);
  FileIo_remove(testFileName);
}

int step5()
{
	//d = FileIo_newDir("testDir");
}

void main()
{
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		testBuffer[i] = (char)i;
	}

	step1();
	step2();
	step3();
	step4();
}
