/* HTTPSocket.h */
#ifndef _HTTPSOCKET_H_
#define _HTTPSOCKET_H_

#include "Object.h"
#include "Types.h"
#include "Class.h"

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#else
#include <winsock2.h>
#endif

typedef struct HTTPSocket HTTPSocket;

// Global Var: TODO Need protection
static int HTTPSocket_isInitialised = 0;

PRIVATE HTTPSocket* HTTPSocket_new(struct sockaddr * addr, int port);
PRIVATE void HTTPSocket_delete(HTTPSocket * this);
PRIVATE HTTPSocket* HTTPSocket_copy(HTTPSocket* this);

/**********************************************//**
  @class HTTPSocket
**************************************************/
struct HTTPSocket
{
#ifndef WIN32
  int fd;
#else
  WSADATA wsa;
  SOCKET fd;
#endif
  int port;
  struct sockaddr * addr;
};

/**********************************************//**
  @private Class Description
**************************************************/
PRIVATE Class httpSocketClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&HTTPSocket_delete,
  .f_copy = (Copy_Operator)&HTTPSocket_copy,
  .f_comp = (Comp_Operator)&HTTPSocket_compare,
  .f_print = (Printer)&HTTPSocket_print,
  .f_size = (Sizer)&HTTPSocket_getSize
};

PRIVATE int HTTPSocket_init()
{
#ifdef WIN32   
  if (WSAStartup(MAKEWORD(2, 2), &this->wsa) != 0) {
    PRINT(("\nError: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError()));
    exit(1);
  }
#endif
}

/**********************************************//**
  @brief Create a new socket.
  @private
  @memberof HTTPServer
  @param[in] none
  @return New instance of class Socket.
**************************************************/
PRIVATE HTTPSocket* HTTPSocket_new(struct sockaddr * addr, int port)
{
  HTTPSocket* this = 0;
  int sockOptions = 1;

  if (HTTPSocket_isInitialised == 0)
  {
    if (!HTTPSocket_init()) return 0;
    HTTPSocket_isInitialised = 1;
  }
  this = (HTTPSocket*)Object_new(sizeof(HTTPSocket), &httpSocketClass);

  if (this == 0) return 0;

  this->fd = socket(AF_INET, SOCK_STREAM, 0);
  if ((this->fd) < 0)
  {
    PRINT(("Socket creation failed\n"));
    return 0;
  }

#ifndef WIN32
  if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, (int*)&sockOptions, sizeof(int)) < 0)
  {
    return 0;
  }
#else
  if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, (char*)&sockOptions, sizeof(int)) < 0)
  {
    return 0;
  }
#endif

  if (bind(this->fd, addr, sizeof(this->server_addr)) < 0)
  {
    PRINT(("HTTPSocket: bind failed"));
    return 0;
  }

  return this;
}

/**********************************************//**
  @brief Close the socket.
  @private
  @memberof HTTPServer
  @param[in] none
**************************************************/
PRIVATE void HTTPSocket_delete(HTTPSocket * this)
{
  if (OBJECT_IS_INVALID(this)) return
  
#ifndef WIN32
  close(this->fd);
#else
  closesocket(this->fd);
  WSACleanup();
#endif

  Object_deallocate(&this->object);
}

/**********************************************//**
  @brief Copy an instance of the class HTTPSocket.
  @private
  @memberof HTTPServer
  @return Copy of the instance
**************************************************/
PRIVATE HTTPSocket* HTTPSocket_copy(HTTPSocket* this)
{
  return 0;
}

/**********************************************//**
  @brief Compare 2 instances of the class HTTPSocket.
  @public
  @memberof HTTPServer
  @return 0 if different, 1 if equal.
**************************************************/
PRIVATE int HTTPSocket_compare(HTTPSocket* this, HTTPSocket* compared)
{
  return 0;
}

/**********************************************//**
  @brief Print an instance of the class HTTPSocket.
  @public
  @memberof HTTPServer
**************************************************/
PRIVATE void HTTPSocket_print(HTTPSocket* this)
{

}

/**********************************************//**
  @brief Get the size of an HTTPResponse. If parameter is 0
  return the size of the class.
  @public
  @memberof HTTPServer
  @return Size of an instance of HTTPResponse
**************************************************/
PRIVATE unsigned int HTTPSocket_getSize(HTTPSocket* this)
{
  return sizeof(HTTPSocket);
}

/**********************************************//**
  @brief Listen to the socket.
  @private
  @memberof HTTPServer
  @param[in] none
**************************************************/
PRIVATE int HTTPSocket_listen(Socket * this)
{
  int result = listen(this->fd, 10);

  if ( result < 0) {
    PRINT(("listen failed\n"));
    exit(1);
  }

  return result;
}

/**********************************************//**
  @brief Accept connection to the socket.
  @private
  @memberof HTTPServer
  @param[in] none
  @return New instance of class Socket.
**************************************************/
PRIVATE void HTTPSocket_accept(Socket * this, struct sockaddr * addr)
{

}

/**********************************************//**
  @brief Bind the socket.
  @private
  @memberof HTTPServer
  @param[in] none
  @return New instance of class Socket.
**************************************************/
PRIVATE int HTTPSocket_readFrom(Socket * this)
{
  int nbBytesRead = recv(*client_fd, &requestBuffer[0], REQUEST_BUFFER_SIZE - 1, 0);
}

/**********************************************//**
  @brief Bind the socket.
  @private
  @memberof HTTPServer
  @param[in] none
  @return New instance of class Socket.
**************************************************/
PRIVATE void HTTPSocket_writeTo(Socket * this)
{
  int nbBytesWritten = send(*client_fd, responseBuffer, nbCharToWrite, 0);
}



#endif /* _HTTPSOCKET_H_ */