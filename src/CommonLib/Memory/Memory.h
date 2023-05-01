/* Memory.h */

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "Types.h"

PUBLIC void* Memory_alloc(unsigned int nbBytes);
PUBLIC void * Memory_realloc(void * pointer, unsigned int prevSizeBytes, unsigned int newSizeBytes);
PUBLIC void Memory_free(void* pointer, unsigned int nbBytes);
PUBLIC void Memory_set(void * pointer, unsigned char val, unsigned int nbBytes);
PUBLIC void Memory_copy(void * pointer, void * src, unsigned int nbBytes);
PUBLIC int Memory_ncmp(void * pointer, void * compared, unsigned int nbBytes);
PUBLIC int Memory_cmp(void * pointer, void * compared);
PUBLIC unsigned int Memory_len(const void * pointer);
PUBLIC void Memory_report();
PUBLIC int Memory_getAllocRequestNb();
PUBLIC int Memory_getFreeRequestNb();

#endif /* _MEMORY_H_ */
