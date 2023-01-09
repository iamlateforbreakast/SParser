all: UT_Pool_01

UT_Pool_01: UT_Pool_01.c ../Pool.c ../Pool.h
	gcc -g UT_Pool_01.c ../Pool.c -I../../include -I.. ../../libCommon.a -o UT_Pool_01
clean:
	rm -rf UT_Pool_01 ../*.o
