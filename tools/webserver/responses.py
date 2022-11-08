import queries
import sqlite3
import os.path

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
       #print(self.data)
       return self.data
    
    def getNodeZero(self, initial_node, depth):
        self.next_node = []
        self.current_node = []
        self.nodes = "["
        self.edges = "["
        self.next_node.append(initial_node)
        for i in range(1,depth):
            print "Level = ", i, "Next nodes ", self.next_node
            self.current_node = self.next_node
            self.next_node = []
            for n in self.current_node:
                node_query = "SELECT NodeId, NodeType, NodePtr, NodeNext, NodePrev FROM Nodes WHERE NodeId={0};".format(n)
                self.cursor.execute(node_query)
                all_rows = self.cursor.fetchall()
                for num, row in enumerate(all_rows):
                    print('NodeId={0} : NodeType={1} : NodePtr={2} : NodeNext={3} : NodePrev={4}'.format(row[0], row[1], row[2], row[3], row[4]))
                    if row[3]!=0:
                        self.next_node.append(int(row[3]))
                    color = "yellow"
                    if int(row[1]) == 2:
                        color = "grey"
                    if int(row[1]) == 3:
                        color = "orange"
                        include_query = "SELECT NodeId, EntryNode FROM Include_Nodes WHERE NodeId={0};".format(row[2])
                        self.cursor.execute(include_query)
                        include_rows = self.cursor.fetchall()
                        for num1, row1 in enumerate(include_rows):
                            print('NodeId={0} : EntryNode={1}'.format(row1[0], row1[1]))
                            if row1[1]!=0:
                                self.next_node.append(int(row1[1]))
                print "next node=",row[3]
                new_node = "{{\"id\":\"{0}\",\"label\":\"{1}\",\"color\":{{\"background\": \"{2}\"}}}}".format(row[0], row[0],color)
                new_edge = "{{\"from\":\"{0}\",\"to\":\"{1}\"}}".format(row[4],row[0]) #"{\"from\":\"{0}\", \"to\":\"{1}\"}".
                if i == 1:
                   self.nodes = "{0}{1}".format(self.nodes, new_node)
                else:
                   self.nodes = "{0},{1}".format(self.nodes, new_node)
                   if i == 2:
                      self.edges = "{0}{1}".format(self.edges, new_edge)
                   else:
                      self.edges = "{0},{1}".format(self.edges, new_edge)
        self.nodes = "{0}]".format(self.nodes)
        self.edges = "{0}]".format(self.edges)
        self.data = "{{\"nodes\": {0},\"edges\": {1}}}".format(self.nodes, self.edges)
        print(self.data)
        return self.data
        
    def getNodeData(self, id):
        content_query = ["SELECT NodeId, Comment FROM Comment_Nodes WHERE NodeId={0};",
                                  "SELECT NodeId, Code FROM Code_Nodes WHERE NodeId={0};",
                                  "SELECT NodeId, Name FROM Include_Nodes WHERE NodeId={0};"]
        node_query = "SELECT NodeId, NodeType, NodePtr, NodeNext, NodePrev FROM Nodes WHERE NodeId={0};".format(id)
        print "getNodeData: ", node_query
        self.cursor.execute(node_query)
        all_rows = self.cursor.fetchall()
        for num, row in enumerate(all_rows):
            print('NodeId={0} : NodeType={1} : NodePtr={2} : NodeNext={3} : NodePrev={4}'.format(row[0], row[1], row[2], row[3], row[4]))
        nodeType = int(row[1])-1
        self.cursor.execute(content_query[nodeType].format(int(row[2])))
        print "getNodeData: ", content_query[nodeType].format(int(row[2]))
        all_rows = self.cursor.fetchall()
        for num, row in enumerate(all_rows):
            print('NodeId={0} : NodeData={1}'.format(row[0], row[1]))
        node_data = "{{\"id\":\"{0}\",\"data\":\"{1}\"}}".format(id,row[1])
        return node_data
        
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
       
class Home:
    def __init__(self, handler, path):
        print "Home Init:"
        f = open("./index.html")
        handler.send_response(200)
        handler.send_header('Content-type',	'text/html')
        handler.end_headers()
        handler.wfile.write(f.read().encode('utf-8'))
        f.close()
        return

class NodeShow:
    def __init__(self, handler, path):
        print "NodeShow Init:"
        handler.send_response(200)
        handler.send_header('Content-type',	'text/json')
        handler.end_headers()
        db_handler=MyDbHandler()
        #handler.wfile.write(db_handler.getTransUnitList())
        handler.wfile.write(db_handler.getNodeZero(1, 50).encode('utf-8'))
        #db_handler.getNodeZero()
        return

class NodeData:
    def __init__(self, handler, path):
        print "NodeData Init:"
        handler.send_response(200)
        handler.send_header('Content-type',	'text/json')
        handler.end_headers()
        db_handler=MyDbHandler()
        nodeId = int(path.split('/')[3])
        handler.wfile.write(db_handler.getNodeData(nodeId).encode('utf-8'))
        return
    
class RainbowJs:
    def __init__(self, handler, path):
        print "RainbowJs Init:"
        f = open("./rainbow-custom.min.js")
        handler.send_response(200)
        handler.send_header('Content-type',	'text/javascript')
        handler.end_headers()
        handler.wfile.write(f.read().encode('utf-8'))
        f.close()
        return    

class ThemeCss:
    def __init__(self, handler, path):
        print "ThemeCss Init:"
        f = open("./github.css")
        handler.send_response(200)
        handler.send_header('Content-type',	'text/css')
        handler.end_headers()
        handler.wfile.write(f.read().encode('utf-8'))
        f.close()
        return
        
class SparseCss:
    def __init__(self, handler, path):
        print "SparseCss Init:"
        f = open("./sparse.css")
        handler.send_response(200)
        handler.send_header('Content-type',	'text/css')
        handler.end_headers()
        handler.wfile.write(f.read().encode('utf-8'))
        f.close()
        return

class VisCss:
    def __init__(self, handler, path):
        print "VisCss Init:"
        f = open("./vis.min.css")
        handler.send_response(200)
        handler.send_header('Content-type',	'text/css')
        handler.end_headers()
        handler.wfile.write(f.read().encode('utf-8'))
        f.close()
        return
        
class VisJs:
    def __init__(self, handler, path):
        print "VisJs Init:"
        f = open("./vis.min.js")
        handler.send_response(200)
        handler.send_header('Content-type',	'text/javascript')
        handler.end_headers()
        handler.wfile.write(f.read().encode('utf-8'))
        f.close()
        return
