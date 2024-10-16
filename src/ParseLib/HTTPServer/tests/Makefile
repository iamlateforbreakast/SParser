all: UT_HTTPServer_01

UT_HTTPServer_01: UT_HTTPServer_01.c ../HTTPServer.c ../HTTPServer.h ../HTTPRequest.h ../HTTPResponse.h
	gcc -g UT_HTTPServer_01.c ../HTTPServer.c \
	../../../AppliLib/libAppli.a \
	../../../CommonLib/libCommon.a \
	-I../../include \
	-I../../../AppliLib/include \
	-I../../../CommonLib/include \
	-o UT_HTTPServer_01

clean:
	rm -rf UT_HTTPServer_01 *.o ../*.o
