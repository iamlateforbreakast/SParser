/* Grammar2.h */

#include "Types.h"
#include "SdbMgr.h"
#include "FileReader.h"

typedef struct Grammar2 Grammar2;

PUBLIC Grammar2 * Grammar2_new(FileReader * fr, SdbMgr * sdbMgr);
PUBLIC void Grammar2_delete(Grammar2 * this);
PUBLIC Grammar2 * Grammar2_copy(Grammar2 * this);
PUBLIC void Grammar2_process(Grammar2 * this);
