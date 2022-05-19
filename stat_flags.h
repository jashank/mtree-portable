/*-
 * SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 1991, 1993 The Regents of the University of California.
 *	All rights reserved.
 */

/*	from CSRG:   @(#)extern.h	8.1 (Berkeley) 5/31/93 */
/*	from NetBSD: stat_flags.h,v 1.4 2003/08/07 09:05:16 agc Exp */
/*	from pkgsrc: stat_flags.h,v 1.1 2003/09/05 18:39:00 jlam Exp */

char	*flags_to_string(u_long, const char *);
int	 string_to_flags(char **, u_long *, u_long *);
