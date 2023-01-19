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

typedef struct Pool
{
    unsigned int isFile;
    unsigned int nbMemChunks;
    unsigned int maxNbMemChunks;
    unsigned int memChunkSize;
    unsigned int nbAllocatedChunks;
    unsigned int firstAvailable;
    unsigned int lastAllocated;
    void * pool;
    FILE * file;
} Pool;

PUBLIC Pool * Pool_new(unsigned int nbMemChunks, unsigned int memChunkSize);
PUBLIC Pool * Pool_newFromFile(char * fileName, unsigned int nbMemChunks, unsigned int memChunkSize);
PUBLIC void Pool_free(Pool * pool);
PUBLIC AllocStatus Pool_alloc(Pool * pool, unsigned int * idx);
PUBLIC void Pool_dealloc(Pool * pool, unsigned int p);
PUBLIC void Pool_write(Pool * pool, unsigned int idx, void * p);
PUBLIC void Pool_read(Pool * pool, unsigned int idx, void * p);
PUBLIC void Pool_report(Pool * pool);
PUBLIC unsigned int Pool_reportSizeInBytes(Pool* pool);
PUBLIC unsigned int Pool_reportNbNodes(Pool* pool);
#endif
