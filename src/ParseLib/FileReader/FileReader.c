/**********************************************//** 
  @file FileReader.c
 
  @brief This file contains the implementation for the class FileReader

  The class FileReader is TBD
 **************************************************/

#include "FileReader.h"
#include "Object.h"
#include "String2.h"
#include "FileMgr.h"
#include "List.h"
  
 /**********************************************//**
   @class FileReader
 **************************************************/
struct FileReader
{
  Object object;
  List * buffers;
  String * fileName;
  String * currentBuffer;
};

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
  
  //this->buffer = FileMgr_load(fileMgr, String_getBuffer(fileName));
  newFileContent = FileMgr_load(fileMgr, String_getBuffer(fileName));
  this->buffers = List_new(this->buffers);
  List_insertHead(this->buffers, newFileContent);
  this->currentBuffer = newFileContent;
  this->fileName = fileName;
  /* associate buffer containing the file to the fileRead */
  
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
  List_delete(this->buffers);
  Object_delete(&this->object);
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
  String * fullPath = 0;
  String * newFileContent = 0;
  
  /* Search for files with name fileName */
  fullPath = FileMgr_searchFile(fileMgr, fileName);
  if (fullPath != 0)
  {
    newFileContent = FileMgr_load(fileMgr, String_getBuffer(fullPath));
    List_insertHead(this->buffers, newFileContent);
    this->currentBuffer = newFileContent;
  }
  
  FileMgr_delete(fileMgr);
  
  return String_getBuffer(newFileContent);
}