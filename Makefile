#	$NetBSD: Makefile,v 1.28 2006/12/14 17:54:19 he Exp $
#	from: @(#)Makefile	8.2 (Berkeley) 4/27/95

.include <bsd.own.mk>

PROG=	mtree
#CPPFLAGS+=-DDEBUG
CPPFLAGS+= -DMTREE
MAN=	mtree.8
SRCS=	compare.c crc.c create.c excludes.c misc.c mtree.c spec.c verify.c \
	getid.c stat_flags.c pack_dev.c
WARNS?=	3

CPPFLAGS+=	-I${NETBSDSRCDIR}/bin/ls -I${NETBSDSRCDIR}/sbin/mknod
.PATH:		${NETBSDSRCDIR}/bin/ls ${NETBSDSRCDIR}/sbin/mknod

.include <bsd.prog.mk>
