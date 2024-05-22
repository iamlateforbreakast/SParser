/* SQLite_test.c */
#include "sqlite3.h"
#include "Object.h"

typedef enum
{
  PROPERTY_object = 1,
  PROPERTY_content,
  PROPERTY_isOwner,
  PROPERTY_size,
  NB_PROPERTIES
} MyObjectProperty;

typedef struct MyObject MyObject;
typedef struct DataAccess DataAccess;
typedef void* (*Allocate)();
typedef void (*Deallocate)(Object*);
typedef void (*Setter)(Object*, MyObjectProperty p, void*);
typedef void* (*Getter)(Object*, MyObjectProperty p);

void MyObject_delete(MyObject* this);
int MyObject_getSize(MyObject* this);

struct DataAccess
{
  void* info;
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
  int size;
};

/**********************************************//**
  @class MyObject
**************************************************/
struct MyObject
{
  Object object;
  DataAccess * data;
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

MyObject* MyObject_new(DataAccess * data)
{
  MyObject* this = (MyObject*)Object_new(sizeof(MyObject), &myObjectClass);;
  
  this->data = data;
  this->data->alloc();
  //data->set(OBJECT, PROPERTY_object);
  //data->addProperty(OBJECT, PROPERTY_content);

  //data->set(&this->object, PROPERTY_isOwner, 0);
  //data->set(&this->object, PROPERTY_size, 0);

  return this;
}

void MyObject_delete(MyObject * this)
{
  this->data->dealloc(&this->object);

  Object_deallocate(&this->object);
}

int MyObject_getSize(MyObject* this)
{
  return sizeof(MyObject);
}

void * SQLite_alloc(void * info)
{
  char* sqlCreate;
  //int result;
  //char* err_msg;
  sqlite3* db = (sqlite3*)((struct SQLite_info*)info)->db;

  sqlCreate = "INSERT INTO MyObjects VALUES(0,0,0);";
  ((struct SQLite_info*)info)->id = (int)sqlite3_last_insert_rowid(db);

  return 0;
}

void SQLite_dealloc(MyObject * object)
{
  char* sqlDelete;
  //int result;
  //char* err_msg;
  sqlite3_stmt* stmt;
  sqlite3* db = (sqlite3*)((struct SQLite_info*)((MyObject*)object)->data->info)->db;
  int id = ((struct SQLite_info*)((MyObject*)object)->data->info)->id;

  sqlDelete = "DELETE FROM MyObjects VALUES(0,0,0) WHERE Id=?;";
  sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, NULL);
  sqlite3_bind_int(stmt, 1, id);
  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
}

void SQLite_setProperty(Object* object, MyObjectProperty p, void * value)
{
  // data.set
  // "INSERT INTO Friends(Name) VALUES ('Tom');"
  char* sqlInsert;
  int result;
  char* err_msg;
  sqlite3* db = (sqlite3*)((struct SQLite_info *)((MyObject*)object)->data->info)->db;
  switch (p) {
    case PROPERTY_content:
      sqlInsert = "INSERT INTO MyObjects(content) VALUES (5);";
      result = sqlite3_exec(db, sqlInsert, 0, 0, &err_msg);
      break;
    case PROPERTY_isOwner:
      sqlInsert = "INSERT INTO MyObjects(isOwner) VALUES (1);";
      result = sqlite3_exec(db, sqlInsert, 0, 0, &err_msg);
      break;
    case PROPERTY_size:
      sqlInsert = "INSERT INTO MyObjects(size) VALUES (10);";
      result = sqlite3_exec(db, sqlInsert, 0, 0, &err_msg);
      break;
    default:
      break;
  }
}

void* SQLite_getProperty(Object* object, MyObjectProperty p)
{
  char* sqlQuery;
  //int result;
  //char* err_msg;
  sqlite3_stmt* stmt;
  sqlite3* db = (sqlite3*)((struct SQLite_info*)((MyObject*)object)->data->info)->db;
  switch (p) {
    case PROPERTY_content:
      sqlQuery = "SELECT * FROM MyObjects WHERE id=?";
      sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, NULL);
      sqlite3_bind_int(stmt, 1, 3);

      while (sqlite3_step(stmt) != SQLITE_DONE) {
        int i;

        for (i = 0; i < 3; i++)
        {
          switch (sqlite3_column_type(stmt, i))
          {
          case (SQLITE3_TEXT):
            printf("%s, ", sqlite3_column_text(stmt, i));
            break;
          case (SQLITE_INTEGER):
            printf("%d, ", sqlite3_column_int(stmt, i));
            break;
          case (SQLITE_FLOAT):
            printf("%g, ", sqlite3_column_double(stmt, i));
            break;
          default:
            break;
          }
        }
      }
      sqlite3_finalize(stmt);
  }
  return 0;
}

void Mem_setProperty(Object * object, MyObjectProperty p, int value)
{
  switch (p) {
    case PROPERTY_content:
      break;
    case PROPERTY_isOwner:
      break;
    case PROPERTY_size:
      break;
    default:
      break;
  }
}

MyObject * MyObject_alloc()
{
  MyObject * this = (MyObject*)Object_new(sizeof(MyObject), &myObjectClass);

  return this;
}

void MyObject_dealloc(MyObject* this)
{
  Object_deallocate(&this->object);
}

void MyObject_setProperty(Object* object, MyObjectProperty p, void * value)
{
  struct MyObject_info* myObject = ((struct MyObject_info*)((MyObject*)object)->data->info);

  switch (p) {
    case PROPERTY_content:
      myObject->content = (Object*)value; // dataAccess(object, 
      break;
    case PROPERTY_isOwner:
      break;
    case PROPERTY_size:
      break;
    default:
      break;
  }
}

void* MyObject_getProperty(Object* object, MyObjectProperty p)
{
  void* result = 0;
  struct MyObject_info * myObject = ((struct MyObject_info*)((MyObject*)object)->data->info);

  switch (p) {
  case PROPERTY_content:
    result = myObject->content;
    break;
  case PROPERTY_isOwner:
    result = myObject->isOwner;
    break;
  case PROPERTY_size:
    result = myObject->size;
    break;
  default:
    // Error
    break;
  }
  return result;
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

  MyObject * testMyObject = 0;

  testMyObject = MyObject_new(&s);

  MyObject_delete(testMyObject);

  return 0;
}

int step2()
{
  struct SQLite_info sqlite_info;
  DataAccess data;
  data.alloc = (Allocate)SQLite_alloc;
  data.dealloc = (Deallocate)SQLite_dealloc;
  data.set = SQLite_setProperty;
  data.get = SQLite_getProperty;
  data.info = &sqlite_info;
  char* err_msg = 0;
  char* initSql = "DROP TABLE IF EXISTS MyObject;"
    "CREATE TABLE MyObject(id INTEGER PRIMARY KEY, content INTEGER, isOwner INTEGER, size INTEGER);";

  int result = sqlite3_open("test.db", &sqlite_info.db);
  result = sqlite3_exec(sqlite_info.db, initSql, 0, 0, &err_msg);

  MyObject* testMyObject = 0;

  testMyObject = MyObject_new(&data);

  //MyObject_delete(testMyObject);

  sqlite3_close(sqlite_info.db);

  return 0;
}

void main()
{ 
  //step1();
  step2();
}