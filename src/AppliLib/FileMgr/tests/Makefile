all: main.c ../FileMgr.c ../FileMgr.h ../FileDesc.c ../FileDesc.h
	gcc -DUNIT_TEST -g ../FileMgr.c ../FileDesc.c main.c ../../../CommonLib/libCommon.a -I../../include -I../../../CommonLib/include -o test1

clean:
	rm -rf test1 *.o ../*.o
