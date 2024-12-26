#ifndef _XMLREADER_H_
#define _XMLREADER_H_
#include "XmlElement.h"
#include "String2.h"

typedef enum
{
  XMLNONE,
  XMLELEMENT
} XmlNode;

typedef struct XmlReader XmlReader;

XmlReader* XmlReader_new(String* s);
void XmlReader_delete(XmlReader* this);
XmlReader* XmlReader_copy(XmlReader* this);
int XmlReader_compare(XmlReader* this, XmlReader* compared);
void XmlReader_print(XmlReader* this);
unsigned int XmlReader_getSize(XmlReader* this);
XmlNode XmlReader_read(XmlReader* this);

#endif /* _XMLREADER_H_ */
