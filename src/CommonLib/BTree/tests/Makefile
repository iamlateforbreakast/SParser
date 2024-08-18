all: UT_BTree_01

UT_BTree_01: UT_BTree_01.c ../BTree.c ../BTree.h ../Node.c ../Node.h TestObject.c TestObject.h
	gcc -g UT_BTree_01.c ../BTree.c ../Node.c TestObject.c -I../../include -I.. ../../libCommon.a -o UT_BTree_01

clean:
	rm -rf UT_BTree_01 ../*.o

.PHONY: all clean test default
