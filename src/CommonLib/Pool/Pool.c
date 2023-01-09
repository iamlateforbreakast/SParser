/*********************************************************************************
* Pool.c
*
*********************************************************************************/
#include "Pool.h"

#define END_OF_QUEUE   (0xFFFFFFFF)
#define END_OF_ALLOC   (0xFFFFFFFE)
#define START_OF_AVAIL (0xFFFFFFFD)

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
   newPool->maxNbMemChunks = nbMemChunks; // TBC
   newPool->nbAllocatedChunks = 0;
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
   newPool->maxNbMemChunks = nbMemChunks;
   newPool->memChunkSize = memChunkSize;
   newPool->firstAvailable = 0;
   newPool->lastAllocated = 0;
   newPool->nbAllocatedChunks = 0;
   newPool->isFile = 1;
   newPool->pool = 0;
   
   
   //fclose(newPool->file);
   // If file exists
   // else
   newPool->file = fopen(fileName,"rb+");
   if (newPool->file != 0)
   {
      printf("File %s exists\n", fileName);
      for (int i=0; i<nbMemChunks; i++)
      {
         MemChunk memChunk;
         fseek(newPool->file, 0, SEEK_SET);
         fread(&memChunk, sizeof(MemChunk), 1, newPool->file);
         newPool->nbAllocatedChunks++;
         if (memChunk.prev == START_OF_AVAIL) newPool->firstAvailable = i;
         if (memChunk.next == END_OF_ALLOC) newPool->lastAllocated = i;
      }
   }
   else
   {
      // Create the file
      printf("File %s is created\n", fileName);
      newPool->file= fopen("test.pool","wb+");
      //newPool->file = fopen(fileName, "wb+");
      if (newPool->file != 0)
      {
         for (int i=0; i<nbMemChunks; i++)
         {
            MemChunk memChunk;
            char emptyByte = 0;
            //= newPool->pool + i * (sizeof(MemChunk) + memChunkSize);
            if (i>0) 
               memChunk.prev = i - 1;
            else
               memChunk.prev = START_OF_AVAIL;
            if (i<newPool->nbMemChunks-1) 
               memChunk.next = i + 1;
            else
               memChunk.next = END_OF_QUEUE;
            memChunk.isFree = 1;
            fwrite(&memChunk, sizeof(MemChunk), 1, newPool->file);
            fwrite(&emptyByte, sizeof(emptyByte), newPool->memChunkSize, newPool->file);
            fflush(newPool->file);
         }
      }
      else
      {
         printf("Cannot create file %s\n", fileName);
         exit(1);
      }
   }
   printf("Pool memChunkSize: %d\n", newPool->memChunkSize);
   
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

PUBLIC AllocStatus Pool_alloc(Pool * pool, unsigned int * ptrIdx)
{
   unsigned int idx = 0;
   long int lastAllocatedOffset = pool->lastAllocated * (sizeof(MemChunk) + pool->memChunkSize);
   long int firstAvailableOffset = pool->firstAvailable * (sizeof(MemChunk) + pool->memChunkSize);
   
   // Case where nothing is allocated
   if (pool->nbAllocatedChunks == 0)
   {
      MemChunk memChunk;
      pool->lastAllocated = 0;
      memChunk.prev = END_OF_QUEUE;
      memChunk.next = END_OF_ALLOC;
      memChunk.isFree = 0;
      fseek(pool->file, 0, SEEK_SET);
      fwrite(&memChunk, sizeof(MemChunk), 1, pool->file);
      fseek(pool->file, sizeof(unsigned int), SEEK_CUR);
      memChunk.prev = START_OF_AVAIL;
      fseek(pool->file, sizeof(unsigned int), SEEK_CUR);
      fwrite(&memChunk.prev, sizeof(unsigned int), 1, pool->file);
      fflush(pool->file);
      pool->firstAvailable = 1;
      pool->nbAllocatedChunks = 1;
      fseek(pool->file, 0, SEEK_SET);
      *ptrIdx = 0;
      return ALLOC_OK;
   }
   // Check if free slots left
   if (pool->nbAllocatedChunks <= pool->maxNbMemChunks)
   {
      *ptrIdx = pool->firstAvailable;
      MemChunk memChunk;
      // Update last allocated chunk
      fseek(pool->file, lastAllocatedOffset, SEEK_SET);
      memChunk.next = pool->firstAvailable;
      fwrite(&memChunk.next, sizeof(unsigned int), 1, pool->file);
      
      // Update old first available
      fseek(pool->file, firstAvailableOffset, SEEK_SET);
      fread(&memChunk, sizeof(MemChunk), 1, pool->file);
      memChunk.prev = pool->lastAllocated;
      pool->lastAllocated = pool->firstAvailable;
      pool->firstAvailable = memChunk.next;
      memChunk.next = END_OF_ALLOC;
      memChunk.isFree = 0;
      fseek(pool->file, -sizeof(MemChunk), SEEK_CUR);
      fwrite(&memChunk, sizeof(MemChunk), 1, pool->file);
      
      // Update new first available
      if (pool->firstAvailable!=END_OF_QUEUE)
      {
         firstAvailableOffset = pool->firstAvailable * (sizeof(MemChunk) + pool->memChunkSize);
         fseek(pool->file, firstAvailableOffset + sizeof(unsigned int), SEEK_SET);
         memChunk.prev = START_OF_AVAIL;
         fwrite(&memChunk.prev, sizeof(unsigned int), 1, pool->file);
      }
      pool->nbAllocatedChunks++;
      return ALLOC_OK;
   }
   
   return ALLOC_FAIL;
}

