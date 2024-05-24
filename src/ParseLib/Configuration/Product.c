/**********************************************//**
  @file Product.c

  @brief This file contains the implementation for the class Product

  The class Product contains the sources for a given product.
**************************************************/
#include "Product.h"
#include "FileMgr.h"
#include "String.h"
#include "Debug.h"
#define DEBUG (0)

/**********************************************//**
  @class Product
**************************************************/
struct Product
{
  Object object;
  String* name;
  String* location;
  List* sources;
  List* includes;
  List* uses;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class productClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&Product_delete,
  .f_copy = (Copy_Operator)0,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)&Product_print,
  .f_size = (Sizer)&Product_getSize
};

PUBLIC Product* Product_new(String* s)
{
  Product* this = 0;

  this = (Product*)Object_new(sizeof(Product), &productClass);

  if (this == 0) return 0;

  this->name = s;
  this->location = 0;
  this->includes = 0;
  this->uses = 0;
  this->sources = 0;

  return this;
}

PUBLIC void Product_delete(Product* this)
{
  if (this == 0) return;

  String_delete(this->location);
  List_delete(this->sources);
  List_delete(this->includes);
  List_delete(this->uses);
  String_delete(this->name);

  Object_deallocate(&this->object);
}

PUBLIC void Product_print(Product* this)
{
  if (this == 0) return;

  PRINT(("Product: %s\n", String_getBuffer(this->name)));
  PRINT(("  Location: %s\n", String_getBuffer(this->location)));
  PRINT(("  Includes:\n"));

  String* s = 0;
  List_resetIterator(this->includes);
  while ((s = List_getNext(this->includes)) != 0)
  {
    PRINT(("    - %s\n", String_getBuffer(s)));
  }
  List_resetIterator(this->includes);

  PRINT(("  Uses:\n"));
  List_resetIterator(this->uses);
  while ((s = List_getNext(this->uses)) != 0)
  {
    PRINT(("    - %s\n", String_getBuffer(s)));
  }
  List_resetIterator(this->uses);

  PRINT(("  Sources:\n"));
  List_resetIterator(this->sources);
  while ((s = List_getNext(this->sources)) != 0)
  {
    PRINT(("    - %s\n", String_getBuffer(s)));
  }
  List_resetIterator(this->sources);

}

PUBLIC unsigned int Product_getSize(Product* this)
{
  return sizeof(Product);
}

PUBLIC String* Product_getName(Product* this)
{
  return this->name;
}
PUBLIC void Product_setLocation(Product* this, String* s)
{
  this->location = s;
}

PUBLIC String* Product_getLocation(Product* this)
{
  return this->location;
}
PUBLIC void Product_setIncludes(Product* this, List * l)
{
  this->includes = l;
}

PUBLIC void Product_setUses(Product* this, List * l)
{
  this->uses = l;
}

PUBLIC void Product_setSources(Product* this, List * l)
{
  this->sources = l;
}
PUBLIC FileMgr* Product_getSourceFiles(Product* this)
{
  FileMgr* fileMgr = FileMgr_new();
  FileMgr_setRootLocation(fileMgr, String_getBuffer(this->location));
  TRACE(("Product: Create FileMgr at root location %s\n", String_getBuffer(this->location)));
  String* includeDir;
  while ((includeDir = List_getNext(this->includes)) != 0)
  {
    TRACE(("Product: Add include: %s\n", String_getBuffer(includeDir)));
    FileMgr_addDirectory(fileMgr, String_getBuffer(includeDir));
  }
  String* sourceFile = 0;
  while ((sourceFile = List_getNext(this->sources)) != 0)
  {
    TRACE(("Product: Add file: %s\n", String_getBuffer(sourceFile)));
    FileMgr_addFile(fileMgr, String_getBuffer(sourceFile));
  }
  FileMgr_print(fileMgr);
  return fileMgr;
}
