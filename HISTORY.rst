:SPDX-License-Identifier: BSD-2-Clause

This repository is very weird:
in an attempt to preserve as much of
the development history as possible,
I've synthesised commits from several sources,
including
`the CSRG archives <https://github.com/csrg/csrg>`_,
`FreeBSD <https://www.freebsd.org/>`_,
`NetBSD <https://www.netbsd.org/>`_,
`OpenBSD <https://www.openbsd.org/>`_,
`pkgsrc <https://www.netbsd.org/>`_,
`libarchive <https://github.com/libarchive/libarchive/>`_,
as well as porting and packaging efforts from
`Debian <https://github.com/jgoerzen/mtree-netbsd>`_ and
`OpenSUSE <https://github.com/archiecobbs/nmtree>`_.

Commit messages include annotations, beginning ``;;``,
with information about the commit itself.

These provenance markers may include:

``;; CSRG-SVN: rNNNNNN``:
   :Browse: <https://SVNweb.FreeBSD.org/csrg>

   Commit derived from CSRG data preserved by Kirk McKusick
   and converted from SCCS to Subversion by John Baldwin.
   Git conversions exist with varying levels of completeness;
   for example,
   <https://github.com/jonathangray/csrg>, or
   <https://github.com/weiss/original-bsd> —
   these were useful for confirming human names.

``;; FreeBSD-Git: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``:
   :Browse: <https://cgit.FreeBSD.org/src/>
   :Browse: <https://github.com/freebsd/freebsd-src>

   Commit derived from FreeBSD at this Git revision.

``;; FreeBSD-SVN: rNNNNNN``:
   :Browse: <https://SVNweb.FreeBSD.org/base>

   Commit derived from FreeBSD at this Subversion revision.
   (Commits were synthesised from Git, not Subversion.
   CVS and Subversion keywords have varying levels of expansion.)

``;; NetBSD-CVS: PATH@rREV``:
   :Browse: <http://CVSweb.netbsd.org/bsdweb.cgi/src/>

   Commit derived from the NetBSD CVS repository at this point.
   (Commits were synthesised from Mercurial, not CVS,
   though I've checked the CVS artefacts for correctness.)

``;; NetBSD-Mercurial: rNNNNNN``:
   :Browse: <https://anonHg.NetBSD.org/src/>

   Commit derived from the NetBSD Mercurial repository at this revision.

``;; pkgsrc-CVS: PATH@rREV``:
   :Browse: <http://CVSweb.netbsd.org/bsdweb.cgi/pkgsrc/>

   Commit derived from the pkgsrc CVS repository at this point.
   (Commits were synthesised from Mercurial, not CVS,
   though I've checked the CVS artefacts for correctness.)

``;; pkgsrc-Mercurial: rNNNNNN``:
   :Browse: <https://anonHg.NetBSD.org/pkgsrc/>

   Commit derived from the pkgsrc Mercurial repository at this revision.

``;; OpenBSD-CVS: PATH@rREV``:
   :Browse: <https://CVSweb.OpenBSD.org/src/>

   Commit derived from the OpenBSD CVS repository at this point:
   these commits were synthesised from CVS artefacts.

``;; libarchive-Git: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``:
   :Browse: <https://github.com/libarchive/libarchive>

   Commit derived from the libarchive Git repository at this revision:
   given how few revisions there are here,
   I could export individual objects and
   generate the fast-import hints by hand.

``;; archiecobbs-Git: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``:
   :Browse: <https://github.com/archiecobbs/nmtree>

   Commit derived from Archie Cobbs' Git repository at this revision.

``;; jgoerzen-Git: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX``:
   :Browse: <https://github.com/jgoerzen/mtree-netbsd>

   Commit derived from John Goerzen's Git repository at this revision.
   (That repository includes a Git import of the pkgsrc CVS history,
   which I've grafted by hand.)

Some version control systems — notably SCCS and CVS —
are bad at dealing with renaming files.
I've used context from around the commit
to reconstruct what filenames should be.
Notable manifestations of this are:

- ``create.c`` was originally called ``cwalk.c``.
