/**********************************************//** 
  @file FileReader.c
 
  @brief This file contains the implementation for the class FileReader

  The class FileReader is TBD
 **************************************************/

#include "FileReader.h"
#include "Object.h"
 
 /**********************************************//**
   @class FileReader
 **************************************************/
struct FileReader
{
  Object object;
  //List * buffers;
};

PUBLIC FileReader * FileReader_new(/* String * s */)
{
  FileReader * result = 0;

  /* this = Object_new(sizeof(FileReader), &FileReader_delete, &FileReader_copy); */
  
  /* FileMgr_open(fileName) */
  
  /* associate buffer containing the file to the fileRead */
  
  return result;
}

PUBLIC void FileReader_delete(FileReader * this)
{
  /* Object_delete(this) */
}

PUBLIC FileReader * FileReader_copy(FileReader * this)
{
  FileReader * copy;
  
  /* copy = Object_copy(this); */
  
  return copy;
}

PUBLIC void FileReader_add(FileReader * this /* , String * s */)
{
}