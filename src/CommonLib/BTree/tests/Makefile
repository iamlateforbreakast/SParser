all: clean default test

test: UT_BTree.c
	./UT_BTree
	
default: UT_BTree.c ../BTree.c ../BTree.h ../Node.c ../Node.h
	gcc -g UT_BTree.c ../BTree.c -I../../include -I.. ../../libCommon.a -o UT_Btree

clean:
	rm -rf UT_BTree ../*.o

.PHONY: all clean test default
