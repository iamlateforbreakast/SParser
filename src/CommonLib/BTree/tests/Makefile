all: UT_BTree

UT_BTree: UT_BTree.c ../BTree.c ../BTree.h ../Node.c ../Node.h TestObject.c TestObject.h
	gcc -g UT_BTree.c ../BTree.c ../Node.c TestObject.c -I../../include -I.. ../../libCommon.a -o UT_BTree

clean:
	rm -rf UT_BTree ../*.o

.PHONY: all clean test default
