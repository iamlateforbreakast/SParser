/* SParse.h */

#ifndef _SPARSE_H_
#define _SPARSE_H_

#include "Types.h"
#include "String2.h"

typedef struct SParse SParse;

PUBLIC SParse *SParse_new(String * sdbName);
PUBLIC void SParse_delete(SParse * this);
PUBLIC SParse * SParse_copy(SParse * this);
PUBLIC void SParse_print(SParse * this);
PUBLIC unsigned int SParse_getSize(SParse * this);
PUBLIC unsigned int SParse_parse(SParse * this, const char * extension);

#endif /* _SPARSE_H_ */
