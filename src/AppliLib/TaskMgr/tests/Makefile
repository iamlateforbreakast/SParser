all: UT_TaskMgr_01

UT_TaskMgr_01: UT_TaskMgr_01.c ../TaskMgr.c ../Task.c ../TaskMgr.h ../Mutex.h ../Task.h
	gcc -g UT_TaskMgr_01.c ../TaskMgr.c ../Task.c -I.. -I../../../CommonLib/include ../../../CommonLib/libCommon.a -lpthread -o UT_TaskMgr_01

clean:
	rm -rf UT_TaskMgr_01 *.o ../*.o

