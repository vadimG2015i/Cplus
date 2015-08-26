Summary: a program to extract Microsoft Cabinet files
Name: cabextract
Version: 0.6
Release: 1
Group: Applications/Archiving
Copyright: GPL
Source: http://www.kyz.uklinux.net/downloads/%{name}-%{version}.tar.gz
URL: http://www.kyz.uklinux.net/cabextract.php3
BuildRoot: %{_tmppath}/%{name}-%{version}-root
Prefix: /usr

%description
Cabinet (.CAB) files are a form of archive, which Microsoft use to
distribute their software, and things like Windows Font Packs. The
cabextract program simply unpacks such files.

%prep
%setup

%build
CFLAGS=${RPM_OPT_FLAGS} ./configure --prefix=%{prefix}
make

%install
rm -rf ${RPM_BUILD_ROOT}
%makeinstall

%clean
rm -rf ${RPM_BUILD_ROOT}

%files
%defattr(0644, root, root, 0755)
%doc AUTHORS ChangeLog NEWS README
%attr(0755, root, root) %{_bindir}/cabextract
%{_mandir}/man1/cabextract.1*
