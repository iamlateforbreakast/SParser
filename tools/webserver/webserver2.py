#Copyright Jon Berg , turtlemeat.com

import string,cgi,time,sqlite3

from router import Router
from os import curdir, sep
import os.path


db_handler = None

#parsed = urlparse('http://user:pass@NetLoc:80/path;parameters?query=argument#fragment')
#print 'scheme  :', parsed.scheme
#print 'netloc  :', parsed.netloc
#print 'path    :', parsed.path
#print 'params  :', parsed.params
#print 'query   :', parsed.query
#print 'fragment:', parsed.fragment
#print 'username:', parsed.username
#print 'password:', parsed.password
#print 'hostname:', parsed.hostname, '(netloc in lower case)'
#print 'port    :', parsed.port

#import json
#json_string = json.dumps({'hello': 'world', 'received': 'ok'})
#self.wfile.write(json_string)
# GET Node/Show/0?depth=10
# GET Node/List?first=10&last=20
# GET Node/total
# POST Node/add/3789?name=a+node&id=10
# POST Node/delete/150
# POST Operation?name=terminate
# router. addRoute(NodeShow,  "Node/show/<int:id>")
# router.addRoute(NodeList, "Node/list?<int: first><int:last>)
# router.route(request)
# router = Router(MyHandler)
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

# class MyHandler(BaseHTTPRequestHandler):
    # def do_GET(self):
        # print(self.path)
        # print parse_qs(self.path[2:]) 
        # try:
            # if self.path.endswith(".html"):
                # f = open(curdir + sep + self.path) #self.path has /test.html
# #note that this potentially makes every file on your computer readable by the internet

                # self.send_response(200)
                # self.send_header('Content-type',	'text/html')
                # self.end_headers()
                # self.wfile.write(f.read())
                # f.close()
                # return
            # if self.path.endswith(".js"):
                # f = open(curdir + sep + self.path) 
                # self.send_response(200)
                # self.send_header('Content-type',	'text/javascript')
                # self.end_headers()
                # self.wfile.write(f.read())
                # f.close()
                # return
            # if self.path.endswith(".css"):
                # f = open(curdir + sep + self.path) 
                # self.send_response(200)
                # self.send_header('Content-type',	'text/css')
                # self.end_headers()
                # self.wfile.write(f.read())
                # f.close()
                # return
            # if self.path.endswith(".json"):   #provide our data
                # self.send_response(200)
                # self.send_header('Content-type',	'text/json')
                # self.end_headers()
                # db_handler=MyDbHandler()
                # self.wfile.write(db_handler.getTransUnitList())
                # return
            # #if self.path.endswith("units.json"):   #provide our data
            # #    self.send_response(200)
            # #    self.send_header('Content-type',	'text/json')
            # #    self.end_headers()
            # #    db_handler=MyDbHandler()
            # #    self.wfile.write(db_handler.getTransUnitList())
            # #    return   
            # return
                
        # except IOError:
            # self.send_error(404,'File Not Found: %s' % self.path)

    # def do_POST(self):
        # global rootnode
        # try:
            # ctype, pdict = cgi.parse_header(self.headers.getheader('content-type'))
            # if ctype == 'multipart/form-data':
                # postparam=cgi.parse_multipart(self.rfile, pdict)
            # self.send_response(200)
            # self.send_header('Content-type',	'text/json')
            # self.end_headers()
            # db_handler=MyDbHandler()
            # self.wfile.write(db_handler.getTransUnitList())
            
        # except :
            # pass

class Home:
    def _init(self):
        pass

class NodeShow:
    def _init(self):
        pass
        
class MyDbHandler:

    #data = "{\"nodes\":[{\"id\":\"1\", \"label\":\"Node 1\"},{\"id\":\"2\", \"label\":\"Node 2\"}],\"edges\":[{\"from\":\"1\", \"to\":\"2\"}]}";

    db = 0;
    cursor = 0;
    
    def __init__(self):
       print("MyDbHandler.init()");
       self.db = sqlite3.connect('test.db');
       self.cursor = self.db.cursor()
       self.nodes = "\"nodes\":["
       self.edges = "\"edges\":[{\"from\":\"1\", \"to\":\"2\"}]"
       queries.getNodeCount(self.cursor)
       queries.getTransUnitCount(self.cursor)
	   
    def __del__(self):
       self.db.close();
       
    def getTransUnitList(self):
       print("Get Translation Unit List")
       data = ""
       self.nodes = "["
       self.edges = "[{\"from\":\"1\", \"to\":\"2\"}]"
       self.cursor.execute('''SELECT Unit_Id, File_Name FROM Translation_Units''')
       all_rows = self.cursor.fetchall()
       for num, row in enumerate(all_rows):
         print('{0} : {1}'.format(row[0], row[1]))
         name = os.path.basename(row[1])
         new_node = "{{\"id\":\"{0}\",\"label\":\"{1}\"}}".format(row[0], name) 
         if num == 0:
           self.nodes = "{0}{1}".format(self.nodes, new_node)
         else:
           self.nodes = "{0},{1}".format(self.nodes, new_node)
       self.nodes = "{0}]".format(self.nodes)
       self.data = "{{\"nodes\": {0},\"edges\": {1}}}".format(self.nodes, self.edges)
       print(self.data)
       return self.data
       
    def get_data(self):
       print("Get Data")       
       self.cursor.execute('''SELECT NodeId, NodeType, NodePtr FROM Nodes''')
       all_rows = self.cursor.fetchall()
       for num, row in enumerate(all_rows):
          print('{0} : {1} : {2}'.format(row[0], row[1], row[2]))
          if row[1] == 2:
            new_node = "{{\"id\":\"{0}\",\"label\":\"Node {0}\",\"color\":{{\"background\": \"orange\"}}}}".format( row[0])
          else:
            new_node = "{{\"id\":\"{0}\",\"label\":\"Node {0}\"}}".format( row[0])
          if num == 0:
            self.nodes = "{0}{1}".format(self.nodes, new_node)
          else:
            self.nodes = "{0},{1}".format(self.nodes, new_node)
       self.nodes = "{0}]".format(self.nodes)
       self.data = "{{{0},{1}}}".format(self.nodes, self.edges)
       print(self.data)
       return self.data

class App:

    db = 0;
    cursor = 0;
	
    def __init__(self):
        self.db = sqlite3.connect('test.db');
        self.cursor = self.db.cursor()
        self.router = Router()
        self.keep_running = 1

    def _del_(self):
        self.db.close()

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
    app.addRoute(Home, "GET","/")
# app.addRoute(SparseCss, "GET","sparse.css")
# app.addRoute(VisJs, "GET","vis.min.js")
    app.addRoute(NodeShow, "GET", "Node/show/<int:id>?depth=10")
# app.run()

if __name__ == '__main__':
    main()
