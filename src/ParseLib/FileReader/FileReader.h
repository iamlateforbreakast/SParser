/* FileReader.h */

#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include "Types.h"
#include "String2.h"

typedef struct FileReader FileReader;

PUBLIC FileReader * FileReader_new();
PUBLIC void FileReader_delete(FileReader * this);
PUBLIC FileReader * FileReader_copy(FileReader * this);
PUBLIC void FileReader_print(FileReader * this);
PUBLIC unsigned int FileReader_getSize(FileReader * this);
PUBLIC char * FileReader_getBuffer(FileReader * this);
PUBLIC String * FileReader_getName(FileReader * this);
PUBLIC char * FileReader_addFile(FileReader * this, String * fileName);
#endif /* _FILEREADER_H_ */
