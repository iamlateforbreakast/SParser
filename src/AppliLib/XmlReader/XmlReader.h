#ifndef _XMLREADER_H_
#define _XMLREADER_H_
#include "XmlElement.h"
#include "String2.h"

typedef struct XmlReader XmlReader;

XmlReader* XmlReader_new(String* s);
void XmlReader_delete(XmlReader* this);
XmlElement* XmlReader_read(XmlReader* this);

#endif /* _XMLREADER_H_ */
