/* HTTPResponse.h */
#ifndef _HTTPRESPONSE_H_
#define _HTTPRESPONSE_H_

#include "Object.h"
#include "Types.h"
#include "Class.h"
#include "String2.h"
#include "Map.h"
#include <stdio.h>

typedef struct HTTPResponse HTTPResponse;

enum Reason
{
  REASON_OK,
  REASON_INVALID
};

PRIVATE HTTPResponse * HTTPResponse_new();
PRIVATE void HTTPResponse_delete(HTTPResponse * this);
PRIVATE HTTPResponse* HTTPResponse_copy(HTTPResponse* this);
PRIVATE int HTTPResponse_compare(HTTPResponse* this, HTTPResponse* compared);
PRIVATE void HTTPResponse_print(HTTPResponse* this);
PRIVATE unsigned int HTTPResponse_getSize(HTTPResponse* this);
PRIVATE void HTTPResponse_setVersion(HTTPResponse* this, int majorVersion, int minorVersion);
PRIVATE void HTTPResponse_setStatusCode(HTTPResponse* this, int code);
PRIVATE void HTTPResponse_setReason(HTTPResponse* this, enum Reason);
PRIVATE void HTTPResponse_setMimeType(HTTPResponse* this, const char* mimeType);
PRIVATE void HTTPResponse_addHeader(HTTPResponse* this, char* key, char* value);
PRIVATE void HTTPResponse_setBody(HTTPResponse* this, char* body);
PRIVATE int HTTPResponse_generate(HTTPResponse* this, char* buffer, int size);

/**********************************************//**
  @class HTTPResponse
**************************************************/
struct HTTPResponse
{
  Object object;
  int statusCode;
  enum Reason reason;
  int majorVersion;
  int minorVersion;
  char* mimeType;
  Map* headers;
  String* body;
  int isValid;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class httpResponseClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&HTTPResponse_delete,
  .f_copy = (Copy_Operator)&HTTPResponse_copy,
  .f_comp = (Comp_Operator)&HTTPResponse_compare,
  .f_print = (Printer)&HTTPResponse_print,
  .f_size = (Sizer)&HTTPResponse_getSize
};

/**********************************************//**
  @brief Create a new instance of the class HTTPResponse.
  @private
  @memberof HTTPServer
  @param[in] none
  @return New instance of class HTTPResponse.
**************************************************/
PRIVATE HTTPResponse* HTTPResponse_new()
{
  HTTPResponse* this = 0;

  this = (HTTPResponse*)Object_new(sizeof(HTTPResponse), &httpResponseClass);

  if (this == 0) return 0;

  this->statusCode = REASON_INVALID;
  this->majorVersion = 0;
  this->minorVersion = 0;
  this->headers = Map_new();
  this->body = 0;
  this->isValid = 0;

  return this;
}

/**********************************************//**
  @brief Delete an instance of the class HTTPResponse.
  @private
  @memberof HTTPServer
**************************************************/
PRIVATE void HTTPResponse_delete(HTTPResponse* this)
{
  if (OBJECT_IS_INVALID(this)) return;

  String_delete(this->body);
  Map_delete(this->headers);
  
  Object_deallocate(&this->object);
}

/**********************************************//**
  @brief Copy an instance of the class HTTPResponse.
  @private
  @memberof HTTPServer
  @return Copy of the instance
**************************************************/
PRIVATE HTTPResponse* HTTPResponse_copy(HTTPResponse* this)
{
  return 0;
}

/**********************************************//**
  @brief Compare 2 instances of the class HTTPResponse.
  @public
  @memberof HTTPServer
  @return 0 if different, 1 if equal.
**************************************************/
PRIVATE int HTTPResponse_compare(HTTPResponse* this, HTTPResponse* compared)
{
  return 0;
}

/**********************************************//**
  @brief Print an instance of the class HTTPResponse.
  @public
  @memberof HTTPServer
**************************************************/
PRIVATE void HTTPResponse_print(HTTPResponse* this)
{

}

/**********************************************//**
  @brief Get the size of an HTTPResponse. If parameter is 0
  return the size of the class.
  @public
  @memberof HTTPServer
  @return Size of an instance of HTTPResponse
**************************************************/
PRIVATE unsigned int HTTPResponse_getSize(HTTPResponse* this)
{
  return sizeof(HTTPResponse);
}

/**********************************************//**
  @brief TBD
  @private
  @memberof HTTPServer
  @return TBD
**************************************************/
PRIVATE void HTTPResponse_setReason(HTTPResponse* this, enum Reason reason)
{
  this-> reason = REASON_OK;
}

/**********************************************//**
  @brief TBD
  @private
  @memberof HTTPServer
  @return TBD
**************************************************/
PRIVATE void HTTPResponse_setStatusCode(HTTPResponse* this, int statusCode)
{
  this->statusCode = statusCode;
}

/**********************************************//**
  @brief TBD
  @private
  @memberof HTTPServer
  @return TBD
**************************************************/
PRIVATE void HTTPResponse_setVersion(HTTPResponse* this, int majorVersion, int minorVersion)
{
  this->majorVersion = majorVersion;
  this->minorVersion = minorVersion;
}

/**********************************************//**
  @brief TBD
  @private
  @memberof HTTPServer
  @return TBD
**************************************************/
PRIVATE void HTTPResponse_setMimeType(HTTPResponse* this, const char* mimeType)
{
  this->mimeType = mimeType;
}

/**********************************************//**
  @brief TBD
  @private
  @memberof HTTPServer
  @return TBD
**************************************************/
PRIVATE void HTTPResponse_addHeader(HTTPResponse* this, char* key, char* value)
{

}

/**********************************************//**
  @brief TBD
  @private
  @memberof HTTPServer
  @return TBD
**************************************************/
PRIVATE void HTTPResponse_setBody(HTTPResponse* this, char* body)
{
   this->body = String_newByRef(body);
}

/**********************************************//**
  @brief TBD
  @private
  @memberof HTTPServer
  @return TBD
**************************************************/
PRIVATE int HTTPResponse_generate(HTTPResponse* this, char * buffer, int size)
{
#ifndef WIN32
  int nbCharToWrite = snprintf(buffer, size,"HTTP/%d.%d %d OK\r\nContent-Type: %s; charset=UTF-8\r\n\r\n%s", 
                        this->majorVersion, this->minorVersion, this->statusCode, this->mimeType, String_getBuffer(this->body));
#else
   int nbCharToWrite = sprintf_s(buffer, size,"HTTP/%d.%d %d OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n%s\r\n\r\n", this->majorVersion, this->minorVersion, this->statusCode, String_getBuffer(this->body));
#endif
  return nbCharToWrite;
}
#endif /* _HTTPRESPONSE_H_ */
