/*-
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2000 Massachusetts Institute of Technology.
 */

/*	from NetBSD: excludes.c,v 1.13 2004/06/20 22:20:18 jmc Exp */
/*	from pkgsrc: excludes.c,v 1.4 2004/08/21 04:10:45 jlam Exp */

#if HAVE_CONFIG_H
#include "config.h"
#endif
#if HAVE_NBTOOL_CONFIG_H
#include "nbtool_config.h"
#endif
#if HAVE_NBCOMPAT_H
#include <nbcompat.h>
#endif

#if HAVE_SYS_CDEFS_H
#include <sys/cdefs.h>
#endif
#if defined(__RCSID) && !defined(lint)
__RCSID("$NetBSD: excludes.c,v 1.13 2004/06/20 22:20:18 jmc Exp $");
#endif

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_SYS_QUEUE_H
#include <sys/queue.h>
#endif

#if HAVE_FNMATCH_H
#include <fnmatch.h>
#endif
#if HAVE_STDIO_H
#include <stdio.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_TIME_H
#include <time.h>
#endif

#if HAVE_LIBUTIL_H
#include <libutil.h>
#endif
#if HAVE_UTIL_H
#include <util.h>
#endif

#include "extern.h"


/*
 * We're assuming that there won't be a whole lot of excludes,
 * so it's OK to use a stupid algorithm.
 */
struct exclude {
	LIST_ENTRY(exclude) link;
	const char *glob;
	int pathname;
};
static LIST_HEAD(, exclude) excludes;


void
init_excludes(void)
{

	LIST_INIT(&excludes);
}

void
read_excludes_file(const char *name)
{
	FILE *fp;
	char *line;
	struct exclude *e;

	fp = fopen(name, "r");
	if (fp == 0)
		err(1, "%s", name);

	while ((line = fparseln(fp, NULL, NULL, NULL,
	    FPARSELN_UNESCCOMM | FPARSELN_UNESCCONT | FPARSELN_UNESCESC))
	    != NULL) {
		if (line[0] == '\0')
			continue;

		if ((e = malloc(sizeof *e)) == NULL)
			mtree_err("memory allocation error");

		e->glob = line;
		if (strchr(e->glob, '/') != NULL)
			e->pathname = 1;
		else
			e->pathname = 0;
		LIST_INSERT_HEAD(&excludes, e, link);
	}
	fclose(fp);
}

int
check_excludes(const char *fname, const char *path)
{
	struct exclude *e;

	/* fnmatch(3) has a funny return value convention... */
#define MATCH(g, n) (fnmatch((g), (n), FNM_PATHNAME) == 0)

	e = LIST_FIRST(&excludes);
	while (e) {
		if ((e->pathname && MATCH(e->glob, path))
		    || MATCH(e->glob, fname)) {
			return (1);
		}
		e = LIST_NEXT(e, link);
	}
	return (0);
}
