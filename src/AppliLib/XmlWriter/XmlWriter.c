/* XmlWriter.c */

#include "XmlWriter.h"
#include "Object.h"
#include "Memory.h"

/**********************************************//**
  @class XmlWriter
**************************************************/
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
  .f_delete = (Destructor)&XmlWriter_delete,
  .f_copy = (Copy_Operator)&XmlWriter_copy,
  .f_comp = (Comp_Operator)&XmlWriter_compare,
  .f_print = (Printer)&XmlWriter_print,
  .f_size = (Sizer)&XmlWriter_getSize
};

/**********************************************//**
  Private method declarations
**************************************************/

/**********************************************//** 
  @brief Create a new instance of the class XmlWriter.
  @public
  @memberof XmlWriter
  @return New instance.
**************************************************/
PUBLIC XmlWriter* XmlWriter_new()
{
}

/**********************************************//** 
  @brief Delete an instance of the class XmlWriter.
  @public
  @memberof XmlWriter
**************************************************/
PUBLIC void XmlWriter_delete(XmlWriter* this)
{
}

/**********************************************//** 
  @brief Copy an instance of the class XmlWriter.
  @public
  @memberof XmlWriter
  @return Copy of instance
**************************************************/
PUBLIC XmlWriter* XmlWriter_copy(XmlWriter* this)
{
  return 0;
}

/**********************************************//** 
  @brief Compare an instance of the class XmlWriter.
  @public
  @memberof XmlWriter
  @return always 0
**************************************************/
PUBLIC int XmlWriter_compare(XmlWriter* this, XmlWriter* compared)
{
  return 0;
}

/**********************************************//** 
  @brief Print an instance of the class XmlWriter.
  @public
  @memberof XmlWriter
**************************************************/
PUBLIC void XmlWriter_print(XmlWriter* this)
{

}

/**********************************************//** 
  @brief Provide the size of the class or an instance
  @public
  @memberof XmlWriter
  @return Size in byte
**************************************************/
PUBLIC unsigned int XmlWriter_getSize(XmlWriter* this)
{
  return sizeof(XmlWriter);
}

/**********************************************//** 
  @brief Write the XML headre for the XML document
  @public
  @memberof XmlWriter
  @return none
**************************************************/
PUBLIC void XmlWriter_startDocument(XmlWriter* this)
{
}

/**********************************************//** 
  @brief Close the XML document
  @public
  @memberof XmlWriter
  @return none
**************************************************/
PUBLIC void XmlWriter_endDocument(XmlWriter* this)
{
}

/**********************************************//** 
  @brief Starts an XML element in XML document
  @public
  @memberof XmlWriter
  @return none
**************************************************/
PUBLIC void XmlWriter_startElement(XmlWriter* this)
{
}

/**********************************************//** 
  @brief Close an XML element in XML document
  @public
  @memberof XmlWriter
  @return none
**************************************************/
PUBLIC void XmlWriter_endElement(XmlWriter* this)
{
}

PUBLIC void XmlWriter_writeElementString(XmlWriter* this, String* string)
{
}

PUBLIC void XmlWriter_writeAttributeString(XmlWriter* this, String* string)
{
}
