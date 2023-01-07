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
} MemChunk;

/*********************************************************************************
* Pool_new
* input: number of memory chunks to allocate
* input: size of memory chunk
* output: A pool of memory
*********************************************************************************/
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

/*********************************************************************************
* Pool_newFromFile
* input: file name
* input: number of memory chunks to allocate
* input: size of memory chunk
* output: A pool of memory
*********************************************************************************/
PUBLIC Pool * Pool_newFromFile(char * fileName,unsigned int nbMemChunks, unsigned int memChunkSize)
{
   Pool * newPool = 0;
   
   newPool = (Pool *)malloc(sizeof(Pool));
   newPool->nbMemChunks = nbMemChunks;
   newPool->memChunkSize = memChunkSize;
   newPool->isFile = 1;
   newPool->pool = 0;
   
   // If file exists
   // else
   newPool->file = fopen(fileName,"rb");
   if (newPool->file)
   {
   }
   else
   {
      // Create the file
      newPool->file = fopen(fileName, "b");
      for (int i=0; i<nbMemChunks; i++)
      {
         MemChunk memChunk;
         //= newPool->pool + i * (sizeof(MemChunk) + memChunkSize);
         if (i>0) 
            memChunk->prev = i - 1;
         if (i<newPool->nbMemChunks-1) 
            memChunk->next = i + 1;
         memChunk->isFree = 1;
         fwrite(&memChunk, sizeof(MemChunk), 1, newPool->file);
      }
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

PUBLIC unsigned int Pool_alloc(Pool * pool)
{
   unisgned int idx = 0;
   // Check if free slots left
   // return nextFree
   // NextFree = nextFree next
   //p = pool->pool + pool->nextAvailable * (sizeof(MemChunk) + pool->memChunkSize);
   
   return idx;
}

PUBLIC void Pool_dealloc(Pool * pool, unsigned int p)
{
}

PUBLIC void Pool_write(Pool * pool, void * p)
{
}

PUBLIC void * Pool_read(Pool * pool, unsigned int idx)
{
   return 0;
}
