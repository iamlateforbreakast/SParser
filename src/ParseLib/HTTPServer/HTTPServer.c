/**********************************************//**
  @file HTTPServer.c
  
  @brief A HTTP Server class.
  
  This class provides server function to serve
  files.
**************************************************/

#include "HTTPServer.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "TaskMgr.h"
#include "FileMgr.h"
#include "Task.h"
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

#ifdef _WIN32
#include <windows.h>	/* WinAPI */

/* Windows sleep in 100ns units */
int msleep(long msec) {
  /* Declarations */
  HANDLE timer;	/* Timer handle */
  LARGE_INTEGER li;	/* Time definition */
  /* Create timer */
  if (!(timer = CreateWaitableTimer(NULL, 1, NULL)))
    return 0;
  /* Set timer properties */
  li.QuadPart = -10000LL * msec;
  if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, 0)) {
    CloseHandle(timer);
    return 0;
  }
  /* Start & wait for timer */
  WaitForSingleObject(timer, 0xFFFFFFFF);
  /* Clean resources */
  CloseHandle(timer);
  /* Slept without problems */
  return 1;
}
#else
#include <pthread.h>
#include <time.h>
#include <errno.h>
int msleep(long msec)
{
  struct timespec ts;
  int res;

  if (msec < 0)
  {
    errno = EINVAL;
    return -1;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);

  return res;
}
#endif

#define REQUEST_BUFFER_SIZE (4096)
#define RESPONSE_BUFFER_SIZE (4096)
#define MAX_CONNECTIONS (4)
#define PORT (8080)

PRIVATE void* HTTPServer_listenTaskBody(void* params);
PRIVATE HTTPResponse* HTTPServer_serveRequest(HTTPRequest* request);

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
  Task* connections[MAX_CONNECTIONS];
};

struct ConnectionParam
{
#ifndef WIN32
  int* client_fd;
#else
  SOCKET client_fd;
#endif
};

struct RouteParam
{
  char* pattern;
  
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

  if (OBJECT_IS_INVALID(this)) return 0;

#ifdef WIN32   
  if (WSAStartup(MAKEWORD(2, 2), &this->wsa) != 0) {
    PRINT(("\nError: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError()));
    exit(1);
  }
#endif

  this->port = PORT;
  if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    PRINT(("Socket failed.\n"));
    HTTPServer_delete(this);
    exit(1);
  }
#ifndef WIN32
  if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, (void*)&sockOptions, sizeof(sockOptions)) < 0) {
#else
  if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, (char*)&sockOptions, sizeof(sockOptions)) < 0) {
#endif
    PRINT(("Setsockopt failed.\n"));
    HTTPServer_delete(this);
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
    PRINT(("Bind failed."));
    HTTPServer_delete(this);
    exit(1);
  }
  return this;
}

/**********************************************//**
  @brief Delete an instance of the class HTTPServer.
  @public
  @memberof HTTPServer
  @param[in] none
**************************************************/
PUBLIC void HTTPServer_delete(HTTPServer* this)
{
  if (OBJECT_IS_INVALID(this)) return;

#ifndef WIN32
  close(this->fd);
#else
  closesocket(this->fd);
  WSACleanup();
#endif
  Object_deallocate(&this->object);
}

/**********************************************//**
  @brief Copy an instance of the class HTTPServer.
  @private
  @memberof HTTPServer
  @return Copy of the instance
**************************************************/
PUBLIC HTTPServer* HTTPServer_copy(HTTPServer* this)
{
  return 0;
}

PUBLIC int HTTPServer_compare(HTTPServer* this, HTTPServer* compared)
{
  return 0;
}

/**********************************************//**
  @brief Print an instance of the class HTTPResponse.
  @public
  @memberof HTTPServer
**************************************************/
PUBLIC void HTTPServer_print(HTTPServer* this)
{

}

