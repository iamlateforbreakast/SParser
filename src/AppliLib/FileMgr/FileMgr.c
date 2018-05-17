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

#include <unistd.h>

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
  String * rootDirectory = String_new(this->rootLocation);
  String * addedDirectory = String_new(directoryName);
  
  /* Merge directory name with current path to have the full path of the directory*/
  FileMgr_mergePath(this, rootDirectory, addedDirectory);
  
  /* add directory to this->directories */
  /* List_insertHead(this->directories,addedDirectory); */
  /* For each directory */
  /* List_forEach(this->directories, FileMgr_listFiles, this); */
  
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
  unsigned int p1_idx = String_length(path1);
  char buffer[FILEMGR_MAX_PATH];
  //char* token = String_getBuffer(path2);
  
  // Initialise result buffer
  memcpy(buffer, String_getBuffer(path1), String_getLength(path1));
  
  buffer[p1_idx] = '/';
  p1_idx++;
  
  for (p2_idx=0; p2_idx<String_getLength(path2); p2_idx++)
  {
    if ((path2->buffer[p2_idx] == '/') || (p2_idx > (path2->length - 1)))
    {
      //Take ../ into account
      if (memcmp(token, "..", 2) == 0)
      {
        p1_idx = p1_idx - 4;
        while ((path1->buffer[p1_idx] != '/') && (p1_idx > 0))
        {
          p1_idx = p1_idx - 1;
        }
        p1_idx = p1_idx + 1;
        token = path2->buffer + p2_idx + 1;
      }
      //Ignore ./ in path2
      else if (memcmp(token, ".", 1) == 0)
      {
        p1_idx = p1_idx - 1;
        token = path2->buffer + p2_idx + 1;
      }
      else
      {
        //p2_idx = p2_idx + 1;
        token = path2->buffer + p2_idx + 1;
        buffer[p1_idx] = '/';
        p1_idx++;
      }
    }
    else
    {
      // Copy char from p2 to p1
      buffer[p1_idx] = path2->buffer[p2_idx];
      p1_idx++;
    }
  }
  
  //Place merged path into path1
  Memory_free(path1->buffer, path1->length);
  path1->length = p1_idx;
  path1->buffer = Memory_alloc(path1->length);
  memcpy(path1->buffer, buffer, path1->length);
  //String_print(path1, "FileMgr: active path ");
}