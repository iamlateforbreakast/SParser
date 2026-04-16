
BUILT_SOURCES = copy-headers

AM_CPPFLAGS = -I$(srcdir)/include

# list of headers
pkginclude_HEADERS = \
    Allocator/Allocator.h \
    Allocator/Malloc.h \
    BTree/BTree.h \
    Debug/Debug.h \
    Error/Error.h \
    FileIo/FileIo.h \
    Handle/Handle.h \
    List/List.h \
    Map/Map.h \
    Memory/Memory.h \
    Object/Class.h \
    Object/Object.h \
    ObjectMgr/ObjectMgr.h \
    ObjectStore/ObjectStore.h \
    Pool/Pool.h \
    SkipList/SkipList.h \
    String/String2.h \
    Times/Times.h \
    Types/Portability.h \
    Types/UserTypes.h \
    Types/Types.h

# List the headers you want to "publish" to the local include folder
PUBLIC_HDRS = $(pkginclude_HEADERS)

# This creates a 'built-include' directory and symlinks/copies files into it
copy-headers:
	@mkdir -p $(srcdir)/include
	@for hdr in $(PUBLIC_HDRS); do \
	    cp -f $(srcdir)/$$hdr $(srcdir)/include/ ; \
	done

# Clean up the generated directory
clean-local:
	rm -rf $(srcdir)/include
    
# 1. Define the library name
noinst_LIBRARIES = libCommon.a

# 2. List every .c and .h file required to build it
libCommon_a_SOURCES = \
    Allocator/Malloc.c \
    Array/Array.c \
    BTree/BTree.c \
    Debug/Debug.c \
    Error/Error.c \
    FileIo/FileIo.c \
    Handle/Handle.c \
    List/List.c \
    Map/Map.c \
    Memory/Memory.c \
    Object/Object.c \
    ObjectMgr/ObjectMgr.c \
    ObjectStore/ObjectStore.c \
    Pool/Pool.c \
    SkipList/SkipList.c \
    String/String2.c \
    Times/Times.c

.PHONY: copy-headers




