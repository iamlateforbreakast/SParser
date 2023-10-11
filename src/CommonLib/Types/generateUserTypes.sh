#!/bin/sh

cat "/* User Types */" > UserTypes.h
cat "#ifndef _USERTYPES_" > UserTypes.h
cat "#define _USERTYPES_" > UserTypes.h

cat "Class * userTypes[] =" > UserTypes.h
cat "{" > UserTypes.h
cat "  &listClass,"  > UserTypes.h
cat "  &stringClass"  > UserTypes.h
cat "};" > UserTypes.h

cat "#endif /* UserTypes.h */" > UserTypes.h
