/**********************************************//** 
  @file Memory.c
  
  @brief This file provides the implementation of the memory functions.
  
  TBD
**************************************************/

#include "Memory.h"
#include "Debug.h"
#include "Error.h"

#include <stdlib.h>
#include <string.h>

#define DEBUG (0)

PRIVATE unsigned int Memory_allocRequestId = 0;
PRIVATE unsigned int Memory_freeRequestId = 0;
PRIVATE unsigned int Memory_nbBytesAllocated = 0;

PUBLIC void* Memory_alloc(unsigned int nbBytes)
{    
  void* p = 0;

  p = malloc(nbBytes);
  Memory_nbBytesAllocated += nbBytes;
  //if (Memory_nbBytesAllocated>Memory_maxNbBytesAllocated) Memory_maxNbBytesAllocated = Memory_nbBytesAllocated;
  Memory_allocRequestId++;
    
  TRACE(("Memory: Malloc %d %d %p\n", Memory_allocRequestId, nbBytes,p));

   //if (p+nbBytes> (void*)Memory_maxAddress) Memory_maxAddress = p + nbBytes;
  return p;
}

PUBLIC void Memory_free(void* pointer, unsigned int nbBytes)
{
  if (pointer!=0)
  {
    Memory_freeRequestId++;
    TRACE(("Memory: Free %d|%d %p\n", Memory_freeRequestId, nbBytes,pointer));
    free(pointer);

    Memory_nbBytesAllocated = Memory_nbBytesAllocated - nbBytes;
    /*    if (p<Memory_minAddress) 
        {
          Memory_minAddress = p;
        }*/
    }
}

PUBLIC void * Memory_realloc(void * pointer, unsigned int prevSizeBytes, unsigned int newSizeBytes)
{
  if (pointer!=0)
  {
    Memory_free(pointer, prevSizeBytes);
  }
  pointer = Memory_alloc(newSizeBytes);
  
  return pointer;
}

PUBLIC void Memory_set(void * pointer, unsigned char val, unsigned int nbBytes)
{
  if (pointer!=0)
  {
    memset(pointer, val, nbBytes);
  }
}

PUBLIC void Memory_copy(void * pointer, void * src, unsigned int nbBytes)
{
  if ((pointer!=0) && (src!=0))
  {
    memcpy(pointer, src, nbBytes);
  }
}

PUBLIC unsigned int Memory_ncmp(void * pointer, void * compared, unsigned int nbBytes)
{
  unsigned int result = 0;
  
  if (memcmp(pointer, compared, nbBytes)==0) result = 1;
  
  return result;
}

PUBLIC unsigned int Memory_cmp(void * pointer, void * compared)
{
  unsigned int result = 0;
  
  if (strcmp(pointer, compared)==0) result=1;
  
  return result;
}

PUBLIC unsigned int Memory_len(const void * pointer)
{
  return strlen(pointer);
}

PUBLIC void Memory_report()
{
  Error_new(ERROR_INFO, "Memory Usage Report:\n"
                                        "Number of memory allocation Requests: %d\n"
                                        "Number of memory de-allocation Requests: %d\n", Memory_allocRequestId, Memory_freeRequestId);
}
