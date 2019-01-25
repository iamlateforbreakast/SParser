#Copyright Jon Berg , turtlemeat.com

import string,cgi,time,sqlite3
from os import curdir, sep
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
#import pri

class MyHandler(BaseHTTPRequestHandler):

    dbinfo = ""; #Data extracted from the DB file
    
    def do_GET(self):
        try:
            if self.path.endswith(".html"):
                f = open(curdir + sep + self.path) #self.path has /test.html
#note that this potentially makes every file on your computer readable by the internet

                self.send_response(200)
                self.send_header('Content-type',	'text/html')
                self.end_headers()
                self.wfile.write(f.read())
                f.close()
                return
            if self.path.endswith(".js"):
                f = open(curdir + sep + self.path) 
                self.send_response(200)
                self.send_header('Content-type',	'text/javascript')
                self.end_headers()
                self.wfile.write(f.read())
                f.close()
                return
            if self.path.endswith(".css"):
                f = open(curdir + sep + self.path) 
                self.send_response(200)
                self.send_header('Content-type',	'text/css')
                self.end_headers()
                self.wfile.write(f.read())
                f.close()
                return
            if self.path.endswith(".json"):   #provide our data
                self.send_response(200)
                self.send_header('Content-type',	'text/json')
                self.end_headers()
                self.wfile.write(self.dbinfo)
                return
                
            return
                
        except IOError:
            self.send_error(404,'File Not Found: %s' % self.path)
     

    def do_POST(self):
        global rootnode
        try:
            ctype, pdict = cgi.parse_header(self.headers.getheader('content-type'))
            if ctype == 'multipart/form-data':
                query=cgi.parse_multipart(self.rfile, pdict)
            self.send_response(301)
            
            self.end_headers()
            upfilecontent = query.get('upfile')
            print "filecontent", upfilecontent[0]
            self.wfile.write("<HTML>POST OK.<BR><BR>");
            self.wfile.write(upfilecontent[0]);
            
        except :
            pass

class MyDbHandler:

    #data = "{\"nodes\":[{\"id\":\"1\", \"label\":\"Node 1\"},{\"id\":\"2\", \"label\":\"Node 2\"}],\"edges\":[{\"from\":\"1\", \"to\":\"2\"}]}";
    data = ""
    nodes = ""
    edges = ""
    db = 0;
    cursor = 0;
    
    def __init__(self):
       self.db = sqlite3.connect('test.db');
       self.cursor = self.db.cursor()
       self.nodes = "\"nodes\":["
       self.edges = "\"edges\":[{\"from\":\"1\", \"to\":\"2\"}]"
    def __del__(self):
       self.db.close();
    
    def get_data(self):
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
   
def main():
    try:
        keep_running = 1
        handler = MyHandler
        db_handler = MyDbHandler();
        handler.dbinfo = db_handler.get_data();
        server = HTTPServer(('', 80), handler)
        print 'started httpserver...'
        while keep_running:
           server.handle_request()
        server.socket.close()
    except KeyboardInterrupt:
        print '^C received, shutting down server'
        keep_running = 0

if __name__ == '__main__':
    main()

