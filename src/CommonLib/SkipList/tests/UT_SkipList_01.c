// SkipList.c : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include "SkipList.h"

#define NB_OBJECTS (3)

SkipList* testList;

int step1()
{
    testList = SkipList_new();

    return 1;
}

int step2()
{
    unsigned int keys[NB_OBJECTS] = { 55, 30, 80 };
    unsigned int value[NB_OBJECTS] = { 1972, 2005, 1945 };

    for (int i = 0; i < NB_OBJECTS; i++)
        SkipList_add(testList, keys[i], &value);

    return 1;
}

int step3()
{
    
    SkipList_print(testList);

    return 1;
}

int step4()
{
    SkipList_free(testList);
    return 1;
}

int main()
{
    step1();
    step2();
    step3();
    step4();
}
