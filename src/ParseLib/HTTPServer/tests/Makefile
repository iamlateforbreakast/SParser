all: UT_HTTPServer_01

UT_HTTPServer_01: UT_HTTPServer_01.c ../HTTPServer.c ../HTTPServer.h
	gcc UT_HTTPServer_01.c ../HTTPServer.c ../../../CommonLib/libCommon.a -I../../include -I../../../CommonLib/include -o UT_HTTPServer_01

clean:
	rm -rf UT_HTTPServer_01 *.o ../*.o
