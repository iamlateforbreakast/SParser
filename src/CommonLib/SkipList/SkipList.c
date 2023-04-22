/**********************************************//**
  @file SkipList.c
  @brief This file contains the implementation of the class SkipList.
  The class List implement the SkipList operations
  - Add
  - Remove
  - Get
**************************************************/

#include "SkipList.h"
#include "Pool.h"
#include <limits.h>
#include <stdlib.h>


#define SKIPLIST_MAX_LEVEL (6)

typedef struct SkipNode SkipNode;

typedef struct SkipNode
{
    unsigned int key;
    void* object;
    unsigned int level;
    unsigned int forward[SKIPLIST_MAX_LEVEL];
} SkipNode;

typedef struct SkipList
{
    unsigned int (*isEqual)(unsigned int, unsigned int);
    unsigned int (*isGreaterOrEqual)(unsigned int, unsigned int);
    unsigned int (*isGreater)(unsigned int, unsigned int);
    unsigned int level;
    unsigned int nbObjects;
    unsigned int maxObjectNb;
    Pool* pool;
    unsigned int headerIdx;
} SkipList;

PRIVATE unsigned int SkipList_randLevel(SkipList* this);
PRIVATE unsigned int SkipList_reportCache(SkipList* this);
PRIVATE unsigned int myGreateOrEqual(unsigned int k1, unsigned int k2);
PRIVATE unsigned int myGreater(unsigned int k1, unsigned int k2);
PRIVATE unsigned int myEqual(unsigned int k1, unsigned int k2);

/**********************************************//**
  @brief SkipList_new
  @param[in] none
  @return New instance of class SkipList.
**************************************************/
PUBLIC SkipList* SkipList_new(unsigned int maxObjectNb)
// PUBLIC SkipList * SkipList_new(Storage * storage)
{
    SkipList* newSkipList = 0;

    newSkipList = (SkipList*)malloc(sizeof(SkipList));
    newSkipList->maxObjectNb = maxObjectNb;
    newSkipList->pool = Pool_new(newSkipList->maxObjectNb, sizeof(SkipNode));
    newSkipList->level = 1;
    newSkipList->nbObjects = 0;
    newSkipList->isGreaterOrEqual = &myGreateOrEqual;
    newSkipList->isGreater = &myGreater;
    newSkipList->isEqual = &myEqual;

    SkipNode* skipNode = Pool_alloc(newSkipList->pool, &newSkipList->headerIdx);
    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++)
    {
        skipNode->forward[i] = newSkipList->headerIdx;
    }
    skipNode->key = INT_MAX;
    skipNode->object = 0;
    skipNode->level = 1;
    Pool_write(newSkipList->pool, newSkipList->headerIdx, skipNode);

    return newSkipList;
}

/**********************************************//**
  @brief SkipList_free
  @param[in] Instance to destroy
  @return None
**************************************************/
PUBLIC void SkipList_free(SkipList* this)
{
    Pool_free(this->pool);
    free(this);
}

/**********************************************//**
  @brief SkipList_add
  @param[in] Key to index object
  @param[in] Object to add to SkipList object.
  @return None
**************************************************/
PUBLIC void SkipList_add(SkipList* this, unsigned int key, void* object)
 {
    unsigned int update[SKIPLIST_MAX_LEVEL];
    unsigned int currentNodeIdx = this->headerIdx;
    
    //printf("[Cache usage] Add start %d\n", SkipList_reportCache(this));
    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) update[i] = 0;

    unsigned int nextNodeIdx = this->headerIdx;
    SkipNode* nextNode = Pool_read(this->pool, this->headerIdx);

    for (int i = this->level - 1; i >= 0; i--)
    {
        while ((*this->isGreater)(nextNode->key,key))
        {
            update[i] = nextNodeIdx;
            nextNodeIdx = nextNode->forward[i];
            Pool_discardCache(this->pool, update[i]);
            nextNode = Pool_read(this->pool, nextNode->forward[i]);
        }
        Pool_discardCache(this->pool, nextNode->forward[i]);
        Pool_discardCache(this->pool, nextNodeIdx);
        nextNode = Pool_read(this->pool, update[i]);
        nextNodeIdx = update[i];
    }

    SkipNode * currentNode = Pool_read(this->pool, update[0]);
    unsigned int currentKey = currentNode->key;
    Pool_discardCache(this->pool, update[0]);

    //printf("[Pool Usage]: %d\n", Pool_reportCacheUsed(this->pool));
    if (key == currentKey)
    {
        currentNode->object = object;
        return;
    }
    else
    {
        unsigned int level = this->level;
        if (update[0] != 0) level = SkipList_randLevel(this);
        if (level > this->level)
        {
            this->level = level;
        }
        unsigned int insertIdx;
        SkipNode* insert = Pool_alloc(this->pool, &insertIdx);
        insert->key = key;
        insert->object = object;
        insert->level = level;
        SkipNode* header = Pool_read(this->pool, this->headerIdx);
        for (int i = 0; i < level; i++)
        {
            if (update[0] != 0)
            {
                if (update[i] != 0)
                {
                    SkipNode* nodeBeforeInsert = Pool_read(this->pool, update[i]);
                    insert->forward[i] = nodeBeforeInsert->forward[i];
                    nodeBeforeInsert->forward[i] = insertIdx;
                    Pool_write(this->pool, update[i], nodeBeforeInsert);
                    if (header->forward[i] == 0) header->forward[i] = insertIdx;
                }
                else
                {
                    header->forward[i] = insertIdx;
                    header->level = level;
                }
            }
            else
            {
                //header->forward[i] = insertIdx;
                insert->forward[i] = this->headerIdx;
            }
        }
        Pool_write(this->pool, this->headerIdx, header);
        if (update[0] == 0) this->headerIdx = insertIdx;
        Pool_write(this->pool, insertIdx, insert);
        this->nbObjects++;
        //printf("[Cache usage] Add end %d\n", SkipList_reportCache(this));

        return;
    }
}

