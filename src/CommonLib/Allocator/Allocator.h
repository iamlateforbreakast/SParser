/* Allocator.h */
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

typedef struct Allocator Allocator;

typedef char* (*AllocateFunction)(Allocator * allocator, unsigned int size);
typedef void (*DeAllocateFunction)(Allocator * allocator, void * ptr);

struct Allocator
{
  AllocateFunction allocate;
  DeAllocateFunction deallocate;
};

Allocator * Allocator_new();
char * Allocator_allocate(Allocator * this, unsigned int size);
//void * Allocator_allocFromClass(Allocator * this, /* Class class*/);
void Allocator_deallocate(Allocator * this, char * ptr);
void Allocator_delete(Allocator * this);
#endif /* _ALLOCATOR_H_ */
