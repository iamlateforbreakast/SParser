/* Configuration.c */
#include "Configuration.h"
#include "Product.h"
#include "Object.h"

struct Configuration
{
  Object object;
  List * products;
};

PRIVATE Configuration_readLabel(Configuration * this);
PRIVATE Configuration_readValue(Configuration * this);
PUBLIC Configuration * Configuration_new(String * input)
{
  Configuration* c = (Configuration*)Object_new(sizeof(Configuration), &configurationClass)
    char* p = String_getBuffer(input);
  while (1)
  {
    s = Configuration_readLabel(this, p);
    if s
    v = Configurationg_readValue(this, p);
    if (String_compare(s, "Location"))
    {
      p
    }
    else if (String_compare(s, "Includes"))
    {
    }
    else if (String_compare(s, "Uses"))
    {
    }
    else if (String_compare(s, "Sources"))
    {
    }
    else
    {
      p = Product_new();
    }
  }
  return 0;
};

PUBLIC void Configuration_delete(Configuration * this)
{
  Object_deallocate(&this->object);
}
  
PRIVATE String* Configuration_readLabel(Configuration* this)
{
  return 0;
}
