all: UT_List_01 UT_List_02 UT_List_03

UT_List_01: UT_List_01.c ../List.c ../List.h TestObject.h TestObject.c
	gcc -g UT_List_01.c ../List.c TestObject.c ../../libCommon.a -I../../include -o UT_List_01

UT_List_02: UT_List_02.c ../List.c ../List.h MyAllocator.c MyAllocator.h TestObject.c TestObject.h
	gcc -g UT_List_02.c ../List.c ../List.h MyAllocator.c TestObject.c ../../libCommon.a -I../../include -o UT_List_02

UT_List_03: UT_List_03.c ../List.c ../List.h TestObject.h TestObject.c
	gcc -g UT_List_03.c ../List.c TestObject.c ../../libCommon.a -I../../include -o UT_List_03
clean:
	rm -rf UT_List_01 UT_List_02 *.o ../*.o
