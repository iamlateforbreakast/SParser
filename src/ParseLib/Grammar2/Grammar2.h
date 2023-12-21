/* Grammar2.h */

#include "Types.h"
#include "SdbMgr.h"
#include "FileReader.h"

typedef struct Grammar2 Grammar2;

PUBLIC Grammar2 * Grammar2_new(FileReader * fr, SdbMgr * sdbMgr);
PUBLIC void Grammar2_delete(Grammar2 * this);
PUBLIC Grammar2 * Grammar2_copy(Grammar2 * this);
PUBLIC void Grammar2_print(Grammar2 * this);
PUBLIC unsigned int Grammar2_getSize(Grammar2 * this);
PUBLIC void Grammar2_process(Grammar2 * this);
PUBLIC FileReader * Grammar2_getFileReader(Grammar2 * grammar); // Not used
PUBLIC SdbMgr * Grammar2_getSdbMgr(Grammar2 * grammar); // Not used
PUBLIC void Grammar2_addToBuffer(Grammar2 * grammar, char * text); // Used by lex.c
PUBLIC void Grammar2_addComment(Grammar2 * this); // Used by parse.c
PUBLIC void Grammar2_addCodeNode(Grammar2 * this);
PUBLIC void Grammar2_addIncludeNode(Grammar2 * this, char * name);
PUBLIC char * Grammar2_processNewFile(Grammar2 * this, String * fileName); // Used by lex.c
PUBLIC void Grammar2_returnToFile(Grammar2 * this); // Used by lex.c