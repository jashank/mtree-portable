/*-
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright 1990, 1993 The Regents of the University of California.
 *	All rights reserved.
 */

/*	from NetBSD: verify.c,v 1.47 2021/03/18 20:02:18 cheusov Exp */
/*	from pkgsrc: verify.c,v 1.6 2010/03/21 16:30:17 joerg Exp */

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
#if 0
static char sccsid[] = "@(#)verify.c	8.1 (Berkeley) 6/6/93";
#else
__RCSID("$NetBSD: verify.c,v 1.47 2021/03/18 20:02:18 cheusov Exp $");
#endif
#endif /* not lint */

#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if HAVE_DIRENT_H
#include <dirent.h>
#endif
#if HAVE_ERRNO_H
#include <errno.h>
#endif
#if HAVE_FNMATCH_H
#include <fnmatch.h>
#endif
#if HAVE_STDIO_H
#include <stdio.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "extern.h"

static NODE *root;
static char path[MAXPATHLEN];

static void	miss(NODE *, char *);
static int	vwalk(void);

int
verify(FILE *fi)
{
	int rval;

	root = spec(fi);
	rval = vwalk();
	miss(root, path);
	return (rval);
}

static int
vwalk(void)
{
	FTS *t;
	FTSENT *p;
	NODE *ep, *level;
	int specdepth, rval;
	char *argv[2];
	char  dot[] = ".";
	argv[0] = dot;
	argv[1] = NULL;

	if ((t = fts_open(argv, ftsoptions, NULL)) == NULL)
		mtree_err("fts_open: %s", strerror(errno));
	level = root;
	specdepth = rval = 0;
	while ((p = fts_read(t)) != NULL) {
		if (check_excludes(p->fts_name, p->fts_path)) {
			fts_set(t, p, FTS_SKIP);
			continue;
		}
		if (!find_only(p->fts_path)) {
			fts_set(t, p, FTS_SKIP);
			continue;
		}
		switch(p->fts_info) {
		case FTS_D:
		case FTS_SL:
			break;
		case FTS_DP:
			if (specdepth > p->fts_level) {
				for (level = level->parent; level->prev;
				    level = level->prev)
					continue;
				--specdepth;
			}
			continue;
		case FTS_DNR:
		case FTS_ERR:
		case FTS_NS:
			warnx("%s: %s", RP(p), strerror(p->fts_errno));
			continue;
		default:
			if (dflag)
				continue;
		}

		if (specdepth != p->fts_level)
			goto extra;
		for (ep = level; ep; ep = ep->next)
			if ((ep->flags & F_MAGIC &&
			    !fnmatch(ep->name, p->fts_name, FNM_PATHNAME)) ||
			    !strcmp(ep->name, p->fts_name)) {
				ep->flags |= F_VISIT;
				if ((ep->flags & F_NOCHANGE) == 0 &&
				    compare(ep, p))
					rval = MISMATCHEXIT;
				if (!(ep->flags & F_IGN) &&
				    ep->type == F_DIR &&
				    p->fts_info == FTS_D) {
					if (ep->child) {
						level = ep->child;
						++specdepth;
					}
				} else
					fts_set(t, p, FTS_SKIP);
				break;
			}

		if (ep)
			continue;
 extra:
		if (!eflag && !(dflag && p->fts_info == FTS_SL)) {
			printf("extra: %s", RP(p));
			if (rflag) {
#if HAVE_STRUCT_STAT_ST_FLAGS
				if (rflag > 1 &&
				    lchflags(p->fts_accpath, 0) == -1)
					printf(" (chflags %s)",
					    strerror(errno));
#endif
				if ((S_ISDIR(p->fts_statp->st_mode)
				    ? rmdir : unlink)(p->fts_accpath)) {
					printf(", not removed: %s",
					    strerror(errno));
				} else
					printf(", removed");
			}
			putchar('\n');
		}
		fts_set(t, p, FTS_SKIP);
	}
	fts_close(t);
	if (sflag)
		warnx("%s checksum: %u", fullpath, crc_total);
	return (rval);
}

