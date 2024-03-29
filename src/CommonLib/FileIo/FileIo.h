#ifndef _FILEIO_H_
#define _FILEIO_H_
#include "String2.h"
#include "Types.h"

typedef enum FileIoStatus
{
	UNKNOWN=0,
	FILE_OPEN,
	DIR_OPEN
} FileIoStatus;

typedef struct FileIo FileIo;

PUBLIC FileIo * FileIo_new();
PUBLIC void FileIo_delete();
PUBLIC FileIo* FileIo_copy(FileIo* this);
PUBLIC int FileIo_comp(FileIo* this, FileIo* compare);
PUBLIC void FileIo_print(FileIo* this);
PUBLIC unsigned int FileIo_getSize(FileIo* this);
PUBLIC void FileIo_openFile(FileIo* this, String* fullFileName);
PUBLIC void FileIo_createFile(FileIo* this, String* fullFileName);
PUBLIC void FileIo_openDir(FileIo* this, String* fullFileName);
PUBLIC void FileIo_createDir(FileIo* this, String* fullDirName);
PUBLIC void FileIo_write(FileIo* this, char* buffer, int length);
PUBLIC void FileIo_read(FileIo* this, char* buffer, int length);
PUBLIC void FileIo_remove(FileIo* this, String* fullFileName);
PUBLIC String * FileIo_getCwd(FileIo* this);
PUBLIC List * FileIo_listDirs(FileIo * this, String * directory);
PUBLIC List* FileIo_listFiles(FileIo* this, String * directory);
PUBLIC int FileIo_fSeekEnd(FileIo * this, int pos);
PUBLIC int FileIo_fSeekSet(FileIo * this, int pos);
PUBLIC int FileIo_ftell(FileIo* this);
PUBLIC FileIoStatus FileIo_isOpen(FileIo * this);
//Opendir
//Readdir

#endif /* _FILEIO_H_ */
