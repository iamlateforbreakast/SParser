/**********************************************//** 
  @file FileDesc.c
  
  @brief The FileDesc class describe a File in the FIleMgr.
  
  The class FileDesc is TBD
**************************************************/

#include "FileDesc.h"
#include "String2.h"
#include "FileIo.h"
#include "Class.h"
#include "Object.h"

/**********************************************//**
  @class FileDesc
**************************************************/
struct FileDesc
{
  Object object;
  String * name;
  String * fullName;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class fileDescClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&FileDesc_delete,
  .f_copy = (Copy_Operator)&FileDesc_copy,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)0,
  .f_size = (Sizer)&FileDesc_getSize
};

PRIVATE String * FileDesc_getBasename(FileDesc * this);
/**********************************************//** 
  @brief TBD
  @public
  @memberof FileDesc
**************************************************/
PUBLIC FileDesc * FileDesc_new()
{
  FileDesc * this = 0;
  
  this = (FileDesc*)Object_new(sizeof(FileDesc), &fileDescClass);
  this->name = 0;
  this->fullName = 0;
  
  return this;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof FileDesc
**************************************************/
PUBLIC void FileDesc_delete(FileDesc * this)
{
  String_delete(this->fullName);
  String_delete(this->name);
  Object_delete(&this->object);
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof FileDesc
**************************************************/
PUBLIC FileDesc * FileDesc_copy(FileDesc * this)
{
  FileDesc * copy = 0;
  
  /* TODO: Not implemented */
  
  return copy;
}
PUBLIC unsigned int FileDesc_getSize(FileDesc* this)
{
  return sizeof(FileDesc);
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof FileDesc
**************************************************/
PUBLIC void FileDesc_setFullName(FileDesc * this, String * fullName)
{
  this->fullName = fullName;
  this->name = FileDesc_getBasename(this);
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof FileDesc
**************************************************/
PUBLIC String * FileDesc_getFullName(FileDesc * this)
{
  return this->fullName;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof FileDesc
**************************************************/
PUBLIC String * FileDesc_getName(FileDesc * this)
{
  return this->name;
}

/**********************************************//** 
  @brief TBD
  @private
  @memberof FileDesc
**************************************************/
PRIVATE String * FileDesc_getBasename(FileDesc * this)
{
  String * result = 0;
  
  char * p = String_getBuffer(this->fullName) + String_getLength(this->fullName);
  
  while (p>=String_getBuffer(this->fullName) )
  {
    if (*p=='/') 
    {
      p++;
      result = String_new(p);
      break;
    }
    p--;
  }
  
  return result;
}

/**********************************************//** 
  @brief Load the content of a file
  @public
  @memberof FileDesc
**************************************************/
PUBLIC String * FileDesc_load(FileDesc * this)
{
  String * fileContent = 0;
  FileIo * f = FileIo_new();

  FileIo_openFile(f, FileDesc_getFullName(this));
  if (f)
  {
	  FileIo_fSeekEnd(f, 0);
	  int length=FileIo_ftell(f);
	  FileIo_fSeekSet(f, 0);
        
	  char * buffer = (char*)Memory_alloc(length+1);
    if (buffer)
    {
      FileIo_read(f, buffer, length);
      buffer[length] = 0;
      fileContent = String_new(buffer); 
    }
  }
  FileIo_delete(f);
  return fileContent;
}
