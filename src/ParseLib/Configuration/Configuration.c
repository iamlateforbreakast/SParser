/* Configuration.c */
#include "Configuration.h"
#include "Product.h"
#include "Object.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"

#define DEBUG (0)

#define IS_KEY(C) (((C>='A') && (C<='Z')) || ((C>='a') && (C<='z')) \
                  || ((C>='0') && (C<='9')) || (C=='_'))
#define IS_COLON(P) (Memory_ncmp(P, ":", 1))
#define IS_LIST(P) (Memory_ncmp(P, "- ", 2))
#define IS_LOCATION_KEY(P) (Memory_ncmp(P,"Location:", 9))
#define IS_INCLUDES_KEY(P) (Memory_ncmp(P,"Includes:", 9))
#define IS_USES_KEY(P) (Memory_ncmp(P,"Uses:", 5))
#define IS_SOURCES_KEY(P) (Memory_ncmp(P,"Sources:", 8))
#define IS_IGNORED(C) ((C==' ') || (C=='\n') || (C=='\r') || (C=='\t'))
#define IS_STRING(C) (((C>='A') && (C<='Z')) || ((C>='a') && (C<='z')) \
                  || ((C>='0') && (C<='9')) || (C=='_'))

#ifdef _WIN32
#define IS_EOL(P) (Memory_ncmp(P, "\r\n", 2))
#else
#define IS_EOL(P) (Memory_ncmp(P, "\n", 1))
#endif

struct Configuration
{
  Object object;
  List* products;
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

PRIVATE List* Configuration_readProducts(Configuration* this, String* s);
PRIVATE String* Configuration_readLocation(Configuration* this, String* s, unsigned int* idx);
PRIVATE List* Configuration_readIncludes(Configuration* this, String* s, unsigned int* idx);
PRIVATE List* Configuration_readUses(Configuration* this, String* s, unsigned int* idx);
PRIVATE List* Configuration_readSources(Configuration* this, String* s, unsigned int* idx);
PRIVATE String* Configuration_readString(Configuration* this, String* s, unsigned int* idx);
PRIVATE List* Configuration_readList(Configuration* this, String* s, unsigned int* idx);
PRIVATE void Configuration_readEndOfLine(Configuration* this, String* s, unsigned int* idx);
PRIVATE unsigned int Configuration_readIndent(Configuration* this, String* s, unsigned int* idx);

PUBLIC Configuration* Configuration_new(String* input)
{
  Configuration* c = (Configuration*)Object_new(sizeof(Configuration), &configurationClass);

  if (c == 0) return 0;

  c->products = Configuration_readProducts(c, input);

  return c;
}

PUBLIC void Configuration_delete(Configuration* this)
{
  List_delete(this->products);

  Object_deallocate(&this->object);
}

PUBLIC void Configuration_print(Configuration* this)
{
  Product* p = 0;

  PRINT(("\nConfiguration\n"));

  List_resetIterator(this->products);

  while ((p = List_getNext(this->products)) != 0)
  {
    Product_print(p);
  }

  List_resetIterator(this->products);
}

PUBLIC unsigned int Configuration_getSize(Configuration* this)
{
  return sizeof(Configuration);
}

PUBLIC FileMgr* Configuration_getFilesForProduct(Configuration* this, Product * product)
{
  FileMgr* fm = 0;

  // location = Product_getLocation(product);
  // fm = FileMgr_new();
  // FileMgr_setRootLocation(fm, location);
  // for each directory in include
  // add directoy to fm
  // for each file in source
  // add file to fm

  return fm;
}

PRIVATE List* Configuration_readProducts(Configuration* this, String* s)
{
  char* p = String_getBuffer(s);
  List* products = 0;
  Product* product = 0;
  unsigned int idx1 = 0;
  unsigned int idx2 = 0;

  while (IS_LIST(p + idx2) && (idx2 < String_getLength(s)))
  {
    idx2 += 2;
    idx1 = idx2;
    //while (IS_KEY(*(p+idx2))) idx2++;

    //String* productName = String_subString(s, idx1, idx2);

    String* productName = Configuration_readString(this, s, &idx2);

    if (productName)
    {
      if (products == 0) products = List_new();
      product = Product_new(productName);
      List_insertHead(products, product, 1);
    }

    if (!IS_COLON(p + idx2))
    {
      return 0;
    }
    idx2++;

    while (IS_IGNORED(*(p + idx2))) idx2++;

    String* location = Configuration_readLocation(this, s, &idx2);

    if (location) product->location = location;

    while (IS_IGNORED(*(p + idx2))) idx2++;

    List* includes = Configuration_readIncludes(this, s, &idx2);

    if (includes) product->includes = includes;

    while (IS_IGNORED(*(p + idx2))) idx2++;

    List* uses = Configuration_readUses(this, s, &idx2);

    if (uses) product->uses = uses;

    while (IS_IGNORED(*(p + idx2))) idx2++;

    List* sources = Configuration_readSources(this, s, &idx2);

    if (sources) product->sources = sources;

    while (IS_IGNORED(*(p + idx2))) idx2++;

    Product_print(product);
  }

  return products;
}

PRIVATE String* Configuration_readLocation(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);

