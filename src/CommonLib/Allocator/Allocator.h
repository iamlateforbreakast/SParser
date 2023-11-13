/* Allocator.h */
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

enum AllocatorType
{
  MALLOC,
  POOLED
};

typedef struct AllocatorParam
{
  int type;
  char * fileName;
} AllocParam;

typedef struct Allocator
{
  AllocParam * param;
} Allocator;

Allocator * Allocator_new(AllocParam * allocParam);
//void * Allocator_alloc(Allocator * this, unsigned int size);
//void * Allocator_allocFromClass(Allocator * this, /* Class class*/);
//void Allocator_dealloc(Allocator * this);
void Allocator_delete(Allocator * this);
#endif /* _ALLOCATOR_H_ */
