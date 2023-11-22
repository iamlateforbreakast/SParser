all: UT_List_02

UT_List_01: UT_List_01.c ../List.c ../List.h
	gcc -g UT_List_01.c ../List.c ../../libCommon.a -I../../include -o UT_List_01

UT_List_02: UT_List_02.c ../List.c ../List.h MyAllocator.c MyAllocator.h TestObject.c TestObject.h
	gcc -g UT_List_02.c ../List.c ../List.h MyAllocator.c TestObject.c ../../libCommon.a -I../../include -o UT_List_02

clean:
	rm -rf UT_List_01 UT_List_02 *.o ../*.o
