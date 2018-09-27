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

  char newBuffer[] = "#include <test.h>\n //COmment 1\n void main()\n{\n}\n";
  
 /**********************************************//**
   @class FileReader
 **************************************************/
struct FileReader
{
  Object object;
  List * buffers;
  String * fileName;
  String * currentBuffer;
  //List * buffers;
};

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

  
  /* Search for files with name fileName */
  
  return newBuffer;
}