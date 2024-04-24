/* Configuration.h */
#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "Types.h"
#include "String2.h"
#include "List.h"
#include "FileMgr.h"

typedef struct Configuration Configuration;

PUBLIC Configuration * Configuration_new(String * input);
PUBLIC void Configuration_delete(Configuration * this);
PUBLIC void Configuration_print(Configuration * this);
PUBLIC unsigned int Configuration_getSize(Configuration * this);
PUBLIC List * Configuration_getProducts(Configuration* this);
#endif /* _CONFIGURATION_H_ */

