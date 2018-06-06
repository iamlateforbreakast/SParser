/* Memory.h */

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "Types.h"

PUBLIC void* Memory_alloc(unsigned int nbBytes);
PUBLIC void Memory_free(void* pointer, unsigned int nbBytes);
PUBLIC void Memory_set(void * pointer, unsigned char val, unsigned int nbBytes);
PUBLIC void Memory_copy(void * pointer, void * src, unsigned int nbBytes);
PUBLIC unsigned int Memory_ncmp(void * pointer, void * compared, unsigned int nbBytes);
PUBLIC unsigned int Memory_cmp(void * pointer, void * compared);
PUBLIC void Memory_report();

#endif /* _MEMORY_H_ */