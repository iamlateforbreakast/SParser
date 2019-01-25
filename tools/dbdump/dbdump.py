import sqlite3

db = sqlite3.connect('../../TESTDB')
db.row_factory = sqlite3.Row
cursor = db.cursor()

print('Functions--------------------------------------------------')

cursor.execute('''SELECT * FROM Function_Declarations''')
for row in cursor:
        # row['name'] returns the name column in the query, row['email'] returns email column.
	print('{0} : {1}, {2}'.format(row['name'], row['type'], row['fname']))

print('Variables--------------------------------------------------')
cursor.execute('''SELECT * FROM Variable_Declarations''')
for row in cursor:
        # row['name'] returns the name column in the query, row['email'] returns email column.
	print('{0} : {1}, {2}'.format(row['name'], row['type'], row['fname']))
db.close()

