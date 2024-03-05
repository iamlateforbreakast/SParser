/* Product.h */
#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include "List.h"
#include "String2.h"
#include "Object.h"

typedef struct Product Product;

PRIVATE Product * Product_new(String * this);
PRIVATE void Product_delete(Product * this);
PRIVATE void Product_print(Product * this);
PRIVATE unsigned int Product_getSize(Product * this);
PRIVATE void Product_setLocation(Product * this, String * s);
PRIVATE void Product_setIncludes(Product * this, String *s);
PRIVATE void Product_setUses(Product * this, String * s);
PRIVATE void Product_setSources(Product * this, String *s);

struct Product
{
  Object object;
  String * name;
  String * location;
  List * grammars;
  List * sources;
  List * includes;
  List * uses;
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

PRIVATE Product * Product_new(String * s)
{
  Product * this = 0;

  this = (Product *)Object_new(sizeof(Product), &productClass);

  return this;
}

PRIVATE void Product_delete(Product * this)
{

}

PRIVATE void Product_print(Product * this)
{

}

PRIVATE unsigned int Product_getSize(Product * this)
{
  return sizeof(Product);
}

PRIVATE void Product_setLocation(Product * this, String * s)
{

}

PRIVATE void Product_setIncludes(Product * this, String * s)
{

}

PRIVATE void Product_setUses(Product * this, String * s)
{

}

PRIVATE void Product_setSources(Product * this, String * s)
{

}
#endif /* _PRODUCT_H_ */
