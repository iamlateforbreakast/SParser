all: UT_FileMgr_01.c ../FileMgr.c ../FileMgr.h ../FileDesc.c ../FileDesc.h
	gcc -DUNIT_TEST -DMAIN -g ../FileMgr.c ../FileDesc.c UT_FileMgr_01.c ../../../CommonLib/libCommon.a -I../../include -I../../../CommonLib/include -o UT_FileMgr_01

clean:
	rm -rf UT_FileMgr_01 *.o ../*.o
