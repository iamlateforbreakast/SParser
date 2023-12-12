/* FileDesc.h */

#ifndef _FILEDESC_H_
#define _FILEDESC_H_

#include "Types.h"
#include "String2.h"

typedef struct FileDesc FileDesc;

PUBLIC FileDesc * FileDesc_new();
PUBLIC void FileDesc_delete(FileDesc * this);
PUBLIC FileDesc * FileDesc_copy(FileDesc * this);
PUBLIC unsigned int FileDesc_getSize(FileDesc* this);
PUBLIC void FileDesc_setFullName(FileDesc * this, String * fullName);
PUBLIC String * FileDesc_getFullName(FileDesc * this);
PUBLIC void FileDesc_setName(FileDesc * this, String * name);
PUBLIC String * FileDesc_getName(FileDesc * this);
PUBLIC String * FileDesc_load(FileDesc * this);

#endif /* _FILEDESC_H_ */
