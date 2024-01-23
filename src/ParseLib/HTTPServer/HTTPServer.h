/* HTTPServer.h */
#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_

#include "Types.h"

typedef struct HTTPServer HTTPServer;

PUBLIC HTTPServer * HTTPServer_new();
PUBLIC void HTTPServer_delete(HTTPServer * this);

#endif /* _HTTPSERVER_H_ */