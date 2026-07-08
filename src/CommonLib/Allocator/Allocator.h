/* Allocator.h */
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

typedef struct Allocator Allocator;

typedef void * (*NewFunction)();
typedef void (*DeleteFunction)(Allocator * allocator);
typedef void * (*AllocateFunction)(Allocator * allocator, unsigned int size);
typedef void (*DeAllocateFunction)(Allocator * allocator, void * ptr);
typedef void * (*ReallocateFunction)(Allocator * allocator, void * ptr, unsigned int size);
typedef unsigned int (*ReportFunction)(Allocator * allocator);

struct Allocator
{
  NewFunction new;
  DeleteFunction delete;
  AllocateFunction allocate;
  DeAllocateFunction deallocate;
  ReallocateFunction reallocate;
  ReportFunction report;
  unsigned int nbAllocatedObjects;
};

Allocator * Allocator_new();
void * Allocator_allocate(Allocator * this, unsigned int size);
void Allocator_deallocate(Allocator * this, void * ptr);
void * Allocator_reallocate(Allocator * this, void * ptr, unsigned int size);
void Allocator_delete(Allocator * this);
#endif /* _ALLOCATOR_H_ */
