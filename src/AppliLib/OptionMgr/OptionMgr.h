/* OptionMgr.h */

#ifndef _OPTIONMGR_H_
#define _OPTIONMGR_H_

#include "Types.h"

typedef struct OptionMgr OptionMgr;

PUBLIC void OptionMgr_delete(OptionMgr * this);
PUBLIC OptionMgr * OptionMgr_copy(OptionMgr * this);
PUBLIC PUBLIC OptionMgr* OptionMgr_getOptionMgr();
PUBLIC unsigned int OptionMgr_readFromFile(OptionMgr * this, const char * fileName);
PUBLIC unsigned int OptionMgr_readFromCmdLine(OptionMgr * this, const int argc, const char ** argv);

#endif /* _OPTIONMGR_H_ */