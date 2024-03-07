/* Product.h */
#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include "List.h"
#include "String2.h"
#include "Object.h"
#include "Debug.h"

typedef struct Product Product;

PRIVATE Product* Product_new(String* this);
PRIVATE void Product_delete(Product* this);
PRIVATE void Product_print(Product* this);
PRIVATE unsigned int Product_getSize(Product* this);
PRIVATE void Product_setLocation(Product* this, String* s);
PRIVATE void Product_setIncludes(Product* this, String* s);
PRIVATE void Product_setUses(Product* this, String* s);
PRIVATE void Product_setSources(Product* this, String* s);

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

PRIVATE Product* Product_new(String* s)
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

PRIVATE void Product_delete(Product* this)
{
  if (this == 0) return;

  String_delete(this->location);
  List_delete(this->sources);
  List_delete(this->includes);
  List_delete(this->uses);
  String_delete(this->name);

  Object_deallocate(&this->object);
}

PRIVATE void Product_print(Product* this)
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

PRIVATE unsigned int Product_getSize(Product* this)
{
  return sizeof(Product);
}

PRIVATE void Product_setLocation(Product* this, String* s)
{

}

PRIVATE void Product_setIncludes(Product* this, String* s)
{

}

PRIVATE void Product_setUses(Product* this, String* s)
{

}

PRIVATE void Product_setSources(Product* this, String* s)
{

}
#endif /* _PRODUCT_H_ */
