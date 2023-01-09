#include "Types.h"
#include "Pool.h"

#define NB_MEM_CHUNKS (4)
#define SIZE_MEM_CHUNK (4)

int main()
{
   Pool * testPool = 0;
   unsigned int idx = 0;
   AllocStatus allocStatus = ALLOC_OK;
   
   remove("test.pool");
   testPool = Pool_newFromFile("test.pool", NB_MEM_CHUNKS, SIZE_MEM_CHUNK);
   Pool_report(testPool);
   allocStatus = Pool_alloc(testPool, &idx);
   Pool_report(testPool);
   
   Pool_dealloc(testPool, idx);
   Pool_report(testPool);
   //idx = Pool_alloc(testPool);
   //Pool_report(testPool);
   //idx = Pool_alloc(testPool);
   //Pool_report(testPool);
   Pool_free(testPool);
}
