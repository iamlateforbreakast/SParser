all: main.c ../FileMgr.c ../FileMgr.h ../FileDesc.c ../FileDesc.h
	gcc -UPRIVATE -g main.c ../FileMgr.c ../FileDesc.c ../../../CommonLib/libCommon.a -I../../include -I../../../CommonLib/include -o test1

clean:
	rm -rf test1 *.o
