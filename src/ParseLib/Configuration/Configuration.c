/**********************************************//**
  @file Configuration.c
  @brief This file contains the implementation for the class Configuration
  The class Configuration lists all the SW products to parse
  including the path to the source files, any dependency.
**************************************************/
#include "Configuration.h"
#include "Product.h"
#include "TransUnit.h"
#include "Grammar.h"
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
#define IS_IGNORED(C) ((C==' ') || (C=='\n') || (C=='\r'))
#define IS_STRING(C) (((C>='A') && (C<='Z')) || ((C>='a') && (C<='z')) \
                  || ((C>='0') && (C<='9')) || (C=='_') || (C=='/') || (C=='\\') || (C=='-') || (C=='.'))
#define IS_FORBIDDEN(C) (C=='\t') 

#ifdef _WIN32
#define IS_EOL(P) (Memory_ncmp(P, "\r\n", 2))
#else
#define IS_EOL(P) (Memory_ncmp(P, "\n", 1))
#endif

/**********************************************//**
  @class Configuration
**************************************************/
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

/**********************************************//**
  @brief Create an instance of configuration class from th input string.
  @public
  @memberof Configuration
  @return Status.
**************************************************/
PUBLIC Configuration* Configuration_new(String* input)
{
  if (input == 0) return 0;
  Configuration* c = (Configuration*)Object_new(sizeof(Configuration), &configurationClass);

  if (c == 0) return 0;

  c->products = Configuration_readProducts(c, input);

  return c;
}

/**********************************************//**
  @brief Destroy an instance of configuration class.
  @public
  @memberof Configuration
**************************************************/
PUBLIC void Configuration_delete(Configuration* this)
{
  List_delete(this->products);

  Object_deallocate(&this->object);
}

/**********************************************//**
  @brief Print an instance of configuration class.
  @public
  @memberof Configuration
**************************************************/
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

/**********************************************//**
  @brief Destroy an instance of configuration class.
  @public
  @memberof Configuration
**************************************************/
PUBLIC unsigned int Configuration_getSize(Configuration* this)
{
  return sizeof(Configuration);
}

/**********************************************//**
  @brief TBD
  @public
  @memberof Configuration
**************************************************/
PUBLIC List * Configuration_getProducts(Configuration* this)
{
  return this->products;
}

/**********************************************//**
  @brief TBD
  @public
  @memberof Configuration
**************************************************/
PUBLIC void Configuration_parseProducts(Configuration* this)
{

}

/**********************************************//**
  @brief TBD
  @private
  @memberof Configuration
**************************************************/
PRIVATE List* Configuration_readProducts(Configuration* this, String* s)
{
  char* p = String_getBuffer(s);
  List* products = 0;
  Product* product = 0;
  unsigned int idx1 = 0;
  unsigned int idx2 = 0;
  while (IS_IGNORED(*(p + idx2))) idx2++;

  while (IS_LIST(p + idx2) && (idx2 < String_getLength(s)))
  {
    idx2 += 2;
    idx1 = idx2;
    //while (IS_KEY(*(p+idx2))) idx2++;

    //String* productName = String_subString(s, idx1, idx2);

    String* productName = Configuration_readString(this, s, &idx2);
    TRACE(("Configuration: Processing product %s\n", String_getBuffer(productName)));

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
    TRACE(("Configuration: Processing location %s\n", String_getBuffer(location)));

    if (location) Product_setLocation(product, location);

    while (IS_IGNORED(*(p + idx2))) idx2++;

    List* includes = Configuration_readIncludes(this, s, &idx2);
    TRACE(("Configuration: Processing includes.\n"));

    if (includes) Product_setIncludes(product, includes);

    while (IS_IGNORED(*(p + idx2))) idx2++;

    List* uses = Configuration_readUses(this, s, &idx2);
    TRACE(("Configuration: Processing uses.\n"));

    if (uses) Product_setUses(product, uses);

    while (IS_IGNORED(*(p + idx2))) idx2++;

    List* sources = Configuration_readSources(this, s, &idx2);
    TRACE(("Configuration: Processing sources.\n"));

    if (sources) Product_setSources(product, sources);

    while (IS_IGNORED(*(p + idx2))) idx2++;

    Product_print(product);
  }

  return products;
}

/**********************************************//**
  @brief TBD
  @private
  @memberof Configuration
**************************************************/
PRIVATE String* Configuration_readLocation(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);

  if (!IS_LOCATION_KEY(p + *idx)) return 0;

  *idx += 10;

  return Configuration_readString(this, s, idx);
}

/**********************************************//**
  @brief TBD
  @private
  @memberof Configuration
**************************************************/
PRIVATE List* Configuration_readIncludes(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);

  if (!IS_INCLUDES_KEY(p + *idx)) return 0;

  *idx += 10;

  return Configuration_readList(this, s, idx);
}

/**********************************************//**
  @brief TBD
  @private
  @memberof Configuration
**************************************************/
PRIVATE List* Configuration_readUses(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);

  if (!IS_USES_KEY(p + *idx)) return 0;

  *idx += 5;

  return Configuration_readList(this, s, idx);
}

/**********************************************//**
  @brief TBD
  @private
  @memberof Configuration
**************************************************/
PRIVATE List* Configuration_readSources(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);

  if (!IS_SOURCES_KEY(p + *idx)) return 0;

  *idx += 8;

  return Configuration_readList(this, s, idx);
}

/**********************************************//**
  @brief TBD
  @private
  @memberof Configuration
**************************************************/
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

/**********************************************//**
  @brief TBD
  @private
  @memberof Configuration
**************************************************/
PRIVATE List* Configuration_readList(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);
  List* l = 0;
  while (*(p + *idx) == ' ') (*idx)++;

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
      while (*(p + *idx) == ' ') (*idx)++;
    }
    (*idx)++;
  }
  else
  {
    //while (IS_IGNORED(*(p + *idx))) (*idx)++;
    Configuration_readEndOfLine(this, s, idx);
    int indent = Configuration_readIndent(this, s, idx);
    int nextIndent = indent;
    while (IS_LIST(p + *idx) && (nextIndent == indent) && (*idx < String_getLength(s)))
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

/**********************************************//**
  @brief TBD
  @private
  @memberof Configuration
**************************************************/
PRIVATE void Configuration_readEndOfLine(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);

  //while (!IS_EOL(p + *idx) && (*idx < String_getLength(s))) (*idx)++;
  while ((*(p + *idx) == '\n') || (*(p+*idx) == '\r'))
  {
    (*idx)++;
  }
}

/**********************************************//**
  @brief TBD
  @private
  @memberof Configuration
**************************************************/
PRIVATE unsigned int Configuration_readIndent(Configuration* this, String* s, unsigned int* idx)
{
  char* p = String_getBuffer(s);
  unsigned int indent = *idx;

  while (IS_IGNORED(*(p + *idx))) (*idx)++;

  return (*idx) - indent;
}
