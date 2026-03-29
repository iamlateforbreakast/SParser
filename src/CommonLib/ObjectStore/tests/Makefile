all: UT_ObjectStore.c ../../../../tests/unit_tests/MyAllocator.c ../ObjectStore.c ../ObjectStore.h
	gcc -g UT_ObjectStore.c ../ObjectStore.c \
        ../../../../tests/unit_tests/MyAllocator.c \
        ../../libCommon.a \
        -I../../include \
        -I../../../../tests/unit_tests \
        -o UT_ObjectStore

clean:
	rm -rf UT_ObjectStore *.o ../*.o
