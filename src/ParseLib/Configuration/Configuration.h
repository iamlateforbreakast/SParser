/* Configuration.h */
#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "Types.h"

typedef struct Configuration Configuration;

PUBLIC Configuration * Configuration_new(String * input);
PUBLIC void Configuration_delete(Configuration * this);

#endif /* _CONFIGURATION_H_ */
