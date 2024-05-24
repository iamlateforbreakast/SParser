/**********************************************//**
  @file HTTPServer.c

  @brief A HTTP Server class.
  This class provides server function to create, start
  HTML pages.
**************************************************/

#include "HTTPServer.h"
#include "Object.h"
#include "Memory.h"
#include "Debug.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef WIN32
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#else
#include <winsock2.h>
#endif

#define REQUEST_BUFFER_SIZE (4096)
#define RESPONSE_BUFFER_SIZE (4096)
/**********************************************//**
  @class HTTPServer
**************************************************/
struct HTTPServer
{
  Object object;
  int port;
  struct sockaddr_in server_addr;
#ifndef WIN32
  int fd;
#else
  WSADATA wsa;
  SOCKET fd;
#endif
};

/**********************************************//**
  @private Class Description
**************************************************/
Class httpServerClass =
{
  .f_new = 0,
  .f_delete = (Destructor)&HTTPServer_delete,
  .f_copy = (Copy_Operator)&HTTPServer_copy,
  .f_comp = (Comp_Operator)&HTTPServer_compare,
  .f_print = (Printer)&HTTPServer_print,
  .f_size = (Sizer)&HTTPServer_getSize
};

/**********************************************//**
  @brief Create a new instance of the class HTTPServer.
  @public
  @memberof HTTPServer
  @param[in] none
  @return New instance of class HHTPServer.
**************************************************/
PUBLIC HTTPServer* HTTPServer_new()
{
  HTTPServer* this = 0;
  int sockOptions = 1;
  this = (HTTPServer*)Object_new(sizeof(HTTPServer), &httpServerClass);

  if (this == 0) return 0;

#ifdef WIN32   
  if (WSAStartup(MAKEWORD(2, 2), &this->wsa) != 0) {
    PRINT(("\nError: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError()));
    exit(1);
  }
#endif

  this->port = 8080;

  if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    PRINT(("socket failed\n"));
    exit(1);
  }
#ifndef WIN32
  if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int))<0) {
#else
  if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, (char*)&sockOptions, sizeof(int)) < 0) {
#endif
    PRINT(("setsocopt failed\n"));
    exit(1);
  }
  // config socket
  this->server_addr.sin_family = AF_INET;
  this->server_addr.sin_addr.s_addr = INADDR_ANY;
  this->server_addr.sin_port = htons(this->port);

  // bind socket to port
  if (bind(this->fd,
    (struct sockaddr*)&this->server_addr,
    sizeof(this->server_addr)) < 0)
  {
    PRINT(("bind failed"));
    exit(1);
  }

  return this;
}

PUBLIC void HTTPServer_delete(HTTPServer* this)
{
#ifndef WIN32
  close(this->fd);
#else
  closesocket(this->fd);
  WSACleanup();
#endif
}

PUBLIC HTTPServer* HTTPServer_copy(HTTPServer* this)
{
  return 0;
}

PUBLIC int HTTPServer_compare(HTTPServer* this, HTTPServer* compared)
{
  return 0;
}

PUBLIC void HTTPServer_print(HTTPServer* this)
{

}

PUBLIC unsigned int HTTPServer_getSize(HTTPServer* this)
{
  return sizeof(HTTPServer);
}

PUBLIC void HTTPServer_start(HTTPServer* this)
{
  char response[] = "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html; charset=UTF-8\r\n\r\n"
      "<doctype !html><html><head><title>Hello World</title></head>"
      "<body><h1>Hello world!</h1></body></html>\r\n";

  // listen for connections
  if (listen(this->fd, 10) < 0) {
    PRINT(("listen failed\n"));
    exit(1);
  }

  while (1) {
    // client info
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    int *client_fd = malloc(sizeof(int));
    char *requestBuffer = (char*)malloc(REQUEST_BUFFER_SIZE);
    char * path = malloc(1024);
    char * method = malloc(10);
    char * version = malloc(10);
    char * host = malloc(256);
    char * userAgent = malloc(256);

    Memory_set(requestBuffer, 0, REQUEST_BUFFER_SIZE);
    // accept client connection
    if ((client_fd) && ((*client_fd = accept(this->fd, 
                           (struct sockaddr *)&client_addr, 
                            &client_addr_len)) < 0)) {
            PRINT(("accept failed"));
            break;
        }
    PRINT(("Received connection\n"));

    int msg_len = 0;

    if ((client_fd) && (requestBuffer))
    {
      msg_len = recv(*client_fd, &requestBuffer[0], REQUEST_BUFFER_SIZE - 1, 0);
    PRINT(("Bytes Received: %d\n%s\n", msg_len, requestBuffer));

      sscanf_s(requestBuffer, "%s %s %s\nHost: %s\nUser-Agent: %s\n",
        method, sizeof(method), path, sizeof(path), version, sizeof(version), host, sizeof(host), userAgent, sizeof(userAgent));
    }
    //sscanf(requestBuffer, "Host: %s\n", host);
    PRINT(("Method: %s\n", method));
    PRINT(("Path: %s\n", path));
    PRINT(("Version: %s\n", version));
    PRINT(("Host: %s\n", host));
    PRINT(("User-Agent: %s\n", userAgent));
    if (client_fd) msg_len = send(*client_fd, response, sizeof(response), 0);
    if (msg_len == 0) {
      PRINT(("Client closed connection\n"));
#ifndef WIN32
      close(this->fd);
#else
      closesocket(this->fd);
#endif
    }

    /*if (msg_len == -1) {
      fprintf(stderr, "recv() failed with error %d\n", -1); //WSAGetLastError());
      //WSACleanup();
      return -1;
    }

      if (msg_len == 0) {
        printf("Client closed connection\n");
        close(this->fd);
        return -1;
      }*/
    free(requestBuffer);
    free(client_fd);
    free(path);
    free(version);
    free(method);
    free(host);
    free(userAgent);
#ifndef WIN32
    close(this->fd);
#else
    closesocket(this->fd);
#endif
    exit(1);
  }
}