PUBLIC void Pool_dealloc(Pool * pool, unsigned int idx)
{
   long int allocatedOffset = idx * (sizeof(MemChunk) + pool->memChunkSize);
   long int firstAvailableOffset = pool->firstAvailable * (sizeof(MemChunk) + pool->memChunkSize);
   
   if (pool->nbAllocatedChunks == pool->maxNbMemChunks)
   {
      MemChunk memChunk;
      fseek(pool->file, -sizeof(MemChunk) - pool->memChunkSize, SEEK_END);
      fread(&memChunk, sizeof(MemChunk), 1, pool->file);
      pool->lastAllocated = memChunk.prev;
      pool->firstAvailable = pool->maxNbMemChunks - 1;
      memChunk.next = END_OF_QUEUE;
      memChunk.prev = START_OF_AVAIL;
      memChunk.isFree = 1;
      
      fseek(pool->file, -sizeof(MemChunk) - pool->memChunkSize, SEEK_END);
      fwrite(&memChunk, sizeof(MemChunk), 1, pool->file);
      pool->nbAllocatedChunks = pool->maxNbMemChunks - 1;
   }
   if (pool->nbAllocatedChunks > 0)
   {
      MemChunk memChunk;
      fseek(pool->file, firstAvailableOffset + sizeof(unsigned int), SEEK_SET);
      memChunk.prev = idx;
      fwrite(&memChunk.prev, sizeof(unsigned int), 1, pool->file);
      
      fseek(pool->file, allocatedOffset, SEEK_SET); 
      fread(&memChunk, sizeof(MemChunk), 1, pool->file);
      unsigned int prevIdx = memChunk.prev;
      unsigned int nextIdx = memChunk.next;
      
      memChunk.prev = START_OF_AVAIL;
      memChunk.next = pool->firstAvailable;
      memChunk.isFree = 1;
      pool->firstAvailable = idx;
      fseek(pool->file, -sizeof(MemChunk), SEEK_CUR);
      fwrite(&memChunk, sizeof(MemChunk), 1, pool->file);
      
      if (prevIdx !=END_OF_QUEUE)
      {
         long int prevOffset =  prevIdx * (sizeof(MemChunk) + pool->memChunkSize);
         fseek(pool->file, prevOffset, SEEK_SET);
         fwrite(&nextIdx, sizeof(unsigned int), 1, pool->file);
      }
      
      if (nextIdx !=END_OF_ALLOC)
      {
         long int nextOffset = nextIdx * (sizeof(MemChunk) + pool->memChunkSize);
         fseek(pool->file, nextOffset + sizeof(unsigned int), SEEK_SET);
         fwrite(&prevIdx, sizeof(unsigned int), 1, pool->file);
      }
      pool->nbAllocatedChunks--;
   }
   
}

PUBLIC void Pool_write(Pool * pool, unsigned int idx, void * p)
{
   long int offset = idx * (sizeof(MemChunk) + pool->memChunkSize) + sizeof(MemChunk);
   
   fseek(pool->file, offset, SEEK_SET);
   //fwrite(pool->file, offset
}

PUBLIC void Pool_read(Pool * pool, unsigned int idx, void * p)
{
   long int offset = idx * (sizeof(MemChunk) + pool->memChunkSize) + sizeof(MemChunk);
   
   fseek(pool->file, offset, SEEK_SET);
   fread(p, pool->memChunkSize, 1, pool->file);
}

PUBLIC void Pool_report(Pool * pool)
{
   unsigned int nbByteRead = 0;
   
   printf("Pool Report:\n");
   printf("  maxNbMemChunks: %d\n", pool->maxNbMemChunks);
   printf("  memChunkSize: %d\n", pool->memChunkSize);
   printf("  nbAllocatedChunks: %d\n", pool->nbAllocatedChunks);
   printf("  firstAvailable: %d\n", pool->firstAvailable);
   printf("  lastAllocated: %d\n", pool->lastAllocated);
   fseek(pool->file, 0, SEEK_SET);
   for (int i = 0; i<pool->maxNbMemChunks; i++)
   {
      MemChunk memChunk;
      //printf("Ftell %d\n",ftell(pool->file));
      nbByteRead = fread(&memChunk, sizeof(MemChunk), 1, pool->file);
      if (nbByteRead!=1) printf("Read failure\n");
      //printf("NbBYteRead %d\n", nbByteRead);
      fseek(pool->file, pool->memChunkSize, SEEK_CUR);
      printf("MemChunk %d\n",i);
      switch (memChunk.prev)
      {
         case END_OF_QUEUE:
            printf("   prev: END_OF_QUEUE\n");
            break;
         case END_OF_ALLOC:
            printf("   prev: END_OF_ALLOC\n");
            break;
         case START_OF_AVAIL:
            printf("   prev: START_OF_AVAIL\n");
            break;
         default:
            printf("   prev: %d\n", memChunk.prev);
            break;
      }
      switch (memChunk.next)
      {
         case END_OF_QUEUE:
            printf("   next: END_OF_QUEUE\n");
            break;
         case END_OF_ALLOC:
            printf("   next: END_OF_ALLOC\n");
            break;
         case START_OF_AVAIL:
            printf("   next: START_OF_AVAIL\n");
            break;
         default:
            printf("   next: %d\n", memChunk.next);
            break;
      }
      
      printf("   isFree: %d\n", memChunk.isFree);
   }
}
