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
   newPool->isFile = 0;
   newPool->pool = (MemChunk *)malloc(sizeof(MemChunk) * newPool->nbMemChunks);
   newPool->file = 0;
   
   for (int i=0; i<newPool->nbMemChunks; i++)
   {
      MemChunk * memChunk = newPool->pool + i * (sizeof(MemChunk) + memChunkSize);
      if (i>0) memChunk->prev = i - 1;
      if (i<newPool->nbMemChunks-1) memChunk->next = i + 1;
      memChunk->isFree = 1;
   }
   
   return newPool;
}

PUBLIC Pool * Pool_newFromFile(char * fileName,unsigned int nbMemChunks, unsigned int memChunkSize)
{
   Pool * newPool = 0;
   FILE * file;
   
   newPool = (Pool *)malloc(sizeof(Pool));
   newPool->nbMemChunks = nbMemChunks;
   newPool->memChunkSize = memChunkSize;
   newPool->isFile = 1;
   newPool->pool = 0;
   
   // If file exists
   // else
   newPool->file = fopen(fileName,"b");
   for (i=0; i<nbMemChunks; i++)
   {
      MemChunk * memChunk = newPool->pool + i * (sizeof(MemChunk) + memChunkSize);
      if (i>0) memChunk->prev = i - 1;
      if (i<newPool->nbMemChunks-1) memChunk->next = i + 1;
      memChunk->isFree = 1;
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
      if (!pool->isFile) 
         free(pool->pool);
      else
         fclose(pool->file);
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
