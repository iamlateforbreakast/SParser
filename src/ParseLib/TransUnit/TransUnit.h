/* TransUnit.h */
#ifndef _TRANSUNIT_H_
#define _TRANSUNIT_H_

#include "Types.h"
#include "FileMgr.h"

typedef struct TransUnit TransUnit;

PUBLIC TransUnit * TransUnit_new(FileDesc * file, FileMgr * fileMgr);
PUBLIC void TransUnit_delete(TransUnit * this);
PUBLIC void TransUnit_print(TransUnit* this);
PUBLIC unsigned int TransUnit_getSize(TransUnit* this);
PUBLIC char* TransUnit_getName(TransUnit* this);
PUBLIC String * TransUnit_getNextBuffer(TransUnit* this);

#endif /* _CONFIGURATION_H_ */
