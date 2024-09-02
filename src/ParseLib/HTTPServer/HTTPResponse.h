/* HTTPResponse.h */
#ifndef _HTTPRESPONSE_H_
#define _HTTPRESPONSE_H_

struct HTTPResponse HTTPResponse;

PUBLIC HTTPResponse * HTTPResponse_new();
PUBLIC void HTTPResponse_delete(HTTPResponse * this);

#endif /* _HTTPRESPONSE_H_ */
