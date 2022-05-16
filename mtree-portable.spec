# Derived from openSUSE:Factory/nmtree, which is
#   Copyright (c) 2021 SUSE LLC

%global	gitref	83e6022c9569c50c683771cd8d7e3390d8262411
Name:		mtree-portable
Version:	2022.05.16
%if "%{_vendor}" == "redhat"
Release:	1%{?dist}
%elif "%{_vendor}" == "suse"
Release:	1
%endif
Summary:	A utility for mapping and checking directory hierarchies.
License:	BSD-3-Clause
Group:		Productivity/File utilities
URL:		https://github.com/jashank/mtree-portable
Source:		https://github.com/jashank/mtree-portable/archive/%{gitref}.tar.gz

BuildRequires:	autoconf
BuildRequires:	automake
BuildRequires:	gcc
BuildRequires:	libbsd-devel
BuildRequires:	libmd-devel
BuildRequires:	libnbcompat-devel
BuildRequires:	make

Provides:	mtree

%description
The mtree utility compares a file hierarchy against a specification,
creates a specification for a file hierarchy, or modifies such a
specification.  This specification can be controlled by the user, but
typically includes file/directory/symlink names, ownership information,
permission bits, and so forth.  It may optionally also include various
hashes, such as SHA-256 or MD5.

%prep
%setup -q

%build
autoreconf --symlink --install --verbose --force

%configure \
	CPPFLAGS="-DBSD4_4 -Dst_mtimespec=st_mtim"
%make_build

%install
%make_install

%files
%attr(0755,root,root) %{_bindir}/mtree
%attr(0644,root,root) %{_mandir}/man5/mtree.5.gz
%attr(0644,root,root) %{_mandir}/man8/mtree.8.gz
%defattr(0644,root,root,0755)
%doc %{_docdir}/%{name}

%changelog
