#ifndef _POOL_
#define _POOL_
/*********************************************************************************
* Pool.h
*
*********************************************************************************/
#include "Types.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum AllocStatus
{
    ALLOC_OK = 0,
    ALLOC_FAIL = 1
} AllocStatus;

typedef struct PoolCache
{
    unsigned int idx;
    unsigned int isUsed;
    void* cache;
} PoolCache;

typedef struct Pool Pool;

PUBLIC Pool* Pool_new(unsigned int nbMemChunks, unsigned int memChunkSize);
PUBLIC Pool* Pool_newFromFile(char* fileName, unsigned int nbMemChunks, unsigned int memChunkSize);
PUBLIC void Pool_free(Pool* pool);
PUBLIC void * Pool_alloc(Pool* pool, unsigned int * ptrIdx);
PUBLIC void Pool_dealloc(Pool* pool, unsigned int p);
PUBLIC void Pool_write(Pool* pool, unsigned int idx, void* ptrContent);
PUBLIC void* Pool_read(Pool* pool, unsigned int idx);
PUBLIC unsigned int Pool_addToChunkCache(Pool* pool, void* p, unsigned int length);
PUBLIC void Pool_report(Pool* pool);
PUBLIC unsigned int Pool_reportSizeInBytes(Pool* pool);
PUBLIC unsigned int Pool_reportNbNodes(Pool* pool);
PUBLIC void* Pool_getCache1(Pool* pool);
PUBLIC void* Pool_getCache2(Pool* pool);
PUBLIC void Pool_discardCache(Pool* pool, unsigned int idx);
#endif /* _POOL_ */
