#include "Types.h"
#include "Pool.h"

#define NB_MEM_CHUNKS (4)
#define SIZE_MEM_CHUNK (16)

Pool* testPool = 0;

int step1()
{
    testPool = Pool_newFromFile("test.pool", NB_MEM_CHUNKS, SIZE_MEM_CHUNK);
    Pool_report(testPool);
}

int step2()
{
    unsigned int idx = 0;
    void * ptrBuffer;
    AllocStatus allocStatus = ALLOC_OK;

    ptrBuffer = Pool_alloc(testPool, &idx);
    Pool_report(testPool);

    Pool_dealloc(testPool, idx);
    Pool_report(testPool);
}

int step3()
{
    unsigned int idx[NB_MEM_CHUNKS];
    void * ptrBuffer;

    for (int i = 0; i < NB_MEM_CHUNKS; i++)
    {
        ptrBuffer = Pool_alloc(testPool, &idx[i]);
    }
    Pool_report(testPool);
}

int step4()
{
    unsigned int idx[] = { 2, 0, 3, 1 };

    for (int i = 0; i < NB_MEM_CHUNKS; i++)
    {
        Pool_dealloc(testPool, idx[i]);
    }
    Pool_report(testPool);
}

int step5()
{
    unsigned int idx[NB_MEM_CHUNKS];
    void * ptrBuffer;

    for (int i = 0; i < 1; i++)
    {
        ptrBuffer = Pool_alloc(testPool, &idx[i]);
    }
    Pool_report(testPool);
}

int step6()
{
    char testData[] = { 'A','B','C','D','E','F','G','H' };

    char * readData;

    for (int i=0;i<8;i++)
    {
        Pool_addToChunkCache(testPool, &testData[0] + i, 1);
    }
    Pool_writeCache(testPool, 0);
    readData = Pool_read(testPool, 0);
    readData[sizeof(testData)] = 0;
    printf("data read back %s\n", &readData[0]);
}

int step7()
{
    Pool_free(testPool);
}

int step11()
{
    testPool = Pool_new(NB_MEM_CHUNKS, SIZE_MEM_CHUNK);
}

int step12()
{
    unsigned int idx = 0;
    void * ptrBuffer;

    ptrBuffer = Pool_alloc(testPool, &idx);
    ptrBuffer = Pool_alloc(testPool, &idx);
    Pool_report(testPool);

    Pool_dealloc(testPool, idx);
    Pool_report(testPool);
}

int step16()
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
    step5();
    step6();
    step7();
    /*step11();
    step12();
    step16();*/
}
