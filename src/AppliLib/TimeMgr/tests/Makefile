all: main.c ../TimeMgr.c ../TimeMgr.h ../Timer.c ../Timer.h
	gcc -g main.c ../TimeMgr.c ../Timer.c ../../../CommonLib/libCommon.a -I../../include -I../../../CommonLib/include -o test1

run:
	./test1 > test.log
	cat test.log | grep Malloc > malloc.txt
	cat test.log | grep Free > free.txt

clean:
	rm -rf test1 *.o
