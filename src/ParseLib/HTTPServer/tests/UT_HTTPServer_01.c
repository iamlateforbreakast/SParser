/* UT_HTTPServer_01.c */

#include "HTTPServer.h"

int step1()
{
    int isPassed = 1;

    HTTPServer * httpServer = 0;

    httpServer = HTTPServer_new();

    HTTPServer_start(httpServer);

    HTTPServer_delete(httpServer);

    return isPassed;
}

int main()
{
  int isPassed = 1;

  step1();

  return isPassed;
}