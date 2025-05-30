/* HTTPRequest.h */
#ifndef _HTTPREQUEST_H_
#define _HTTPREQUEST_H_

#include "Object.h"
#include "Types.h"
#include "Class.h"
#include "String2.h"
#include "Map.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct HTTPRequest HTTPRequest;

PRIVATE HTTPRequest * HTTPRequest_new(char * buffer);
PRIVATE void HTTPRequest_delete(HTTPRequest * this);
PRIVATE HTTPRequest * HTTPRequest_copy(HTTPRequest * this);
PRIVATE int HTTPRequest_compare(HTTPRequest* this, HTTPRequest* compared);
PRIVATE void HTTPRequest_print(HTTPRequest * this);
PRIVATE unsigned int HTTPRequest_getSize(HTTPRequest* this);
PRIVATE String* HTTPRequest_getPath(HTTPRequest* this);
PRIVATE enum Method HTTPRequest_getMethod(HTTPRequest* this);
PRIVATE int HTTPRequest_isValid(HTTPRequest* this);
PRIVATE int HTTPRequest_parseBuffer(HTTPRequest* this, char* buffer);

enum Method 
{
  METHOD_GET=0,
  METHOD_POST,
  METHOD_PUT,
  METHOD_PATCH,
  METHOD_DELETE,
  METHOD_INVALID
};

static char* methods_text[] = { "GET", "POST", "PUT", "PATCH", "DELETE" };
/**********************************************//**
  @class HTTPRequest
**************************************************/
struct HTTPRequest
{
  Object object;
  enum Method method;
  String * path;
  int majorVersion;
  int minorVersion;
  Map* headers;
  String * body;
  int isValid;
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
  @private
  @memberof HTTPServer
  @param[in] none
  @return New instance of class HTTPRequest.
**************************************************/
PRIVATE HTTPRequest * HTTPRequest_new(char * buffer)
{
  HTTPRequest* this = 0;

  this = (HTTPRequest*)Object_new(sizeof(HTTPRequest), &httpRequestClass);

  if (OBJECT_IS_INVALID(this)) return 0;

  this->method = METHOD_INVALID;
  this->path = 0;
  this->majorVersion = 0;
  this->minorVersion = 0;
  this->headers = Map_new();
  this->body = 0;
  this->isValid = HTTPRequest_parseBuffer(this, buffer);

  return this;
}

/**********************************************//**
  @brief Delete an instance of the class HTTPRequest.
  @private
  @memberof HTTPRequest
**************************************************/
PRIVATE void HTTPRequest_delete(HTTPRequest * this)
{
  if (OBJECT_IS_INVALID(this)) return;

  String_delete(this->path);
  Map_delete(this->headers);

  Object_deallocate(&this->object);
}

/**********************************************//**
  @brief Copy an instance of the class HTTPRequest.
  @private
  @memberof HTTPRequest
  @return Copy of the instance
**************************************************/
PRIVATE HTTPRequest * HTTPRequest_copy(HTTPRequest * this)
{
  if (OBJECT_IS_INVALID(this)) return 0;

  return 0;
}

/**********************************************//**
  @brief Compare 2 instances of the class HTTPRequest.
  @public
  @memberof HTTPRequest
  @return 0 if different, 1 if equal.
**************************************************/
PRIVATE int HTTPRequest_compare(HTTPRequest * this, HTTPRequest * compared)
{
  return 0;
}

/**********************************************//**
  @brief Print an instance of the class HTTPRequest.
  @public
  @memberof HTTPRequest
**************************************************/
PRIVATE void HTTPRequest_print(HTTPRequest * this)
{
  if (this->method == METHOD_INVALID) PRINT(("Method: INVALID\n"));
  if (this->method == METHOD_GET) PRINT(("Method: GET\n"));
  if (this->method == METHOD_POST) PRINT(("Method: POST\n"));
  //PRINT(("Path: %s\n", String_getBuffer(this->path)));
  //PRINT(("Version: %d.%d\n", this->majorVersion, this->minorVersion));
  //PRINT(("Host: %s\n", host));
  //PRINT(("User-Agent: %s\n", userAgent)); 
}

/**********************************************//**
  @brief Get the size of an HTTPRequest. If parameter is 0
  return the size of the class.
  @public
  @memberof HTTPRequest
  @return Number of items.
**************************************************/
PRIVATE unsigned int HTTPRequest_getSize(HTTPRequest * this)
{
  return sizeof(HTTPRequest);
}

PRIVATE String * HTTPRequest_getPath(HTTPRequest* this)
{
  return this->path;
}

PRIVATE enum Method HTTPRequest_getMethod(HTTPRequest* this)
{
  return this->method;
}

PRIVATE int HTTPRequest_isValid(HTTPRequest* this)
{
  return this->isValid;
}

PRIVATE int HTTPRequest_parseBuffer(HTTPRequest* this, char* buffer)
{
  int isValid = 0;
  char * path_start = buffer;
  int path_length = 0;

  /* Read method*/
  for (enum Method i = METHOD_GET; i < METHOD_INVALID; i++)
  {
    if (Memory_ncmp(buffer, methods_text[i], Memory_len(methods_text[i]) - 1))
    {
      this->method = i;
      path_start = buffer + Memory_len(methods_text[i]) + 1;
      isValid = 1;
      break;
    }
  }

  /* Read path */
  while ((path_length < (int)Memory_len(buffer)) && (*(path_start + path_length) != ' '))
  {
    path_length++;
  }
  
  char* path_buffer = Memory_alloc(path_length + 1);
  Memory_copy(path_buffer, path_start, path_length + 1);
  path_buffer[path_length + 1] = 0;

  this->path = String_newByRef(path_buffer);
  char * version_start = path_start + path_length + 1;

  /* Read version */
  if (Memory_ncmp(version_start, "HTTP/1.1", Memory_len("HTTP/1.1") - 1))
  {
    this->majorVersion = 1;
    this->minorVersion = 1;
    isValid = isValid && 1;
  }

  return isValid;
}
#endif /* _HTTPREQUEST_H_ */
