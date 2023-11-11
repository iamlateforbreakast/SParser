/* Allocator.h */
#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_
typedef struct Allocator Allocator;

enum AllocatorType
{
  MALLOC,
  POOLED
};

struct AllocatorParam
{
  int type;
  char * fileName;
};

struct Allocator
{
};

Allocator * Allocator_new();
void * Allocator_alloc(Allocator * this, unsigned int size);
void * Allocator_allocFromClass(Allocator * this, /* Class class*/);
void Allocator_dealloc(Allocator * this);
void Allocator_delete(Allocator * this);
#endif /* _ALLOCATOR_H_ */
