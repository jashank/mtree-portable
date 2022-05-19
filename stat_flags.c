/*-
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 1993 The Regents of the University of California
 *	All rights reserved.
 */

/*	from CSRG:   @(#)stat_flags.c	8.2 (Berkeley) 7/28/94 */
/*	from NetBSD: stat_flags.c,v 1.3 2022/04/19 20:32:17 rillig Exp */
/*	from pkgsrc: stat_flags.c,v 1.5 2011/07/27 15:31:00 seb Exp */

#if HAVE_CONFIG_H
#include "config.h"
#endif
#if 0
#if HAVE_NBTOOL_CONFIG_H
#include "nbtool_config.h"
#else
#define HAVE_STRUCT_STAT_ST_FLAGS 1
#endif
#endif
#if HAVE_NBCOMPAT_H
#include <nbcompat.h>
#endif

#if HAVE_SYS_CDEFS_H
#include <sys/cdefs.h>
#endif
#if !defined(lint)
#if 0
static char sccsid[] = "@(#)stat_flags.c	8.2 (Berkeley) 7/28/94";
#else
__RCSID("$NetBSD: stat_flags.c,v 1.5 2011/07/27 15:31:00 seb Exp $");
#endif
#endif /* not lint */

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if HAVE_FTS_H
#include <fts.h>
#endif
#if HAVE_STDDEF_H
#include <stddef.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif

#include "stat_flags.h"

#define	SAPPEND(s) do {							\
	if (prefix != NULL)						\
		(void)strlcat(string, prefix, sizeof(string));		\
	(void)strlcat(string, s, sizeof(string));			\
	prefix = ",";							\
} while (/* CONSTCOND */ 0)

/*
 * flags_to_string --
 *	Convert stat flags to a comma-separated string.  If no flags
 *	are set, return the default string.
 */
char *
flags_to_string(u_long flags, const char *def)
{
	static char string[128];
	const char *prefix;

	string[0] = '\0';
	prefix = NULL;
#if HAVE_STRUCT_STAT_ST_FLAGS
	if (flags & UF_APPEND)
		SAPPEND("uappnd");
	if (flags & UF_IMMUTABLE)
		SAPPEND("uchg");
	if (flags & UF_NODUMP)
		SAPPEND("nodump");
	if (flags & UF_OPAQUE)
		SAPPEND("opaque");
	if (flags & SF_APPEND)
		SAPPEND("sappnd");
	if (flags & SF_ARCHIVED)
		SAPPEND("arch");
	if (flags & SF_IMMUTABLE)
		SAPPEND("schg");
#ifdef SF_SNAPSHOT
	if (flags & SF_SNAPSHOT)
		SAPPEND("snap");
#endif
#endif
	if (prefix == NULL)
		strlcpy(string, def, sizeof(string));
	return (string);
}

#define	TEST(a, b, f) {							\
	if (!strcmp(a, b)) {						\
		if (clear) {						\
			if (clrp)					\
				*clrp |= (f);				\
			if (setp)					\
				*setp &= ~(f);				\
		} else {						\
			if (setp)					\
				*setp |= (f);				\
			if (clrp)					\
				*clrp &= ~(f);				\
		}							\
		break;							\
	}								\
}

/*
 * string_to_flags --
 *	Take string of arguments and return stat flags.  Return 0 on
 *	success, 1 on failure.  On failure, stringp is set to point
 *	to the offending token.
 */
int
string_to_flags(char **stringp, u_long *setp, u_long *clrp)
{
	int clear;
	char *string, *p;

	if (setp)
		*setp = 0;
	if (clrp)
		*clrp = 0;

#if HAVE_STRUCT_STAT_ST_FLAGS
	string = *stringp;
	while ((p = strsep(&string, "\t ,")) != NULL) {
		clear = 0;
		*stringp = p;
		if (*p == '\0')
			continue;
		if (p[0] == 'n' && p[1] == 'o') {
			clear = 1;
			p += 2;
		}
		switch (p[0]) {
		case 'a':
			TEST(p, "arch", SF_ARCHIVED);
			TEST(p, "archived", SF_ARCHIVED);
			return (1);
		case 'd':
			clear = !clear;
			TEST(p, "dump", UF_NODUMP);
			return (1);
		case 'n':
				/*
				 * Support `nonodump'. Note that
				 * the state of clear is not changed.
				 */
			TEST(p, "nodump", UF_NODUMP);
			return (1);
		case 'o':
			TEST(p, "opaque", UF_OPAQUE);
			return (1);
		case 's':
			TEST(p, "sappnd", SF_APPEND);
			TEST(p, "sappend", SF_APPEND);
			TEST(p, "schg", SF_IMMUTABLE);
			TEST(p, "schange", SF_IMMUTABLE);
			TEST(p, "simmutable", SF_IMMUTABLE);
			return (1);
		case 'u':
			TEST(p, "uappnd", UF_APPEND);
			TEST(p, "uappend", UF_APPEND);
			TEST(p, "uchg", UF_IMMUTABLE);
			TEST(p, "uchange", UF_IMMUTABLE);
			TEST(p, "uimmutable", UF_IMMUTABLE);
			return (1);
		default:
			return (1);
		}
	}
#endif

	return (0);
}
