/* Configuration.c */
#include "Configuration.h"
#include "Product.h"
#include "Object.h"
#include "Memory.h"
#include "Error.h"

#define IS_LABEL_LETTER(C) (((C>'A') && (C<'Z')) || ((C>'a') && (C<'z')) || (C=='_'))

struct Configuration
{
  Object object;
  List * products;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class configurationClass =
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&Configuration_delete,
  .f_copy = (Copy_Operator)0,
  .f_comp = (Comp_Operator)0,
  .f_print = (Printer)&Configuration_print,
  .f_size = (Sizer)&Configuration_getSize
};

PRIVATE String * Configuration_readLabel(Configuration * this, char * p);
PRIVATE String * Configuration_readValue(Configuration * this, char * p);

PUBLIC Configuration * Configuration_new(String * input)
{
  Configuration* c = (Configuration*)Object_new(sizeof(Configuration), &configurationClass);
  char* ptr = String_getBuffer(input);
  int nbCharRead = 0;
  Product * product = 0;

  while (nbCharRead<String_getLength(input))
  {
    String * s = Configuration_readLabel(c, ptr);
    
    if (s==0) Error_new(ERROR_FATAL, "Configuration error: Cannot read label\n");

    String * v = Configuration_readValue(c, ptr);

    if (v==0) Error_new(ERROR_FATAL, "Configuration error: cannot read value\n");

    if (Memory_ncmp(String_getBuffer(s), "Location", 8))
    {
      Product_setLocation(product, v);
    }
    else if (Memory_ncmp(String_getBuffer(s), "Includes", 8))
    {
      Product_setIncludes(product, v);
    }
    else if (Memory_ncmp(String_getBuffer(s), "Uses", 4))
    {
      Product_setUses(product, v);
    }
    else if (Memory_ncmp(String_getBuffer(s), "Sources", 7))
    {
      Product_setSources(product, v);
    }
    else
    {
      product = Product_new(s);
      List_insertHead(c->products, product, 1);
    }
  }

  return c;
};

PUBLIC void Configuration_delete(Configuration * this)
{
  Object_deallocate(&this->object);
}

PUBLIC void Configuration_print(Configuration * this)
{

}

PUBLIC unsigned int Configuration_getSize(Configuration * this)
{
  return sizeof(Configuration);
}

PRIVATE String* Configuration_readLabel(Configuration* this, char * p)
{
  while (!IS_LABEL_LETTER(*p)) p++;

  while (IS_LABEL_LETTER(*p)) p++;

  while (!IS_COLON(*p)) p++;
  return 0;
}

PRIVATE String * Configuration_readValue(Configuration * this, char * p)
{
  return 0;
}