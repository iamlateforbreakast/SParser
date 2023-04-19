#ifndef _FILEIO_H_
#define _FILEIO_H_

typedef enum FileIoStatus
{
	UNKNOWN=0,
	FILE_OPEN,
	DIR_OPEN
} FileIoStatus;

typedef struct FileIo FileIo;

FileIo * FileIo_new();
void FileIo_delete();
void FileIo_read();
void FileIo_write();
//Opendir
//Readdir

#endif /* _FILEIO_H_ */
