/**********************************************//**
  @file Pool.c

  @brief This file contains the implementation of the class Pool.

  The class List implement the Pool operations
  - Alloc
  - De-alloc
**************************************************/
#include "Pool.h"
#include "Memory.h"
#include <stdio.h>

#define CACHE_NB (6)
#define END_OF_QUEUE   (0xFFFFFFFF)
#define END_OF_ALLOC   (0xFFFFFFFE)
#define START_OF_AVAIL (0xFFFFFFFD)

/**********************************************//**
  @private
**************************************************/
PRIVATE AllocStatus Pool_allocInMemory(Pool* pool, unsigned int* ptrIdx);
PRIVATE AllocStatus Pool_allocInFile(Pool* pool, unsigned int* ptrIdx);
PRIVATE void Pool_deallocInMemory(Pool* pool, unsigned int idx);
PRIVATE void Pool_deallocInFile(Pool* pool, unsigned int idx);
PRIVATE void Pool_reportInFile(Pool* pool);
PRIVATE void Pool_reportInMemory(Pool* pool);
PRIVATE void Pool_readInFile(Pool* pool, unsigned int idx, void* p);
PRIVATE void Pool_readInMemory(Pool* pool, unsigned int idx, void* p);
PRIVATE void Pool_writeInFile(Pool* pool, unsigned int idx, void* p);
PRIVATE void Pool_writeInMemory(Pool* pool, unsigned int idx, void* p);
PRIVATE unsigned int Pool_reportCacheUsed(Pool* pool);

/**********************************************//**
  @private
**************************************************/
typedef struct MemChunk
{
    unsigned int next;
    unsigned int prev;
    unsigned int isFree;
} MemChunk;

/**********************************************//**
  @private
**************************************************/
typedef struct Pool
{
    unsigned int isFile;
    unsigned int nbMemChunks;
    unsigned int maxNbMemChunks;
    unsigned int memChunkSize;
    unsigned int nbAllocatedChunks;
    unsigned int firstAvailable;
    unsigned int lastAllocated;
    unsigned int cacheUsed;
    PoolCache chunkCache[CACHE_NB];
    void* pool;
    FILE* file;
} Pool;

/**********************************************//**
  @brief Create a new instance of the class Pool in RAM
  @public
  @param[in] number of memory chunks to allocate.
  @param[in] size of memory chunk.
  @return New instance.
**************************************************/
PUBLIC Pool* Pool_new(unsigned int nbMemChunks, unsigned int memChunkSize)
{
    Pool* newPool = 0;

    newPool = (Pool*)malloc(sizeof(Pool));
    newPool->nbMemChunks = nbMemChunks;
    newPool->maxNbMemChunks = nbMemChunks; // TBC
    newPool->nbAllocatedChunks = 0;
    newPool->memChunkSize = memChunkSize;
    newPool->firstAvailable = 0;
    newPool->lastAllocated = 0;
    newPool->isFile = 0;
    newPool->pool = (MemChunk*)malloc(newPool->nbMemChunks * (sizeof(MemChunk) + newPool->memChunkSize));
    newPool->file = 0;
    for (int i = 0; i < CACHE_NB; i++)
    {
        //newPool->writeChunkCache[i] = (char*)malloc(sizeof(MemChunk) + newPool->memChunkSize);
        newPool->chunkCache[i].idx = 0;
        newPool->chunkCache[i].isUsed = 0;
        newPool->chunkCache[i].cache = (char*)malloc(sizeof(MemChunk) + newPool->memChunkSize);
    }
    newPool->cacheUsed = 0;

    for (unsigned int i = 0; i < newPool->nbMemChunks; i++)
    {
        MemChunk* memChunk = (MemChunk*)((char*)newPool->pool + i * (sizeof(MemChunk) + memChunkSize));
        if (i < newPool->nbMemChunks - 1) memChunk->next = i + 1;
        if (i > 0)
        {
            memChunk->prev = i - 1;
        }
        else
        {
            memChunk->prev = START_OF_AVAIL;
        }
        if (i == newPool->maxNbMemChunks - 1)
        {
            memChunk->next = END_OF_QUEUE;
        }
        else
        {
            memChunk->next = i + 1;
        }
        memChunk->isFree = 1;
    }

    return newPool;
}

