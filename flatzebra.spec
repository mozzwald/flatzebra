Name:           flatzebra
Version:        0.1.6
Release:        1%{?dist}
Summary:        Small Engine for 2D video games like BurgerSpace
Summary(fr):    Petit moteur pour jeux vidéo 2D comme BurgerSpace

Group:          Development/Libraries
License:        GPLv2+
URL:            http://sarrazip.com/dev/burgerspace.html
Source0:        http://sarrazip.com/dev/%{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  SDL-devel       >= 1.2.4
BuildRequires:  SDL_image-devel >= 1.2.2
BuildRequires:  SDL_mixer-devel >= 1.2.4

%description
Small game engine library required by BurgerSpace and other games.

%description -l fr
Petit moteur de jeu requis par BurgerSpace et autres jeux.


%package devel
Summary:        C++ header files for the flatzebra library
Summary(fr):    En-têtes C++ pour la librairie flatzebra.
Group:          Development/Libraries
Requires:       %{name} = %{version}-%{release}

%description devel
C++ header files to develop with the flatzebra library.

%description -l fr devel
En-têtes C++ pour développer avec la librairie flatzebra.


%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT

 
%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%defattr(-,root,root,-)
%{_libdir}/lib*.so.*
%doc %{_defaultdocdir}/*


%files devel
%defattr(-,root,root,-)
%{_includedir}/*
%{_libdir}/lib*.so
%{_libdir}/lib*.la
%{_libdir}/pkgconfig/*


%changelog
