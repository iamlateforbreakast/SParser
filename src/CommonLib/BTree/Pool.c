/*********************************************************************************
* Pool.c
*
*********************************************************************************/
#include "Pool.h"

typedef struct MemChunk
{
   unsigned int next;
   unsigned int prev;
   unsigned int isFree;
}

PUBLIC Pool * Pool_new(unsigned int nbMemChunks, unsigned int memChunkSize)
{
   Pool * newPool = 0;
   
   newPool = (Pool *)malloc(sizeof(Pool));
   newPool->nbMemChunks = nbMemChunks;
   newPool->memChunkSize = memChunkSize;
   
   return newPool;
}

PUBLIC Pool * Pool_newFromFile(char * fileName,unsigned int nbMemChunks, unsigned int memChunkSize)
{
   Pool * newPool = 0;
   FILE * file;
   
   newPool = (Pool *)malloc(sizeof(Pool));
   newPool->nbMemChunks = nbMemChunks;
   newPool->memChunkSize = memChunkSize;
   // If file exists
   // else
   file = fopen(fileName,"b");
   for (i=0; i<nbMemChunks; i++)
   {
      
   }
   
   return newPool;
}

PUBLIC void Pool_free(Pool * pool)
{
   if (pool)
   {
      // If is file
      // Close file
      // Else free mem
      free(pool);
   }
}

PUBLIC void * Pool_alloc(Pool * pool)
{
   // Check if free slots left
   // return nextFree
   // NextFree = nextFree next
}

PUBLIC void Pool_dealloc(Pool * pool, void * p)
{
}
