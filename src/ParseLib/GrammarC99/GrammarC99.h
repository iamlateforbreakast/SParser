/* GrammarC99.h */
#ifndef _GRAMMARC99_H_
#define _GRAMMARC99_H_

#include "Types.h"
#include "FileMgr.h"
#include "Grammar.h"

typedef struct GrammarC99 GrammarC99;

PUBLIC Grammar* GrammarC99_new(FileDesc * fileDesc, FileMgr * fm);
PUBLIC void GrammarC99_delete(Grammar* this);
PUBLIC void GrammarC99_print(Grammar* this);
PUBLIC unsigned int GrammarC99_getSize(Grammar* this);
PUBLIC void GrammarC99_process(GrammarC99* this);

#endif /* _GRAMMARC99_H_ */
