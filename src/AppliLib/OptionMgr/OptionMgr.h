/* OptionMgr.h */

#ifndef _OPTIONMGR_H_
#define _OPTIONMGR_H_

#include "Types.h"
#include "String2.h"

typedef struct OptionMgr OptionMgr;

PUBLIC void OptionMgr_delete(OptionMgr * this);
PUBLIC OptionMgr * OptionMgr_copy(OptionMgr * this);
PUBLIC PUBLIC OptionMgr* OptionMgr_getRef();
PUBLIC String * OptionMgr_getOption(OptionMgr * this, const char * name);
PUBLIC void OptionMgr_setOption(OptionMgr * this, const char * optionName, String * value);
PUBLIC unsigned int OptionMgr_readFromFile(OptionMgr * this);
PUBLIC unsigned int OptionMgr_readFromCmdLine(OptionMgr * this, const int argc, const char ** argv);
PUBLIC unsigned int OptionMgr_isOptionEnabled(OptionMgr* this, const char * optionName);

#endif /* _OPTIONMGR_H_ */
