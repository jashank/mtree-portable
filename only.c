/*-
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2013 The NetBSD Foundation, Inc.
 *	All rights reserved.
 *	This code is derived from software contributed to
 *	The NetBSD Foundation by Christos Zoulas.
 */

/*	from NetBSD: only.c,v 1.3 2017/09/07 04:04:13 nakayama Exp */

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
__RCSID("$NetBSD: only.c,v 1.3 2017/09/07 04:04:13 nakayama Exp $");
#endif

#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#if HAVE_ERR_H
#include <err.h>
#endif
#if HAVE_STDBOOL_H
#include <stdbool.h>
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

struct hentry {
	char *str;
	uint32_t hash;
	struct hentry *next;
};

static struct hentry *table[1024];
static bool loaded;

static uint32_t
hash_str(const char *str)
{
	const uint8_t *s = (const uint8_t *)str;
	uint8_t c;
	uint32_t hash = 0;
	while ((c = *s++) != '\0')
		hash = hash * 33 + c;           /* "perl": k=33, r=r+r/32 */
	return hash + (hash >> 5);
}

static bool
hash_find(const char *str, uint32_t *h)
{
	struct hentry *e;
	*h = hash_str(str) % __arraycount(table);

	for (e = table[*h]; e; e = e->next)
		if (e->hash == *h && strcmp(e->str, str) == 0)
			return true;
	return false;
}

static void
hash_insert(char *str, uint32_t h)
{
	struct hentry *e;
	char *x;

	if ((e = malloc(sizeof(*e))) == NULL)
		mtree_err("memory allocation error");
	if ((x = strdup(str)) == NULL)
		mtree_err("memory allocation error");

	e->str = x;
	e->hash = h;
	e->next = table[h];
	table[h] = e;
}

static void
fill(char *str)
{
	uint32_t h;
	char *ptr = strrchr(str, '/');

	if (ptr == NULL)
		return;

	*ptr = '\0';
	if (!hash_find(str, &h)) {
		hash_insert(str, h);
		fill(str);
	}
	*ptr = '/';
}

void
load_only(const char *fname)
{
	FILE *fp;
	char *line;
	size_t len, lineno;

	if ((fp = fopen(fname, "r")) == NULL)
		err(1, "Cannot open `%s'", fname);

	while ((line = fparseln(fp, &len, &lineno, NULL, FPARSELN_UNESCALL))) {
		uint32_t h;
		if (hash_find(line, &h))
			err(1, "Duplicate entry %s", line);
		hash_insert(line, h);
		fill(line);
		free(line);
	}

	fclose(fp);
	loaded = true;
}

bool
find_only(const char *path)
{
	uint32_t h;

	if (!loaded)
		return true;
	return hash_find(path, &h);
}
