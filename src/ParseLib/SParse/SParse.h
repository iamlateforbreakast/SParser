/* SParse.h */

#ifndef _SPARSE_H_
#define _SPARSE_H_

#include "Types.h"

typedef struct SParse SParse;

PUBLIC SParse *SParse_new(/* Sdb name */);
PUBLIC void SParse_delete(SParse * this);
PUBLIC SParse * SParse_copy(SParse * this);

#endif /* _SPARSE_H_ */
