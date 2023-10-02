import re
from urlparse import urlparse, parse_qs
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer

get_rules = []
post_rules = []

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

class HTTPHandler(BaseHTTPRequestHandler):    
    def do_GET(self):
        o = urlparse(self.path)
        print("GET: path: ",o.path)
        for rule in get_rules:
            #print "Rule: ", rule[0]
            m = re.match(rule[0], o.path)
            if m:
                print(" Matched -->Rule: ",rule[0])
                print(rule[1](self, o.path))
    def do_POST(self):
        o = urlparse(self.path)
        print("POST: path: ",o.path)
        
class Router():

    def __init__(self):
        self.server = HTTPServer(('', 8080), HTTPHandler)
        
    def addRoute(self, className, method, rule):
        pattern = ''
        tokens = rule.split('?')
     	#print"Token: ",tokens
        endpoint = tokens[:1]
        if len(endpoint):
    	    #print "Endpoint: ",str(endpoint[0])
            l=re.split('<|>', endpoint[0])
            print("l: ",l)
            for s in l:
               if s[0:3] == "int":
                  pattern = pattern + "(\d+)"
               else:
                 pattern = pattern + s
        #print "Pattern: ",pattern
        args = tokens[1:]
        if len(args):
           #print "args: ", args
           n = re.split('=|&', args[0])
           #print "n: ",n
           self.nextString = "name"
           for s in n:
               if self.nextString == "name":
                  #print "s: ",s
                  pattern = pattern + s + "="
                  self.nextString = "value"
               else:
                  if s.isdigit():
                     pattern = pattern + "(\d+)" + "&"
                  else:
                     pattern = pattern + "[a-zA-Z]+" + "&"
                  self.nextString = "param"
        #print "Pattern: ",pattern
        tuple = (pattern, className)
        if method == "GET":
            get_rules.append(tuple)
	    if method == "POST":
		    post_rules.append(tuple)
        return self
	  
    def process_request(self):
       self.server.handle_request()
       # for each rules
       # if match then class().get/put/deleter
       return 1
   
    def cleanup(self):
       return