static void
miss(NODE *p, char *tail)
{
	int create;
	char *tp;
	const char *type;
	uint32_t flags;

	for (; p; p = p->next) {
		if (p->flags & F_OPT && !(p->flags & F_VISIT))
			continue;
		if (p->type != F_DIR && (dflag || p->flags & F_VISIT))
			continue;
		strcpy(tail, p->name);
		if (!(p->flags & F_VISIT)) {
			/* Don't print missing message if file exists as a
			   symbolic link and the -q flag is set. */
			struct stat statbuf;

			if (qflag && stat(path, &statbuf) == 0 &&
			    S_ISDIR(statbuf.st_mode))
				p->flags |= F_VISIT;
			else
				(void)printf("%s missing", path);
		}
		switch (p->type) {
		case F_BLOCK:
		case F_CHAR:
			type = "device";
			break;
		case F_DIR:
			type = "directory";
			break;
		case F_LINK:
			type = "symlink";
			break;
		default:
			putchar('\n');
			continue;
		}

		create = 0;
		if (!(p->flags & F_VISIT) && uflag) {
			if (mtree_Wflag || p->type == F_LINK)
				goto createit;
			if (!(p->flags & (F_UID | F_UNAME)))
			    printf(
				" (%s not created: user not specified)", type);
			else if (!(p->flags & (F_GID | F_GNAME)))
			    printf(
				" (%s not created: group not specified)", type);
			else if (!(p->flags & F_MODE))
			    printf(
				" (%s not created: mode not specified)", type);
			else
 createit:
			switch (p->type) {
			case F_BLOCK:
			case F_CHAR:
				if (mtree_Wflag)
					continue;
				if (!(p->flags & F_DEV))
					printf(
				    " (%s not created: device not specified)",
					    type);
				else if (mknod(path,
				    p->st_mode | nodetoino(p->type),
				    p->st_rdev) == -1)
					printf(" (%s not created: %s)\n",
					    type, strerror(errno));
				else
					create = 1;
				break;
			case F_LINK:
				if (!(p->flags & F_SLINK))
					printf(
				    " (%s not created: link not specified)\n",
					    type);
				else if (symlink(p->slink, path))
					printf(
					    " (%s not created: %s)\n",
					    type, strerror(errno));
				else
					create = 1;
				break;
			case F_DIR:
				if (mkdir(path, S_IRWXU|S_IRWXG|S_IRWXO))
					printf(" (not created: %s)",
					    strerror(errno));
				else
					create = 1;
				break;
			default:
				mtree_err("can't create create %s",
				    nodetype(p->type));
			}
		}
		if (create)
			printf(" (created)");
		if (p->type == F_DIR) {
			if (!(p->flags & F_VISIT))
				putchar('\n');
			for (tp = tail; *tp; ++tp)
				continue;
			*tp = '/';
			miss(p->child, tp + 1);
			*tp = '\0';
		} else
			putchar('\n');

		if (!create || mtree_Wflag)
			continue;
		if ((p->flags & (F_UID | F_UNAME)) &&
		    (p->flags & (F_GID | F_GNAME)) &&
		    (lchown(path, p->st_uid, p->st_gid))) {
			printf("%s: user/group/mode not modified: %s\n",
			    path, strerror(errno));
			printf("%s: warning: file mode %snot set\n", path,
			    (p->flags & F_FLAGS) ? "and file flags " : "");
			continue;
		}
		if (p->flags & F_MODE) {
			if (lchmod(path, p->st_mode))
				printf("%s: permissions not set: %s\n",
				    path, strerror(errno));
		}
#if HAVE_STRUCT_STAT_ST_FLAGS
		if ((p->flags & F_FLAGS) && p->st_flags) {
			if (iflag)
				flags = p->st_flags;
			else
				flags = p->st_flags & ~SP_FLGS;
			if (lchflags(path, flags))
				printf("%s: file flags not set: %s\n",
				    path, strerror(errno));
		}
#endif	/* HAVE_STRUCT_STAT_ST_FLAGS */
	}
}