//int skiplist_insert(skiplist* list, int key, int value) {
//    snode* update[SKIPLIST_MAX_LEVEL + 1];
//    snode* x = list->header;
//    int i, level;
//    for (i = list->level; i >= 1; i--) {
//        while (x->forward[i]->key < key)
//            x = x->forward[i];
//        update[i] = x;
//    }
//    x = x->forward[1];
//
//    if (key == x->key) {
//        x->value = value;
//        return 0;
//    }
//    else {
//        level = rand_level();
//        if (level > list->level) {
//            for (i = list->level + 1; i <= level; i++) {
//                update[i] = list->header;
//            }
//            list->level = level;
//        }
//
//        x = (snode*)malloc(sizeof(snode));
//        x->key = key;
//        x->value = value;
//        x->forward = (snode**)malloc(sizeof(snode*) * (level + 1));
//        for (i = 1; i <= level; i++) {
//            x->forward[i] = update[i]->forward[i];
//            update[i]->forward[i] = x;
//        }
//    }
//    return 0;
//}
/**********************************************//**
  @brief SkipList_delete
  @param[in] Key of object to remove
  @return Object removed from SkipList object.
**************************************************/
PUBLIC void* SkipList_delete(SkipList* this, unsigned int key)
{
    void* removedObject = 0;
    unsigned int update[SKIPLIST_MAX_LEVEL];
    unsigned int currentNodeIdx = this->headerIdx;

    //printf("[Cache usage] delete start %d\n", SkipList_reportCache(this));

    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) update[i] = 0;

    unsigned int nextNodeIdx = this->headerIdx;
    SkipNode* nextNode = Pool_read(this->pool, this->headerIdx);

    for (int i = this->level - 1; i >= 0; i--)
    {
        
        while (nextNode->key < key)
        {
            update[i] = nextNodeIdx;
            nextNodeIdx = nextNode->forward[i];
            Pool_discardCache(this->pool, update[i]);
            nextNode = Pool_read(this->pool, nextNode->forward[i]);
        }
        Pool_discardCache(this->pool, nextNode->forward[i]);
        Pool_discardCache(this->pool, nextNodeIdx);
        nextNode = Pool_read(this->pool, update[i]);
        nextNodeIdx = update[i];
    }

    SkipNode* currentNode = Pool_read(this->pool, update[0]);
    //unsigned int currentKey = currentNode->key;
    
    SkipNode* header = Pool_read(this->pool, this->headerIdx);
    
    if (header->key == key)
    {
        unsigned int nodeToRemoveIdx = this->headerIdx;
        SkipNode* nodeToRemove = Pool_read(this->pool, nodeToRemoveIdx);
        this->headerIdx = nodeToRemove->forward[0];
        SkipNode* newHeader = Pool_read(this->pool, this->headerIdx);
        for (int i= newHeader->level; i<nodeToRemove->level; i++) newHeader->forward[i] = nodeToRemove->forward[i];
        newHeader->level = this->level;
        Pool_write(this->pool, this->headerIdx, newHeader);
        removedObject = nodeToRemove->object;
        Pool_dealloc(this->pool, nodeToRemoveIdx);
        this->nbObjects--;
        return removedObject;
    }
    SkipNode* nodeToRemove = Pool_read(this->pool, currentNode->forward[0]);
    unsigned int nodeToRemoveIdx = currentNode->forward[0];
    Pool_discardCache(this->pool, update[0]);
    if (key == nodeToRemove->key)
    {   
        SkipNode* nodeBeforeRemoval;
        for (int i = 0; i < nodeToRemove->level; i++)
        {
            nodeBeforeRemoval = Pool_read(this->pool, update[i]);
            nodeBeforeRemoval->forward[i] = nodeToRemove->forward[i];
            Pool_write(this->pool, update[i], nodeBeforeRemoval);
        }
        removedObject = nodeToRemove->object;
        Pool_dealloc(this->pool, nodeToRemoveIdx);
        this->nbObjects--;
        return removedObject;
    }
    return 0;
    //printf("[Cache usage] delete end %d\n", SkipList_reportCache(this));
}

