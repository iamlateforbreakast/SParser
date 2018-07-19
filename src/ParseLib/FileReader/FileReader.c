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
  //List * buffers;
};

PUBLIC FileReader * FileReader_new(String * fileName)
{
  FileReader * this = 0;
  FileMgr * fileMgr = FileMgr_getRef();
  
  this = Object_new(sizeof(FileReader), &FileReader_delete, &FileReader_copy);
  
  FileMgr_load(fileMgr, fileName);
  
  /* associate buffer containing the file to the fileRead */
  
  FileMgr_delete(fileMgr);
  
  return this;
}

PUBLIC void FileReader_delete(FileReader * this)
{
  /* Object_delete(this) */
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
  
  return result;
}

PUBLIC void FileReader_add(FileReader * this /* , String * s */)
{
}
