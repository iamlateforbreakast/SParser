all: UT_BTree UT_Pool

UT_BTree: UT_BTree.c ../BTree.c ../BTree.h ../Node.c ../Node.h ../Pool.c ../Pool.h
	gcc -g UT_BTree.c ../BTree.c -I../../include -I.. ../../libCommon.a -o UT_Btree

UT_Pool: UT_Pool.c ../Pool.c ../Pool.h
	gcc -g UT_Pool.c -I../../include -I.. ../../libCommon.a -o UT_Pool
clean:
	rm -rf test1 *.o
