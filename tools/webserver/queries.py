import string,sqlite3

def getTransUnitList(dbCursor):
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

def getNodeList(dbCursor):
    pass
	
def getNodeCount(dbCursor):
    dbCursor.execute("SELECT count(*) FROM Nodes")
    count = dbCursor.fetchone()[0]
    print "Total Number of Nodes: ", count
    return count
	
def getTransUnitCount(dbCursor):
    dbCursor.execute("SELECT count(*) FROM Translation_Units")
    count = dbCursor.fetchone()[0]
    print "Total Number of Translation Units: ", count
    return count