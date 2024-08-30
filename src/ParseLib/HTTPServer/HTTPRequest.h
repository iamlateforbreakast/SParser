/**********************************************//**
  @file HTTPServer.c
  
  @brief A HTTP Server class.
    This class provides server function to create, start
	  HTML pages.
**************************************************/

#include "Object.h"
#include "Types.h"
#include "Class.h"
#include "String2.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct HTTPRequest HTTPRequest;

PRIVATE HTTPRequest * HTTPRequest_new(String * s);
PRIVATE void HTTPRequest_delete(HTTPRequest * this);
PRIVATE HTTPRequest * HTTPRequest_copy(HTTPRequest * this);
PRIVATE int HTTPRequest_compare(HTTPRequest* this, HTTPRequest* compared);
PRIVATE void HTTPRequest_print(HTTPRequest * this);
PRIVATE unsigned int HTTPRequest_getSize(HTTPRequest* this);

enum Method 
{
  METHOD_GET,
  METHOD_POST
};

/**********************************************//**
  @class HTTPRequest
**************************************************/
struct HTTPRequest
{
  Object object;
  enum Method method;
  String * path;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class httpRequestClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&HTTPRequest_delete,
  .f_copy = (Copy_Operator)&HTTPRequest_copy,
  .f_comp = (Comp_Operator)&HTTPRequest_compare,
  .f_print = (Printer)&HTTPRequest_print,
  .f_size = (Sizer)&HTTPRequest_getSize
};

/**********************************************//**
  @brief Create a new instance of the class HTTPRequest.
  @public
  @memberof HTTPServer
  @param[in] none
  @return New instance of class HTTPRequest.
**************************************************/
PRIVATE HTTPRequest * HTTPRequest_new(String* s)
{
  HTTPRequest* this = 0;

  this = (HTTPRequest*)Object_new(sizeof(HTTPRequest), &httpRequestClass);

  if (this == 0) return 0;

  return this;
}

/**********************************************//**
  @brief Delete an instance of the class HTTPRequest.
  @public
  @memberof HTTPRequest
**************************************************/
PRIVATE void HTTPRequest_delete(HTTPRequest * this)
{
}

/**********************************************//**
  @brief Copy an instance of the class HTTPRequest.
  @public
  @memberof HTTPRequest
  @return Copy of the instance
**************************************************/
PRIVATE HTTPRequest * HTTPRequest_copy(HTTPRequest * this)
{
  return 0;
}

PRIVATE int HTTPRequest_compare(HTTPRequest * this, HTTPRequest * compared)
{
  return 0;
}

PRIVATE void HTTPRequest_print(HTTPRequest * this)
{

}

PRIVATE unsigned int HTTPRequest_getSize(HTTPRequest * this)
{
  return sizeof(HTTPRequest);
}
