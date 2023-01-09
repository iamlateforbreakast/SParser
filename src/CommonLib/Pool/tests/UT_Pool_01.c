#include "Types.h"
#include "Pool.h"

#define NB_MEM_CHUNKS (4)
#define SIZE_MEM_CHUNK (4)

Pool * testPool = 0;

int step1()
{
   testPool = Pool_newFromFile("test.pool", NB_MEM_CHUNKS, SIZE_MEM_CHUNK);
   Pool_report(testPool);
}

int step2()
{
   unsigned int idx = 0;
   AllocStatus allocStatus = ALLOC_OK;

   allocStatus = Pool_alloc(testPool, &idx);
   Pool_report(testPool);

   Pool_dealloc(testPool, idx);
   Pool_report(testPool);
}

int step3()
{
   unsigned int idx[NB_MEM_CHUNKS];
   AllocStatus allocStatus = ALLOC_OK;

   for (int i=0; i<NB_MEM_CHUNKS; i++)
   {
      allocStatus = Pool_alloc(testPool, &idx[i]);
   }
   Pool_report(testPool);
}

int step4()
{
   Pool_free(testPool);
}

int main()
{
   remove("test.pool");
  
   step1();
   step2(); 
   step3(); 
   step4(); 
   //idx = Pool_alloc(testPool);
   //Pool_report(testPool);
   //idx = Pool_alloc(testPool);
   //Pool_report(testPool);
   Pool_free(testPool);
}