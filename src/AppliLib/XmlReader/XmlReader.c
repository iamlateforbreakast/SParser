/* XmlReader.c */
#include "XmlReader.h"
#include "Object.h"
#include "Memory.h"
#include "Error.h"
#include "Debug.h"

#define BUFFER_SIZE (512) /* In bytes */

#define IS_ELEMENT_LETTER(C) ((((C)>='A') && ((C)<='Z')) || (((C)>='a') && ((C)<='z')) \
|| ((C)=='_') || ((C)=='.') || ((C)=='-'))

/**********************************************//**
  @class Xmlreader
**************************************************/
struct XmlReader
{
  Object object;
  char * content;
  char * attrValue;
  char * readPtr;
  int nbCharRead;
  int length;
  int line;
  int col;
  int contentUse;
  int attrValueUse;
  int isInsideElement;
  int isError;
  XmlNode node;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class xmlReaderClass = 
{
  .f_new = (Constructor)0,
  .f_delete = (Destructor)&XmlReader_delete,
  .f_copy = (Copy_Operator)&XmlReader_copy,
  .f_comp = (Comp_Operator)&XmlReader_compare,
  .f_print = (Printer)&XmlReader_print,
  .f_size = (Sizer)&XmlReader_getSize
};

/**********************************************//**
  Private method declarations
**************************************************/
PRIVATE int XmlReader_consumeVersion(XmlReader* this);
PRIVATE int XmlReader_consumeComment(XmlReader* this);
PRIVATE int XmlReader_consumeEndElement(XmlReader* this);
PRIVATE int XmlReader_consumeElement(XmlReader* this);
PRIVATE int XmlReader_consumeAttribute(XmlReader* this);
PRIVATE int XmlReader_consumeName(XmlReader* this);
PRIVATE int XmlReader_consumeSpace(XmlReader* this);
PRIVATE int XmlReader_consumeString(XmlReader* this);
PRIVATE int XmlReader_consumeOneChar(XmlReader* this);

/**********************************************//** 
  @brief Create a new instance of the class XmlReader.
  @public
  @memberof XmlReader
  @return New instance.
**************************************************/
PUBLIC XmlReader* XmlReader_new(String* string)
{
  XmlReader* this = 0;

  if (OBJECT_IS_INVALID(string)) return 0;

  this = (XmlReader*)Object_new(sizeof(XmlReader),&xmlReaderClass);

  if (OBJECT_IS_INVALID(this)) return 0;
  
  this->readPtr = String_getBuffer(string);
  this->length = String_getLength(string);
  this->nbCharRead = 0;
  this->node = XMLNONE;
  this->content = (char*)Memory_alloc(BUFFER_SIZE);
  this->attrValue = (char*)Memory_alloc(BUFFER_SIZE);
  this->contentUse = 0;
  this->attrValueUse = 0;
  this->line = 1;
  this->col = 1;
  this->isInsideElement = 0;
  this->isError = 0;

  return this;
}

/**********************************************//** 
  @brief Delete an instance of the class XmlReader.
  @public
  @memberof XmlReader
**************************************************/
PUBLIC void XmlReader_delete(XmlReader* this)
{
  if (OBJECT_IS_INVALID(this)) return;

  this->readPtr = 0;
  this->node = XMLNONE;

  Memory_free(this->content, BUFFER_SIZE);
  Memory_free(this->attrValue, BUFFER_SIZE);
  
  /* De-allocate the base object */
  Object_deallocate(&this->object);
  
}

/**********************************************//** 
  @brief Copy an instance of the class XmlReader.
  @public
  @memberof XmlReader
  @return Copy of instance
**************************************************/
PUBLIC XmlReader* XmlReader_copy(XmlReader* this)
{

  return 0;

}

/**********************************************//** 
  @brief Compare an instance of the class XmlReader.
  @public
  @memberof XmlReader
  @return always 0
**************************************************/
PUBLIC int XmlReader_compare(XmlReader* this, XmlReader* compared)
{
  return 0;
}

/**********************************************//** 
  @brief Print an instance of the class XmlReader.
  @public
  @memberof XmlReader
**************************************************/
PUBLIC void XmlReader_print(XmlReader* this)
{

}

/**********************************************//** 
  @brief Provide the size of the class or an instance
  @public
  @memberof XmlReader
  @return Size in byte
**************************************************/
PUBLIC unsigned int XmlReader_getSize(XmlReader* this)
{
  return sizeof(XmlReader);
}

/**********************************************//** 
  @brief read the next xml node from the XmlREaderstream
  @public
  @memberof XmlReader
  @return Size in byte
**************************************************/
PUBLIC XmlNode XmlReader_read(XmlReader * this)
{
  this->isError = 0;
  this->node = XMLNONE;

  /* While node has not been found */
  while ((this->nbCharRead<this->length) && (this->node==XMLNONE) && !this->isError )
  {
    if (Memory_ncmp(this->readPtr, "<!--",4))
    {
      this->isError = XmlReader_consumeComment(this);
      this->node = XMLCOMMENT;
    }
    else if (Memory_ncmp(this->readPtr, "<?xml",5))
    {
      this->isError = XmlReader_consumeVersion(this);
      this->node = XMLVERSION;
    }
    else if(Memory_ncmp(this->readPtr, "</",2))
    {
      this->isError = XmlReader_consumeEndElement(this);
      this->node = XMLENDELEMENT;
    }
    else if (*this->readPtr == ' ' && this->isInsideElement)
    {
      this->isError = XmlReader_consumeAttribute(this);
      this->node = XMLATTRIBUTE;
    }
    else if (*this->readPtr == '>' && this->isInsideElement)
    {
      XmlReader_consumeOneChar(this);
      this->isInsideElement = 0;
    }
    else if (Memory_ncmp(this->readPtr, "<", 1))
    {
      XmlReader_consumeElement(this);
      this->node = XMLELEMENT;
    }
    else
    {
      XmlReader_consumeString(this);
      this->node = XMLTEXT;
    }
  }

  return this->node;
}

/**********************************************//** 
  @brief Provide the content of the current node.
  @public
  @memberof XmlReader
  @return String with content
**************************************************/
PUBLIC String* XmlReader_getContent(XmlReader* this)
{
  String* content = String_new(this->content);
  
  this->content[0] = 0;
  this->contentUse = 0;

  return content;
}

/**********************************************//** 
  @brief Provide the value of the current attribue.
  @public
  @memberof XmlReader
  @return String with content
**************************************************/
PUBLIC String* XmlReader_getAttributeValue(XmlReader* this)
{
  return String_new(this->attrValue);
}

/**********************************************//** 
  @brief Read the version node from the XmlReaderstream
  @public
  @memberof XmlReader
  @return Size in byte
**************************************************/
PUBLIC int XmlReader_consumeVersion(XmlReader* this)
{
  this->content[0] = 0;

  this->nbCharRead += 5;
  this->readPtr += 5;

  while (this->nbCharRead<this->length)
  {
    if (Memory_ncmp(this->readPtr, "?>", 2))
    {
      this->nbCharRead += 2;
      this->readPtr += 2;
      return 1;
    }
    this->nbCharRead += 1;
    this->readPtr += 1;
  }

  return 0;
}

/**********************************************//** 
  @brief Read the next comment node from the XmlREaderstream
  @public
  @memberof XmlReader
  @return int
**************************************************/
PUBLIC int XmlReader_consumeComment(XmlReader* this)
{
  this->content[0] = 0;

  this->nbCharRead += 4;
  this->readPtr += 4;

  while (this->nbCharRead<this->length)
  {
    if (Memory_ncmp(this->readPtr, "-->", 3))
    {
      this->nbCharRead += 3;
      this->readPtr += 3;
      return 0;
    }
    this->nbCharRead += 1;
    this->readPtr += 1;
  }
  return 1;
}
 
/**********************************************//** 
  @brief Read the next end element node from the XmlReaderstream
  @public
  @memberof XmlReader
  @return int
**************************************************/
PUBLIC int XmlReader_consumeEndElement(XmlReader* this)
{
  this->nbCharRead += 2;
  this->readPtr += 2;
  XmlReader_consumeName(this);
  if (*this->readPtr=='>')
  {
    this->nbCharRead++;
    this->readPtr++;
  }
  
  this->isInsideElement = 0;

  return 0;
}

/**********************************************//** 
  @brief Read the next element node from the XmlReaderstream
  @public
  @memberof XmlReader
  @return int
**************************************************/
PUBLIC int XmlReader_consumeElement(XmlReader* this)
{
  this->nbCharRead += 1;
  this->readPtr += 1;

  while (this->nbCharRead < this->length)
  {
    if (IS_ELEMENT_LETTER(*this->readPtr))
    {
      if (this->contentUse < BUFFER_SIZE - 1)
      {
        this->content[this->contentUse++] = *this->readPtr;
      }
      else
      {
        return 1; // Error: element name too long for buffer
      }
      XmlReader_consumeOneChar(this);
    }
    else if (*this->readPtr == '>')
    {
      this->nbCharRead++;
      this->readPtr++;
      this->content[this->contentUse] = 0;
      this->contentUse = 0;
      this->isInsideElement = 0;
      return 0;
    }
    else
    {
      this->content[this->contentUse] = 0;
      this->contentUse = 0;
      /* Stay positioned at the space or '>' — do not consume */
      this->isInsideElement = 1;
      return 0;
    }
  }
  return 1;
}

/**********************************************//** 
  @brief Read an attribute (name="value")
  @private
**************************************************/
PRIVATE int XmlReader_consumeAttribute(XmlReader* this)
{
  /* 1. Clear previous buffers */
  this->content[0] = 0;
  this->contentUse = 0;
  this->attrValue[0] = 0;
  this->attrValueUse = 0;

  /* 2. Skip leading whitespace */
  while (this->nbCharRead < this->length && (*this->readPtr == ' ' || *this->readPtr == '\t')) {
    XmlReader_consumeOneChar(this);
  }

  /* 3. Consume Attribute Name (e.g., 'id') */
  while (this->nbCharRead < this->length && IS_ELEMENT_LETTER(*this->readPtr)) {
    if (this->contentUse < BUFFER_SIZE - 1) {
      this->content[this->contentUse++] = *this->readPtr;
    }
    XmlReader_consumeOneChar(this);
  }
  this->content[this->contentUse] = '\0';

  /* 4. Expect '=' */
  while (this->nbCharRead < this->length && *this->readPtr != '=') {
    XmlReader_consumeOneChar(this);
  }
  XmlReader_consumeOneChar(this); // Skip '='

  /* 5. Expect quote (single or double) */
  char quoteType = *this->readPtr;
  if (quoteType != '\'' && quoteType != '\"') return 1; // Error: malformed XML
  XmlReader_consumeOneChar(this); // Skip opening quote

  /* 6. Consume Attribute Value */
  while (this->nbCharRead < this->length && *this->readPtr != quoteType) {
    if (this->attrValueUse < BUFFER_SIZE - 1) {
      this->attrValue[this->attrValueUse++] = *this->readPtr;
    }
    XmlReader_consumeOneChar(this);
  }
  this->attrValue[this->attrValueUse] = '\0';
  
  XmlReader_consumeOneChar(this); // Skip closing quote

  return 0; // Success
}

PRIVATE int XmlReader_consumeName(XmlReader* this)
{
  while (this->nbCharRead<this->length)
  {
    if (IS_ELEMENT_LETTER(*this->readPtr))
    {
      this->nbCharRead++;
      this->readPtr++;
    }
    else
      break;
  }

  return 0;
}

PRIVATE int XmlReader_consumeSpace(XmlReader* this)
{
  while (this->nbCharRead<this->length)
  {
    if (*this->readPtr==' ')
    {
      this->nbCharRead++;
      this->readPtr++;
    }
    else
      break;
  }

  return 0;
}

PRIVATE int XmlReader_consumeOneChar(XmlReader* this)
{
  if (*this->readPtr == '\n')   // check before advancing
  {
    this->col = 0;
    this->line++;
  }
  else
  {
    this->col++;
  }
  this->nbCharRead++;
  this->readPtr++;
  return 0;
}

PRIVATE int XmlReader_consumeString(XmlReader* this)
{
  while (this->nbCharRead<this->length)
  {
    if (*this->readPtr=='<')
    {
      /* String is read, do not consume */
      this->content[this->contentUse] = 0;
      return 0;
    }
    else if (*this->readPtr == '\n')
    {
      /* ignore this character */
      XmlReader_consumeOneChar(this);
    }
    else
    {
      if (this->contentUse < BUFFER_SIZE - 1)
      {
        this->content[this->contentUse++] = *this->readPtr;
        XmlReader_consumeOneChar(this);
      }
      else
      {
        this->content[this->contentUse] = 0;
        this->contentUse = 0;
        /* Stay positioned at the next char — do not consume */
        return 0; 
      }
    }
  }

  return 1;
}