/**********************************************//**
  @brief Get the size of an HTTPServer. If parameter is 0
  return the size of the class.
  @public
  @memberof HTTPServer
  @return Number of items.
**************************************************/
PUBLIC unsigned int HTTPServer_getSize(HTTPServer* this)
{
  return sizeof(HTTPServer);
}

/**********************************************//**
  @brief Starts an instance of an HTTPServer.
  @public
  @memberof HTTPServer
**************************************************/
PUBLIC void HTTPServer_start(HTTPServer* this)
{
  // listen for connections
  if (listen(this->fd, 10) < 0) {
    PRINT(("Listen failed.\n"));
    HTTPServer_delete(this);
    exit(1);
  }

  struct sockaddr_in client_addr;
  int client_addr_len = sizeof(client_addr);
  int *client_fd = malloc(sizeof(int) * MAX_CONNECTIONS);

  TaskMgr* taskMgr = TaskMgr_getRef();
  int nbRequests = 0;
  while (nbRequests<MAX_CONNECTIONS)
  {
    if ((*client_fd = accept(this->fd,
                           (struct sockaddr *)&client_addr, 
                           &client_addr_len)) < 0) 
    {
      PRINT(("Accept failed.\n"));
      exit(1);
    }
    PRINT(("Received connection request.\n"));

    
    void * params[5];
    params[0] = &client_fd;
    Task* connectionListen = Task_create(&HTTPServer_listenTaskBody, 1, params);

    Task_start(connectionListen);
    nbRequests++;
  }

  int timer = 0;
  while (timer<50)
  {
    msleep(100);
    timer++;
  }

  TaskMgr_stop(taskMgr);
  //Task_destroy(connectionListen);
  TaskMgr_delete(taskMgr);

  //free(client_fd);
#ifndef WIN32
  close(this->fd);
#else
  closesocket(this->fd);
#endif
}

/**********************************************//**
  @brief Starts thread listening to a socket and 
  answering a HTTP request.
  @public
  @memberof HTTPServer
**************************************************/
PRIVATE void* HTTPServer_listenTaskBody(void* params)
{ 
  //int msg_len = 0;
#ifndef WIN32   
  int* client_fd = ((struct ConnectionParam**)params)[0]->client_fd;
#else
  SOCKET client_fd = ((struct ConnectionParam**)params)[0]->client_fd;
#endif
  char* requestBuffer = (char*)malloc(REQUEST_BUFFER_SIZE);
  Memory_set(requestBuffer, 0, REQUEST_BUFFER_SIZE);

  PRINT(("Listen body starts\n Client FD:%d\n", *client_fd));
  /* request = Connection_waitForHttpRequest*/
  if ((client_fd) && (requestBuffer))
  {
    int nbCharRead = 0;
    do 
    {
      int msg_len = recv(*client_fd, &requestBuffer[nbCharRead], REQUEST_BUFFER_SIZE - 1, 0);
      if (msg_len>0)
      {
        nbCharRead += msg_len;
        HTTPRequest* request = HTTPRequest_new(requestBuffer);
        HTTPResponse* response = HTTPServer_serveRequest(request);
        HTTPRequest_delete(request);
        char* responseBuffer = (char*)malloc(RESPONSE_BUFFER_SIZE);
        Memory_set(responseBuffer, 0, RESPONSE_BUFFER_SIZE);
        int nbCharToWrite = HTTPResponse_generate(response, responseBuffer, RESPONSE_BUFFER_SIZE - 1);
        if ((client_fd) && (responseBuffer))
        {
          int msg_len = send(*client_fd, responseBuffer, nbCharToWrite, 0);
          if (msg_len == 0) 
          {
            PRINT(("Client closed connection\n"));
          }
          free(responseBuffer);
          HTTPResponse_delete(response);
          PRINT(("Response completed.\n"));
          break;
        }
      }
      else if (msg_len == 0)
      {
        PRINT(("Connection closed.\n"));
        break;
      }
      else
      {  
        PRINT(("Error reading socket.\n"));
        return 0;
      }
    } while (1);
    #ifndef WIN32
    close(*client_fd);
    #else
    closesocket(client_fd);
    #endif
  }
  return 0;
}

