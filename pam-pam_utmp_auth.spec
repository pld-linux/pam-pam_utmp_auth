%define 	modulename pam_utmp_auth
Summary:	PAM module for login without password for logged users
Summary(pl.UTF-8):   Moduł PAM pozwalający na logowanie się bez hasła
Name:		pam-%{modulename}
Version:	1.0
Release:	2
Epoch:		1
License:	GPL v2
Group:		Base
Vendor:		Wojtek Kaniewski <wojtekka@irc.pl>
Source0:	pam_utmp.c
# based on ftp://dev.null.pl/pub/pam_utmp.c with small changes,
# don't put into df - it's only small text file.
URL:		http://dev.null.pl/
BuildRequires:	pam-devel
Provides:	%{modulename}
Obsoletes:	pam_utmp_auth
BuildRoot:	%{tmpdir}/%{name}-%{version}-root-%(id -u -n)

%description
PAM module which allow login without password for currently logged
users.

%description -l pl.UTF-8
Moduł PAM pozwalający na logowanie się bez podawania hasła dla już
zalogowanych użytkowników.

%prep
%setup -q -c -T
cp -f %{SOURCE0} .

%build
%{__cc} %{rpmcflags} -fPIC -c pam_utmp.c
ld -shared -x -o %{modulename}.so pam_utmp.o -lpam
head -n 19 pam_utmp.c >README

%install
rm -rf $RPM_BUILD_ROOT
install -D %{modulename}.so $RPM_BUILD_ROOT/%{_lib}/security/%{modulename}.so

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(644,root,root,755)
%doc README
%attr(755,root,root) /%{_lib}/security/%{modulename}.so
