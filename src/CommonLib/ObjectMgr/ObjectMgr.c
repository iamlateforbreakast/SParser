/**********************************************//** 
  @file ObjectMgr.c
  
  @brief TBD
  @details TBD
**************************************************/

#include "ObjectMgr.h"
#include "Object.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NB_OBJECTS (500)

typedef struct ObjectInfo
{
  Object * ptr;
  unsigned int prevId;
  unsigned int nextId; 
} ObjectInfo;

struct ObjectMgr
{
  Object object;
  unsigned int nbBytesAllocated;
  unsigned int maxNbBytesAllocated;
  unsigned int allocRequestId;
  unsigned int freeRequestId;
  unsigned int nbAllocatedObjects;
  void* maxAddress;
  void* minAddress;
  ObjectInfo allocatedObjects[MAX_NB_OBJECTS];
  unsigned int freeSpace;
  unsigned int usedSpace;
};

PRIVATE ObjectMgr * objectMgr = 0;

PRIVATE ObjectMgr * ObjectMgr_new()
{
  ObjectMgr * this = 0;
  
  this = (ObjectMgr*)malloc(sizeof(ObjectMgr));
    
  this->object.delete = &ObjectMgr_delete;
  this->object.copy = &ObjectMgr_copy;
  this->object.size = sizeof(ObjectMgr);
  this->object.refCount = 0;
  this->object.id =0;
  
  this->nbBytesAllocated = 0;
  this->maxNbBytesAllocated = 0;
  this->allocRequestId = 0;
  this->allocRequestId = 0;
  this->freeRequestId = 0;
  this->maxAddress = (void*)0x0;
  this->minAddress = (void*)0xFFFFFFFF;
  this->nbAllocatedObjects = 0;
  memset(this->allocatedObjects, 0, MAX_NB_OBJECTS * sizeof(Object*));
  
  return this;
}

PUBLIC void ObjectMgr_delete(ObjectMgr * this)
{
  this->object.refCount = this->object.refCount - 1;
  
  if (this->object.refCount == 0)
  {
    free(this);
    this = 0;
  } 
}

PUBLIC ObjectMgr * ObjectMgr_copy(ObjectMgr * this)
{
  ObjectMgr * copy = 0;
  
  return copy;
}

PUBLIC ObjectMgr * ObjectMgr_getRef()
{
  if (objectMgr==0)
  {
    objectMgr = ObjectMgr_new();
  }
  else
  {
    objectMgr->object.refCount++;
  }
  
  return objectMgr;
}

PUBLIC void ObjectMgr_report(ObjectMgr * this)
{
	printf("Nb bytes not freed: %d\n", this->nbBytesAllocated);
  printf("Max nb bytes used: %d\n", this->maxNbBytesAllocated);
  printf("Nb alloc request %d\n", this->allocRequestId);
  printf("Nb free requests %d\n", this->freeRequestId);
  printf("Lower Address %p\n", this->minAddress);
  printf("Upper Address %p\n", this->maxAddress);
}

PUBLIC Object * ObjectMgr_allocate(ObjectMgr * this, unsigned int size)
{
  Object * result = 0;
  
  /* Ensure size requested is >0 */
  if (size>0)
  {
    if (this->nbAllocatedObjects<MAX_NB_OBJECTS)
    {
      result = (Object*)malloc(size);
      
      if (result!=0)
      {
        this->allocatedObjects[this->usedSpace].nextId = this->freeSpace;
        this->allocatedObjects[this->freeSpace].prevId = this->usedSpace;
        this->usedSpace = this->freeSpace;
        this->freeSpace = this->allocatedObjects[this->freeSpace].nextId;
        this->allocatedObjects[this->freeSpace].prevId = 0xFFFFFFFF;
        this->nbAllocatedObjects++;
      }
      else
      {
        /* Error case: Failure to allocate object */
        exit(1);
      }
    }
    else
    {
      /* Error case: too many object already created */
      exit(1); 
    }
  }
  else
  {
    /* Error case: request to create a 0 bytes object */
    exit(1);
  }
  
  return result;
}

PUBLIC void ObjectMgr_deallocate(ObjectMgr * this, Object * object)
{
  unsigned int idx =  object->id;
  
  if (object!=0)
  {
    this->allocatedObjects[this->allocatedObjects[idx].nextId].prevId = this->allocatedObjects[idx].prevId;
    this->allocatedObjects[this->allocatedObjects[idx].prevId].nextId = this->allocatedObjects[idx].nextId;
    this->allocatedObjects[idx].nextId = this->freeSpace;
    this->allocatedObjects[idx].prevId = 0xFFFFFFFF;
  }
  else
  {
    /* Error case: request to free a NULL pointer */
    exit(1);
  }
}