PRIVATE HTTPResponse* HTTPServer_serveRequest(HTTPRequest* request)
{ 
  HTTPResponse* response = HTTPResponse_new();

  HTTPResponse_setVersion(response, 1, 1);
  HTTPResponse_setStatusCode(response, 200);
  HTTPResponse_setReason(response, REASON_OK);

  FileMgr* fm = FileMgr_new();

  PRINT(("Requested path %s\n", String_getBuffer(HTTPRequest_getPath(request))));

  if (String_matchWildcard(HTTPRequest_getPath(request), "/index.html"))
  {
    PRINT(("Load index.html\n"));
    FileDesc* fd = FileMgr_addFile(fm, "index.html");
    if (fd)
    {
      String* content = content = FileMgr_load(fm, "index.html");
      HTTPResponse_setMimeType(response, "text/html");
      HTTPResponse_setBody(response, String_getBuffer(content));
    }
    else
    {
      String* errorMessage = String_newByRef("<doctype !html><html><head><title>Error</title></head>"
        "<body><h1>Error!</h1></body></html>\r\n");
      HTTPResponse_setMimeType(response, "text/html");  
      //HTTPResponse_addHeader(response, "Content - Type", "text/html; charset=UTF-8");
      HTTPResponse_setBody(response, String_getBuffer(errorMessage));
    }
  }
  else if (String_matchWildcard(HTTPRequest_getPath(request), "/hello.css"))
  {
    PRINT(("Loading hello.css: "));
    FileDesc* fd = FileMgr_addFile(fm, "hello.css");
    if (fd)
    {
      String* content = FileMgr_load(fm, "hello.css"); 
      HTTPResponse_setBody(response, String_getBuffer(content));
      HTTPResponse_setMimeType(response, "text/css");
      PRINT(("Done\n"));
    }
    else
    {
      PRINT(("Cannot find hello.css.\n"));
    }
  }
  else if (String_matchWildcard(HTTPRequest_getPath(request), "/hello.js"))
  {
    PRINT(("Loading hello.js: "));
    FileDesc* fd = FileMgr_addFile(fm, "hello.js");
    if (fd)
    {
      String* content = FileMgr_load(fm, "hello.js");
      HTTPResponse_setBody(response, String_getBuffer(content));
      HTTPResponse_setMimeType(response, "text/javascript");
      //HTTPResponse_addHeader(response, "Content - Type", "text/javascript; charset=UTF-8");
      PRINT(("Done\n"));
    }
    else
    {
      PRINT(("Cannot find hello.js.\n"));
      exit(1);
    }
  }
  else if (String_matchWildcard(HTTPRequest_getPath(request), "/favicon.ico"))
  {
    PRINT(("Loading favicon.ico: "));
    FileDesc* fd = FileMgr_addFile(fm, "favicon.ico");
    if (fd)
    {
      String* content = FileMgr_load(fm, "favicon.ico");
      HTTPResponse_setMimeType(response, "text/x-icon");
      //HTTPResponse_addHeader(response, "Content - Type", "text/x-icon; charset=UTF-8");
      HTTPResponse_setBody(response, String_getBuffer(content));
      PRINT(("Done\n"));
    }
    else
    {
      PRINT(("Cannot find favicon.ico.\n"));
      exit(1);
    }
  }
  else
  {
    String* errorMessage = String_newByRef("<doctype !html><html><head><title>Error</title></head>"
      "<body><h1>Error!</h1></body></html>\r\n");
    HTTPResponse_setBody(response, String_getBuffer(errorMessage));
    //String_delete(errorMessage);
  }

  FileMgr_delete(fm);

  //String_delete(content);
  

  return response;
}
