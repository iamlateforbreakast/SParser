/* Ast.c */

#include "sqlite3.h"

#include "Ast.h"
#include "Object.h"
#include "DataAccess.h"

/**********************************************//**
  @class Ast
**************************************************/
struct Ast
{
  Object object;
  DataAccess* data;
};

struct AstData
{
  int a;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class astClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&Ast_delete,
  .f_copy = (Copy_Operator)&Ast_copy,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)&Ast_print,
  .f_size = (Sizer)&Ast_getSize
};

Ast* Ast_new(DataAccess* data)
{
  Ast * this;

  this = (Ast*)Object_new(sizeof(Ast), &astClass);

  if (OBJECT_IS_INVALID(this)) return 0;

  this->data = data;
  this->data->alloc(data->info);

  return this;
}

void Ast_delete(Ast* this)
{
  if (OBJECT_IS_INVALID(this)) return;

  this->data->dealloc(&this->object);

  Object_deallocate(&this->object);
}

Ast* Ast_copy(Ast* this)
{
  return 0;
}

void Ast_print(Ast* this)
{
}

int Ast_getSize(Ast* this)
{
  return sizeof(Ast);
}

void SQLite_init(void * info)
{
  char* err_msg = 0;
  struct SQLite_info *d = (struct SQLite_info *)info;
  sqlite3 ** db = &(d->db);
  char* initSql = "DROP TABLE IF EXISTS Ast;"
                  "CREATE TABLE Ast(id INTEGER PRIMARY KEY, node_id INTEGER);"
                  "DROP TABLE IF EXISTS AstNodes"
                  "CREATE TABLE AstNodes(node_id INTEGER, parent_id INTEGER, type INTEGER );";

  int result = sqlite3_open("test.db", db);
  result = sqlite3_exec(*db, initSql, 0, 0, &err_msg);
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

  sqlCreate = "INSERT INTO Ast VALUES(NULL,0);";
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

  sqlDelete = "DELETE FROM Ast WHERE Id=?;";
  sqlite3_prepare_v2(db, sqlDelete, -1, &stmt, NULL);
  sqlite3_bind_int(stmt, 1, id);
  sqlite3_step(stmt);

  sqlite3_finalize(stmt);
}