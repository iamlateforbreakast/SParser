
BUILT_SOURCES = copy-headers

# list of headers
pkginclude_HEADERS = \
    Map/Map.h \
    Types/Types.h

# List the headers you want to "publish" to the local include folder
PUBLIC_HDRS = $(pkginclude_HEADERS)

# This creates a 'built-include' directory and symlinks/copies files into it
copy-headers:
	@mkdir -p $(top_builddir)/include
	@for hdr in $(PUBLIC_HDRS); do \
	    cp -f $(srcdir)/$$hdr $(top_builddir)/include/ ; \
	done

# Clean up the generated directory
clean-local:
	rm -rf $(top_builddir)/include
    
# 1. Define the library name
noinst_LIBRARIES = libCommon.a

# 2. List every .c and .h file required to build it
libCommon_a_SOURCES = \
    Allocator/Malloc.c \
    Array/Array.c \
    BTree/BTree.c \
    Debug/Debug.c \
    Error/Error.c \
    Memory/Memory.c \
    Map/Map.c






