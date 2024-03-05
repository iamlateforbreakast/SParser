/* Configuration.c */
#include "Configuration.h"
#include "Product.h"
#include "Object.h"

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
  char* p = String_getBuffer(input);
  int nbCharRead = 0;

  while (nbCharRead<String_getLength(input))
  {
    String * s = Configuration_readLabel(c, p);
    
    if (s==0) exit(0);

    String * v = Configuration_readValue(c, p);

    if (v==0) exit(0);

    if (String_compare(s, "Location"))
    {
      Product_setLocation(p, v);
    }
    else if (String_compare(s, "Includes"))
    {
      Product_setIncludes(p, v);
    }
    else if (String_compare(s, "Uses"))
    {
      Product_setUses(p, v);
    }
    else if (String_compare(s, "Sources"))
    {
      Product_setSources(p, v);
    }
    else
    {
      p = Product_new(s);
      List_insertHead(c->products, p, 1);
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
  return 0;
}

PRIVATE String * Configuration_readValue(Configuration * this, char * p)
{
  return 0;
}