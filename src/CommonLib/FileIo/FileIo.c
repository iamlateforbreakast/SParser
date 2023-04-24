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
  if (this)
  {
    this->status = UNKNOWN;
    this->f = 0;
  }
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
  this->f = fopen(String_getBuffer(fullFileName), "wb");
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
    this->f = fopen(String_getBuffer(fullFileName), "wb");
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

PUBLIC void FileIo_remove(FileIo* this, String * fullFileName)
{
    remove(String_getBuffer(fullFileName));
}

PUBLIC void FileIo_createDir(FileIo* this, String * fullDirName)
{

}

PUBLIC List* FileIo_listDir(FileIo* this)
{
    List* result = List_new();
#ifdef _WIN32
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    TCHAR dir[MAX_PATH];

    char * dirText = "C:\\Users\\remion_t\\source\\repos\\SParse-SP6\\UT_FileIo_01\\*";
    hFind = FindFirstFile(_T("*"), &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("FindFirstFile failed (%d)\n", GetLastError());
    }
    else
    {        
        do {
            //_tprintf(_T("The first file found is %s\n"), FindFileData.cFileName);
            char text[MAX_PATH];
            size_t nb = 0;
            wcstombs_s(&nb, text, 100, FindFileData.cFileName, wcslen(FindFileData.cFileName));
            printf("=>%s\n", text);
            String* s = String_new("text");
            List_insertTail(result, s);
        } while (FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }
#else
  struct dirent *directoryEntry = 0;
  FileIo * dir = 0;
  //FileDesc * fileDesc= 0;
  String * fullName = 0;
  String * name = 0;
  
  //dir = FileIo_new(String_getBuffer(directory));
  
#if 0
  if (dir!=0)
  {
    while ((directoryEntry = readdir(dir)) != NULL) 
    {
      if (directoryEntry->d_type != DT_DIR)
      {
        fileDesc = FileDesc_new();
        fullName = String_copy(directory);
        name = String_new(directoryEntry->d_name);
        FileMgr_mergePath(this, fullName, name);
        FileDesc_setFullName(fileDesc, fullName);
        List_insertHead(this->files, (void*)fileDesc);
        String_delete(name);
      }
      else
      {
        if ((Memory_ncmp(directoryEntry->d_name,"..",2)==0) 
           && (Memory_ncmp(directoryEntry->d_name,".",1)==0))
        {
          fullName = String_copy(directory);
          name = String_new(directoryEntry->d_name);
          FileMgr_mergePath(this, fullName, name);
          List_insertHead(this->directories,fullName);
          String_delete(name);
        }
      }
    }
  }
#endif

#endif
    return result;
}

int FileIo_fSeekEnd(FileIo * this, int pos)
{

}

int FileIo_fSeekSet(FileIo * this, int pos)
{

}

String * FileIo_getcwd(FileIo * this)
{
  
}