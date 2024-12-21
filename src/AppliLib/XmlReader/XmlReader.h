#ifndef _XMLREADER_H_
#define _XMLREADER_H_

typedef struct XmlReader XmlReader;

XmlReader* XmlReader_new();
void XmlReader_delete(XmlReader* this);

#endif /* _XMLREADER_H_ */
