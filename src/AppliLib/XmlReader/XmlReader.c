/* XmlReader.c */
#include "XmlReader.h"
#include "Object.h"
#include "Memory.h"
#include "Error.h"

#define BUFFER_SIZE (512) /* In bytes */

#define IS_ELEMENT_LETTER(C) ((((C)>='A') && ((C)<='Z')) || (((C)>='a') && ((C)<='z')) \
|| ((C)=='_') || ((C)=='.') || ((C)=='-'))

/**********************************************//**
  @class Xmlreader
**************************************************/
struct XmlReader
{
  Object object;
  char * buffer;
  char * readPtr;
  int nbCharRead;
  int length;
  int line;
  int col;
  int bufferUse;
  int isInsideElement;
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
  this->buffer = (char*)Memory_alloc(BUFFER_SIZE);
  this->bufferUse = 0;
  this->line = 1;
  this->col = 1;
  this->isInsideElement = 0;

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

  Memory_free(this->buffer, BUFFER_SIZE);

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
  this->node = XMLNONE;

  while ((this->nbCharRead<this->length) && (this->node==XMLNONE))
  {
    if (Memory_ncmp(this->readPtr, "<!--",4))
    {
      XmlReader_consumeComment(this);
      this->node = XMLCOMMENT;
    }
    else if (Memory_ncmp(this->readPtr, "<?xml",5))
    {
      XmlReader_consumeVersion(this);
      this->node = XMLVERSION;
    }
    else if(Memory_ncmp(this->readPtr, "</",2))
    {
      XmlReader_consumeEndElement(this);
      this->node = XMLENDELEMENT;
    }
    else if (*this->readPtr == ' ' && this->isInsideElement)
    {
      XmlReader_consumeAttribute(this);
      this->node = XMLATTRIBUTE;
    }
    else if (*this->readPtr == '>' && this->isInsideElement)
    {
      this->nbCharRead++;
      this->readPtr++;
      this->isInsideElement = 0;
    }
    else if (Memory_ncmp(this->readPtr, "<", 1))
    {
      XmlReader_consumeElement(this);
      this->node = XMLELEMENT;
    }
    else
    {
      if (*this->readPtr=='<')
      {
        this->node = XMLTEXT;
      }
      this->readPtr++;
      this->nbCharRead++;
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
  String* content = String_new(this->buffer);

  return content;
}

/**********************************************//** 
  @brief Read the version node from the XmlReaderstream
  @public
  @memberof XmlReader
  @return Size in byte
**************************************************/
PUBLIC int XmlReader_consumeVersion(XmlReader* this)
{
  this->buffer[0] = 0;

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

  return 1;
}

/**********************************************//** 
  @brief Read the next comment node from the XmlREaderstream
  @public
  @memberof XmlReader
  @return int
**************************************************/
PUBLIC int XmlReader_consumeComment(XmlReader* this)
{
  this->buffer[0] = 0;

  this->nbCharRead += 4;
  this->readPtr += 4;

  while (this->nbCharRead<this->length)
  {
    if (Memory_ncmp(this->readPtr, "-->", 3))
    {
      this->nbCharRead += 3;
      this->readPtr += 3;
      return 1;
    }
    this->nbCharRead += 1;
    this->readPtr += 1;
  }
  return 0;
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

  return 1;
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
      if (this->bufferUse < BUFFER_SIZE - 1)
      {
        this->buffer[this->bufferUse++] = *this->readPtr;
      }
      XmlReader_consumeOneChar(this);
    }
    else
    {
      this->buffer[this->bufferUse] = 0;
      this->bufferUse = 0;
      /* Stay positioned at the space or '>' — do not consume */
      this->isInsideElement = 1;
      return 1;
    }
  }
  return 0;
}

PRIVATE int XmlReader_consumeAttribute(XmlReader* this)
{
  XmlReader_consumeSpace(this);

  /* Capture attribute name into buffer */
  int i = 0;
  while (this->nbCharRead < this->length && IS_ELEMENT_LETTER(*this->readPtr))
  {
    if (i < BUFFER_SIZE - 1)
    {
      this->buffer[i++] = *this->readPtr;
    }
    this->nbCharRead++;
    this->readPtr++;
  }
  this->buffer[i] = 0;

  /* Consume '=' */
  if (*this->readPtr == '=')
  {
    this->nbCharRead++;
    this->readPtr++;
  }
  else
  {
    Error_new(ERROR_NORMAL, "XmlReader: expected '=' in attribute\n");
    return 0;
  }

  /* Consume quoted value, matching open and close delimiter */
  if ((*this->readPtr == '"') || (*this->readPtr == '\''))
  {
    char openQuote = *this->readPtr;
    this->nbCharRead++;
    this->readPtr++;
    while (this->nbCharRead < this->length && *this->readPtr != openQuote)
    {
      this->nbCharRead++;
      this->readPtr++;
    }
    if (*this->readPtr == openQuote)
    {
      this->nbCharRead++;
      this->readPtr++;
    }
  }
  else
  {
    Error_new(ERROR_NORMAL, "XmlReader: expected quoted attribute value\n");
    return 0;
  }

  return 1;
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

  return 1;
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

  return 1;
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
  return 1;
}
