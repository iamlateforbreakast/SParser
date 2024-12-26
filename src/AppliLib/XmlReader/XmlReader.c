/* XmlReader.c */
#include "XmlReader.h"
#include "Object.h"

struct XmlReader
{
  Object object;
  char * buffer;
  char * readPtr;
  int length;
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
  XmlReader* this = 0;

  this = (XmlReader*)Object_new(sizeof(XmlReader),&xmlReaderClass);

  if (OBJECT_IS_INVALID(this)) return 0;
  
  this->buffer = String_getBuffer(string);
  this->readPtr = this->buffer;
  this->length = String_getLength(string);
  
  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class XmlReader.
  @public
  @memberof XmlReader
**************************************************/
void XmlReader_delete(XmlReader* this)
{
  if (OBJECT_IS_VALID(this))
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

XmlNode XmlReader_read(XmlReader * this)
{
  int nbCharRead = 0;

  while ((nbCharRead<this->length) && (*(this->readPtr)!='<'))
  {
    this->readPtr++;
    nbCharRead++;
  }

  return XMLNONE;
}