all: SQLite_test

SQLite_test: SQLite_test.c
	gcc -g SQLite_test.c ../../src/CommonLib/libCommon.a \
               -I../../src/CommonLib/include -lsqlite3 -o SQLite_test

clean:
	rm -rf SQLite_test *.o
