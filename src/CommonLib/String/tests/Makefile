all: UT_String_01 UT_String_02

UT_String_01: UT_String_01.c ../String2.c ../String2.h
	gcc -g UT_String_01.c ../String2.c ../../libCommon.a -I../../include -o UT_String_01

UT_String_02: UT_String_02.c ../String2.c ../String2.h MyAllocator.c MyAllocator.h
	gcc -g UT_String_02.c ../String2.c MyAllocator.c ../../libCommon.a -I../../include -o UT_String_02

clean:
	rm -rf UT_String_01 UT_String_02 *.o ../*.o
