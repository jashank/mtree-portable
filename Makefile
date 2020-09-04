#	From: @(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD$

.include <bsd.own.mk>

.PATH: ${SRCTOP}/usr.bin/cksum

PROG=	fmtree
MAN=	fmtree.8 mtree.5
SRCS=	compare.c crc.c create.c excludes.c misc.c mtree.c spec.c verify.c
SRCS+=	specspec.c

CFLAGS+= -DWITH_MD5 -DWITH_SHA1 -DWITH_RMD160 -DWITH_SHA256
LIBADD=	md

.if ${MK_NMTREE} == "no"
LINKS=	${BINDIR}/fmtree ${BINDIR}/mtree
MLINKS=	fmtree.8 mtree.8
.endif

CLEANFILES+=	fmtree.8

fmtree.8: mtree.8
	${CP} ${.ALLSRC} ${.TARGET}

.include <bsd.prog.mk>
