/* FileReader.h */

#ifndef _FILEREADER_H_
#define _FILEREADER_H_

#include "Types.h"

typedef struct FileReader FileReader;

PUBLIC FileReader * FileReader_new();
void FileReader_delete(FileReader * this);

#endif /* _FILEREADER_H_ */
