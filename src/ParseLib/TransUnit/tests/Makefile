all: UT_TransUnit_01 UT_MacroStore_01

#%.d: %.c
#	gcc -MM -I../../include -I../../../CommonLib/include -I../../../AppliLib/include -o $@ $<
 
UT_TransUnit_01: UT_TransUnit_01.c ../TransUnit.c ../TransUnit.h
	gcc -DUNIT_TEST -DMAIN -g UT_TransUnit_01.c ../TransUnit.c \
	-I../../include -I../../../CommonLib/include -I../../../AppliLib/include \
	-L../../../CommonLib -L../../../AppliLib -L../.. \
	-lParse -lAppli -lCommon \
	-o UT_TransUnit_01

UT_MacroStore_01: UT_MacroStore_01.c ../MacroStore.h
	gcc -DUNIT_TEST -DMAIN -g UT_MacroStore_01.c \
	-I../../include -I../../../CommonLib/include -I../../../AppliLib/include \
	-L../../../CommonLib -L../../../AppliLib -L../.. \
	-lParse -lAppli -lCommon \
	-o UT_MacroStore_01

UT_Buffer_01: UT_Buffer_01.c ../Buffer.h
	gcc -DUNIT_TEST -DMAIN -g UT_Buffer_01.c \
   -I../../include -I../../../CommonLib/include -I../../../AppliLib/include \
   -L../../../CommonLib -L../../../AppliLib -L../.. \
   -lParse -lAppli -lCommon \
   -o UT_Buffer_01

clean:
	rm -rf UT_TransUnit_01 UT_MacroStore_01 *.o ../*.o *.d
