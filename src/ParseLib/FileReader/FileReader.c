/**********************************************//** 
  @file FileReader.c
 
  @brief This file contains the implementation for the class FileReader

  The class FileReader is TBD
 **************************************************/

#include "FileReader.h"
#include "Object.h"
#include "String2.h"
#include "FileMgr.h"
#include "OptionMgr.h"
#include "List.h"

struct IncludeInfo
{
  Object object;
  String * pattern;
  List * dirs;
};

 /**********************************************//**
   @class FileReader
 **************************************************/
struct FileReader
{
  Object object;
  List * buffers;
  String * fileName;
  String * currentBuffer;
  List * preferredDirs;
};

PRIVATE void FileReader_getListPreferredDir(FileReader * this);
PRIVATE void FileReader_deleteListPreferredDir(FileReader * this);

/**********************************************//** 
  @brief Create a new FileReader object.
  @public
  @memberof FileReader
  @return Created FileReader object.
**************************************************/
PUBLIC FileReader * FileReader_new(String * fileName)
{
  FileReader * this = 0;
  FileMgr * fileMgr = FileMgr_getRef();
  String * newFileContent = 0;
  
  this = (FileReader*)Object_new(sizeof(FileReader), (Destructor)&FileReader_delete, (Copy_operator)&FileReader_copy);
  
  
  this->buffers = List_new();
  this->preferredDirs = List_new();
  
  /* Build list of all directories to be used to lookup additional files (include) */
  FileReader_getListPreferredDir(this);
  
  /* associate buffer containing the file to the fileRead */
  newFileContent = FileMgr_load(fileMgr, String_getBuffer(fileName));
  List_insertHead(this->buffers, newFileContent);
  this->currentBuffer = newFileContent;
  this->fileName = fileName;
  
  FileMgr_delete(fileMgr);
  
  return this;
}

/**********************************************//** 
  @brief Delete an instance of a FileReader object.
  @public
  @memberof FileReader
**************************************************/
PUBLIC void FileReader_delete(FileReader * this)
{
  if (this!=0)
  {
    List_delete(this->buffers);
    FileReader_deleteListPreferredDir(this);
    List_delete(this->preferredDirs);
    Object_delete(&this->object);
  }
}

PUBLIC FileReader * FileReader_copy(FileReader * this)
{
  FileReader * copy = 0;
  
  /* copy = Object_copy(this); */
  
  return copy;
}

PUBLIC char * FileReader_getBuffer(FileReader * this)
{
  char * result = 0;
  
  result = String_getBuffer(this->currentBuffer);
  
  return result;
}

PUBLIC String * FileReader_getName(FileReader * this)
{
  return this->fileName;
}

PUBLIC char * FileReader_addFile(FileReader * this, String * fileName)
{
  FileMgr * fileMgr = FileMgr_getRef();
  List * dirList = List_new();
  String * fullPath = 0;
  String * newFileContent = 0;
  struct IncludeInfo * dirInfo = 0;
  char * result = 0;
  
  /* Check in the fileName matches a pattern with a default search path */
  while ((dirInfo = (struct IncludeInfo *)List_getNext(this->preferredDirs))!=0)
  {
    if (String_matchWildcard(fileName,String_getBuffer(dirInfo->pattern )))
    {
      dirList = List_copy(dirInfo->dirs);
    }
  }
  
  /* In all cases make sure the current dir. is in the search path */
  List_insertTail(dirList, String_new("./"));
  
  fullPath = FileMgr_searchFile(fileMgr, fileName, dirList);
  
  if (fullPath != 0)
  {
    newFileContent = FileMgr_load(fileMgr, String_getBuffer(fullPath));
    List_insertHead(this->buffers, newFileContent);
    this->currentBuffer = newFileContent;
    result = String_getBuffer(newFileContent);
  }
  else
  {
    /* Error case: Can not find the include file */
  }

  FileMgr_delete(fileMgr);
  
  return result;
}

PRIVATE void FileReader_getListPreferredDir(FileReader * this)
{
  OptionMgr * optionMgr = OptionMgr_getRef();
  String * optionValue = 0;
  char * buf = 0;
  unsigned i, j, state = 0;
  struct IncludeInfo * prefDir = 0;
  
  optionValue = OptionMgr_getOption(optionMgr, "Include Path");
  
  if (optionValue!=0)
  {
    buf = String_getBuffer(optionValue);
  
    for (i=0; i<String_getLength(optionValue);i++)
    {
      switch (state)
      {
        case 0:
          if (buf[i]!='[')
          {
            /* TODO: Error case */
          }
          else
          {
            state = 1;
          }
          break;
        case 1:
          if (buf[i]!=' ') 
          {
            state = 2;
            j = i;
          }
          break;
        case 2:
          if (buf[i]==' ')
          { 
             prefDir = (struct IncludeInfo *)Object_new(sizeof(struct IncludeInfo), 0, 0);
             prefDir->pattern = String_subString(optionValue, j, i-j);
             prefDir->dirs = List_new();
             state = 3;
          }
          break;
        case 3:
          if (buf[i]!=' ')
          {
            state =4;
            j = i;
          }
          break;
        case 4:
          if (buf[i]==' ') 
          {
            List_insertHead(prefDir->dirs, String_subString(optionValue, j, i-j));
            state = 5;
          }
          if (buf[i]==']')
          {
            List_insertHead(prefDir->dirs, String_subString(optionValue, j, i-j));
            state = 6;
          }
          break;
        case 5:
          if (buf[i]!=' ')
          {
            state = 4;
            j = i;
          }      
          break;
        case 6:
          List_insertHead(prefDir->dirs, String_subString(optionValue, j, i-j-1));
          break;
        default:
          break;
      }
    }
    if (state==6)
    {
      List_insertHead(this->preferredDirs, prefDir);
    }
    else
    {
      /* TODO: Syntax error */
    }
  }
  
  OptionMgr_delete(optionMgr);
}

PRIVATE void FileReader_deleteListPreferredDir(FileReader * this)
{
  struct IncludeInfo * dirInfo = 0;
  
  while ((dirInfo = (struct IncludeInfo *)List_getNext(this->preferredDirs))!=0)
  {
    String_delete(dirInfo->pattern);
    List_delete(dirInfo->dirs);
  }
}