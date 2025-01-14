all: UT_TimeMgr_01.c ../TimeMgr.c ../TimeMgr.h ../Timer.c ../Timer.h
	gcc -DUNIT_TEST -DMAIN -g UT_TimeMgr_01.c ../TimeMgr.c ../Timer.c ../../../CommonLib/libCommon.a -I../../include -I../../../CommonLib/include -o UT_TimeMgr_01

clean:
	rm -rf UT_TimeMgr_01 *.o ../*.o
