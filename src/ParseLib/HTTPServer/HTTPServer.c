/* HTTPServer.c */

#include "HTTPServer.h"

struct HTTPServer
{

};

Class httpServerClass =
{
    .f_new = 0,
    .f_delete = (Destructor)&HTTPServer_delete
};

PUBLIC HTTPServer * HTTPServer_new()
{
    return 0;
}

PUBLIC void HTTPServer_delete(HTTPServer * this)
{

}