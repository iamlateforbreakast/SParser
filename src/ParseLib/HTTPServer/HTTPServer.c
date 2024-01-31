/* HTTPServer.c */

#include "HTTPServer.h"
#include "Object.h"
#include "Debug.h"

#include <stdlib.h>

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
#ifndef WIN32
#else
  WSADATA wsa;
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
  this = (HTTPServer*)Object_new(sizeof(HTTPServer), &httpServerClass);

  if (this == 0) return 0;

#ifdef WIN32   
  if (WSAStartup(MAKEWORD(2, 2), &this->wsa) != 0) {
    printf("\nError: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
    exit(1);
  }
#endif

  this->port = 8080;

  if ((this->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    PRINT(("socket failed\n"));
    exit(1);
  }
  if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(int){1}, sizeof(int))<0) {
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
    printf("bind failed");
    exit(1);
  }

  return this;
}

PUBLIC void HTTPServer_delete(HTTPServer* this)
{
  close(this->fd);
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

  char szBuff[100];

  // listen for connections
  if (listen(this->fd, 10) < 0) {
    printf("listen failed");
    exit(1);
  }

  while (1) {
    // client info
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int *client_fd = malloc(sizeof(int));
    char *responseBuffer = (char*)malloc(4096);

    // accept client connection
    if ((*client_fd = accept(this->fd, 
                           (struct sockaddr *)&client_addr, 
                            &client_addr_len)) < 0) {
            PRINT(("accept failed"));
            break;
        }
    PRINT(("Received connection\n"));

    int msg_len = recv(*client_fd, &responseBuffer[0], 4095, 0);

    
    printf("Bytes Received: %d, message: %s\n", msg_len, responseBuffer);

    msg_len = send(*client_fd, response, sizeof(response), 0);
    if (msg_len == 0) {
      printf("Client closed connection\n");
      close(this->fd);
      return -1;
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
    free(responseBuffer);
    free(client_fd);
    close(this->fd);
    exit(1);
  }
  
}

/*
#define MSG_SIZE 1024
#define REPLY_SIZE 65536

  int main(int argc, char* argv[])
  {
    int s = -1;
    struct sockaddr_in server;
    char message[MSG_SIZE] = { 0 }, server_reply[REPLY_SIZE] = { 0 };
    int recv_size = 0;

#if defined(_WIN32) || defined(_WIN64)    
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
      printf("\nError: Windows socket subsytsem could not be initialized. Error Code: %d. Exiting..\n", WSAGetLastError());
      exit(1);
    }
#endif

    //Create a socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("Error: Could not create socket: %s. Exiting..\n", strerror(errno));
      exit(1);
    }

    // Fill in server's address
    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = inet_addr("172.217.160.238"); // google.com
    server.sin_family = AF_INET;
    server.sin_port = htons(80);

    // Connect to server
    if (connect(s, (struct sockaddr*)(&server), sizeof(server)) < 0) {
      printf("Error: Could not connect to server: %s. Exiting..\n", strerror(errno));
      exit(1);
    }

    // Send HTTP request
    strcpy(message, "GET / HTTP/1.1\r\n\r\n");
    if (send(s, message, strlen(message), 0) < 0) {
      printf("Error: Could not send http request to server: %s. Exiting..\n", strerror(errno));
      exit(1);
    }

    // Receive a reply from the server
    printf("\nWaiting for server reply..\n");
    if ((recv_size = recv(s, server_reply, REPLY_SIZE, 0)) < 0) {
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
  } // end of main 
  */
  /*
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_PORT    5019


  int main(int argc, char** argv) {

    char response[] = "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/html; charset=UTF-8\r\n\r\n"
      "<doctype !html><html><head><title>Hello World</title></head>"
      "<body><h1>Hello world!</h1></body></html>\r\n";

    char szBuff[100];
    int msg_len;
    int addr_len;
    struct sockaddr_in local, client_addr;

    SOCKET sock, msg_sock;
    WSADATA wsaData;

    if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) {
      // stderr: standard error are printed to the screen.
      fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
      //WSACleanup function terminates use of the Windows Sockets DLL. 
      WSACleanup();
      return -1;
    }
    // Fill in the address structure
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(DEFAULT_PORT);

    sock = socket(AF_INET, SOCK_STREAM, 0);  //TCp socket


    if (sock == INVALID_SOCKET) {
      fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
      WSACleanup();
      return -1;
    }

    if (bind(sock, (struct sockaddr*)&local, sizeof(local)) == SOCKET_ERROR) {
      fprintf(stderr, "bind() failed with error %d\n", WSAGetLastError());
      WSACleanup();
      return -1;
    }


    //waiting the connection
    if (listen(sock, 5) == SOCKET_ERROR) {
      fprintf(stderr, "listen() failed with error %d\n", WSAGetLastError());
      WSACleanup();
      return -1;
    }


    printf("Waiting the connection........\n");

    while (1) {
      addr_len = sizeof(client_addr);
      msg_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
      if (msg_sock == INVALID_SOCKET) {
        fprintf(stderr, "accept() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
      }

      if (msg_sock == -1) {
        perror("Unable to accept connection.");
        continue;
      }

      printf("accepted connection from %s, port %d\n",
        inet_ntoa(client_addr.sin_addr),
        htons(client_addr.sin_port));

      msg_len = recv(msg_sock, szBuff, sizeof(szBuff), 0);



      printf("Bytes Received: %d, message: %s from %s\n", msg_len, szBuff, inet_ntoa(client_addr.sin_addr));



      msg_len = send(msg_sock, response, sizeof(response) - 1, 0);
      if (msg_len == 0) {
        printf("Client closed connection\n");
        closesocket(msg_sock);
        return -1;
      }

      if (msg_len == SOCKET_ERROR) {
        fprintf(stderr, "recv() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
      }

      if (msg_len == 0) {
        printf("Client closed connection\n");
        closesocket(msg_sock);
        return -1;
      }
      closesocket(msg_sock);
    }
    WSACleanup();
  }
  */
