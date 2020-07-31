import queries
import re
from urlparse import urlparse, parse_qs
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer


		
class HTTPHandler(BaseHTTPRequestHandler):

    get_rules = []
    post_rules = []

    def do_GET(self):
       pass

    def do_POST(self):
        pass

    def addGetRule(self, tuple):
	    self.get_rules.append(tuple)
    def addPostRule(self, tuple):
        self.get_rules.append(tuple)

class Router():
    def __init__(self):
        #self.handler = HTTPHandler()
        self.server = HTTPServer(('', 80), HTTPHandler)

    def addRoute(self, className, method, rule):
        pattern = []
        tokens = rule.split('?')
     	print"Token: ",tokens
        endpoint = tokens[:-1]
        if len(endpoint):
    	    print "Endpoint: ",str(endpoint[0])
            l=re.split('<|>', endpoint[0])
            print "l: ",l
        args = tokens[-1:]
        if len(args):
           print "args: ", args
           #n = re.split('<|>', args)
           #tuple = ( pattern, l, n )
           #if method == "GET":
               #self.handler.addGetRule(tuple)
	       #if method == "POST":
		       #self.handler.addPostRule(tuple)
        return self
	  
    def process_request(self):
        # self.server.handle_request()
       # for each rules
       # if match then class().get/put/deleter
       return 1
   
    def cleanup(self):
       return
