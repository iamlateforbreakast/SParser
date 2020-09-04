#Copyright Jon Berg , turtlemeat.com

import string,cgi,time,sqlite3

from router import Router
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import os.path
from responses import Home, NodeShow,NodeData, SparseCss,VisJs, VisCss

# GET Node/Show/0?depth=10
# GET Node/Data/0
# GET Node/List?first=10&last=20
# GET Node/total
# POST Node/add/3789?name=a+node&id=10
# POST Node/delete/150
# POST Operation?name=terminate

#message_parts = [
#                'CLIENT VALUES:',
#               'client_address=%s (%s)' % (self.client_address,
#                                            self.address_string()),
#                'command=%s' % self.command,
#                'path=%s' % self.path,
#                'real path=%s' % parsed_path.path,
#                'query=%s' % parsed_path.query,
#                'request_version=%s' % self.request_version,
#                '',
#                'SERVER VALUES:',
#                'server_version=%s' % self.server_version,
#               'sys_version=%s' % self.sys_version,
#                'protocol_version=%s' % self.protocol_version,
#                '',
#                'HEADERS RECEIVED:',
#                ]

class App:

    db = 0;
    cursor = 0;
	
    def __init__(self):
        self.router = Router()
        self.keep_running = 1

    def run(self):
        try:
            while self.keep_running:
               self.keep_running = self.router.process_request()
        except KeyboardInterrupt:
            print '^C received, shutting down server'
            self.keep_running = 0
        
    def addRoute(self, className, method, rule):
        self.router.addRoute(className, method, rule)

def main():
    app=App()
    app.addRoute(Home, "GET","/index.html")
    app.addRoute(SparseCss, "GET","/sparse.css")
    app.addRoute(VisJs, "GET","/vis.min.js")
    app.addRoute(VisCss, "GET","/vis.min.css")
    app.addRoute(NodeShow, "GET", "/Node/show/<int:id>")
    app.addRoute(NodeData, "GET","/Node/data/<int:id>")
    app.run()

if __name__ == '__main__':
    main()