/**********************************************//**
* @brief Create a new instance of the class Pool in a file
* @public
* @param[in] File name
* @param[in] Number of memory chunks to allocate
* @param[in] Size of memory chunk
* return A pool of memory
**************************************************/
PUBLIC Pool* Pool_newFromFile(char* fileName, unsigned int nbMemChunks, unsigned int memChunkSize)
{
    Pool* newPool = 0;

    newPool = (Pool*)malloc(sizeof(Pool));
    newPool->nbMemChunks = nbMemChunks;
    newPool->maxNbMemChunks = nbMemChunks;
    newPool->memChunkSize = memChunkSize;
    newPool->firstAvailable = 0;
    newPool->lastAllocated = 0;
    newPool->nbAllocatedChunks = 0;
    newPool->isFile = 1;
    newPool->pool = 0;
    for (int i = 0; i < CACHE_NB; i++)
    {
        //newPool->writeChunkCache[i] = (char*)malloc(sizeof(MemChunk) + newPool->memChunkSize);
        newPool->chunkCache[i].idx = 0;
        newPool->chunkCache[i].isUsed = 0;
        newPool->chunkCache[i].cache = (char*)malloc(sizeof(MemChunk) + newPool->memChunkSize);
    }
    newPool->cacheUsed = 0;
    //fclose(newPool->file);
    // If file exists
    // else
    newPool->file = fopen(fileName, "rb+");
    if (newPool->file != 0)
    {
        printf("File %s exists\n", fileName);
        for (unsigned int i = 0; i < nbMemChunks; i++)
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
        newPool->file = fopen(fileName, "wb+");
        //newPool->file = fopen("test.pool", "wb+");
        //newPool->file = fopen(fileName, "wb+");
        if (newPool->file != 0)
        {
            for (unsigned int i = 0; i < nbMemChunks; i++)
            {
                MemChunk memChunk;
                char emptyByte = 0;
                //= newPool->pool + i * (sizeof(MemChunk) + memChunkSize);
                if (i > 0)
                    memChunk.prev = i - 1;
                else
                    memChunk.prev = START_OF_AVAIL;
                if (i < newPool->nbMemChunks - 1)
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

/**********************************************//**
  @brief Pool_free
  @param[in] none
  @return none
**************************************************/
PUBLIC void Pool_free(Pool* pool)
{
    if (pool)
    {
        if (!pool->isFile)
            free(pool->pool);
        else
            fclose(pool->file);
        free(pool);
    }
}

/**********************************************//**
  @brief Pool_alloc
  @param[in] none
  @return Reference to cache position, NULL is cache full
**************************************************/
PUBLIC void* Pool_alloc(Pool* pool, unsigned int* ptrIdx)
{
    if (pool->isFile)
    {
        Pool_allocInFile(pool, ptrIdx);
        // TBC: Consider failure
    }
    else
    {
        Pool_allocInMemory(pool, ptrIdx);     
    }
    for (int i = 0; i < CACHE_NB; i++)
    {
        if (pool->chunkCache[i].isUsed == 0)
        {
            pool->chunkCache[i].isUsed = 1;
            pool->chunkCache[i].idx = *ptrIdx;
            Memory_set(pool->chunkCache[i].cache, 0, pool->memChunkSize);
            pool->cacheUsed = Pool_reportCacheUsed(pool);
            return pool->chunkCache[i].cache;
        }
    }
    // No cache position left
    printf("Error: Pool out of cache\n");
    exit(2);
    return NULL;
}

/**********************************************//**
  @brief Pool_dealloc
  @param[in] none
  @return none
**************************************************/
PUBLIC void Pool_dealloc(Pool* pool, unsigned int idx)
{
    for (int i = 0; i < CACHE_NB; i++)
    {
        if (pool->chunkCache[i].idx == idx) 
        {
            pool->chunkCache[i].isUsed = 0;
            pool->cacheUsed = Pool_reportCacheUsed(pool);
        }
    }
    if (pool->isFile)
        Pool_deallocInFile(pool, idx);
    else
        Pool_deallocInMemory(pool, idx);
}

/**********************************************//**
  @brief Pool_writeCache
  @param[in] none
  @return none none
**************************************************/
PUBLIC void Pool_write(Pool* pool, unsigned int idx, void * ptrContent)
{
    for (int i = 0; i < CACHE_NB; i++)
    {
        // A node with index idx has been cached, use this position
        if ((pool->chunkCache[i].idx == idx) && (pool->chunkCache[i].isUsed == 1))
        {
            // Release the cacke postion
            pool->chunkCache[i].isUsed = 0;
            pool->cacheUsed = Pool_reportCacheUsed(pool);
            break;
        }
    }
    if (pool->isFile)
        Pool_writeInFile(pool, idx, ptrContent);
    else
        Pool_writeInMemory(pool, idx, ptrContent);
    //printf("[Pool Cache] %d\n", pool->cacheUsed);
    //pool->cacheUsed = 0;
}

/**********************************************//**
  @brief Pool_read
  @param[in] none
  @return none
**************************************************/
PUBLIC void* Pool_read(Pool* pool, unsigned int idx /*, void * ptrContent*/)
{
    int foundPos = -1;
    int freePos = -1;
    for (int i = 0; i < CACHE_NB; i++)
    {
        // A node with index idx has been cached, use this position
        if ((pool->chunkCache[i].idx == idx) && (pool->chunkCache[i].isUsed == 1))
        {
            foundPos = i;
            break;
        }
        // Keep track of free cache position
        if (pool->chunkCache[i].isUsed == 0)
        {
            freePos = i;
        }
    }
    if (foundPos < 0) 
        if (freePos < 0)
        {
            // No space left in cache
            printf("Error: No space in cache\n");
            exit(2);
            return NULL;
        }
        else
        {
            foundPos = freePos;
        }
    // Read the pool vavlue in the cache found
    if (pool->isFile)
        Pool_readInFile(pool, idx, pool->chunkCache[foundPos].cache);
    else
        Pool_readInMemory(pool, idx, pool->chunkCache[foundPos].cache);

    pool->chunkCache[foundPos].isUsed = 1;
    pool->chunkCache[foundPos].idx = idx;
    pool->cacheUsed = Pool_reportCacheUsed(pool);
    //printf("[Pool Cache] %d\n", pool->cacheUsed);

    return pool->chunkCache[foundPos].cache;
}

/**********************************************//**
  @brief Pool_report
  @param[in] none
  @return none
**************************************************/
PUBLIC void Pool_report(Pool* pool)
{
    if (pool->isFile)
        Pool_reportInFile(pool);
    else
        Pool_reportInMemory(pool);
}

/**********************************************//**
  @brief Pool_reportSizeInBytes
  input: none
  @return none
**************************************************/
PUBLIC unsigned int Pool_reportSizeInBytes(Pool* pool)
{
    return (pool->nbMemChunks * (sizeof(MemChunk) + pool->memChunkSize));
}

/**********************************************//**
  @brief Pool_reportNbNodes
  @param[in] none
  @return none
**************************************************/
PUBLIC unsigned int Pool_reportNbNodes(Pool* pool)
{
    return pool->nbAllocatedChunks;
}

#if 0
/**********************************************//**
  @brief Pool_addToChunkCache
  @param[in] none
  @return none
**************************************************/
PUBLIC unsigned int Pool_addToChunkCache(Pool* pool, void* p, unsigned int length)
{
    char* dest = (char*)pool->writeChunkCache + pool->cacheUsed;
    if (pool->cacheUsed + length > pool->memChunkSize) length = pool->memChunkSize - pool->cacheUsed;

    Memory_copy(dest, p, length);
    pool->cacheUsed += length;

    return length;
}
#endif

PUBLIC void Pool_discardCache(Pool* pool, unsigned int idx)
{
    for (int i = 0; i < CACHE_NB; i++)
    {
        if (pool->chunkCache[i].idx == idx) 
        {
            pool->chunkCache[i].isUsed = 0;
            pool->cacheUsed = Pool_reportCacheUsed(pool);
        }
    }
}

PUBLIC void Pool_discardAllCache(Pool* pool)
{
    for (int i = 0; i < CACHE_NB; i++)
    {
        pool->chunkCache[i].isUsed = 0;
    }
    pool->cacheUsed = Pool_reportCacheUsed(pool);
}

/**********************************************//**
  @brief Pool_reportInMemory
  @param[in] none
  @return none
**************************************************/
PRIVATE void Pool_reportInMemory(Pool* pool)
{
    printf("Pool Report:\n");
    printf("  maxNbMemChunks: %d\n", pool->maxNbMemChunks);
    printf("  memChunkSize: %d\n", pool->memChunkSize);
    printf("  nbAllocatedChunks: %d\n", pool->nbAllocatedChunks);
    printf("  firstAvailable: %d\n", pool->firstAvailable);
    printf("  lastAllocated: %d\n", pool->lastAllocated);
    //fseek(pool->file, 0, SEEK_SET);
    for (unsigned int i = 0; i < pool->maxNbMemChunks; i++)
    {
        MemChunk* memChunk = (MemChunk*)((char*)pool->pool + i * (sizeof(MemChunk) + pool->memChunkSize));
        printf("MemChunk %d\n", i);
        switch (memChunk->prev)
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
            printf("   prev: %d\n", memChunk->prev);
            break;
        }
        switch (memChunk->next)
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
            printf("   next: %d\n", memChunk->next);
            break;
        }
        printf("   isFree: %d\n", memChunk->isFree);
    }
}

/**********************************************//**
  @brief Pool_reportInFile
  @param[in] none
  @return none
**************************************************/
PRIVATE void Pool_reportInFile(Pool* pool)
{
    unsigned int nbByteRead = 0;

    printf("Pool Report:\n");
    printf("  maxNbMemChunks: %d\n", pool->maxNbMemChunks);
    printf("  memChunkSize: %d\n", pool->memChunkSize);
    printf("  nbAllocatedChunks: %d\n", pool->nbAllocatedChunks);
    printf("  firstAvailable: %d\n", pool->firstAvailable);
    printf("  lastAllocated: %d\n", pool->lastAllocated);
    fseek(pool->file, 0, SEEK_SET);
    for (unsigned int i = 0; i < pool->maxNbMemChunks; i++)
    {
        MemChunk memChunk;
        //printf("Ftell %d\n",ftell(pool->file));
        nbByteRead = fread(&memChunk, sizeof(MemChunk), 1, pool->file);
        if (nbByteRead != 1) printf("Read failure\n");
        //printf("NbBYteRead %d\n", nbByteRead);
        fseek(pool->file, pool->memChunkSize, SEEK_CUR);
        printf("MemChunk %d\n", i);
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

/**********************************************//**
  @brief Pool_allocInMemory
  @param[in] none
  @return none
**************************************************/
PRIVATE AllocStatus Pool_allocInMemory(Pool* pool, unsigned int* ptrIdx)
{
    //unsigned int idx = 0;
    long int lastAllocatedOffset = pool->lastAllocated * (sizeof(MemChunk) + pool->memChunkSize);
    long int firstAvailableOffset = pool->firstAvailable * (sizeof(MemChunk) + pool->memChunkSize);

    if (pool->nbAllocatedChunks == 0)
    {
        *ptrIdx = pool->firstAvailable;
        MemChunk* memChunk = (MemChunk*)((char*)pool->pool + firstAvailableOffset);
        pool->lastAllocated = 0;
        pool->firstAvailable = memChunk->next;
        memChunk->prev = END_OF_QUEUE;
        memChunk->next = END_OF_ALLOC;
        memChunk->isFree = 0;

        memChunk = (MemChunk*)((char*)pool->pool + pool->firstAvailable * (sizeof(MemChunk) + pool->memChunkSize));
        memChunk->prev = START_OF_AVAIL;
        pool->nbAllocatedChunks = 1;

        return ALLOC_OK;
    }
    // Check if free slots left
    if (pool->nbAllocatedChunks <= pool->maxNbMemChunks)
    {
        *ptrIdx = pool->firstAvailable;
        MemChunk* memChunk = (MemChunk*)((char*)pool->pool + lastAllocatedOffset);
        memChunk->next = pool->firstAvailable;

        // Update old first available
        memChunk = (MemChunk*)((char*)pool->pool + firstAvailableOffset);
        memChunk->prev = pool->lastAllocated;
        pool->lastAllocated = pool->firstAvailable;
        pool->firstAvailable = memChunk->next;
        memChunk->next = END_OF_ALLOC;
        memChunk->isFree = 0;
        
        // Update new first available
        if (pool->firstAvailable != END_OF_QUEUE)
        {
            firstAvailableOffset = pool->firstAvailable * (sizeof(MemChunk) + pool->memChunkSize);
            memChunk = (MemChunk*)((char*)pool->pool + firstAvailableOffset);
            memChunk->prev = START_OF_AVAIL;
        }
        pool->nbAllocatedChunks++;
        return ALLOC_OK;
    }
    return ALLOC_FAIL;
}

/**********************************************//**
  @brief Pool_allocInFile
  @param[in] none
  @return none
**************************************************/
PRIVATE AllocStatus Pool_allocInFile(Pool* pool, unsigned int* ptrIdx)
{
    //unsigned int idx = 0;
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
        fseek(pool->file, -((long int)sizeof(MemChunk)), SEEK_CUR);
        fwrite(&memChunk, sizeof(MemChunk), 1, pool->file);

        // Update new first available
        if (pool->firstAvailable != END_OF_QUEUE)
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

/**********************************************//**
  @brief Pool_deallocInMemory
  @param[in] none
  @return none
**************************************************/
PRIVATE void Pool_deallocInMemory(Pool* pool, unsigned int idx)
{
    long int allocatedOffset = idx * (sizeof(MemChunk) + pool->memChunkSize);
    long int firstAvailableOffset = pool->firstAvailable * (sizeof(MemChunk) + pool->memChunkSize);

    if (pool->nbAllocatedChunks > 0)
    {
        MemChunk* memChunk = (MemChunk*)((char*)pool->pool + firstAvailableOffset);
        if (pool->firstAvailable != END_OF_QUEUE)
        {
            memChunk->prev = idx;
        }
        memChunk = (MemChunk*)((char*)pool->pool + allocatedOffset);
        unsigned int prevIdx = memChunk->prev;
        unsigned int nextIdx = memChunk->next;

        memChunk->prev = START_OF_AVAIL;
        memChunk->next = pool->firstAvailable;
        memChunk->isFree = 1;
        pool->firstAvailable = idx;

        if (prevIdx != END_OF_QUEUE)
        {
            long int prevOffset = prevIdx * (sizeof(MemChunk) + pool->memChunkSize);
            memChunk = (MemChunk*)((char*)pool->pool + prevOffset);
            memChunk->next = nextIdx;
        }

        if (nextIdx != END_OF_ALLOC)
        {
            long int nextOffset = nextIdx * (sizeof(MemChunk) + pool->memChunkSize);
            memChunk = (MemChunk*)((char*)pool->pool + nextOffset);
            memChunk->prev = prevIdx;
        }
        pool->nbAllocatedChunks--;
    }
}

/**********************************************//**
  @brief Pool_deallocInFile
  @param[in] none
  @return none
**************************************************/
PRIVATE void Pool_deallocInFile(Pool* pool, unsigned int idx)
{
    long int allocatedOffset = idx * (sizeof(MemChunk) + pool->memChunkSize);
    long int firstAvailableOffset = pool->firstAvailable * (sizeof(MemChunk) + pool->memChunkSize);

    /*if (pool->nbAllocatedChunks == pool->maxNbMemChunks)
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
       return;
    }*/
    if (pool->nbAllocatedChunks > 0)
    {
        MemChunk memChunk;
        if (pool->firstAvailable != END_OF_QUEUE)
        {
            fseek(pool->file, firstAvailableOffset + sizeof(unsigned int), SEEK_SET);
            memChunk.prev = idx;
            fwrite(&memChunk.prev, sizeof(unsigned int), 1, pool->file);
        }
        fseek(pool->file, allocatedOffset, SEEK_SET);
        fread(&memChunk, sizeof(MemChunk), 1, pool->file);
        unsigned int prevIdx = memChunk.prev;
        unsigned int nextIdx = memChunk.next;

        memChunk.prev = START_OF_AVAIL;
        memChunk.next = pool->firstAvailable;
        memChunk.isFree = 1;
        pool->firstAvailable = idx;
        fseek(pool->file, -((long int)sizeof(MemChunk)), SEEK_CUR);
        fwrite(&memChunk, sizeof(MemChunk), 1, pool->file);

        if (prevIdx != END_OF_QUEUE)
        {
            long int prevOffset = prevIdx * (sizeof(MemChunk) + pool->memChunkSize);
            fseek(pool->file, prevOffset, SEEK_SET);
            fwrite(&nextIdx, sizeof(unsigned int), 1, pool->file);
        }

        if (nextIdx != END_OF_ALLOC)
        {
            long int nextOffset = nextIdx * (sizeof(MemChunk) + pool->memChunkSize);
            fseek(pool->file, nextOffset + sizeof(unsigned int), SEEK_SET);
            fwrite(&prevIdx, sizeof(unsigned int), 1, pool->file);
        }
        pool->nbAllocatedChunks--;
    }

}

/**********************************************//**
  @brief Pool_writeInFile
  @param[in] none
  @return none
**************************************************/
PRIVATE void Pool_writeInFile(Pool* pool, unsigned int idx, void* p)
{
    long int offset = idx * (sizeof(MemChunk) + pool->memChunkSize) + sizeof(MemChunk);

    fseek(pool->file, offset, SEEK_SET);
    fwrite(p, pool->memChunkSize, 1, pool -> file);
}

/**********************************************//**
  @brief Pool_writeInMemory
  @param[in] none
  @return none
**************************************************/
PRIVATE void Pool_writeInMemory(Pool* pool, unsigned int idx, void* p)
{
    long int offset = idx * (sizeof(MemChunk) + pool->memChunkSize) + sizeof(MemChunk);

    Memory_copy((char*)pool->pool + offset, p, pool->memChunkSize);
}

/**********************************************//**
  @brief Pool_readInFile
  @param[in] none
  @return none
**************************************************/
PRIVATE void Pool_readInFile(Pool* pool, unsigned int idx, void* p)
{
    long int offset = idx * (sizeof(MemChunk) + pool->memChunkSize) + sizeof(MemChunk);

    fseek(pool->file, offset, SEEK_SET);
    fread(p, pool->memChunkSize, 1, pool->file);
}

/**********************************************//**
  @brief Pool_readInMemory
  @param[in] none
  @return none
**************************************************/
PRIVATE void Pool_readInMemory(Pool* pool, unsigned int idx, void* p)
{
    long int offset = idx * (sizeof(MemChunk) + pool->memChunkSize) + sizeof(MemChunk);

    Memory_copy(p, (char*)pool->pool + offset, pool->memChunkSize);
}

PRIVATE unsigned int Pool_reportCacheUsed(Pool* pool)
{
    unsigned int count=0;
    for (int i=0; i<CACHE_NB; i++)
       if (pool->chunkCache[i].isUsed) count++;
    return count;
}
