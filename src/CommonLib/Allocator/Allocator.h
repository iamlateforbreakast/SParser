/* Allocator.h */

typedef struct Allocator Allocator;

struct Allocator
{
};

Allocator * Allocator_new();
void Allocator_delete(Allocator * this);
