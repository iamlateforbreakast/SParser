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
    unsigned int memChunkSize;
    unsigned int nbAllocatedChunks;
    void * pool;
    FILE * file;
} Pool;

PUBLIC Pool * Pool_new(unsigned int nbMemChunks, unsigned int memChunkSize);
PUBLIC Pool * Pool_newFromFile(char * fileName, unsigned int nbMemChunks, unsigned int memChunkSize);
PUBLIC void Pool_free(Pool * pool);
PUBLIC void * Pool_alloc(Pool * pool);
PUBLIC void Pool_dealloc(Pool * pool, unsigned int p);
#endif
