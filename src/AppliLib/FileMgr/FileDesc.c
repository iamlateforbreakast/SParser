/**********************************************//** 
  @file FileDesc.c
  
  @brief The FileDesc class describe a File in the FIleMgr.
  
  The class FileDesc is TBD
**************************************************/

#include "FileDesc.h"
#include "String2.h"
#include "Object.h"

struct FileDesc
{
  Object object;
  String * name;
  String * fullName;
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
  
  this = (FileDesc*)Object_new(sizeof(FileDesc),(Destructor)&FileDesc_delete, (Copy_operator)&FileDesc_copy);
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
