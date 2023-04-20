#include "FileIo.h"
#include "String2.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#else
#include <unistd.h>
#endif
struct FileIo
{
  FILE* f;
  int status;
};

PUBLIC FileIo * FileIo_new()
{
  FileIo* this = (FileIo*)malloc(sizeof(FileIo));
  this->status = UNKNOWN;
  this->f = 0;
  return this;
}

PUBLIC void FileIo_delete(FileIo* this)
{
  if (this)
  {
	  if (this->status == FILE_OPEN)
	  {
		  fclose(this->f);
	  }
	free(this);
  }
}

PUBLIC void FileIo_openFile(FileIo * this, String * fullFileName)
{
#ifdef _WIN32
  errno_t err = fopen_s(&this->f, "test.file" /*String_getBuffer(fullFileName)*/, "rb+");
  printf("Errno: %d\n", err);
#else
  this->f = fopen(String_getBuffer(fullFileName), "rb+");
#endif
  if (this->f)
  {
	this->status = 1;
  }
}

PUBLIC void FileIo_createFile(FileIo* this, String* fullFileName)
{
#ifdef _WIN32
	errno_t err = fopen_s(&this->f, "test.file" /*String_getBuffer(fullFileName)*/, "wb");
	printf("Errno: %d\n", err);
#else
	this->f = fopen("test.file" /*String_getBuffer(fullFileName)*/, "wb");
#endif
	if (this->f)
	{
		this->status = FILE_OPEN;
	}
}

PUBLIC void FileIo_openDir(FileIo* this, String* fullFileName)
{
	//this->f = fopen(String_getBuffer(fullFileName), "rb"));
	if (this->f)
	{
		this->status = FILE_OPEN;
	}
}

PUBLIC void FileIo_write(FileIo* this, char * buffer, int length)
{
  if ((this) && (this->status == FILE_OPEN))
  {
	int l = fwrite(buffer, length, 1, this->f);
	printf("Length=%d\n", l);
  }
}

PUBLIC void FileIo_read(FileIo* this, char * buffer, int length)
{
  if ((this) && (this->status == FILE_OPEN))
  {
	fread(buffer, length, 1, this->f);
  }
}

PUBLIC void FileIo_remove(FileIo* this)
{

}

PUBLIC void FileIo_createDir(FileIo* this, String * fullDirName)
{

}

PUBLIC List* FileIo_listDir(FileIo* this)
{
#ifdef _WIN32
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	TCHAR dir[MAX_PATH];

	char * dirText = "C:\\Users\\remion_t\\source\\repos\\SParse-SP6\\UT_FileIo_01\\*";
	hFind = FindFirstFile(_T("*"), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return;
	}
	else
	{
		_tprintf(_T("The first file found is %s\n"),FindFileData.cFileName);
		
		
		do {
			_tprintf(_T("The first file found is %s\n"), FindFileData.cFileName);
			char* text[100];
			size_t nb = 0;
			wcstombs_s(&nb, text, 100, FindFileData.cFileName, wcslen(FindFileData.cFileName));
			printf("=>%s\n", text);
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
#else

#endif
}
