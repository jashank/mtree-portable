/*-
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright (c) 1998, 2001 The NetBSD Foundation, Inc.
 *	All rights reserved.
 *	This code is derived from software contributed to
 *	The NetBSD Foundation by Charles M. Hannum.
 */

/*	from NetBSD: pack_dev.h,v 1.8 2013/06/14 16:28:20 tsutsui Exp */
/*	from pkgsrc: pack_dev.h,v 1.4 2008/11/06 02:14:52 jschauma Exp */

#ifndef	_PACK_DEV_H
#define	_PACK_DEV_H

typedef	dev_t pack_t(int, u_long [], const char **);

pack_t	*pack_find(const char *);
pack_t	 pack_native;

#define	major_netbsd(x)		((int32_t)((((x) & 0x000fff00) >>  8)))
#define	minor_netbsd(x)		((int32_t)((((x) & 0xfff00000) >> 12) | \
					   (((x) & 0x000000ff) >>  0)))
#define	makedev_netbsd(x,y)	((dev_t)((((x) <<  8) & 0x000fff00) | \
					 (((y) << 12) & 0xfff00000) | \
					 (((y) <<  0) & 0x000000ff)))

#endif	/* _PACK_DEV_H */
