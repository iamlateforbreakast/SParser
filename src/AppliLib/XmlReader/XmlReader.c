/* XmlReader.c */
#include "XmlReader.h"
#include "Object.h"
#include "Memory.h"

struct XmlReader
{
  Object object;
  char * buffer;
  char * readPtr;
  int nbCharRead;
  int length;
};

PUBLIC int XmlReader_readVersion(XmlReader* this);
PUBLIC int XmlReader_readComment(XmlReader* this);
PUBLIC int XmlReader_readEndElement(XmlReader* this);
PUBLIC int XmlReader_readElement(XmlReader* this);

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
  this->nbCharRead = 0;

  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class XmlReader.
  @public
  @memberof XmlReader
**************************************************/
void XmlReader_delete(XmlReader* this)
{
  if (OBJECT_IS_INVALID(this)) return;

  /* De-allocate the base object */
  Object_deallocate(&this->object);
  
}

/**********************************************//** 
  @brief Copy an instance of the class XmlReader.
  @public
  @memberof XmlReader
  @return Copy of instance
**************************************************/
XmlReader* XmlReader_copy(XmlReader* this)
{

}

/**********************************************//** 
  @brief Compare an instance of the class XmlReader.
  @public
  @memberof XmlReader
  @return always 0
**************************************************/
int XmlReader_compare(XmlReader* this, XmlReader* compared)
{
  return 0;
}

/**********************************************//** 
  @brief Print an instance of the class XmlReader.
  @public
  @memberof XmlReader
**************************************************/
void XmlReader_print(XmlReader* this)
{

}

/**********************************************//** 
  @brief Provide the size of the class or an instance
  @public
  @memberof XmlReader
  @return Size in byte
**************************************************/
unsigned int XmlReader_getSize(XmlReader* this)
{
  return sizeof(XmlReader);
}

/**********************************************//** 
  @brief read the next xml node from the XmlREaderstream
  @public
  @memberof XmlReader
  @return Size in byte
**************************************************/
XmlNode XmlReader_read(XmlReader * this)
{
  XmlNode node = XMLNONE;

  while ((this->nbCharRead<this->length) && (node==XMLNONE))
  {
    if (Memory_ncmp(this->readPtr, "<!--",4))
    {
      this->nbCharRead += XmlReader_readComment(this);
      node = XMLCOMMENT;
    }
    else if (Memory_ncmp(this->readPtr, "<?xml",4))
    {
      this->nbCharRead += XmlReader_readVersion(this);
      node = XMLVERSION;
    }
    else if(Memory_ncmp(this->readPtr, "</",2))
    {
      this->nbCharRead += XmlReader_readEndElement(this);
      node = XMLENDELEMENT;
    }
    else if (Memory_ncmp(this->readPtr, "<", 1))
    {
      this->nbCharRead += XmlReader_readElement(this);
      node = XMLELEMENT;
    }
    else
    {
      this->readPtr++;
      this->nbCharRead++;
    }
  }

  return node;
}

PUBLIC int XmlReader_readVersion(XmlReader* this)
{

}

PUBLIC int XmlReader_readComment(XmlReader* this)
{

}

PUBLIC int XmlReader_readEndElement(XmlReader* this)
{

}

PUBLIC int XmlReader_readElement(XmlReader* this)
{

}