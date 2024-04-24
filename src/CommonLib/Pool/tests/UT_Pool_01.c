#include "Types.h"
#include "Pool.h"
#include "Debug.h"

#define DEBUG (0)
#ifdef _WIN32
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}
#else
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("\x1b[32mPassed\x1b[0m\n");} \
                          else { printf("\x1b[31mFailed\x1b[0m\n"); return 0;}
#endif
                          
#define NB_MEM_CHUNKS (4)
#define SIZE_MEM_CHUNK (16)

Pool* testPool = 0;

int step1()
{
    testPool = Pool_newFromFile("test.pool", NB_MEM_CHUNKS, SIZE_MEM_CHUNK);
    Pool_report(testPool);
    return 0;
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
    return 0;
}

int step3()
{
    unsigned int idx[NB_MEM_CHUNKS];
    void * ptrBuffer;

    for (int i = 0; i < NB_MEM_CHUNKS; i++)
    {
        ptrBuffer = Pool_alloc(testPool, &idx[i]);
        Pool_discardCache(testPool, idx[i]);
    }
    Pool_report(testPool);
    return 0;
}

int step4()
{
    unsigned int idx[] = { 2, 0, 3, 1 };

    for (int i = 0; i < NB_MEM_CHUNKS; i++)
    {
        Pool_dealloc(testPool, idx[i]);
    }
    Pool_report(testPool);
    return 0;
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
    return 0;
}

int step6()
{
    char testData[] = { 'A','B','C','D','E','F','G','H' };

    //char * readData = Pool_getCache1(testPool);

    //for (int i=0;i<8;i++)
    //{
    //    Pool_addToChunkCache(testPool, &testData[0] + i, 1);
    //}
    Pool_write(testPool, 0, testData);
    char* readData = Pool_read(testPool, 0);
    readData[sizeof(testData)] = 0;
    PRINT(("data read back %s\n", &readData[0]));
    return 0;
}

int step7()
{
    Pool_delete(testPool);
    return 0;
}

int step11()
{
    testPool = Pool_new(NB_MEM_CHUNKS, SIZE_MEM_CHUNK);
    return 0;
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
    return 0;
}

int step16()
{
    Pool_delete(testPool);
    return 0;
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
