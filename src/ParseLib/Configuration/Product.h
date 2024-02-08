/* Product.h */
#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include "List.h"
#include "String2.h"
#include "Object.h"

typedef struct Product Product;

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

#endif /* _PRODUCT_H_ */
