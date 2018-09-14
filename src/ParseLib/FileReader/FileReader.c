/**********************************************//** 
  @file FileReader.c
 
  @brief This file contains the implementation for the class FileReader

  The class FileReader is TBD
 **************************************************/

#include "FileReader.h"
#include "Object.h"
#include "String2.h"
#include "FileMgr.h"
 
 /**********************************************//**
   @class FileReader
 **************************************************/
struct FileReader
{
  Object object;
  String * buffer;
  String * fileName;
  //List * buffers;
};

PUBLIC FileReader * FileReader_new(String * fileName)
{
  FileReader * this = 0;
  FileMgr * fileMgr = FileMgr_getRef();
  
  this = (FileReader*)Object_new(sizeof(FileReader), (Destructor)&FileReader_delete, (Copy_operator)&FileReader_copy);
  
  this->buffer = FileMgr_load(fileMgr, String_getBuffer(fileName));
  this->fileName = fileName;
  /* associate buffer containing the file to the fileRead */
  
  FileMgr_delete(fileMgr);
  
  return this;
}

PUBLIC void FileReader_delete(FileReader * this)
{
  String_delete(this->buffer);
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
  
  result = String_getBuffer(this->buffer);
  
  return result;
}

PUBLIC String * FileReader_getName(FileReader * this)
{
  return this->fileName;
}