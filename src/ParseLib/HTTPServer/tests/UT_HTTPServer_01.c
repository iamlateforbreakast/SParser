/* HTTPServer.h */
#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

#include "Types.h"

typedef struct HTTPServer HTTPServer;

PUBLIC HTTPServer * HTTPServer_new();
PUBLIC void HTTPServer_delete(HTTPServer * this);
PUBLIC HTTPServer* HTTPServer_copy(HTTPServer* this);
PUBLIC int HTTPServer_compare(HTTPServer* this, HTTPServer* compared);
PUBLIC void HTTPServer_print(HTTPServer* this);
PUBLIC unsigned int HTTPServer_getSize(HTTPServer* this);
//PUBLIC void HTTPServer_start(HTTPServer* this);
#endif /* _HTTPSERVER_H_ */
