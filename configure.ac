dnl // Process this file with autoconf to produce a configure script

AC_PREREQ([2.71])
AC_INIT([sparse], [0.1], [toto@notreal.com])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([README.md])

AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_RANLIB

AC_PATH_XTRA

AM_PROG_LEX
AC_PROG_YACC

PRIMARY_TARGET='sparse'
MAKELIB="ar rv \$@"

# Checks for libraries (e.g., sqlite3)
AC_CHECK_LIB([sqlite3], [sqlite3_open], [], [
  AC_MSG_ERROR([sqlite3 is required])
])

AC_CONFIG_FILES([Makefile src/Makefile src/CommonLib/Makefile src/AppliLib/Makefile src/ParseLib/Makefile doc/Makefile tests/Makefile])
AC_OUTPUT
