all: UT_SkipList_01.c UT_SkipList_02.c ../SkipList.c ../SkipList.h
	gcc -g UT_SkipList_01.c ../SkipList.c ../../libCommon.a -I../../include -o UT_SkipList_01
	gcc -g UT_SkipList_02.c ../SkipList.c ../../libCommon.a -I../../include -o UT_SkipList_02

clean:
	rm -rf UT_SkipList_01 UT_SkipList_02 *.o