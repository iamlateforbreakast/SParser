/* Configuration.c */
#include "Configuration.h"
#include "Product.h"
#include "Object.h"

struct Configuration
{
  Object object;
  List * products;
};

PUBLIC Configuration * Configuration_new(String * input)
{
  return 0;
};

PUBLIC void Configuration_delete(Configuration * this)
{
  
}
