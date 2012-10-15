# RPM specfile for ilcmdc static libs

# Package contains both ROOT and IlcRoot
# static libs needed by mStreamRecorder
# in order to ROOT-ify the incoming raw
# data
# Example:
# in $ILC_ROOT: mkdir RPMS; mkdir RPMS/i386;
# then: rpmbuild --verbose --define "_topdir $ILC_ROOT" --define "_tmppath $ILC_ROOT" -bb RAW/ilcmdc.spec
# then: cp $ILC_ROOT/RPMS/i386/ilcmdc-*.rpm <destination>

Summary: IlcMDC static libraries
Name: ilcmdc
Version: 4.09.Release
Release: 1.SLC4
# Copyright: CERN Ilc Off-line
License: CERN Ilc Off-line
Vendor: ILC Core Off-line Group
URL: http://ilcinfo.cern.ch
Group: Applications/Ilc
Prefix: /opt/%{name}
BuildRoot: %{_tmppath}/%{name}-root

# automatic dependencies
AutoReqProv: yes

# list here required RPM packages for runtime
Requires: glibc

Provides: ilcmdc

# description of the package
%description
Package contains both ROOT and IlcRoot
static libs needed by mStreamRecorder
in order to ROOT-ify the incoming raw
data. The package version correspond to
the IlcRoot one.

# list of files to be installed
%files
%defattr (-,root,root)
%{prefix}/lib/libIlcMDC.a
%{prefix}/lib/libRoot.a
%{prefix}/lib/libpcre.a
%{prefix}/lib/libfreetype.a
%{prefix}/include/mdc.h
