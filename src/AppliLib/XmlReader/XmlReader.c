/* XmlReader.c */
#include "XmlReader.h"
#include "Object.h"
#include "Memory.h"

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

  XmlNode node;
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
  
  this->buffer = String_getBuffer(string);
  this->readPtr = this->buffer;
  this->length = String_getLength(string);
  this->nbCharRead = 0;
  this->node = XMLNONE;
  this->buffer = (char*)Memory_alloc(BUFFER_SIZE);
  this->bufferUse = 0;

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
    else if (Memory_ncmp(this->readPtr, "<?xml",4))
    {
      XmlReader_consumeVersion(this);
      this->node = XMLVERSION;
    }
    else if(Memory_ncmp(this->readPtr, "</",2))
    {
      XmlReader_consumeEndElement(this);
      this->node = XMLENDELEMENT;
    }
    else if (Memory_ncmp(this->readPtr, "<", 1))
    {
      XmlReader_consumeElement(this);
      this->node = XMLELEMENT;
    }
    else
    {
      this->readPtr++;
      this->nbCharRead++;
      if (*this->readPtr=='<')
      {
        this->node = XMLTEXT;
      }
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
  this->nbCharRead += 4;
  this->readPtr += 4;

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
  XmlReader_consumeName(this);
  if (*this->readPtr=='>')
  {
    this->nbCharRead++;
    this->readPtr++;
  }

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
  
  while (this->nbCharRead<this->length)
  {
    if (IS_ELEMENT_LETTER(*this->readPtr))
    {
      this->buffer[this->bufferUse] = *this->readPtr;
      this->bufferUse++;
      XmlReader_consumeOneChar(this);
    }
    else
    {
      if (*this->readPtr=='>')
      {
        this->nbCharRead++;
        this->readPtr++;
      }
      this->buffer[this->bufferUse] = 0;
      this->bufferUse = 0;
      
      return 1;
    }
  }
  return 0;
}

PUBLIC int XmlReader_consumeAttribute(XmlReader* this)
{
  XmlReader_consumeSpace(this);
  
  XmlReader_consumeName(this);

  if (*this->readPtr!='=') { /*error*/}

  XmlReader_consumeString(this);

  if (*this->readPtr=='>')
  {
    this->nbCharRead++;
    this->readPtr++;
  }

  return 0;
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

PRIVATE int XmlReader_consumeString(XmlReader* this)
{
  if ((*this->readPtr=='"') || (*this->readPtr=='\''))
  {
    this->nbCharRead++;
    this->readPtr++;
  }
  else
  {
    /* error */
  }
  while (this->nbCharRead<this->length)
  {
    if ((*this->readPtr=='"') || (*this->readPtr=='\''))
    {
      this->nbCharRead++;
      this->readPtr++;
      break;
    }
    else
    {
      this->nbCharRead++;
      this->readPtr++;
    }
  }

  return 1;
}

PRIVATE int XmlReader_consumeOneChar(XmlReader* this)
{
  this->nbCharRead++;
  this->readPtr++;
  if (*this->readPtr=='\n')
  {
    this->col = 0;
    this->line++;
  }
  else
  {
    this->col++;
  }
  
  return 1;
}
