all: UT_Object_01

UT_Object_01: UT_Object_01.c ../Object.c ../Object.h ../../../../tests/unit_tests/TestObject.c
	gcc -g UT_Object_01.c ../Object.c \
        ../../../../tests/unit_tests/TestObject.c \
        ../../libCommon.a \
        -I../../include -I../../../../tests/unit_tests \
        -o UT_Object_01

clean:
	rm -rf UT_Object_01 *.o ../*.o
