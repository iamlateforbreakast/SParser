/* Allocator.h */
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

typedef struct Allocator Allocator;

typedef void * (*NewFunction)();
typedef void (*DeleteFunction)(Allocator * allocator);
typedef char* (*AllocateFunction)(Allocator * allocator, unsigned int size);
typedef void (*DeAllocateFunction)(Allocator * allocator, char * ptr);
typedef unsigned int (*ReportFunction)(Allocator * allocator);

struct Allocator
{
  NewFunction new;
  DeleteFunction delete;
  AllocateFunction allocate;
  DeAllocateFunction deallocate;
  ReportFunction report;
  unsigned int nbAllocatedObjects;
};

Allocator * Allocator_new();
char * Allocator_allocate(Allocator * this, unsigned int size);
//void * Allocator_allocFromClass(Allocator * this, /* Class class*/);
void Allocator_deallocate(Allocator * this, char * ptr);
void Allocator_delete(Allocator * this);
#endif /* _ALLOCATOR_H_ */
