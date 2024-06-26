/* SQLite_test.c */
#include "sqlite3.h"
#include "Object.h"
#include "Debug.h"

typedef enum
{
  PROPERTY_content = 1,
  PROPERTY_isOwner,
  PROPERTY_count,
  NB_PROPERTIES
} MyObjectProperty;

typedef struct MyObject MyObject;
typedef struct DataAccess DataAccess;
typedef void (*Initialise)(void *);
typedef void (*Finalise)(void *);
typedef void* (*Allocate)(void *);
typedef void (*Deallocate)(Object *);
typedef void (*Setter)(Object *, MyObjectProperty p, void *);
typedef void (*Getter)(Object *, MyObjectProperty p, void *);

void MyObject_delete(MyObject* this);
int MyObject_getSize(MyObject* this);

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

struct SQLite_info
{
  sqlite3* db;
  int id;
};

struct MyObject_info
{
  Object* content;
  int isOwner;
  int count;
};

/**********************************************//**
  @class MyObject
**************************************************/
struct MyObject
{
  Object object;
  DataAccess* data;
};

Class myObjectClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&MyObject_delete,
  .f_copy = (Copy_Operator)0,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)0,
  .f_size = (Sizer)&MyObject_getSize
};

void MyObject_registerType()
{

}

MyObject* MyObject_new(DataAccess* data)
{
  MyObject* this = (MyObject*)Object_new(sizeof(MyObject), &myObjectClass);;

  this->data = data;
  this->data->alloc(data->info);

  return this;
}

void MyObject_delete(MyObject* this)
{
  this->data->dealloc(&this->object);

  Object_deallocate(&this->object);
}

int MyObject_getSize(MyObject* this)
{
  return sizeof(MyObject);
}

void MyObject_setIsOwner(MyObject * this, int value)
{
  this->data->set(&this->object, PROPERTY_isOwner, &value);
}

int MyObject_getIsOwner(MyObject * this)
{
  int result = 0;

  this->data->get(&this->object, PROPERTY_isOwner, &result);

  return result;
}

void MyObject_setCount(MyObject * this, int value)
{
  this->data->set(&this->object, PROPERTY_count, &value);
}

int MyObject_getCount(MyObject * this)
{
  int result = 0;

  this->data->get(&this->object, PROPERTY_count, &result);

  return result;
}

void SQLite_init(void * info)
{
  char* err_msg = 0;
  struct SQLite_info *d = (struct SQLite_info *)info;
  sqlite3 ** db = &(d->db);
  char* initSql = "DROP TABLE IF EXISTS MyObject;"
    "CREATE TABLE MyObject(id INTEGER PRIMARY KEY, content INTEGER, isOwner INTEGER, count INTEGER);";

  int result = sqlite3_open("test.db", db);
  result = sqlite3_exec(*db, initSql, 0, 0, &err_msg);
  
  //PRINT(("DB1 pointer is %x\n", *db));
}

void SQLite_final(void* info)
{
  sqlite3* db = (sqlite3*)(((struct SQLite_info*)info)->db);
  sqlite3_close(db);
}

void* SQLite_alloc(void* info)
{
  char* sqlCreate;
  int result;
  char* err_msg = 0;
  struct SQLite_info * local_info = info;
  //sqlite3* db = (sqlite3*)(((struct SQLite_info*)info)->db);

  //PRINT(("DB pointer is %x\n", local_info->db));

  sqlCreate = "INSERT INTO MyObject VALUES(NULL,0,0,0);";
  result = sqlite3_exec(local_info->db, sqlCreate, 0, 0, &err_msg);
  if (result != SQLITE_OK)
  {
    PRINT(("SQL Error: %s\n", err_msg));
    //sqlite3_free(err_msg);
  }
  ((struct SQLite_info*)info)->id = (int)sqlite3_last_insert_rowid(local_info->db);

  return 0;
}

void SQLite_dealloc(MyObject* object)
{
  char* sqlDelete;
  //int result;
  //char* err_msg;
  sqlite3_stmt* stmt;
  sqlite3* db = (sqlite3*)((struct SQLite_info*)((MyObject*)object)->data->info)->db;
  int id = ((struct SQLite_info*)((MyObject*)object)->data->info)->id;

  sqlDelete = "DELETE FROM MyObject WHERE Id=?;";
  sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, NULL);
  sqlite3_bind_int(stmt, 1, id);
  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
}

