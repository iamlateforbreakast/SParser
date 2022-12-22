#ifndef _POOL_
#define _POOL_
/*********************************************************************************
* Pool.h
*
*********************************************************************************/
#include "CommonTypes.h"

typedef struct Pool
{
	unsigned int isFile;
} Pool;

PUBLIC Pool * Pool_new(unsigned int nbMemChunk, unsigned int memChunkSize);
PUBLIC Pool * Pool_newFromFile(char * fileName);
PUBLIC void Pool_free(Pool * pool);
PUBLIC void Pool_alloc(Pool * pool, unsigned int size);
PUBLIC Pool_dealloc();
#endif
