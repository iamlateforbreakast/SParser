all: UT_ObjectStore.c MyAllocator.c ../ObjectStore.c ../ObjectStore.h
	gcc -g UT_ObjectStore.c MyAllocator.c ../ObjectStore.c ../../libCommon.a -I../../include -o UT_ObjectStore

clean:
	rm -rf UT_ObjectStore *.o ../*.o
