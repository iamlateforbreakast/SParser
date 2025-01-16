typedef struct MyObject MyObject;
typedef struct DataAccess DataAccess;
typedef void (*Initialise)(void *);
typedef void (*Finalise)(void *);
typedef void* (*Allocate)(void *);
typedef void (*Deallocate)(Object *);
typedef void (*Setter)(Object *, int p, void *);
typedef void (*Getter)(Object *, int p, void *);

struct DataAccess
{
  void* info;
  Initialise init;
  Finalise final;
  Allocate alloc;
  Deallocate dealloc;
  Setter set;
  Getter get;
};