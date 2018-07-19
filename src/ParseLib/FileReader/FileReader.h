/* FileReader.h */

#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include "Types.h"

typedef struct FileReader FileReader;

PUBLIC FileReader * FileReader_new();
PUBLIC void FileReader_delete(FileReader * this);
PUBLIC FileReader * FileReader_copy(FileReader * this);
PUBLIC char * Filereader_getBuffer(FileReader * this);
#endif /* _FILEREADER_H_ */
