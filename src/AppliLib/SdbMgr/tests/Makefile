all: main.c ../SdbMgr.c ../SdbMgr.h ../SdbRequest.c ../SdbRequest.h
	gcc -g main.c ../SdbRequest.c ../SdbMgr.c ../../../CommonLib/libCommon.a -I../../include -I../../../CommonLib/include -lsqlite3 -o test1

clean:
	rm -rf test1 *.o
