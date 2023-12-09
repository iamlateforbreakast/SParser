all: UT_SdbMgr_01.c ../SdbMgr.c ../SdbMgr.h ../SdbRequest.c ../SdbRequest.h
	gcc -g UT_SdbMgr_01.c ../SdbRequest.c ../SdbMgr.c ../../../CommonLib/libCommon.a -I../../include -I../../../CommonLib/include -lsqlite3 -o UT_SdbMgr_01

clean:
	rm -rf UT_SdbMgr_01 *.o ../*.o
