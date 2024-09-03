/**********************************************//**
  @file FileIo.c

  @brief A FileIo class.
  This class provides a status and operation for 
  various File I/O operations.
**************************************************/
#include "FileIo.h"
#include "String2.h"
#include "List.h"
#include "Object.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#define DEBUG (0)

/**********************************************//**
  @class FileiIo
**************************************************/
struct FileIo
{
  Object object;
  FILE* f;
  int status;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class fileIoClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&FileIo_delete,
  .f_copy = (Copy_Operator)&FileIo_copy,
  .f_comp = (Comp_Operator)&FileIo_comp,
  .f_print = (Printer)&FileIo_print,
  .f_size = (Sizer)&FileIo_getSize
};

/********************************************************//**
  @brief Create a new instance of the class FileIo.
  @public
  @memberof FileIo
  @return New FileIo instance or NULL if failed to allocate.
*************************************************************/
PUBLIC FileIo * FileIo_new()
{
  FileIo* this = (FileIo*)Object_new(sizeof(FileIo), &fileIoClass);

  if (this)
  {
    this->status = UNKNOWN;
    this->f = 0;
  }
  return this;
}

/********************************************************//**
  @brief Delte an instance of the class FileIo.
  @public
  @memberof FileIo
  @return none
*************************************************************/
PUBLIC void FileIo_delete(FileIo* this)
{
  if (this)
  {
    if (this->status == FILE_OPEN)
    {
      fclose(this->f);
    }
    Object_deallocate(&this->object);
  }
}

/********************************************************//**
  @brief Copy an instance of the class FileIo.
  @public
  @memberof FileIo
  @return Copy of the instance.
*************************************************************/
PUBLIC FileIo* FileIo_copy(FileIo* this)
{
  return 0;
}

/********************************************************//**
  @brief Compare an instance of the class FileIo to another one.
  @public
  @memberof FileIo
  @return 0 if equal.
*************************************************************/
PUBLIC int FileIo_comp(FileIo* this, FileIo* compare)
{
  return 0;
}

/********************************************************//**
  @brief Print an instance of the class FileIo.
  @public
  @memberof FileIo
  @return none.
*************************************************************/
PUBLIC void FileIo_print(FileIo* this)
{
}

/********************************************************//**
  @brief Return the size of an instance of the class FileIo.
  @public
  @memberof FileIo
  @return Size in bytes.
*************************************************************/
PUBLIC unsigned int FileIo_getSize(FileIo* this)
{
  if (this == 0) return sizeof(FileIo);

  return sizeof(FileIo);
}

/********************************************************//**
  @brief Open an instance of the class FileIo for reading/writing.
  @public
  @memberof FileIo
  @param[in] String Full path of file to open
  @return none
*************************************************************/
PUBLIC void FileIo_openFile(FileIo * this, String * fullFileName)
{
#ifdef _WIN32
  errno_t err = fopen_s(&this->f, String_getBuffer(fullFileName), "rb+");
  TRACE(("Errno: %d\n", err));
#else
  this->f = fopen(String_getBuffer(fullFileName), "rb+");
#endif
  if (this->f)
  {
    this->status = 1;
  }
}

/********************************************************//**
  @brief Create a new file.
  @public
  @memberof FileIo
  @param[in] String Full path of file to create
  @return none
*************************************************************/
PUBLIC void FileIo_createFile(FileIo* this, String* fullFileName)
{
#ifdef _WIN32
    errno_t err = fopen_s(&this->f, String_getBuffer(fullFileName), "wb");
    TRACE(("Errno: %d\n", err));
#else
    this->f = fopen(String_getBuffer(fullFileName), "wb");
#endif
    if (this->f)
    {
        this->status = 1;
    }
}

/********************************************************//**
  @brief Create a new file.
  @public
  @memberof FileIo
  @param[in] String Full path of file to create
  @return none
*************************************************************/
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
    TRACE(("Length=%d\n", l));
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

PUBLIC List * FileIo_listDirs(FileIo * this, String * directory)
{
  List* result = 0;
#ifdef _WIN32
  WIN32_FIND_DATAA FindFileData;
  HANDLE hFind;
  //TCHAR dir[MAX_PATH];

  //char * dirText = "C:\\Users\\remion_t\\source\\repos\\SParse-SP6\\UT_FileIo_01\\*";
  hFind = FindFirstFileA("*", &FindFileData);
  if (hFind == INVALID_HANDLE_VALUE)
  {
    printf("FindFirstFile failed (%d)\n", GetLastError());
  }
  else
  {        
    do {
      //_tprintf(_T("The first file found is %s\n"), FindFileData.cFileName);
      if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 2)
      {
        //char text[MAX_PATH];
        size_t nb = 0;
        //wcstombs_s(&nb, text, 100, FindFileData.cFileName, wcslen(FindFileData.cFileName));
        printf("=>%s\n", FindFileData.cFileName);
        String* s = String_new(FindFileData.cFileName);
        List_insertTail(result, s, 1);
      }
    } while (FindNextFileA(hFind, &FindFileData));
    FindClose(hFind);
  }
