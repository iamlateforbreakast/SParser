/* HTTPServer.c */

#include "HTTPServer.h"
#include "Object.h"
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/ip.h>
#else
#include <winsock2.h>
#endif
/**********************************************//**
  @class HTTPServer
**************************************************/
struct HTTPServer
{
  Object object;
  int port;
  struct sockaddr_in server_addr;
  int fd;
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
PUBLIC HTTPServer * HTTPServer_new()
{
  HTTPServer * this = 0;

  this = (HTTPServer*)Object_new(sizeof(HTTPServer),&httpServerClass);

  if (this==0) return 0;

  if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket failed");
        exit(1);
  }
  // config socket
  this->server_addr.sin_family = AF_INET;
  this->server_addr.sin_addr.s_addr = INADDR_ANY;
  this->server_addr.sin_port = htons(this->port);

  // bind socket to port
  if (bind(this->fd, 
            (struct sockaddr *)&this->server_addr, 
            sizeof(this->server_addr)) < 0)
  {
    printf("bind failed");
    exit(1);
  }
  
  return 0;
}

PUBLIC void HTTPServer_delete(HTTPServer * this)
{

}

PUBLIC HTTPServer * HTTPServer_copy(HTTPServer * this)
{
  return 0;
}

PUBLIC int HTTPServer_compare(HTTPServer * this, HTTPServer * compared)
{
  return 0;
}

PUBLIC void HTTPServer_print(HTTPServer * this)
{

}

PUBLIC unsigned int HTTPServer_getSize(HTTPServer * this)
{
  return sizeof(HTTPServer);
} 

PUBLIC void HTTPServer_start(HTTPServer * this)
{
// listen for connections
  if (listen(this->fd, 10) < 0) {
        printf("listen failed");
        exit(1);
  }

  //Task taskHandler = 

  //Task_start(Task);


}

PUBLIC void HTTPServer_handleConnection()
{

}

/*

#define MSG_SIZE 1024
#define REPLY_SIZE 65536

int main(int argc, char *argv[])
{
    int s = -1;
    struct sockaddr_in server;
    char message[MSG_SIZE] = {0}, server_reply[REPLY_SIZE] = {0};
    int recv_size = 0;

#if defined(_WIN32) || defined(_WIN64)    
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0) {
        printf("\nError: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
        exit(1);
    }
#endif
    
    //Create a socket
    if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)    {
        printf("Error: Could not create socket: %s. Exiting..\n", strerror(errno));
        exit(1);
    }

    // Fill in server's address
    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = inet_addr("172.217.160.238"); // google.com
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    // Connect to server
    if (connect(s, (struct sockaddr *)(&server), sizeof(server)) < 0) {
        printf("Error: Could not connect to server: %s. Exiting..\n", strerror(errno));
        exit(1);
    }
    
    // Send HTTP request
    strcpy(message, "GET / HTTP/1.1\r\n\r\n");
    if(send(s, message, strlen(message), 0) < 0) {
        printf("Error: Could not send http request to server: %s. Exiting..\n", strerror(errno));
        exit(1);
    }
    
    // Receive a reply from the server
    printf("\nWaiting for server reply..\n");
    if((recv_size = recv(s, server_reply, REPLY_SIZE, 0)) < 0) {
        printf("Error: Something wrong happened while getting reply from server: %s. Exiting..\n", strerror(errno));
        exit(1);
    }

    server_reply[REPLY_SIZE - 1] = 0;

    printf("\nServer Reply:\n\n");
    printf("%s\n", server_reply);

    // Close the socket
#if defined(_WIN32) || defined(_WIN64)  
    closesocket(s);
    WSACleanup();
#else
    close(s);
#endif

    exit(0);
} // end of main */
