# 1. Define the library name
noinst_LIBRARIES = libCommon.a

# 2. List every .c and .h file required to build it
libCommon_a_SOURCES = \
    Map/map.c \
    Map/map_utils.c \
    Vector/vector.c \
    Vector/vector_internal.h \
    Utils/helpers.c \
    include/common.h

# This tells Automake where the 'public' headers should go
pkginclude_HEADERS = \
    include/common.h \
    Map/map.h \
    Vector/vector.h

# List the headers you want to "publish" to the local include folder
PUBLIC_HDRS = $(pkginclude_HEADERS)

# This creates a 'built-include' directory and symlinks/copies files into it
all-local:
	@mkdir -p $(top_builddir)/include
	@for hdr in $(PUBLIC_HDRS); do \
	    cp -f $(srcdir)/$$hdr $(top_builddir)/include/ ; \
	done

# Clean up the generated directory
clean-local:
	rm -rf $(top_builddir)/include
