/**********************************************//** 
  @file FileMgr.c
  
  @brief The FileMgr class manages a list of files contained
  in a group of locations.
  
  The class FileMgr is TBD
**************************************************/

#include "FileMgr.h"
#include "String2.h"
#include "Class.h"
#include "Object.h"
#include "List.h"
#include "FileDesc.h"
#include "Memory.h"
#include "Error.h"
#include "FileIo.h"

//#include <unistd.h>
//#include <dirent.h>
//#include <stat.h>
//#include <stdio.h>

#define FILEMGR_MAX_PATH (1024)

/**********************************************//**
  @class FileMgr
**************************************************/
struct FileMgr
{
  Object object;
  List * files;
  List * directories;
  char * separator;
  String * rootLocation;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class fileMgrClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&FileMgr_delete,
  .f_copy = (Copy_Operator)&FileMgr_copy,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)0
};

/**********************************************//**
  @private
**************************************************/
PRIVATE FileMgr * fileMgr = 0;

/**********************************************//**
  @private
**************************************************/
PRIVATE void FileMgr_mergePath(FileMgr* this, String* path1, String* path2);
PRIVATE void FileMgr_listFiles(FileMgr * this, String * directory);
PRIVATE FileDesc * FileMgr_isManaged(FileMgr * this, String * fullName);
PRIVATE unsigned int FileMgr_existFS(FileMgr * this, String * fullName);

