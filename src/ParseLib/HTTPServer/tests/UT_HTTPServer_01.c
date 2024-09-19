/* UT_HTTPServer_01.c */

#include "HTTPServer.h"
#include "ObjectMgr.h"
#include "Memory.h"

int step1()
{
    int isPassed = 1;

    HTTPServer * httpServer = 0;

    httpServer = HTTPServer_new();

    HTTPServer_start(httpServer);

    HTTPServer_delete(httpServer);

  ObjectMgr* objectMgr = ObjectMgr_getRef();
  ObjectMgr_reportUnallocated(objectMgr);
  ObjectMgr_delete(objectMgr);
    return isPassed;
}

int main()
{
  int isPassed = 1;

  step1();

  Memory_report();
  return isPassed;
}
