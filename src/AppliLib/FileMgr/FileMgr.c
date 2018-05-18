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

#include <unistd.h>
#include <dirent.h>
//#include <stat.h>

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
    exit(2);
  }
  
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
  
  /* Merge directory name with current path to have the full path of the directory*/
  FileMgr_mergePath(this, fullPathDirectory, addedDirectory);
  
  /* add directory to this->directories */
  List_insertHead(this->directories,fullPathDirectory);
  
  /* For each directory */
  /* List_forEach(this->directories, FileMgr_listFiles, this); */
  fullPathDirectory = List_getNext(this->directories);
  
  while (fullPathDirectory!=0)
  {
    FileMgr_listFiles(this, fullPathDirectory);
    fullPathDirectory = List_getNext(this->directories);
  }
  
  String_delete(addedDirectory);
  
  return result;
}

PUBLIC unsigned int FileMgr_addFile(FileMgr * this, const char * fileName)
{
  unsigned int result = 0;
  
  /* Merge curent path with fileName */
  
  /* Check existence */
  
  /* If exists add to the list of files */
  
  return result;
}

PUBLIC String* FileMgr_load(FileMgr* this, const char * fileName)
{
  String * fileContent = 0;
  
  /* Find file in list */
  
  return fileContent;
}

PRIVATE void FileMgr_listFiles(FileMgr * this, String * directory)
{
  struct dirent *directoryEntry = NULL;
  DIR * dir = 0;
  FileDesc * fileDesc= 0;
  String * fullName = 0;
  
  dir = opendir(String_getBuffer(directory));
  
  if (dir!=0)
  {
    while ((directoryEntry = readdir(dir)) != NULL) 
    {
      if (directoryEntry->d_type != DT_DIR)
      {
        fileDesc = FileDesc_new();
        fullName = String_copy(directory);
        FileMgr_mergePath(this, fullName, String_new(directoryEntry->d_name));
        FileDesc_setFullName(fileDesc, fullName);
        List_insertHead(this->files, (void*)fileDesc);
      }
    }
  }
}

PRIVATE void FileMgr_changeDirectory(FileMgr * this)
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
  
  // Initialise result buffer
  memcpy(buffer, String_getBuffer(path1), String_getLength(path1));
  
  p1_idx = buffer + String_getLength(path1);
  if (*p1_idx != '/')
  {
    *p1_idx = '/';
    p1_idx++;
  }
  
  for (p2_idx=String_getBuffer(path2); p2_idx<(String_getBuffer(path2)+String_getLength(path2)); p2_idx++)
  {
      //Take ../ into account
      if (memcmp(p2_idx, "..", 2) == 0)
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
      else if (memcmp(p2_idx, ".", 1) == 0)
      {
        //p1_idx = p1_idx - 1;
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
  printf("Merged path: %s\n", buffer);
}