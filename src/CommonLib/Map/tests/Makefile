all: UT_Map_01 UT_Map_02

UT_Map_01: UT_Map_01.c ../Map.c ../Map.h Words1000.h
	gcc -g UT_Map_01.c ../Map.c ../MapEntry.c TestObject.c ../../libCommon.a -I../../include -o UT_Map_01

UT_Map_02: UT_Map_02.c ../Map.c ../Map.h ./MyAllocator.c ./MyAllocator.h
	gcc -g UT_Map_02.c ./MyAllocator.c ../Map.c ../MapEntry.c ../../libCommon.a -I../../include -o UT_Map_02

clean:
	rm -rf UT_Map_01 UT_Map_02 *.o ../*.o