/**********************************************//** 
  @brief Create an instance of the class FileMgr.
  @private
  @memberof FileMgr
  @return New instance.
**************************************************/
PRIVATE FileMgr * FileMgr_new()
{
  FileMgr * this = 0;
  FileIo * f = FileIo_new();
  this = (FileMgr*)Object_new(sizeof(FileMgr), &fileMgrClass);
  this->files = List_new();
  this->directories = List_new();
  this->rootLocation = FileIo_getCwd(f);
#ifdef _WIN32
  this->separator = "\\";
#else
  this->separator = "/";
#endif
  
  /* TODO: Check this->rootLocation has a valid length */
  
  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class FileMgr.
  @public
  @memberof FileMgr
**************************************************/
PUBLIC void FileMgr_delete(FileMgr * this)
{
  if (fileMgr!=0)
  {
    if (fileMgr->object.refCount==1)
    {
      List_delete(fileMgr->files);
      List_delete(fileMgr->directories);
      String_delete(fileMgr->rootLocation);
      Object_delete(&fileMgr->object);
      fileMgr = 0;
      this = 0;
    }
    else if (fileMgr->object.refCount>1)
    {
      fileMgr->object.refCount--;
    }
  }
}

/**********************************************//** 
  @brief Copy an instance of the class FileMgr.
  @public
  @memberof FileMgr
  @return New instance
**************************************************/
PUBLIC FileMgr * FileMgr_copy(FileMgr * this)
{
  this->object.refCount++;
  
  return this;
}

/**********************************************//** 
  @brief Get a reference to the singleton instance of FileMgr.
  @public
  @memberof FileMgr
  @return Reference to the singleton.
**************************************************/
PUBLIC FileMgr* FileMgr_getRef()
{
  if (fileMgr==0)
  {
    fileMgr = FileMgr_new();
  }
  else
  {
    fileMgr->object.refCount++;
  }
  
  return fileMgr;
}

/**********************************************//** 
  @brief Set the root location
  @public
  @memberof FileMgr
  @return Status.
**************************************************/
PUBLIC unsigned int FileMgr_setRootLocation(FileMgr* this, const char * location)
{
  unsigned int result = 0;
  
  String * newLocation = String_new(location);
  String * currentLocation = String_copy(this->rootLocation);

  FileMgr_mergePath(this, currentLocation, newLocation);
  String_stealBuffer(this->rootLocation, currentLocation);
  
  //Memory_copy(this->rootLocation, String_getBuffer(currentLocation), FILEMGR_MAX_PATH);
  
  String_delete(newLocation);
  String_delete(currentLocation);
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof FileMgr
  @return Status.
**************************************************/
PUBLIC char * FileMgr_getRootLocation(FileMgr* this)
{
  char * result = String_getBuffer(this->rootLocation);
  
  return result;
}

/**********************************************//** 
  @brief Add all files in the given directory to the list of managed files.
  @public
  @memberof FileMgr
  @return Status.
**************************************************/
PUBLIC unsigned int FileMgr_addDirectory(FileMgr * this, const char * directoryName)
{
  //static int nbCalls = 0;
  
  unsigned int result = 0;
  String * fullPathDirectory = String_copy(this->rootLocation);
  String * addedDirectory = String_new(directoryName);
  
  /* TODO: Check directoryName has a valid length */
  
  /* Merge directory name with current path to have the full path of the directory*/
  FileMgr_mergePath(this, fullPathDirectory, addedDirectory);
  Error_new(ERROR_INFO, "Full directoryPath: %s\n", String_getBuffer(fullPathDirectory));
  /* TODO: Check if merged path exist on filesystem */
  
  /* add directory to this->directories */
  List_insertHead(this->directories,fullPathDirectory);
  
  /* For each directory */
  /* List_forEach(this->directories, FileMgr_listFiles, this); */
  fullPathDirectory = List_getNext(this->directories);
  //printf("Full directoryPath: %s\n", String_getBuffer(fullPathDirectory));
  while (fullPathDirectory!=0)
  {
    printf("Full directoryPath: %s\n", String_getBuffer(fullPathDirectory));
    FileMgr_listFiles(this, fullPathDirectory);
    fullPathDirectory = List_getNext(this->directories);
  }
  
  String_delete(addedDirectory);
  
  return result;
}

/**********************************************//** 
  @brief Add a files to the list of managed files.
  @public
  @memberof FileMgr
  @return Status.
**************************************************/
PUBLIC String * FileMgr_addFile(FileMgr * this, const char * fileName)
{
  String * result = 0;
  FileDesc * fileDesc = 0;
  String * fullName = String_copy(this->rootLocation);
  String * addedFile = String_new(fileName);
  
  /* Merge current path with fileName */
  FileMgr_mergePath(this, fullName, addedFile);
  
  /* Check file is not already managed */
  if (!FileMgr_isManaged(this, fullName))
  {
    /* Check file exists on filesystem */
    if (FileMgr_existFS(this, fullName))
    {
      /* If exists add to the list of files */
      fileDesc = FileDesc_new();
      FileDesc_setFullName(fileDesc, fullName);
      List_insertHead(this->files, (void*)fileDesc);
      result = fullName;
    }
    else
    {
      String_delete(fullName);
    }
  }
  else
  {
    String_delete(fullName);
  }
  
  String_delete(addedFile);
  
  return result;
}

/**********************************************//** 
  @brief Load a managed file into a String.
  @public
  @memberof FileMgr
  @parameter File Name
  @return Content of file.
**************************************************/
PUBLIC String* FileMgr_load(FileMgr* this, const char * fileName)
{
  String * result = 0;
  String * name = String_new(fileName);
  FileIo * f = FileIo_new();
  FileDesc * fd = 0;
  
  char * buffer = 0;
  unsigned int length = 0;
  
  fd = FileMgr_isManaged(this, name);
  Error_new(ERROR_INFO,"Loading file: %s\n", fileName);

  if (fd)
  {
    /* Open file */
    //f=fopen(String_getBuffer(FileDesc_getFullName(fd)),"rb");
    FileIo_openFile(f, FileDesc_getFullName(fd));
    if (f)
    {
	    FileIo_fSeekEnd(f, 0);
	    length=FileIo_ftell(f);
	    FileIo_fSeekSet(f, 0);
        
	    buffer = (char*)Memory_alloc(length+1);
      if (buffer)
      {
        FileIo_read(f, buffer, length);
        buffer[length] = 0;
        result = String_new(0);
        String_setBuffer(result, buffer);
      }
      FileIo_delete(f);
    }
  }
  else
  {
    /* ERROR case: File is not in the list of managed files */
    Error_new(ERROR_FATAL, "File is not in the list of managed files.");
  }
  
  String_delete(name);
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof FileMgr
  @return TBD
**************************************************/
PUBLIC List * FileMgr_filterFiles(FileMgr * this, const char * pattern)
{
  List * result = List_new();
  FileDesc * fd = 0;
  String * s = 0;
  
  while ((fd = List_getNext(this->files))!=0)
  {
    if (String_matchWildcard(FileDesc_getName(fd), pattern))
    {
      List_insertHead(result, fd);
    }
  }
  
  return result;
}

PRIVATE void FileMgr_listFiles(FileMgr * this, String * directory)
{
  FileIo* f = FileIo_new();
  List * fileList = FileIo_listFiles(f, directory);
  List * dirs = FileIo_listDirs(f, directory);
  String * fileName = 0;
  String * dirName = 0;
  FileDesc * fileDesc= 0;

  while ((fileName = List_getNext(fileList)) != 0)
  {
    fileDesc = FileDesc_new();
    String* fullFileName = String_copy(directory);
    FileMgr_mergePath(this, fullFileName, fileName);
    FileDesc_setFullName(fileDesc, fullFileName);
    List_insertTail(this->files, (void*)fileDesc);
    Error_new(ERROR_INFO,"List files: %s\n", String_getBuffer(fullFileName));
    //String_delete(name);
  }
  while ((dirName = List_getNext(dirs))!=0)
  {
    String * fullDirPath = String_copy(directory);
    FileMgr_mergePath(this, fullDirPath, dirName);
    List_insertHead(this->directories, (void*)fullDirPath);
    Error_new(ERROR_INFO,"Add directory: %s\n", String_getBuffer(fullDirPath));
  }
  FileIo_delete(f);
  List_delete(fileList);
  List_delete(dirs);
}

/**************************************************
 @brief FileMgr_mergePath
 
 This function merges 2 paths into one.
 
 @param [in/out] path1: String* - Path to merge
 @param [in]     path2: String* - Path to merge
 @return: none
**************************************************/
PRIVATE void FileMgr_mergePath(FileMgr* this, String* path1, String* path2)
{
  String * s = 0;
  String * twoDots = String_new("..");
  String * oneDot = String_new(".");
  String* result = String_new(0);
  // TODO: CHeck initial condition of validity length > 0
  
  //Error_new(ERROR_INFO, "mergePath: path1 %s\n", String_getBuffer(path1));
  //Error_new(ERROR_INFO, "mergePath: path2 %s\n", String_getBuffer(path2));

  /* TODO: check if path2 is absolute path in which case copy and return */
  
  List* tokenPath1 = String_splitToken(path1, this->separator);
  List* tokenPath2 = String_splitToken(path2, this->separator);
  s = List_removeTail(tokenPath2);
  while (s!=0)
  {
    if (String_compare(s, twoDots)==0)
    {
      String_delete(s);
      String_delete(List_removeHead(tokenPath1));
    }
    else if (String_compare(s, oneDot)==0)
    {
      String_delete(s);
    }
    else
    {
      List_insertHead(tokenPath1, s);
    }
    s = List_removeTail(tokenPath2);
  }
  s = List_getNext(tokenPath1);
  #ifndef _WIN32
  String_append(result, this->separator);
  #endif
  String_append(result, String_getBuffer(s));

  while ((s = List_getNext(tokenPath1)) != 0)
  {
    String_append(result, this->separator);
    String_append(result, String_getBuffer(s));
    //printf("%s\n", String_getBuffer(s)); 
  }

  if (String_getLength(result)>1024) 
  {
    Error_new(ERROR_INFO, "String length = %d\n", String_getLength(result));
    Error_new(ERROR_INFO, "Str length = %d\n", Memory_len(String_getBuffer(result)));
  }
  //Error_new(ERROR_INFO,"Merged path: %s\n", String_getBuffer(result));
  String_stealBuffer(path1, result);
  String_delete(result);
  List_delete(tokenPath1);
  List_delete(tokenPath2);
  //String_delete(result);
  String_delete(twoDots);
  String_delete(oneDot);
  String_delete(s);
}

/**************************************************
 @brief FileMgr_searchFile
 
 This function search for a file in the file of file by its name
 or by its partial path in all the preferred
 
 @memberof FileMgr
 @param [in]     name: String* - name of the file.
 @return: none
**************************************************/
PUBLIC FileDesc * FileMgr_searchFile(FileMgr * this, String * name, List * preferredDir)
{
  String * result = 0;
  //String * temp = 0;
  String * d = 0;
  FileDesc * c = 0;
  unsigned int isFound = 0;
  String * fullPath = 0;
  
  /* For each directory in preferred list */
  while (((d = List_getNext(preferredDir))!=0)&&(!isFound))
  {
    fullPath = String_copy(this->rootLocation);
    FileMgr_mergePath(this, d, name);
    FileMgr_mergePath(this, fullPath, d);
    
    //c=FileMgr_isManaged(this, fullPath);
    c=FileMgr_isManaged(this, name);
    printf("Searching file %s in %s\n", String_getBuffer(fullPath), String_getBuffer(d));
    if (c!=0)
    {
      isFound = 1;
      result = c;
    }
    else
    {
      /* Check file exists on filesystem */
      if (FileMgr_existFS(this, fullPath))
      {
        /* If exists add to the list of files */
        isFound = 1;
        c = FileDesc_new();
        FileDesc_setFullName(c, fullPath);
        List_insertHead(this->files, (void*)c);
        result = fullPath;
      }
      else
      {
        String_delete(fullPath);
      }
    }
  }
  
  /* If more than one file was found matching return 0 */
  if (isFound == 0)
  {
    return 0;
  }
  
  if (isFound > 1)
  {
    Error_new(ERROR_INFO,"Several managed files match the name %s\n", String_getBuffer(name));
    return 0;
  }
  
  return result;
}

/**************************************************
 @brief FileMgr_mergePath
 
 This function merges 2 paths into one.
 
 @param [in/out] path1: String* - Path to merge
 @param [in]     path2: String* - Path to merge
 @return: none
**************************************************/
PRIVATE FileDesc * FileMgr_isManaged(FileMgr * this, String * fullName)
{
  FileDesc * result = 0;
  FileDesc * fd = 0;
  unsigned int isFound = 0;
  
  /* Find file in list */
  while ((fd = List_getNext(this->files))!=0)
  {
    if (String_compare(FileDesc_getName(fd), fullName)==0)
    {
      isFound ++;
      result = fd;
      Error_new(ERROR_DBG,"Matched %s\n", String_getBuffer(FileDesc_getFullName(fd)));
    }
  }
  
  if (isFound==0)
  {
    result = 0;
  }
  if (isFound > 1)
  {
    result = 0;
  }
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @details TBD
  @private
  @memberof FileMgr
**************************************************/
PRIVATE unsigned int FileMgr_existFS(FileMgr * this, String * fullName)
{
  unsigned int result = 0;
  FileIo * f = FileIo_new();

  //f=fopen(String_getBuffer(fullName),"rb");
  FileIo_openFile(f, fullName);
  if (f) 
  {
    result = 1;
    FileIo_delete(f);
  }
  return result;
}
