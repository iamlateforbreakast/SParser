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

/**********************************************//** 
  @brief Allocate a memory area of nbBytes
  @public
  @param[in] nbBytes Number of bytes
  @return pointer to allocated area
  @memberof Memory
**************************************************/
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

/**********************************************//** 
  @brief Free an allocate area of memory
  @public
  @param[in] pointer Pointer to area to free
  @param[in] nbBytes Number of bytes
  @memberof Memory
**************************************************/
PUBLIC void Memory_free(void* pointer, unsigned int nbBytes)
{
  if (pointer!=0)
  {
    Memory_freeRequestId++;
    TRACE(("Memory: Free %d %d %p\n", Memory_freeRequestId, nbBytes,pointer));
    free(pointer);

    Memory_nbBytesAllocated = Memory_nbBytesAllocated - nbBytes;
    /*    if (p<Memory_minAddress) 
        {
          Memory_minAddress = p;
        }*/
    }
}

/**********************************************//** 
  @brief Re-allocate an area of memory
  @public
  @param[in] pointer Pointer to area
  @param[in] prevSizeBytes Previous size of area in bytes
  @param[in] newSizeBytes New size of area in bytes
  @memberof Memory
**************************************************/
PUBLIC void * Memory_realloc(void * pointer, unsigned int prevSizeBytes, unsigned int newSizeBytes)
{
  void *p = realloc(pointer, newSizeBytes);

  Memory_nbBytesAllocated = Memory_nbBytesAllocated - prevSizeBytes + newSizeBytes;

#if 0
  if (pointer!=0)
  {
    Memory_free(pointer, prevSizeBytes);
  }
  pointer = Memory_alloc(newSizeBytes);
#endif

  return p;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Memory
**************************************************/
PUBLIC void Memory_set(void * pointer, unsigned char val, unsigned int nbBytes)
{
  if (pointer!=0)
  {
    memset(pointer, val, nbBytes);
  }
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Memory
**************************************************/
PUBLIC void Memory_copy(void * pointer, void * src, unsigned int nbBytes)
{
  if ((pointer!=0) && (src!=0))
  {
    memcpy(pointer, src, nbBytes);
  }
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Memory
**************************************************/
PUBLIC int Memory_ncmp(void * pointer, void * compared, unsigned int nbBytes)
{
  unsigned int result = 0;
  
  if ((pointer!=0) && (compared!=0) && (memcmp(pointer, compared, nbBytes)==0))
    result = 1;
  
  return result;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Memory
**************************************************/
PUBLIC int Memory_cmp(void * pointer, void * compared)
{
  
  return strcmp(pointer, compared);
  
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Memory
**************************************************/
PUBLIC unsigned int Memory_len(const void * pointer)
{
  return (unsigned int)strlen(pointer);
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Memory
**************************************************/
PUBLIC void Memory_report()
{
  PRINT(("Memory Usage Report:\n"
         "Number of memory allocation Requests: %d\n"
         "Number of memory de-allocation Requests: %d\n", Memory_allocRequestId, Memory_freeRequestId));
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Memory
**************************************************/
PUBLIC int Memory_getAllocRequestNb()
{
  return Memory_allocRequestId;
}

/**********************************************//** 
  @brief TBD
  @public
  @memberof Memory
**************************************************/
PUBLIC int Memory_getFreeRequestNb()
{
  return Memory_freeRequestId;
}
