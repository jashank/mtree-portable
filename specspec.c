/*-
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 2003 Poul-Henning Kamp.
 *	All rights reserved.
 */

/*	from NetBSD: specspec.c,v 1.4 2017/12/14 18:34:41 christos Exp */

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
__RCSID("$NetBSD: specspec.c,v 1.4 2017/12/14 18:34:41 christos Exp $");
#endif

#if HAVE_ERR_H
#include <err.h>
#endif
#if HAVE_GRP_H
#include <grp.h>
#endif
#if HAVE_PWD_H
#include <pwd.h>
#endif
#if HAVE_STDINT_H
#include <stdint.h>
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
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "mtree.h"
#include "extern.h"

#define FF(a, b, c, d) \
	(((a)->flags & (c)) && ((b)->flags & (c)) && ((a)->d) != ((b)->d))
#define FS(a, b, c, d) \
	(((a)->flags & (c)) && ((b)->flags & (c)) && strcmp((a)->d,(b)->d))
#define FM(a, b, c, d) \
	(((a)->flags & (c)) && ((b)->flags & (c)) && memcmp(&(a)->d,&(b)->d, sizeof (a)->d))

static void
shownode(NODE *n, int f, char const *path)
{
	struct group *gr;
	struct passwd *pw;

	printf("%s%s %s", path, n->name, inotype(nodetoino(n->type)));
	if (f & F_CKSUM)
		printf(" cksum=%lu", n->cksum);
	if (f & F_GID)
		printf(" gid=%d", n->st_gid);
	if (f & F_GNAME) {
		gr = getgrgid(n->st_gid);
		if (gr == NULL)
			printf(" gid=%d", n->st_gid);
		else
			printf(" gname=%s", gr->gr_name);
	}
	if (f & F_MODE)
		printf(" mode=%o", n->st_mode);
	if (f & F_NLINK)
		printf(" nlink=%ju", (uintmax_t)n->st_nlink);
	if (f & F_SIZE)
		printf(" size=%jd", (intmax_t)n->st_size);
	if (f & F_UID)
		printf(" uid=%d", n->st_uid);
	if (f & F_UNAME) {
		pw = getpwuid(n->st_uid);
		if (pw == NULL)
			printf(" uid=%d", n->st_uid);
		else
			printf(" uname=%s", pw->pw_name);
	}
	if (f & F_MD5)
		printf(" %s=%s", MD5KEY, n->md5digest);
	if (f & F_SHA1)
		printf(" %s=%s", SHA1KEY, n->sha1digest);
	if (f & F_RMD160)
		printf(" %s=%s", RMD160KEY, n->rmd160digest);
	if (f & F_SHA256)
		printf(" %s=%s", SHA256KEY, n->sha256digest);
	if (f & F_SHA384)
		printf(" %s=%s", SHA384KEY, n->sha384digest);
	if (f & F_SHA512)
		printf(" %s=%s", SHA512KEY, n->sha512digest);
	if (f & F_FLAGS)
		printf(" flags=%s", flags_to_string(n->st_flags, "none"));
	printf("\n");
}

static int
mismatch(NODE *n1, NODE *n2, int differ, char const *path)
{

	if (n2 == NULL) {
		shownode(n1, differ, path);
		return (1);
	}
	if (n1 == NULL) {
		printf("\t");
		shownode(n2, differ, path);
		return (1);
	}
	if (!(differ & keys))
		return(0);
	printf("\t\t");
	shownode(n1, differ, path);
	printf("\t\t");
	shownode(n2, differ, path);
	return (1);
}

