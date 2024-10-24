/* Socket.h */
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

typedef struct Socket Socket;

struct Socket
{
#ifndef WIN32
  int fd;
#else
  WSADATA wsa;
  SOCKET fd;
#endif
};

PRIVATE void Socket_init()
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
PRIVATE int Socket_create(Socket * this)
{
  this->fd = socket(AF_INET, SOCK_STREAM, 0);
  if ((this->fd) < 0)
  {
    PRINT(("Socket creation failed\n"));
    exit(1);
  }

  #ifndef WIN32
  if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))<0) {
#else
  if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, (char*)&sockOptions, sizeof(int)) < 0) {
#endif

  return 1;
}

/**********************************************//**
  @brief Bind the socket.
  @private
  @memberof HTTPServer
  @param[in] none
  @return New instance of class Socket.
**************************************************/
PRIVATE void Socket_bind(Socket * this, struct sockaddr * addr)
{
  if (bind(this->fd, addr, sizeof(this->server_addr)) < 0)
  {
    PRINT(("bind failed"));
    exit(1);
  }
}

/**********************************************//**
  @brief Listen to the socket.
  @private
  @memberof HTTPServer
  @param[in] none
**************************************************/
PRIVATE int Socket_listen(Socket * this)
{
  int result = listen(this->fd, 10);

  if ( result < 0) {
    PRINT(("listen failed\n"));
    exit(1);
  }

  return result;
}

/**********************************************//**
  @brief Bind the socket.
  @private
  @memberof HTTPServer
  @param[in] none
  @return New instance of class Socket.
**************************************************/
PRIVATE void Socket_accept(Socket * this, struct sockaddr * addr)
{

}

/**********************************************//**
  @brief Bind the socket.
  @private
  @memberof HTTPServer
  @param[in] none
  @return New instance of class Socket.
**************************************************/
PRIVATE int Socket_readFrom(Socket * this)
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
PRIVATE void Socket_writeTo(Socket * this)
{
  int nbBytesWritten = send(*client_fd, responseBuffer, nbCharToWrite, 0);
}

/**********************************************//**
  @brief Close the socket.
  @private
  @memberof HTTPServer
  @param[in] none
**************************************************/
PRIVATE void Socket_close(Socket * this)
{
#ifndef WIN32
  close(this->fd);
#else
  closesocket(this->fd);
  WSACleanup();
#endif
}

#endif /* _HTTPSOCKET_H_ */