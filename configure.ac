#-
# SPDX-License-Identifier: BSD-2-Clause
#

AC_PREREQ([2.69])
AC_INIT([mtree],[1.0.0],[https://github.com/jashank/mtree-portable/issues])
AC_CONFIG_SRCDIR([mtree.c])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([1.9 foreign])

# Allow changing program name
AC_ARG_PROGRAM

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
AC_CHECK_LIB(bsd, fparseln)
AC_CHECK_LIB(util, fparseln)
AC_CHECK_LIB(nbcompat, pwcache_groupdb)
AC_CHECK_LIB(md, SHA512_File)
AC_CHECK_LIB(md, SHA512File)

# Checks for header files.
AC_CHECK_HEADERS([ \
	sys/cdefs.h \
	sys/param.h \
	sys/queue.h \
	sys/stat.h \
	sys/sysmacros.h \
	sys/types.h \
	assert.h \
	bsd/sys/cdefs.h \
	ctype.h \
	dirent.h \
	err.h \
	errno.h \
	fcntl.h \
	fnmatch.h \
	fts.h \
	grp.h \
	limits.h \
	libutil.h \
	md5.h \
	netdb.h \
	pwd.h \
	ripemd.h \
	rmd160.h \
	sha.h \
	sha1.h \
	sha2.h \
	sha256.h \
	sha384.h \
	sha512.h \
	stdarg.h \
	stdbool.h \
	stddef.h \
	stdint.h \
	stdio.h \
	stdlib.h \
	string.h \
	time.h \
	unistd.h \
	util.h \
	vis.h \
	nbcompat.h \
])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_UID_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_CHECK_MEMBERS([struct stat.st_flags])
AC_CHECK_DECLS([UF_SETTABLE, SF_SETTABLE])
AH_BOTTOM([/* Define to 1 if the user- and root-changeable masks were detected */
#if HAVE_STRUCT_STAT_ST_FLAGS && HAVE_DECL_UF_SETTABLE && HAVE_DECL_SF_SETTABLE
#define HAVE_FILE_FLAGS 1
#endif
])


# Checks for library functions.
AC_REPLACE_FNMATCH
AC_CHECK_FUNCS([ \
	endgrent \
	endpwent \
	getcwd \
	gethostname \
	MD5File \
	MD5_File \
	RIPEMD160_File \
	RIPEMD160File \
	RMD160_File \
	RMD160File \
	SHA1_File \
	SHA1File \
	SHA256_File \
	SHA256File \
	SHA384_File \
	SHA384File \
	SHA512_File \
	SHA512File \
	mkdir \
	strchr \
	strpbrk \
	strstr \
	strtol \
	strtoul ])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
