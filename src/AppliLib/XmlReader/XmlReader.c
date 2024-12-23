/* XmlReader.c */
#include "XmlReader.h"
#include "Object.h"

struct XmlReader
{
  Object object;
};

/**********************************************//**
  @private Class Description
**************************************************/
Class xmlReaderClass = 
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&XmlReader_delete,
  .f_copy = (Copy_Operator)&XmlReader_copy,
  .f_comp = (Comp_Operator)&XmlReader_compare,
  .f_print = (Printer)&XmlReader_print,
  .f_size = (Sizer)&XmlReader_getSize
};

/**********************************************//** 
  @brief Create a new instance of the class XmlReader.
  @public
  @memberof XmlReader
  @return New instance.
**************************************************/
XmlReader* XmlReader_new(String* string)
{
  XmlReader* this;

  this = (XmlReader*)Object_new(sizeof(XmlReader),&xmlReaderClass);

  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class XmlReader.
  @public
  @memberof XmlReader
**************************************************/
void XmlReader_delete(XmlReader* this)
{
  if (this!=0)
  {
    /* De-allocate the base object */
      Object_deallocate(&this->object);
  }
}

XmlReader* XmlReader_copy(XmlReader* this)
{

}

int XmlReader_compare(XmlReader* this, XmlReader* compared)
{
  return 0;
}

void XmlReader_print(XmlReader* this)
{

}

unsigned int XmlReader_getSize(XmlReader* this)
{
  return sizeof(XmlReader);
}