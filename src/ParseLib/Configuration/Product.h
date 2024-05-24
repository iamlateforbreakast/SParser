/* Product.h */
#ifndef _PRODUCT_H_
#define _PRODUCT_H_

#include "FileMgr.h"
#include "List.h"
#include "String2.h"
#include "Object.h"
#include "Debug.h"

typedef struct Product Product;

PUBLIC Product* Product_new(String * this);
PUBLIC void Product_delete(Product * this);
PUBLIC void Product_print(Product * this);
PUBLIC unsigned int Product_getSize(Product * this);
PUBLIC String* Product_getName(Product* this);
PUBLIC void Product_setLocation(Product * this, String * s);
PUBLIC String* Product_getLocation(Product* this);
PUBLIC void Product_setIncludes(Product * this, List * l);
PUBLIC void Product_setUses(Product * this, List * l);
PUBLIC void Product_setSources(Product * this, List * l);
PUBLIC FileMgr * Product_getSourceFiles(Product * this);
#endif /* _PRODUCT_H_ */
