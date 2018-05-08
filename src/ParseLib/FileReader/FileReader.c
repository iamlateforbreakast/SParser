/**********************************************//** 
  @file FileReader.c

  @class FileReader
 
  @brief This file contains the prototye for the class FileReader

  The class FileReader is TBD
 **************************************************/

#include "FileReader.h"
#include "Object.h"
 
struct FileReader
{
  Object object;
};

PUBLIC FileReader * FileReader_new()
{
  FileReader * result = 0;

  /* this = Object_new(sizeof(FileReader), &FileReader_delete, &FileReader_copy); */
  
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

