/**********************************************//**
  @file HTTPServer.c

  @brief A HTTP Server class.
  This class provides server function to create, start
  HTML pages.
**************************************************/

#include "HTTPServer.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "TaskMgr.h"
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
int msleep(long msec) {
  HANDLE timer;	/* Timer handle */
  LARGE_INTEGER li;	/* Time definition */
  if (!(timer = CreateWaitableTimer(NULL, 1, NULL)))
    return 0;
  li.QuadPart = -10000LL * msec;
  if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, 0)) {
    CloseHandle(timer);
    return 0;
  }
  WaitForSingleObject(timer, 0xFFFFFFFF);
  CloseHandle(timer);
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
PRIVATE void* HTTPServer_listenTaskBody(void* params);
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
struct ConnectionParam
{
  int* client_fd;
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
    PRINT(("setsockopt failed\n"));
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

  // listen for connections
  if (listen(this->fd, 10) < 0) {
    PRINT(("listen failed\n"));
    exit(1);
  }

    // client info
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    int *client_fd = malloc(sizeof(int));
    char *requestBuffer = (char*)malloc(REQUEST_BUFFER_SIZE);

    Memory_set(requestBuffer, 0, REQUEST_BUFFER_SIZE);
    // accept client connection
    if ((client_fd) && ((*client_fd = accept(this->fd, 
                           (struct sockaddr *)&client_addr, 
                            &client_addr_len)) < 0)) {
            PRINT(("accept failed"));
          exit(1);
        }
    PRINT(("Received connection\n"));

    int msg_len = 0;
  TaskMgr* taskMgr = TaskMgr_getRef();
  struct ConnectionParam params[1];
  params[0].client_fd = client_fd;
  Task * connectionListen = Task_create(&HTTPServer_listenTaskBody, 1, (void**)&params);
  Task_start(connectionListen);
  int t = 0;
  while (t<50)
  {
    msleep(100);
    t++;
  }

  TaskMgr_stop(taskMgr);
  Task_destroy(connectionListen);
  TaskMgr_delete(taskMgr);
  free(requestBuffer);
  free(client_fd);
#ifndef WIN32
  close(this->fd);
#else
  closesocket(this->fd);
#endif
}
PRIVATE void* HTTPServer_listenTaskBody(void* params)
{
  int msg_len = 0;
  int* client_fd = ((struct ConnectionParam*)params)->client_fd;
  char* requestBuffer = (char*)malloc(REQUEST_BUFFER_SIZE);
  PRINT(("Listen body starts\n Client FD:%d\n", client_fd));
    if ((client_fd) && (requestBuffer))
    {
      /* request = HTTPRequest_read(); */
      msg_len = recv(*client_fd, &requestBuffer[0], REQUEST_BUFFER_SIZE - 1, 0);
    //PRINT(("Bytes Received: %d\n%s\n", msg_len, requestBuffer));

    HTTPRequest* request = HTTPRequest_new(requestBuffer);
    HTTPRequest_print(request);
    HTTPRequest_delete(request);

    HTTPResponse* response = HTTPResponse_new();
    HTTPResponse_setVersion(response, 1, 1);
    HTTPResponse_setStatusCode(response, 200);
    HTTPResponse_setReason(response, REASON_OK);
    HTTPResponse_addHeader(response, "Content - Type", "text/html; charset=UTF-8");
    char response_body[] = "<doctype !html><html><head><title>A Message: Hello World</title></head>"
      "<body><h1>Hello world!</h1></body></html>\r\n";
    HTTPResponse_setBody(response, response_body);

    char* responseBuffer = (char*)malloc(RESPONSE_BUFFER_SIZE);
    Memory_set(responseBuffer, 0, RESPONSE_BUFFER_SIZE);
    int nbCharToWrite = HTTPResponse_generate(response, responseBuffer, RESPONSE_BUFFER_SIZE - 1);
    if ((client_fd) && (responseBuffer))
    {
       msg_len = send(*client_fd, responseBuffer, nbCharToWrite, 0);
      if (msg_len == 0) {
        PRINT(("Client closed connection\n"));
      }
    }
    free(responseBuffer);
    HTTPResponse_delete(response);
  }
  return 0;
}
#if 0
if ((client_fd) && (requestBuffer))
{
  msg_len = recv(*client_fd, &requestBuffer[0], REQUEST_BUFFER_SIZE - 1, 0);
  HTTPRequest* request = HTTPRequest_new(requestBuffer);
  HTTPRequest_print(request);
  HTTPRequest_delete(request);
}
/* HTTPResponse_send(*client_fd); */
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
#ifndef WIN32
    close(this->fd);
#else
    closesocket(this->fd);
#endif
#endif
