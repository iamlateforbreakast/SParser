/*********************************************************************************
* Pool.c
*
*********************************************************************************/
#include "Pool.h"

PUBLIC Pool * Pool_new(unsigned int nbMemChunks, unsigned int memChunkSize)
{
}

PUBLIC Pool * Pool_newFromFile(Pool * pool, char * fileName)
{
   // If file exists
   // else
}

PUBLIC void Pool_free(Pool * pool)
{
   // If is file
   // Close file
   // Else free mem
}

PUBLIC void * Pool_alloc(Pool * pool, unsigned int size)
{
   // Check if free slots left
   // return nextFree
   // NextFree = nextFree next
}

PUBLIC void Pool_dealloc(Pool * pool, void * p)
{
}
