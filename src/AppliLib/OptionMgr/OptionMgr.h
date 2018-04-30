/* OptionMgr.h */

#ifndef _OPTIONMGR_H_
#define _OPTIONMGR_H_

#include "Types.h"

typedef struct OptionMgr OptionMgr;

PUBLIC void OptionMgr_delete(OptionMgr * this);
PUBLIC OptionMgr * OptionMgr_copy(OptionMgr * this);
PUBLIC PUBLIC OptionMgr* OptionMgr_getOptionMgr();

#endif /* _OPTIONMGR_H_ */