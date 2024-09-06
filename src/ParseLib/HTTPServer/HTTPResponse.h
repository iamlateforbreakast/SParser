/* HTTPResponse.h */
#ifndef _HTTPRESPONSE_H_
#define _HTTPRESPONSE_H_

#include "Object.h"
#include "Types.h"
#include "Class.h"
#include "String2.h"
#include "Map.h"

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
PRIVATE void HTTPResponse_addHeader(HTTPResponse* this, char* key, char* value);
PRIVATE void HTTPResponse_setBody(HTTPResponse* this, char* body);

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

  return this;
}

/**********************************************//**
  @brief Delete an instance of the class HTTPResponse.
  @private
  @memberof HTTPServer
**************************************************/
PRIVATE void HTTPResponse_delete(HTTPResponse* this)
{

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
  @return Number of items.
**************************************************/
PRIVATE unsigned int HTTPResponse_getSize(HTTPResponse* this)
{
  return sizeof(HTTPResponse);
}
#endif /* _HTTPRESPONSE_H_ */
