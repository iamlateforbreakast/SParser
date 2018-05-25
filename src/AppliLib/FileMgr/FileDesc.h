/* FileDesc.h */

#ifndef _FILEDESC_H_
#define _FILEDESC_H_

#include "Types.h"
#include "String2.h"

typedef struct FileDesc FileDesc;

PUBLIC FileDesc * FileDesc_new();
PUBLIC void FileDesc_delete(FileDesc * this);
PUBLIC FileDesc * FileDesc_copy(FileDesc * this);
PUBLIC void FileDesc_setFullName(FileDesc * this, String * fullName);
PUBLIC String * FileDesc_getFullName(FileDesc * this);
PUBLIC void FileDesc_setName(FileDesc * this, String * name);

#endif /* _FILEDESC_H_ */
