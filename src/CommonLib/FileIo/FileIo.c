#include "FileIo.h"
#include "String2.h"
#include <stdio.h>
#include <stdlib.h>

struct FileIo
{
  FILE* f;
  int status;
};

PUBLIC FileIo * FileIo_new()
{
  FileIo* this = (FileIo*)malloc(sizeof(FileIo));
  this->status = 0;
  this->f = 0;
  return this;
}

PUBLIC void FileIo_delete(FileIo* this)
{
	fclose(this->f);
	free(this);
}

PUBLIC void FileIo_openFile(FileIo * this, String * fullFileName)
{
#ifdef _WIN32
	errno_t err = fopen_s(&this->f, String_getBuffer(fullFileName), "rb+");
#else
	this->f = fopen(String_getBuffer(fullFileName), "rb"));
#endif
	if (this->f)
	{
		this->status = 1;
	}
}

PUBLIC void FileIo_openDir(FileIo* this, String* fullFileName)
{
	//this->f = fopen(String_getBuffer(fullFileName), "rb"));
	if (this->f)
	{
		this->status = 1;
	}
}

PUBLIC void FileIo_write(FileIo* this)
{

}

PUBLIC void FileIo_read(FileIo* this)
{

}

PUBLIC void FileIo_remove(FileIo* this)
{

}

PUBLIC void FileIo_createFile(FileIo* this, String * fullFileName)
{

}

PUBLIC void FileIo_createDir(FileIo* this, String * fullDirName)
{

}