static int
compare_nodes(NODE *n1, NODE *n2, char const *path)
{
	int differs;

	if (n1 != NULL && n1->type == F_LINK)
		n1->flags &= ~F_MODE;
	if (n2 != NULL && n2->type == F_LINK)
		n2->flags &= ~F_MODE;
	differs = 0;
	if (n1 == NULL && n2 != NULL) {
		differs = n2->flags;
		mismatch(n1, n2, differs, path);
		return (1);
	}
	if (n1 != NULL && n2 == NULL) {
		differs = n1->flags;
		mismatch(n1, n2, differs, path);
		return (1);
	}
	if (n1->type != n2->type) {
		differs = 0;
		mismatch(n1, n2, differs, path);
		return (1);
	}
	if (FF(n1, n2, F_CKSUM, cksum))
		differs |= F_CKSUM;
	if (FF(n1, n2, F_GID, st_gid))
		differs |= F_GID;
	if (FF(n1, n2, F_GNAME, st_gid))
		differs |= F_GNAME;
	if (FF(n1, n2, F_MODE, st_mode))
		differs |= F_MODE;
	if (FF(n1, n2, F_NLINK, st_nlink))
		differs |= F_NLINK;
	if (FF(n1, n2, F_SIZE, st_size))
		differs |= F_SIZE;
	if (FS(n1, n2, F_SLINK, slink))
		differs |= F_SLINK;
	if (FM(n1, n2, F_TIME, st_mtimespec))
		differs |= F_TIME;
	if (FF(n1, n2, F_UID, st_uid))
		differs |= F_UID;
	if (FF(n1, n2, F_UNAME, st_uid))
		differs |= F_UNAME;
	if (FS(n1, n2, F_MD5, md5digest))
		differs |= F_MD5;
	if (FS(n1, n2, F_SHA1, sha1digest))
		differs |= F_SHA1;
	if (FS(n1, n2, F_RMD160, rmd160digest))
		differs |= F_RMD160;
	if (FS(n1, n2, F_SHA256, sha256digest))
		differs |= F_SHA256;
	if (FS(n1, n2, F_SHA384, sha384digest))
		differs |= F_SHA384;
	if (FS(n1, n2, F_SHA512, sha512digest))
		differs |= F_SHA512;
	if (FF(n1, n2, F_FLAGS, st_flags))
		differs |= F_FLAGS;
	if (differs) {
		mismatch(n1, n2, differs, path);
		return (1);
	}
	return (0);
}
static int
walk_in_the_forest(NODE *t1, NODE *t2, char const *path)
{
	int r, i;
	NODE *c1, *c2, *n1, *n2;
	char *np;

	r = 0;

	if (t1 != NULL)
		c1 = t1->child;
	else
		c1 = NULL;
	if (t2 != NULL)
		c2 = t2->child;
	else
		c2 = NULL;
	while (c1 != NULL || c2 != NULL) {
		n1 = n2 = NULL;
		if (c1 != NULL)
			n1 = c1->next;
		if (c2 != NULL)
			n2 = c2->next;
		if (c1 != NULL && c2 != NULL) {
			if (c1->type != F_DIR && c2->type == F_DIR) {
				n2 = c2;
				c2 = NULL;
			} else if (c1->type == F_DIR && c2->type != F_DIR) {
				n1 = c1;
				c1 = NULL;
			} else {
				i = strcmp(c1->name, c2->name);
				if (i > 0) {
					n1 = c1;
					c1 = NULL;
				} else if (i < 0) {
					n2 = c2;
					c2 = NULL;
				}
			}
		}
		if (c1 == NULL && c2->type == F_DIR) {
			asprintf(&np, "%s%s/", path, c2->name);
			i = walk_in_the_forest(c1, c2, np);
			free(np);
			i += compare_nodes(c1, c2, path);
		} else if (c2 == NULL && c1->type == F_DIR) {
			asprintf(&np, "%s%s/", path, c1->name);
			i = walk_in_the_forest(c1, c2, np);
			free(np);
			i += compare_nodes(c1, c2, path);
		} else if (c1 == NULL || c2 == NULL) {
			i = compare_nodes(c1, c2, path);
		} else if (c1->type == F_DIR && c2->type == F_DIR) {
			asprintf(&np, "%s%s/", path, c1->name);
			i = walk_in_the_forest(c1, c2, np);
			free(np);
			i += compare_nodes(c1, c2, path);
		} else {
			i = compare_nodes(c1, c2, path);
		}
		r += i;
		c1 = n1;
		c2 = n2;
	}
	return (r);
}

int
mtree_specspec(FILE *fi, FILE *fj)
{
	int rval;
	NODE *root1, *root2;

	root1 = spec(fi);
	root2 = spec(fj);
	rval = walk_in_the_forest(root1, root2, "");
	rval += compare_nodes(root1, root2, "");
	if (rval > 0)
		return (MISMATCHEXIT);
	return (0);
}
