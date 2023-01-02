#ifndef _POOL_
#define _POOL_
/*********************************************************************************
* Pool.h
*
*********************************************************************************/
#include "Types.h"
#include <stdlib.h>

typedef struct Pool
{
    unsigned int isFile;
    unsigned int nbMemChunks;
    unsigned int maxNbMemChunks;
    unsigned int memChunksSize;
    unsigned int nbAllocatedChunks;
    void * pool;
    FILE * file;
} Pool;

PUBLIC Pool * Pool_new(unsigned int nbMemChunks, unsigned int memChunkSize);
PUBLIC Pool * Pool_newFromFile(char * fileName);
PUBLIC void Pool_free(Pool * pool);
PUBLIC void * Pool_alloc(Pool * pool, unsigned int size);
PUBLIC void Pool_dealloc();
#endif