void SQLite_setProperty(Object* object, MyObjectProperty p, void* value)
{
  char* sqlInsert;
  //int result;
  //char* err_msg;
  sqlite3_stmt* stmt;
  sqlite3* db = (sqlite3*)((struct SQLite_info*)((MyObject*)object)->data->info)->db;
  int id = ((struct SQLite_info*)((MyObject*)object)->data->info)->id;
  switch (p) {
  case PROPERTY_content:
    sqlInsert = "UPDATE MyObject SET content = ? WHERE id = ?;";
    sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, *(int*)value);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    break;
  case PROPERTY_isOwner:
    sqlInsert = "UPDATE MyObject SET isOwner = ? WHERE Id = ?;";
    sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, *(int*)value);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    break;
  case PROPERTY_count:
    sqlInsert = "UPDATE MyObject SET count = ? WHERE Id = ?;";
    sqlite3_prepare_v2(db, sqlInsert, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, *(int*)value);
    sqlite3_bind_int(stmt, 2, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    break;
  default:
    break;
  }
}

void SQLite_getProperty(Object* object, MyObjectProperty p, void * var)
{
  char* sqlQuery;
  int * result = (int*)var;
  //char* err_msg;
  sqlite3_stmt* stmt;
  sqlite3* db = (sqlite3*)((struct SQLite_info*)((MyObject*)object)->data->info)->db;
  int id = ((struct SQLite_info*)((MyObject*)object)->data->info)->id;
  switch (p) {
  case PROPERTY_content:
    sqlQuery = "SELECT content FROM MyObject WHERE id=?";
    sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);

    sqlite3_step(stmt);
    *result = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    break;
  case PROPERTY_isOwner:
    sqlQuery = "SELECT isOwner FROM MyObject WHERE id=?";
    sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);

    sqlite3_step(stmt);
    *result = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    break;
  case PROPERTY_count:
    sqlQuery = "SELECT count FROM MyObject WHERE id=?";
    sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, id);

    sqlite3_step(stmt);
    *result = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    break;
  default:
    break;
  }
}

void Mem_setProperty(Object* object, MyObjectProperty p, int value)
{
  switch (p) {
  case PROPERTY_content:
    break;
  case PROPERTY_isOwner:
    break;
  case PROPERTY_count:
    break;
  default:
    break;
  }
}

MyObject* MyObject_alloc(void* info)
{
  MyObject* this = (MyObject*)Object_new(sizeof(MyObject), &myObjectClass);

  return this;
}

void MyObject_dealloc(MyObject* this)
{
  Object_deallocate(&this->object);
}

void MyObject_setProperty(Object* object, MyObjectProperty p, void* value)
{
  struct MyObject_info* myObject = ((struct MyObject_info*)((MyObject*)object)->data->info);

  switch (p) {
  case PROPERTY_content:
    myObject->content = (Object*)value; // dataAccess(object, 
    break;
  case PROPERTY_isOwner:
    myObject->isOwner = *(int*)value;
    break;
  case PROPERTY_count:
    myObject->count = *(int*)value;
    break;
  default:
    break;
  }
}

void MyObject_getProperty(Object* object, MyObjectProperty p, void * var)
{
  int * result = var;
  struct MyObject_info* myObject = ((struct MyObject_info*)((MyObject*)object)->data->info);

  switch (p) {
  case PROPERTY_content:
    *result = myObject->content;
    break;
  case PROPERTY_isOwner:
    *result = myObject->isOwner;
    break;
  case PROPERTY_count:
    *result = myObject->count;
    break;
  default:
    // Error
    break;
  }
}

int step1()
{
  DataAccess s;
  struct MyObject_info info;
  s.info = &info;
  s.alloc = (Allocate)MyObject_alloc;
  s.dealloc = (Deallocate)MyObject_dealloc;
  s.set = MyObject_setProperty;
  s.get = MyObject_getProperty;

  MyObject* testMyObject = 0;

  testMyObject = MyObject_new(&s);
  MyObject_setIsOwner(testMyObject, 1);
  MyObject_setCount(testMyObject, 20);

  PRINT(("DataAccess: MyObject\n"));
  PRINT(("  IsOwner = %d\n", MyObject_getIsOwner(testMyObject)));
  PRINT(("  Count: %d\n", MyObject_getCount(testMyObject)));

  //MyObject_delete(testMyObject);

  return 0;
}

int step2()
{
  struct SQLite_info sqlite_info;
  DataAccess data;
  data.init = (Initialise)SQLite_init;
  data.final = (Finalise)SQLite_final;
  data.alloc = (Allocate)SQLite_alloc;
  data.dealloc = (Deallocate)SQLite_dealloc;
  data.set = (Setter)SQLite_setProperty;
  data.get = (Getter)SQLite_getProperty;
  data.info = &sqlite_info;
  
  data.init(data.info);

  MyObject* testMyObject = MyObject_new(&data);
  
  int isOwner = 1;
  
  MyObject_setIsOwner(testMyObject, 1);
  MyObject_setCount(testMyObject, 20);

  PRINT(("DataAccess: SQLite\n"));
  PRINT(("IsOwner = %d\n", MyObject_getIsOwner(testMyObject)));
  PRINT(("Count: %d\n", MyObject_getCount(testMyObject)));

  MyObject_delete(testMyObject);

  data.final(data.info);

  return 0;
}

void main()
{
  step1();
  step2();
}
