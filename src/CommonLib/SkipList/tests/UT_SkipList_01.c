// SkipList.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "SkipList.h"
#include "Debug.h"

#define DEBUG (0)
#define UT_ASSERT(cond) if ((cond)) \
                          { printf("Passed\n");} \
                          else { printf("Failed\n"); return 0;}

#define NB_OBJECTS (10000)

SkipList* testList;

unsigned int keys[] = { 55, 30, 80, 10, 40, 120, 5, 20, 500 };
unsigned int value[] = { 1972, 2005, 1945, 1875, 2001, 1515, 1066, 0, 1789 };

unsigned int randomKeys[NB_OBJECTS];
unsigned int randomValues[NB_OBJECTS];

int step1()
{
    PRINT(("Step 1: Test 1 - Build a SkipList: "));

    testList = SkipList_new(NB_OBJECTS + 1);

    void * checkPtr = testList;

    printf("Object id: %d\n",*(unsigned int*)checkPtr);
    checkPtr+=sizeof(unsigned int) + 3 * sizeof(void*);
    printf("Ref count: %d\n",*(unsigned int*)checkPtr);
    
    UT_ASSERT((1));

    return 1;
}

int step2()
{
    PRINT(("Step 2: Test 1 - Add items: "));

    for (int i = 0; i < sizeof(keys)/sizeof(unsigned int); i++)
    {
        SkipList_add(testList, keys[i], &value[i]);
        SkipList_print(testList);
    }
        

    return 1;
}

int step3()
{
    
    SkipList_print(testList);

    return 1;
}

int step4()
{
    SkipList_delete(testList);
    return 1;
}

int step5()
{
    testList = SkipList_new(NB_OBJECTS + 1);

    for (int i=0; i<NB_OBJECTS; i++)
    {
        randomKeys[i] = (i + 1) * 5;
        randomValues[i] = (i + 1) * 15;
    }

    for (int i=0; i<NB_OBJECTS; i++)
    {
        int j = (rand() % NB_OBJECTS);
        unsigned int swap = randomKeys[i];
        randomKeys[i] = randomKeys[j];
        randomKeys[j] = swap;
        //printf("Swapping %d and %d\n",i,j);
    }

    for (int i=0; i<NB_OBJECTS; i++)
    {
        SkipList_add(testList, randomKeys[i], &randomValues[i]);
        //printf("Adding %d\n", randomKeys[i]);
    }
    SkipList_print(testList);
    SkipList_delete(testList);
    return 1;
}

int main()
{
    step1();
    //step2();
    //step3();
    //step4();
    //step5();
}
