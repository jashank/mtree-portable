#	From: @(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD$

PROG=	mtree
SRCS=	compare.c crc.c create.c misc.c mtree.c spec.c verify.c setflags.c
MAN8=	mtree.8
.PATH:	${.CURDIR}/../../usr.bin/cksum ${.CURDIR}/../../lib/libc/gen

.if !defined(WORLD)
DPADD+=	${LIBMD}
LDADD+=	-lmd
CFLAGS+= -DMD5 -DSHA1 -DRMD160
.endif

.include <bsd.prog.mk>