  if (!IS_LOCATION_KEY(p + *idx)) return 0;

  *idx += 10;

  return Configuration_readString(this, s, idx);
}

PRIVATE List* Configuration_readIncludes(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);

  if (!IS_INCLUDES_KEY(p + *idx)) return 0;

  *idx += 10;

  return Configuration_readList(this, s, idx);
}

PRIVATE List* Configuration_readUses(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);

  if (!IS_USES_KEY(p + *idx)) return 0;

  *idx += 5;

  return Configuration_readList(this, s, idx);
}

PRIVATE List* Configuration_readSources(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);

  if (!IS_SOURCES_KEY(p + *idx)) return 0;

  *idx += 8;

  return Configuration_readList(this, s, idx);
}

PRIVATE String* Configuration_readString(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);
  unsigned int idx1 = *idx;

  while (IS_STRING(*(p + *idx)))
  {
    (*idx)++;
  }

  return String_subString(s, idx1, *idx - idx1);;
}

PRIVATE List* Configuration_readList(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);
  unsigned int idx1 = *idx;
  List* l = 0;

  if (*(p + *idx) == '[')
  {
    l = List_new();
    (*idx)++;
    while (*(p + *idx) != ']')
    {
      String* item = Configuration_readString(this, s, idx);
      TRACE(("Configuration: --> %s\n", String_getBuffer(item)));
      List_insertHead(l, item, 1);
      if (*(p + *idx) == ',') (*idx)++;
    }
    (*idx)++;
  }
  else
  {
    //while (IS_IGNORED(*(p + *idx))) (*idx)++;
    Configuration_readEndOfLine(this, s, idx);
    int indent = Configuration_readIndent(this, s, idx);
    int nextIndent = indent;
    while (IS_LIST(p + *idx) && (nextIndent == indent))
    {
      *idx += 2;
      String* item = Configuration_readString(this, s, idx);
      TRACE(("Configuration: --> %s\n", String_getBuffer(item)));
      if (l == 0) l = List_new();
      List_insertHead(l, item, 1);
      Configuration_readEndOfLine(this, s, idx);
      nextIndent = Configuration_readIndent(this, s, idx);
    }
  }

  return l;
}

PRIVATE void Configuration_readEndOfLine(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);

  while (!IS_EOL(p + *idx) && (*idx < String_getLength(s))) (*idx)++;
  *idx += 2;
}

PRIVATE unsigned int Configuration_readIndent(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);
  unsigned int indent = *idx;

  while (IS_IGNORED(*(p + *idx))) (*idx)++;

  return (*idx) - indent;
}
