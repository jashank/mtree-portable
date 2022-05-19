/*-
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 1991, 1993 The Regents of the University of California.
 *	All rights reserved.
 */

/*	from CSRG:   @(#)extern.h	8.1 (Berkeley) 6/6/93 */
/*	from NetBSD: extern.h,v 1.40 2021/03/18 20:02:18 cheusov Exp */
/*	from pkgsrc: extern.h,v 1.5 2011/07/27 15:31:00 seb Exp */

#if HAVE_CONFIG_H
#include "config.h"
#endif
#if HAVE_NBTOOL_CONFIG_H
#include "nbtool_config.h"
#endif
#if HAVE_NBCOMPAT_H
#include <nbcompat.h>
#endif

#if HAVE_ERR_H
#include <err.h>
#endif
#if HAVE_FTS_H
#include <fts.h>
#endif
#if HAVE_STDBOOL_H
#include <stdbool.h>
#endif

#if HAVE_LIBUTIL_H
#include <libutil.h>
#endif
#if HAVE_UTIL_H
#include <util.h>
#endif

#ifndef __arraycount
#define __arraycount(x) (sizeof((x)) / sizeof((x)[0]))
#endif

#if HAVE_NETDB_H
/* For MAXHOSTNAMELEN on some platforms. */
#include <netdb.h>
#endif

#include "mtree.h"
#include "stat_flags.h"

#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 256
#endif

enum flavor {
	F_MTREE,
	F_FREEBSD9,
	F_NETBSD6
};

void	 addtag(slist_t *, char *);
int	 check_excludes(const char *, const char *);
int	 compare(NODE *, FTSENT *);
int	 crc(int, uint32_t *, uint32_t *);
void	 cwalk(FILE *);
void	 dump_nodes(FILE *, const char *, NODE *, int);
void	 init_excludes(void);
int	 matchtags(NODE *);
void	mtree_err(const char *, ...)
	__attribute__((__format__(__printf__, 1, 2)))
	__attribute__((noreturn));
const char *nodetype(u_int);
u_int	 parsekey(const char *, int *);
void	 parsetags(slist_t *, char *);
u_int	 parsetype(const char *);
void	 read_excludes_file(const char *);
const char *rlink(const char *);
int	 verify(FILE *);
void	 load_only(const char *fname);
bool	 find_only(const char *path);

extern int	bflag, dflag, eflag, iflag, jflag, lflag, mflag,
		nflag, qflag, rflag, sflag, tflag, uflag;
extern int	mtree_Mflag, mtree_Sflag, mtree_Wflag;
extern size_t	mtree_lineno;
extern enum flavor	flavor;
extern uint32_t crc_total;
extern int	ftsoptions, keys;
extern char	fullpath[];
extern slist_t	includetags, excludetags;
