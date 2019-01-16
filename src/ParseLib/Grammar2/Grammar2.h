/* Grammar2.h */

#include "Types.h"
#include "SdbMgr.h"
#include "FileReader.h"

typedef struct Grammar2 Grammar2;

PUBLIC Grammar2 * Grammar2_new(FileReader * fr, SdbMgr * sdbMgr);
PUBLIC void Grammar2_delete(Grammar2 * this);
PUBLIC Grammar2 * Grammar2_copy(Grammar2 * this);
PUBLIC void Grammar2_process(Grammar2 * this);
PUBLIC FileReader * Grammar2_getFileReader(Grammar2 * grammar);
PUBLIC SdbMgr * Grammar2_getSdbMgr(Grammar2 * grammar);
PUBLIC void Grammar2_addToBuffer(Grammar2 * grammar, char * text);
PUBLIC void Grammar2_addComment(Grammar2 * this);
PUBLIC void Grammar2_addCodeNode(Grammar2 * this);
PUBLIC char * Grammar2_processNewFile(Grammar2 * this, String * fileName);