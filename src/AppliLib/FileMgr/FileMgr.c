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
  char rootLocation[FILEMGR_MAX_PATH];
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
  
  this = (FileMgr*)Object_new(sizeof(FileMgr), &fileMgrClass);
  this->files = List_new();
  this->directories = List_new();
  if (getcwd(this->rootLocation, FILEMGR_MAX_PATH)==0)
  {
    /* Error case: Cannot obtain the FileMgr root location. */
    Error_new(ERROR_FATAL, "Cannot obtain the FileMgr root location");
  }
  
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
  if (this!=0)
  {
    if (this->object.refCount==1)
    {
      List_delete(this->files);
      List_delete(this->directories);
      // TODO: String_delete(this->rootLocation);
      Object_delete(&this->object);
      fileMgr = 0;
    }
    else if (this->object.refCount>1)
    {
      this->object.refCount--;
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
  @brief TBD
  @public
  @memberof FileMgr
  @return Status.
**************************************************/
PUBLIC unsigned int FileMgr_setRootLocation(FileMgr* this, const char * location)
{
  unsigned int result = 0;
  
  String * newLocation = String_new(location);
  String * currentLocation = String_new(this->rootLocation);

  FileMgr_mergePath(this, currentLocation, newLocation);
  
  Memory_copy(this->rootLocation, String_getBuffer(currentLocation), FILEMGR_MAX_PATH);
  
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
  char * result = this->rootLocation;
  
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
  String * fullPathDirectory = String_new(this->rootLocation);
  String * addedDirectory = String_new(directoryName);
  
  /* TODO: Check directoryName has a valid length */
  
  /* Merge directory name with current path to have the full path of the directory*/
  FileMgr_mergePath(this, fullPathDirectory, addedDirectory);
  
  /* TODO: Check if merged path exist on filesystem */
  
  /* add directory to this->directories */
  List_insertHead(this->directories,fullPathDirectory);
  
  /* For each directory */
  /* List_forEach(this->directories, FileMgr_listFiles, this); */
  fullPathDirectory = List_getNext(this->directories);
  
  while (fullPathDirectory!=0)
  {
    FileMgr_listFiles(this, fullPathDirectory);
    fullPathDirectory = List_getNext(this->directories);
    #if 0
    if (String_getLength(fullPathDirectory)>1000) 
    {
      printf("String length = %d\n", String_getLength(fullPathDirectory));
      printf("Nb calls = %d\n", nbCalls);
    }
    nbCalls++;
    #endif
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
  String * fullName = String_new(this->rootLocation);
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
  FileIo * f = 0;
  FileDesc * fd = 0;
  
  char * buffer = 0;
  unsigned int length = 0;
  
  fd = FileMgr_isManaged(this, name);
  
  if (fd)
  {
    /* Open file */
    //f=fopen(String_getBuffer(FileDesc_getFullName(fd)),"rb");
    f = FileIo_new(String_getBuffer(FileDesc_getFullName(fd)));
    if (f)
    {
	    FileIo_fSeekEnd(f, 0);
	    length=ftell(f);
	    FileIo_fSeekSet(f, 0);
        
	    buffer = (char*)Memory_alloc(length+1);
      if (buffer)
      {
        FileIo_read(f, buffer, length);
        buffer[length] = 0;
        result = String_new(0);
        String_setBuffer(result, buffer);
	    FileIo_delete(f);
      }
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
      s = String_getRef(FileDesc_getFullName(fd));
      List_insertHead(result, s);
    }
  }
  
  return result;
}

PRIVATE void FileMgr_listFiles(FileMgr * this, String * directory)
{
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
  char * p2_idx = 0;
  char * p1_idx = 0;
  char buffer[FILEMGR_MAX_PATH];
  char * mergedPath = 0;
  unsigned int bufferLength = String_getLength(path1);
  
  // TODO: CHeck initial condition of validity length > 0
  
  /* TODO: check if path2 is absolute path in which case copy and return */
  
  // Initialise result buffer
  Memory_copy(buffer, String_getBuffer(path1), bufferLength);
  
  p1_idx = buffer + String_getLength(path1);
  
  if (*(p1_idx-1)  != '/')
  {
    *p1_idx = '/';
    p1_idx++;
  }
   *p1_idx = 0;
  
  for (p2_idx=String_getBuffer(path2); p2_idx<(String_getBuffer(path2)+String_getLength(path2)); p2_idx++)
  {
      //Take ../ into account
      if (Memory_ncmp(p2_idx, "..", 2) == 1)
         //((memcmp(p2_idx, "..",2)==0) && (p2_idx == (String_getBuffer(path2)+String_getLength(path2)))))
      {
        p1_idx = p1_idx - 2;
        while ((*p1_idx != '/') && (p1_idx > buffer))
        {
          p1_idx--;
        }
        p1_idx++;
        p2_idx = p2_idx + 2;
      }
      //Ignore ./ in path2
      else if ((Memory_ncmp(p2_idx, "./", 2) == 1) ||
                 ((Memory_ncmp(p2_idx, ".", 1) == 1) && (p2_idx == (String_getBuffer(path2)+String_getLength(path2)-1))))
      {
        //p1_idx = p1_idx - 1;
        p2_idx = p2_idx + 1;
      }
      else
      {
        //p2_idx = p2_idx + 1;
        *p1_idx = *p2_idx;
        //*p1_idx = '/';
        p1_idx++;
      }
  }
  
  *p1_idx = 0;
  
  //Place merged path into path1
  mergedPath = (char *)Memory_alloc(p1_idx - buffer+1);
  Memory_copy(mergedPath, buffer, p1_idx-buffer+1);
  
  String_setBuffer(path1, mergedPath);
  
  if (String_getLength(path1)>1000) 
  {
    printf("String length = %d\n", String_getLength(path1));
    printf("Str length = %d\n", Memory_len(String_getBuffer(path1)));
  }
  Error_new(ERROR_INFO,"Merged path: %s\n", buffer);
}

/**************************************************
 @brief FileMgr_searchFile
 
 This function search for a file in the file of file by its name
 or by its partial path in all the preferred
 
 @memberof FileMgr
 @param [in]     name: String* - name of the file.
 @return: none
**************************************************/
PUBLIC String * FileMgr_searchFile(FileMgr * this, String * name, List * preferredDir)
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
    fullPath = String_new(this->rootLocation);
    FileMgr_mergePath(this, d, name);
    FileMgr_mergePath(this, fullPath, d);
    
    c=FileMgr_isManaged(this, fullPath);
    if (c!=0)
    {
      isFound = 1;
      result = FileDesc_getFullName((FileDesc*)c);
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
    if (String_compare(FileDesc_getFullName(fd), fullName)==0)
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
  FileIo_openFile(f, String_getBuffer(fullName));
  if (f) 
  {
    result = 1;
    //fclose(f);
  }
  return result;
}
