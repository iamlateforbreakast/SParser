/* FileMgr.h */

#ifndef _FILEMGR_H_
#define _FILEMGR_H_

#include "Types.h"
#include "String2.h"

typedef struct FileMgr FileMgr;

PUBLIC void FileMgr_delete(FileMgr * this);
PUBLIC FileMgr * FileMgr_copy(FileMgr * this);
PUBLIC String* FileMgr_load(FileMgr* this, const char * fileName);
PUBLIC void FileMgr_close(FileMgr* this, String* fileName);
PUBLIC FileMgr* FileMgr_getRef();
PUBLIC unsigned int FileMgr_addDirectory(FileMgr * this, const char * directoryName);
PUBLIC unsigned int FileMgr_addFile(FileMgr * this, const char * fileName);
#endif /* _FILEMGR_H_ */

/* PUBLIC String* FileMgr_getRootPath(FileMgr* this);
PUBLIC List* FileMgr_filterFiles(FileMgr* this, String* filter);
PUBLIC String* FileMgr_searchAndLoad(FileMgr* this, String* fileName);
PUBLIC List* FileMgr_getFiles(FileMgr* this); */