#else
  struct dirent *directoryEntry = 0;
  String * name = 0;
  DIR * dirHandle = opendir(String_getBuffer(directory));
  printf("%s\n",String_getBuffer(directory));
  if (dirHandle!=0)
  {
    result = List_new();
    while ((directoryEntry = readdir(dirHandle)) != NULL) 
    {
      if (directoryEntry->d_type == DT_DIR)
      {
        if ((Memory_ncmp(directoryEntry->d_name,"..",2)==0) 
           && (Memory_ncmp(directoryEntry->d_name,".",1)==0))
        {
          //String * fullName = String_copy(directory);
          name = String_new(directoryEntry->d_name);
          //FileMgr_mergePath(this, fullName, name);
          List_insertHead(result,name, 1);
          printf("%s\n",String_getBuffer(name));
        }
      }
    }
  }
#endif
return result;
}

PUBLIC List* FileIo_listFiles(FileIo* this, String* directory)
{
  List* result = 0;
#ifdef _WIN32
  WIN32_FIND_DATAA FindFileData;
  HANDLE hFind;
  //TCHAR dir[MAX_PATH];

  //char * dirText = "C:\\Users\\remion_t\\source\\repos\\SParse-SP6\\UT_FileIo_01\\*";
  String* filter = String_copy(directory);
  String_append(filter, "\\*");
  hFind = FindFirstFileA(String_getBuffer(filter), &FindFileData);
  if (hFind == INVALID_HANDLE_VALUE)
  {
    printf("FindFirstFile failed (%d)\n", GetLastError());
  }
  else
  {        
    result = List_new();
    do {
      //_tprintf(_T("The first file found is %s\n"), FindFileData.cFileName);
      if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
      {
        //char text[MAX_PATH];
        size_t nb = 0;
        //wcstombs_s(&nb, text, 100, FindFileData.cFileName, wcslen(FindFileData.cFileName));
        TRACE(("=>%s\n", FindFileData.cFileName));
        String* s = String_new(FindFileData.cFileName);
        List_insertTail(result, s, 1);
      }
    } while (FindNextFileA(hFind, &FindFileData));
    FindClose(hFind);
  }
  String_delete(filter);
#else
  struct dirent *directoryEntry = 0;
  String * name = 0;
  DIR * dirHandle = opendir(String_getBuffer(directory));
  printf("%s\n",String_getBuffer(directory));
  if (dirHandle!=0)
  {
    result = List_new();
    while ((directoryEntry = readdir(dirHandle)) != NULL) 
    {
      //struct stat buf;
      //stat(directoryEntry->d_name, &buf);
      //printf("Item %s %d\n", directoryEntry->d_name, S_ISREG(buf.st_mode));
      if ((directoryEntry->d_type == DT_REG) || (directoryEntry->d_type == DT_LNK))
      {
        //fileDesc = FileDesc_new();
        //fullName = String_copy(directory);
        name = String_new(directoryEntry->d_name);
        printf("%s\n",String_getBuffer(name));
        //FileMgr_mergePath(this, fullName, name);
        //FileDesc_setFullName(fileDesc, fullName);
        List_insertHead(result, (void*)name, 1);
      }
      else
      {
        #if 0
        if ((Memory_ncmp(directoryEntry->d_name,"..",2)==0) 
           && (Memory_ncmp(directoryEntry->d_name,".",1)==0))
        {
          fullName = String_copy(directory);
          name = String_new(directoryEntry->d_name);
          FileMgr_mergePath(this, fullName, name);
          List_insertHead(this->directories,fullName);
          String_delete(name);
        }
        #endif
      }
    }
  }
#endif

  return result;
}

PUBLIC int FileIo_fSeekEnd(FileIo * this, int pos)
{
  if (this->status == 1)
    fseek(this->f, pos, SEEK_END);

  return 0;
}

PUBLIC String * FileIo_getCwd(FileIo * this)
{
#ifdef _WIN32
  TCHAR workDir[1024];
  DWORD retVal = GetCurrentDirectory(1024, workDir);
  char workingDirectory[1024];
  size_t nb = 0;
  wcstombs_s(&nb, workingDirectory, 100, workDir, wcslen(workDir));
  if ((retVal == 0) || (retVal > 1024))
  {
    Error_new(ERROR_FATAL, "FileIo: Cannot obtain the root location");
  }
#else
  char * workingDirectory = get_current_dir_name();

#endif
  return String_new(workingDirectory);
}

PUBLIC int FileIo_fSeekSet(FileIo* this, int pos)
{
  if (this->status == 1)
    fseek(this->f, pos, SEEK_SET);

  return 0;
}

PUBLIC int FileIo_ftell(FileIo* this)
{
  if (this->status == 1)
    return ftell(this->f);
  return 0;
}
PUBLIC FileIoStatus FileIo_isOpen(FileIo * this)
{
  if (this->status == FILE_OPEN) return 1;
  return 0;
}
