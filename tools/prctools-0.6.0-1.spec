
Summary: A set of tools for building PalmOS applications.
Name: prc-tools
Version: 0.6.0
Release: 1
Copyright: GPL
Group: Development/Tools
Source: prc-tools-0.6.0.tar
BuildRoot: /usr/tmp/%{name}-build
#BuildRoot: /usr/src/redhat/BUILD/prc-tools-0.6.0

%description
This revision of prc-tools was upgraded by Michael Solokov from prc-tools-0.5
and includes support for multiple segments and GLib shared libraries.

%prep
%setup -q

%install
rm -rf $RPM_BUILD_ROOT
mkdir $RPM_BUILD_ROOT
cp -rp . $RPM_BUILD_ROOT

%clean
#rm -rf $RPM_BUILD_ROOT

%files -f /home/jcollins/build/build-rpm/inventory
%defattr(-,root,root)



