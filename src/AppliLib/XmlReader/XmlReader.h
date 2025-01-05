#ifndef _XMLREADER_H_
#define _XMLREADER_H_
#include "XmlElement.h"
#include "String2.h"

typedef enum
{
  XMLNONE,
  XMLCOMMENT,
  XMLVERSION,
  XMLELEMENT,
  XMLENDELEMENT,
  XMLATTRIBUTE,
  XMLTEXT
} XmlNode;

typedef struct XmlReader XmlReader;

PUBLIC XmlReader* XmlReader_new(String* s);
PUBLIC void XmlReader_delete(XmlReader* this);
PUBLIC XmlReader* XmlReader_copy(XmlReader* this);
PUBLIC int XmlReader_compare(XmlReader* this, XmlReader* compared);
PUBLIC void XmlReader_print(XmlReader* this);
PUBLIC unsigned int XmlReader_getSize(XmlReader* this);
PUBLIC XmlNode XmlReader_read(XmlReader* this);
PUBLIC String* XmlReader_getContent(XmlReader* this);

#endif /* _XMLREADER_H_ */