//int skiplist_delete(skiplist* list, int key) {
//    int i;
//    snode* update[SKIPLIST_MAX_LEVEL + 1];
//    snode* x = list->header;
//    for (i = list->level; i >= 1; i--) {
//        while (x->forward[i]->key < key)
//            x = x->forward[i];
//        update[i] = x;
//    }
//
//    x = x->forward[1];
//    if (x->key == key) {
//        for (i = 1; i <= list->level; i++) {
//            if (update[i]->forward[i] != x)
//                break;
//            update[i]->forward[1] = x->forward[i];
//        }
//        skiplist_node_free(x);
//
//        while (list->level > 1 && list->header->forward[list->level]
//            == list->header)
//            list->level--;
//        return 0;
//    }
//    return 1;
//}

PUBLIC void* SkipList_get(SkipList* this, unsigned int key)
{
    void* foundObject = 0;
    unsigned int update[SKIPLIST_MAX_LEVEL];
    unsigned int currentNodeIdx = this->headerIdx;

    //printf("[Cache usage] delete start %d\n", SkipList_reportCache(this));

    for (int i = 0; i < SKIPLIST_MAX_LEVEL; i++) update[i] = 0;

    unsigned int nextNodeIdx = this->headerIdx;
    SkipNode* nextNode = Pool_read(this->pool, this->headerIdx);

    for (int i = this->level - 1; i >= 0; i--)
    {

        while (nextNode->key <= key)
        {
            update[i] = nextNodeIdx;
            nextNodeIdx = nextNode->forward[i];
            Pool_discardCache(this->pool, update[i]);
            nextNode = Pool_read(this->pool, nextNode->forward[i]);
        }
        Pool_discardCache(this->pool, nextNode->forward[i]);
        Pool_discardCache(this->pool, nextNodeIdx);
        nextNode = Pool_read(this->pool, update[i]);
        nextNodeIdx = update[i];
    }

    SkipNode* currentNode = Pool_read(this->pool, update[0]);
    unsigned int currentKey = currentNode->key;
    Pool_discardCache(this->pool, update[0]);
    if (currentKey == key)
    {
        foundObject = currentNode->object;
    }
    
    return foundObject;
}

/**********************************************//**
  @brief SkipList_print
  @param[in] None
  @return None
**************************************************/
PUBLIC void SkipList_print(SkipList* this)
{
    printf("NbObject: %d\n", this->nbObjects);
    printf("Level: %d\n", this->level);
    printf("Header node: %d\n", this->headerIdx);

    unsigned int currentNodeIdx = this->headerIdx;
    SkipNode* skipNode = Pool_read(this->pool, currentNodeIdx);
    for (int i = 0; i < this->nbObjects; i++)
    {
        printf("SkipNode: %d ", currentNodeIdx);
        printf(" Level: %d ", skipNode->level);
        printf(" Key: %d ", skipNode->key);
        for (int j = 0; j < skipNode->level; j++)
        {
            printf(" Forward[%d]: %d", j, skipNode->forward[j]);
        }
        printf("\n");
        Pool_discardCache(this->pool, currentNodeIdx);
        currentNodeIdx = skipNode->forward[0];
        skipNode = Pool_read(this->pool, skipNode->forward[0]);
    }
    Pool_discardCache(this->pool, this->headerIdx);
    Pool_discardCache(this->pool, currentNodeIdx);
}
 
PRIVATE unsigned int SkipList_randLevel(SkipList* this)
{
    int level = 1;
    while (rand() < RAND_MAX / 2 && level < SKIPLIST_MAX_LEVEL)
        level++;
    return level;
}

PRIVATE unsigned int SkipList_reportCache(SkipList* this)
{
    return Pool_reportCacheUsed(this->pool);
}

PRIVATE unsigned int myGreateOrEqual(unsigned int k1, unsigned int k2)
{
    return (k1 <= k2);
}

PRIVATE unsigned int myGreater(unsigned int k1, unsigned int k2)
{
    return (k1 < k2);
}

PRIVATE unsigned int myEqual(unsigned int k1, unsigned int k2)
{
    return (k1 == k2);
}
