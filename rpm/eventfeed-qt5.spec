%define _name eventfeed
Name:       libeventfeed-qt5

Summary:    D-Bus interface for MeegoTouch Events
Version:    0.1.4
Release:    0
Group:      System/GUI/Other
License:    BSD License
URL:        http://www.meego.com
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Sql)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  qt5-tools
BuildRequires:  doxygen

%description
This library provides D-Bus interface to MeegoTouch Events.

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/libeventfeed-qt5.so.*
%exclude %{_usr}/bin/eventfeedd
%exclude %{_datadir}/dbus-1/*
%exclude %{_usr}/bin/fakefeeder
%exclude %{_libdir}/debug/*
%exclude %{_libdir}/debug/.build-id/*

%package -n libeventfeed-qt5-devel
Summary:    Development files for libeventfeed
License:    BSD License
Group:      Development/Libraries
Requires:   libeventfeed-qt5 = %{version}

%description -n libeventfeed-qt5-devel
This package contains development files for libeventfeed.

%files -n libeventfeed-qt5-devel
%defattr(-,root,root,-)
%{_includedir}/eventfeed-qt5/*.h
%{_datadir}/qt5/mkspecs/features/eventfeed-qt5.prf
%{_libdir}/libeventfeed-qt5.so

%package -n libeventfeed-qt5-tests
Summary:    Test suite for libeventfeed
License:    BSD License
Group:      Development/Libraries
Requires:   libeventfeed-qt5 = %{version}

%description -n libeventfeed-qt5-tests
This package contains test suite for libeventfeed.

%files -n libeventfeed-qt5-tests
%defattr(-,root,root,-)
/opt/tests/libeventfeed-qt5/*

%package -n libmeegotouchevents-qt5
Summary:    D-Bus interface for MeegoTouch Events
Group:      Applications/System
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n libmeegotouchevents-qt5
This library provides D-Bus interface to MeegoTouch Events.

%post -n libmeegotouchevents-qt5 -p /sbin/ldconfig
%postun -n libmeegotouchevents-qt5 -p /sbin/ldconfig

%files -n libmeegotouchevents-qt5
%defattr(-,root,root,-)
%{_libdir}/libmeegotouchevents-qt5.so.*

%package -n libmeegotouchevents-qt5-devel
Summary:    Development files for libmeegotouchevents-qt5
Group:      Development/Libraries
Requires:   libmeegotouchevents-qt5 = %{version}

%description -n libmeegotouchevents-qt5-devel
This package contains development files for libmeegotouchevents-qt5.

%files -n libmeegotouchevents-qt5-devel
%defattr(-,root,root,-)
%{_libdir}/libmeegotouchevents-qt5.so
%{_includedir}/meegotouchevents-qt5/*.h
%{_datadir}/qt5/mkspecs/features/meegotouchevents-qt5.prf

%package -n libmeegotouchevents-qt5-doc
Summary:    Documentation for libmeegotouchevents-qt5
Group:      Documentation

%description -n libmeegotouchevents-qt5-doc
This package contains documentation for libmeegotouchevents.

%files -n libmeegotouchevents-qt5-doc
%defattr(-,root,root,-)
%{_datadir}/doc/libmeegotouchevents-qt5/*

%prep
%setup -q -n %{name}-%{version}

%build

%qmake5

QT_SELECT=5 make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake_install
