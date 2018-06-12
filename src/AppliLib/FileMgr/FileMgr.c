/**********************************************//** 
  @file FileMgr.c
  
  @brief The FileMgr class manages a list of files contained
  in a group of locations.
  
  The class FileMgr is TBD
**************************************************/

#include "FileMgr.h"
#include "String2.h"
#include "Object.h"
#include "List.h"
#include "FileDesc.h"
#include "Memory.h"

#include <unistd.h>
#include <dirent.h>
//#include <stat.h>
#include <stdio.h>

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
  @private
**************************************************/
PRIVATE FileMgr * fileMgr = 0;

/**********************************************//**
  @private
**************************************************/
PRIVATE void FileMgr_changeDirectory(FileMgr * this);
PRIVATE void FileMgr_mergePath(FileMgr* this, String* path1, String* path2);
PRIVATE void FileMgr_listFiles(FileMgr * this, String * directory);
PRIVATE unsigned int FileMgr_isManaged(FileMgr * this, String * fullName);

/**********************************************//** 
  @brief Create an instance of the class FileMgr.
  @private
  @memberof FileMgr
  @return New instance.
**************************************************/
PRIVATE FileMgr * FileMgr_new()
{
  FileMgr * this = 0;
  
  this = (FileMgr*)Object_new(sizeof(FileMgr),(Destructor)&FileMgr_delete, (Copy_operator)&FileMgr_copy);
  this->files = List_new();
  this->directories = List_new();
  if (getcwd(this->rootLocation, FILEMGR_MAX_PATH)==0)
  {
    /* Error case: Cannot obtain the FileMgr root location. */
    /* e = Error_set(E_ERROR_FATAL, "Error", 0);
        Error_raise(e); */
    exit(2);
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
    List_delete(this->files);
    List_delete(this->directories);
    // TODO: String_delete(this->rootLocation);
    Object_delete(&this->object);
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
  @brief Add all files in the given directory to the list of managed files.
  @public
  @memberof FileMgr
  @return Status.
**************************************************/
PUBLIC unsigned int FileMgr_addDirectory(FileMgr * this, const char * directoryName)
{
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
PUBLIC unsigned int FileMgr_addFile(FileMgr * this, const char * fileName)
{
  unsigned int result = 0;
  String * fullName = String_new(this->rootLocation);
  String * addedFile = String_new(fileName);
  
  /* Merge current path with fileName */
  FileMgr_mergePath(this, fullName, addedFile);
  
  /* Check file is not already managed */
  if (!FileMgr_isManaged(this, fullName)
  {
    /* Check file exists on filesystem */
    if (FileMgr_existFS(this, fullName))
    {
      /* If exists add to the list of files */
      FileDesc_setFullName(fileDesc, fullName);
      List_insertHead(this->files, (void*)fileDesc);
      result = 1;
    }
    else
    {
      /* TODO: Free fullName */
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
  @return Content of file.
**************************************************/
PUBLIC String* FileMgr_load(FileMgr* this, const char * fileName)
{
  String * result = 0;
  String * fullName = String_new(this->rootLocation);
  String * name = String_new(fileName);
  FILE * f = NULL;

  unsigned int isFound = 0;
  char * buffer = 0;
  unsigned int length = 0;
  
  /* Normalise file name */
  FileMgr_mergePath(this, fullName, name);
  
  if (FileMgr_isManaged(this, fullName))
  {
  
    /* Open file */
    f=fopen(String_getBuffer(fullName),"rb");
    if (f)
    {
	    fseek(f, 0, SEEK_END);
	    length=ftell(f);
	    fseek(f, 0 , SEEK_SET);
        
	    buffer = (char*)malloc(length+1);
      if (buffer)
      {
        fread(buffer, length, 1, f);
        buffer[length] = 0;
        result = String_new(buffer);
	      fclose(f);
      }
    }
  }
  else
  {
    /* ERROR case: File is not in the list of managed files */
    exit(2);
  }
  
  return result;
}

PRIVATE void FileMgr_listFiles(FileMgr * this, String * directory)
{
  struct dirent *directoryEntry = NULL;
  DIR * dir = 0;
  FileDesc * fileDesc= 0;
  String * fullName = 0;
  String * name = 0;
  
  dir = opendir(String_getBuffer(directory));
  
  if (dir!=0)
  {
    while ((directoryEntry = readdir(dir)) != NULL) 
    {
      if (directoryEntry->d_type != DT_DIR)
      {
        fileDesc = FileDesc_new();
        name = String_new(directoryEntry->d_name);
        fullName = String_copy(directory);
        FileMgr_mergePath(this, fullName, name);
        FileDesc_setFullName(fileDesc, fullName);
        List_insertHead(this->files, (void*)fileDesc);
      }
      else
      {
        if ((Memory_ncmp(directoryEntry->d_name,"..",2)==0) && (Memory_ncmp(directoryEntry->d_name,".",1)==0))
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
  unsigned int bufferLength = String_getLength(path1);
  
  // TODO: CHeck initial condition of validity length > 0
  
  /* TODO: check if path2 is absolute path in which case copy and return */
  
  // Initialise result buffer
  memcpy(buffer, String_getBuffer(path1), bufferLength);
  
  
  p1_idx = buffer + String_getLength(path1);
  if (*(p1_idx-1)  != '/')
  {
    *p1_idx = '/';
    p1_idx++;
  }
  
  for (p2_idx=String_getBuffer(path2); p2_idx<(String_getBuffer(path2)+String_getLength(path2)); p2_idx++)
  {
      //Take ../ into account
      if (memcmp(p2_idx, "..", 2) == 0)
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
      else if ((memcmp(p2_idx, "./", 2) == 0) ||
                 ((memcmp(p2_idx, ".", 1) == 0) && (p2_idx == (String_getBuffer(path2)+String_getLength(path2)-1))))
      {
        p1_idx = p1_idx - 1;
        p2_idx = p2_idx + 2;
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
  String_delete(path1);
  path1 = String_new(buffer);

  if (String_getLength(path1)>1000) 
  {
    printf("String length = %d\n", String_getLength(path1));
    printf("Str length = %d\n", strlen(String_getBuffer(path1)));
  }
  printf("Merged path: %s\n", buffer);
}

PRIVATE FileDesc * FileMgr_searchFile(FileMgr * this, String * name)
{
  FileDesc * result = 0;
  FileDesc * c = 0;
  unsigned int isFound = 0;
  
   /* Find file in list */
  while ((c = List_getNext(this->files))!=0)
  {
    if (String_isContained(FileDesc_getFullName((FileDesc*)c), name))
    {
      isFound ++;
      result =c;
    }
  }
  
  if (isFound==0)
  {
    return 0;
  }
  if (isFound > 1)
  {
    return 0;
  }
  
  return result;
}

PRIVATE unsigned int FileMgr_isManaged(FileMgr * this, String * fullName)
{
  unsigned int result = 0;
  FileDesc * c = 0;
  unsigned int isFound = 0;
  
  /* Find file in list */
  while ((c = List_getNext(this->files))!=0)
  {
    if (String_isEqual(FileDesc_getFullName((FileDesc*)c), fullName))
    {
      isFound ++;
      result =1;
    }
  }
  
  if (isFound==0)
  {
    return 0;
  }
  if (isFound > 1)
  {
    return 0;
  }
  
  return result;
}

PRIVATE unsigned int FileMgr_existFS(FileMgr * this, String * fullName)
{
  unsigned int result = 0;
  
  return result;
}