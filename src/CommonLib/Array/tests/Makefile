all: clean default

default: UT_Array_01.c ../Array.c ../Array.h
	gcc -g UT_Array_01.c -I../../include -I.. ../../libCommon.a -o UT_Array_01

clean:
	rm -rf UT_Array_01 *.o ../*.o

.PHONY: all clean test default
