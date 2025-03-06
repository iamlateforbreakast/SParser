/* DataAccess.h */
#ifndef _DATAACCESS_H_
#define _DATAACCESS_H_
#include "Object.h"
#include "SdbMgr.h"

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

#if 0
DataAccess* DataAccess_newSQLite()
{
  DataAccess* data;
  struct SQLite_info sqlite_info;

  data->init = (Initialise)SQLite_init;
  data->final = (Finalise)SQLite_final;
  data->alloc = (Allocate)SQLite_alloc;
  data->dealloc = (Deallocate)SQLite_dealloc;
  data->set = (Setter)SQLite_setProperty;
  data->get = (Getter)SQLite_getProperty;
  data->info = &sqlite_info;
}
#endif

#endif