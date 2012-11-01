# -*- mode: cmake -*-

# Top level CMakeList.txt file for the IlcRoot Build System
#
# Author: Johny Jose (johny.jose@cern.ch)
#         Port of previous Makefile build to cmake


cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

#--------------------------------------------------------------------------------#
# Set Basic CMake Configuration                                                  #
#--------------------------------------------------------------------------------#

project(IlcRoot CXX C Fortran)

# Environment variables
set(ILC_ROOT $ENV{ILC_ROOT})
set(ILC $ENV{ILC})
set(ILC_INSTALL $ENV{ILC_INSTALL})
if(${ILC_INSTALL} MATCHES ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Install dir should be different from build dir")
endif(${ILC_INSTALL} MATCHES ${CMAKE_BINARY_DIR})
# Environment for SHUTTLE
set(DIMDIR $ENV{DIMDIR})
set(ODIR $ENV{ODIR})

if(NOT ILC)
  message(WARNING "Environment variable ILC is not set")
endif(NOT ILC)

if(NOT ILC_ROOT)
  message(FATAL_ERROR "Please set ILC_ROOT to the IlcRoot source directory")
endif(NOT ILC_ROOT)

# Set IlcRoot Installation directory 
if(NOT ILC_INSTALL)
  message(STATUS "IlcRoot files will be installed in ${ILC_ROOT}. You can specify a different location by setting ILC_INSTALL environment variable")
  set(ILC_INSTALL ${ILC_ROOT})
else()
  message(STATUS "IlcRoot files will be installed in ${ILC_INSTALL}")
endif(NOT ILC_INSTALL)
set(CMAKE_INSTALL_PREFIX ${ILC_INSTALL})

get_filename_component(PROJECT_SOURCE_DIR "${PROJECT_SOURCE_DIR}" REALPATH)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CPACK_COMPONENTS_ALL MDC)

include(ILC_CMake)

#--------------------------------------------------------------------------------#
# Find Packages                                                                  #
#--------------------------------------------------------------------------------#

# Subversion
# ------------------------------

find_package(Subversion REQUIRED)

# ROOT
# ------------------------------

find_package(ROOT REQUIRED)

# IRST Rule Checker 
# ------------------------------

find_package(RuleChecker)

# DATE
# ------------------------------ 

find_package(DATE)

# GEANT3
# ------------------------------

find_package(GEANT3)

# AMORE
# ------------------------------

find_package(AMORE)

# IlcRoot CPack configuration                                                    
#-------------------------------

find_package(CPack)


#--------------------------------------------------------------------------------#
# Environment/Compiler Configuration                                             #
#--------------------------------------------------------------------------------#

ILC_CheckOutOfSourceBuild()
ILC_RootConfig(CMAKE_CXX_COMPILER --cxx)
ILC_RootConfig(CMAKE_C_COMPILER --cc)
ILC_RootConfig(CMAKE_Fortran_COMPILER --f77)
ILC_ConfigurePlatform()
ILC_ConfigureCompiler()


#--------------------------------------------------------------------------------#
# IlcRoot Path Settings                                                          #
#--------------------------------------------------------------------------------#

ILC_DevFlagsOutput()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin/tgt_${ILC_TARGET}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/tgt_${ILC_TARGET}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib/tgt_${ILC_TARGET}")
set(CMAKE_INCLUDE_EXPORT_DIRECTORY "${PROJECT_BINARY_DIR}/include")
file(MAKE_DIRECTORY ${CMAKE_INCLUDE_EXPORT_DIRECTORY})
set(EXPORTDIRS "${ILC_INSTALL}/include")
set(BINLIBDIRS "-L${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")


#--------------------------------------------------------------------------------#
# IlcRoot Modules                                                                #
#--------------------------------------------------------------------------------#

# Exclude Module from all target
# ------------------------------

#set(EXCLUDEMODULES THydjet SHUTTLE)


# Standard Modules
# ------------------------------
# set(ILCROOTMODULES STEER PHOS TRD TPC ZDC MUON PMD FMD TOF ITS ACORDE HMPID T0 BCM STRUCT VZERO THijing THbtp EMCAL THerwig TEPEMGEN FASTSIM TPHIC RAW MONITOR ANALYSIS JETAN HLT STAT TTherminator CORRFW DPMJET TDPMjet PWGPP PWG PWGCF PWGGA PWGHF PWGDQ PWGJE PWGLF PWGUD TRIGGER PYTHIA8 TAmpt OADB MFT TEvtGen)

set(ILCROOTMODULES STEER PVBAR PVEC RSTACK DCH TARGET STRUCT THijing THbtp THerwig TEPEMGEN FASTSIM TPHIC RAW DISPLAY ANALYSIS JETAN STAT TTherminator DPMJET TDPMjet PYTHIA8 TAmpt OADB)

if(THydjet)
  list(APPEND ILCROOTMODULES THydjet)
endif(THydjet)

# if(SHUTTLE)
#   list(APPEND ILCROOTMODULES SHUTTLE)
# endif(SHUTTLE)

# Additional Modules
# ------------------------------
list(APPEND ILCROOTMODULES TUHKMgen EPOS)

# EVE
# ------------------------------
ILC_RootConfig(EVE --has-opengl)
if(NOT EVE STREQUAL "no")
  list(APPEND ILCROOTMODULES EVE)
endif(NOT EVE STREQUAL "no")

# CERN Modules
# ------------------------------
# set(CERNMODULES LHAPDF HIJING MICROCERN HERWIG)
set(CERNMODULES HIJING MICROCERN HERWIG)

# Check for Pythia 6
if(EXISTS ${ROOTINCDIR}/TPythia6.h)
  set(PYTHIA6 TRUE)
  list(APPEND CERNMODULES PYTHIA6 EVGEN)
elseif(NOT NOPYTHIA)
  message(FATAL_ERROR "ROOT must have Pythia6 enabled !")
endif(EXISTS ${ROOTINCDIR}/TPythia6.h)

# Set Final list of modules 
set(MODULES ${ILCROOTMODULES} ${CERNMODULES} ILCROOT)

# Set Module directories
set(MODDIRS ${MODULES})

# Libraries for Linking
# ------------------------------
# set(ILCLIBS MUON TPC PMD TRD FMD TOF ITS PHOS ACORDE HMPID VZERO ZDC STRUCT T0 EVGEN STEER TRIGGER MFT)
set(ILCLIBS PVBAR PVEC RSTACK DCH TARGET STRUCT EVGEN STEER)
set(LIBS ${ROOTCLIBS} ${ROOTPLIBS} ${SYSLIBS})

# Configure ARVerion.h
# ------------------------------
if(Subversion_FOUND)
  if(EXISTS ${PROJECT_SOURCE_DIR}/.svn/ )
    Subversion_WC_INFO(${PROJECT_SOURCE_DIR} PROJECT)
    set(ILCROOT_SVN_REVISION ${PROJECT_WC_REVISION})
    set(ILCROOT_SVN_BRANCH ${PROJECT_WC_URL})
    string(REGEX MATCH "[^/]+$" ILCROOT_SVN_BRANCH ${ILCROOT_SVN_BRANCH})

  elseif(EXISTS ${PROJECT_SOURCE_DIR}/../.svn/ )
    Subversion_WC_INFO(${PROJECT_SOURCE_DIR} PROJECT)
    set(ILCROOT_SVN_REVISION ${PROJECT_WC_REVISION})
    set(ILCROOT_SVN_BRANCH ${PROJECT_WC_URL})
    string(REGEX MATCH "[^/]+$" ILCROOT_SVN_BRANCH ${ILCROOT_SVN_BRANCH})

  # git-svn repository
  elseif(EXISTS ${PROJECT_SOURCE_DIR}/.git/svn/ )
    execute_process(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND git svn info OUTPUT_VARIABLE OUTPUTVAR)
    string(REPLACE "\n" ";" OUTPUTVAR ${OUTPUTVAR})
    foreach(f ${OUTPUTVAR})
      if(${f} MATCHES URL)
	string(REGEX MATCH "[^/]+$" ILCROOT_SVN_BRANCH ${f})
      elseif(${f} MATCHES Revision)
	string(REGEX MATCH "[^: ]+$" ILCROOT_SVN_REVISION ${f})
      endif(${f} MATCHES URL)   
    endforeach()

  # pure git repository
  elseif(EXISTS ${PROJECT_SOURCE_DIR}/.git/)
    execute_process(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND git describe --contains HEAD OUTPUT_VARIABLE ILCROOT_SVN_BRANCH)
    execute_process(WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} COMMAND git show-ref -s HEAD OUTPUT_VARIABLE ILCROOT_SVN_REVISION)


  else(NOT ${PROJECT_SOURCE_DIR}/.svn/)
    set(ILCROOT_SVN_REVISION -1)
    set(ILCROOT_SVN_BRANCH unknown)

  endif(EXISTS ${PROJECT_SOURCE_DIR}/.svn/ )

  message(STATUS "ILCroot Revision - ${ILCROOT_SVN_REVISION} Branch - ${ILCROOT_SVN_BRANCH}")
  if(EXISTS ${CMAKE_INCLUDE_EXPORT_DIRECTORY}/ARVersion.h)
    file(REMOVE ${CMAKE_INCLUDE_EXPORT_DIRECTORY}/ARVersion.h)
  endif(EXISTS ${CMAKE_INCLUDE_EXPORT_DIRECTORY}/ARVersion.h)
  configure_file(${PROJECT_SOURCE_DIR}/cmake/ARVersion.h.tmp ${CMAKE_INCLUDE_EXPORT_DIRECTORY}/ARVersion.h @ONLY)


endif(Subversion_FOUND)
string (REPLACE "-" "." ILCROOT_SVN_BRANCH_SPEC "${ILCROOT_SVN_BRANCH}")

#--------------------------------------------------------------------------------#
# Build Modules                                                                  #
#--------------------------------------------------------------------------------#

# Module Variables
# ------------------------------
set(ALLLIBS)
set(ALLEXECS)
set(INCLUDEFILES)
set(BINLIBS)
set(EXPORTFILES)

# Top Level Build Targets
# ------------------------------

add_custom_target(alilibs-static)
add_custom_target(include-headers ALL)
add_custom_target(DA-all)
add_custom_target(check-all) 
add_custom_target(check-hxml)
add_custom_target(smell-all) 
add_custom_target(htmldoc) 
add_custom_target(par-all)
add_custom_target(test-par-all)

#NY file(GLOB CFILES_LIST ${CMAKE_SOURCE_DIR}/macros/*.C)

add_custom_command(TARGET htmldoc
		   PRE_BUILD
		   COMMAND mkdir -p html
		   COMMAND rm -rf html/roothtml
		   COMMAND rm -f html/picts
		   COMMAND cd html &&
		   	    ilcroot -q -b \"mkhtml.C(0,1)\"
		   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})


# Add Modules	
# ------------------------------
foreach(MODULE ${MODDIRS})
  set(${MODULE}ALIBS)
  configure_file(${PROJECT_SOURCE_DIR}/cmake/Module.tmp ${PROJECT_SOURCE_DIR}/${MODULE}/CMakeLists.txt @ONLY)
  add_custom_target(${MODULE}-all ALL)
  add_custom_target(${MODULE}-par-all)
  add_custom_target(test-${MODULE}-par-all)
  add_custom_target(${MODULE}-all-static)
  add_dependencies(alilibs-static "${MODULE}-static")
  add_subdirectory(${MODULE})
  
#NY  set(dir "${CMAKE_SOURCE_DIR}/${MODULE}")
#NY  if (EXISTS "${dir}")
#NY      file(GLOB CFILES_MOD ${dir}/*.C)
#NY      list(LENGTH CFILES_MOD len)
#NY      if (${len} GREATER 0)
#NY        list(APPEND CFILES_LIST "${CFILES_MOD}")
#NY      endif (${len} GREATER 0)
#NY  endif(EXISTS "${dir}")
endforeach(MODULE)

#NY foreach(cfile ${CFILES_LIST})
#NY     add_custom_command(TARGET htmldoc
#NY 		       PRE_BUILD
#NY 		       COMMAND cd html && 
#NY 		    	       ilcroot -b -q \"mkhtml.C(\"${cfile}\")\" > /dev/null
#NY  		       WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
#NY endforeach(cfile)

if(DATE_FOUND)
  include (CMakeDA)
endif(DATE_FOUND)

if(RULECHECKER_FOUND)
    add_custom_target(factfile DEPENDS ${FACTFILE})
    add_custom_command( OUTPUT ${FACTFILE}
                        COMMAND ${CMAKE_COMMAND} -E echo "Re-generating factfile"
                        COMMAND ${JAVA_RUNTIME}  -Xmx1024M -jar ${FACTEXTRACTOR_JAR} ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}			
			DEPENDS check-hxml ${CMAKE_BINARY_DIR}/check-hxml-touchfile ${_factfile_deps} 
                        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif(RULECHECKER_FOUND)

#--------------------------------------------------------------------------------#
# Additional Targets                                                             #
#--------------------------------------------------------------------------------#

# IlcMdc RPM
# ------------------------------

execute_process(
COMMAND root-config --libdir 
OUTPUT_VARIABLE ROOTALIBDIR)
string(REPLACE "\n" "" ROOTALIBDIR ${ROOTALIBDIR}) 

execute_process(
COMMAND root-config --arch 
OUTPUT_VARIABLE ARCHITECTURE)


set(AEXT a)
set(LIBPATH ${CMAKE_BINARY_DIR}/lib/tgt_${ILC_TARGET})
set(BINPATH ${CMAKE_BINARY_DIR}/bin/tgt_${ILC_TARGET})
set(EXPORTDIR ${ILC_ROOT}/include)
set(BINLIBDIRS -L${ILC_ROOT}/${LIBPATH})
set(RAWDIRO ${CMAKE_BINARY_DIR}/RAW)
set(ILCMDCSPECFILE ${RAWDIRO}/ilcmdc.spec)
set(RAWDatabaseALIB ${LIBPATH}/libRAWDatabase.${AEXT})
set(ESDALIB ${LIBPATH}/libESD.${AEXT})
set(STEERBaseALIB ${LIBPATH}/libSTEERBase.${AEXT})
set(MDCALIB ${LIBPATH}/libMDC.${AEXT})
set(MODDIRO RAW/tgt_${ILC_TARGET})


if( ${ARCHITECTURE} MATCHES "linuxx8664gcc" )
set(ILCMDCARCHDIR x86_64)
elseif( ${ARCHITECTURE} MATCHES "linux" )
set(ILCMDCARCHDIR i386)
elseif( ${ARCHITECTURE} MATCHES "linuxia64gcc" )
set(ILCMDCARCHDIR ia64)
elseif(NOT DEFINED ${ARCHITECTURE})
message("ilcmdc-rpm: Unknown architecture" )
else( ${ARCHITECTURE} MATCHES "linuxx8664gcc" )
message("ilcmdc-rpm: Unknown architecture" )
endif( ${ARCHITECTURE} MATCHES "linuxx8664gcc" )


add_custom_target( ilcmdc-rpm
DEPENDS ilcmdc-static ilcmdc-specfile
)

add_custom_command(
TARGET ilcmdc-rpm
POST_BUILD 
COMMAND rm -rf ${ILC_ROOT}/ilcmdc-root
COMMAND mkdir -p ${ILC_ROOT}/ilcmdc-root/opt/ilcmdc/lib
COMMAND mkdir -p ${ILC_ROOT}/ilcmdc-root/opt/ilcmdc/include
COMMAND cp ${ILC_ROOT}/RAW/mdc.h ${ILC_ROOT}/ilcmdc-root/opt/ilcmdc/include
COMMAND cp ${LIBPATH}/libIlcMDC.a ${ILC_ROOT}/ilcmdc-root/opt/ilcmdc/lib
COMMAND cp ${ROOTALIBDIR}/libRoot.a ${ILC_ROOT}/ilcmdc-root/opt/ilcmdc/lib
COMMAND cp ${ROOTALIBDIR}/liblzma.a ${ILC_ROOT}/ilcmdc-root/opt/ilcmdc/lib
COMMAND cp ${ROOTALIBDIR}/libpcre.a ${ILC_ROOT}/ilcmdc-root/opt/ilcmdc/lib
COMMAND cp ${ROOTALIBDIR}/libfreetype.a ${ILC_ROOT}/ilcmdc-root/opt/ilcmdc/lib
COMMAND cp ${ROOTALIBDIR}/libpcre.a ${ILC_ROOT}/ilcmdc-root/opt/ilcmdc/lib
COMMAND rm -rf ${ILC_ROOT}/RPMS
COMMAND mkdir -p ${ILC_ROOT}/RPMS/${ILCMDCARCHDIR}/
COMMAND rpmbuild --verbose --define "_topdir ${ILC_ROOT}" --define "_tmppath ${ILC_ROOT}" -bb ${ILCMDCSPECFILE} 
COMMAND cp -p ${ILC_ROOT}/RPMS/${ILCMDCARCHDIR}/ilcmdc-*.rpm .
COMMAND rm -rf ${ILC_ROOT}/ilcmdc-root
COMMAND rm -rf ${ILC_ROOT}/RPMS
COMMAND echo "***** ilcmdc RPM created and put in ${CMAKE_BINARY_DIR} folder *****"
)

add_custom_command(
TARGET ${RAWDIRO}
COMMAND mkdir-p ${RAWDIRO}
)


add_custom_target( ilcmdc-specfile
DEPENDS ${RAWDIRO}
)

add_custom_command( 
TARGET ilcmdc-specfile
POST_BUILD
COMMAND rm -rf ${ILCMDCSPECFILE}
COMMAND @echo "***** Making ilcmdc RPM spec-file ${ILCMDCSPECFILE} *****"
COMMAND @echo '\# RPM specfile for ilcmdc static libs' >> ${ILCMDCSPECFILE} 
COMMAND @echo '\# Package contains both ROOT and IlcRoot' >> ${ILCMDCSPECFILE}
COMMAND @echo '\# static libs needed by mStreamRecorder' >> ${ILCMDCSPECFILE}
COMMAND @echo '\# in order to ROOT-ify the incoming raw' >> ${ILCMDCSPECFILE}
COMMAND @echo '\# data' >> ${ILCMDCSPECFILE}
COMMAND @echo '\# Example how-to build ilcmdc RPM:' >> ${ILCMDCSPECFILE}
COMMAND @echo '\# cd ${ILC_ROOT}' >> ${ILCMDCSPECFILE}
COMMAND @echo '\# make ilcmdc-rpm' >> ${ILCMDCSPECFILE}
COMMAND @echo "" >> ${ILCMDCSPECFILE}
COMMAND @echo "Summary: IlcMDC static libraries" >> ${ILCMDCSPECFILE}
COMMAND @echo "Name: ilcmdc" >> ${ILCMDCSPECFILE}
COMMAND @echo "Version:  ${ILCROOT_SVN_BRANCH_SPEC}" >> ${ILCMDCSPECFILE}
COMMAND @echo "Release: ${ILCROOT_SVN_REVISION}" >> ${ILCMDCSPECFILE}
COMMAND @echo '\# Copyright: CERN Ilc Off-line' >> ${ILCMDCSPECFILE}
COMMAND @echo "License: CERN Ilc Off-line" >> ${ILCMDCSPECFILE}
COMMAND @echo "Vendor: ILC Core Off-line Group" >> ${ILCMDCSPECFILE}
COMMAND @echo "URL: http://ilcinfo.cern.ch" >> ${ILCMDCSPECFILE}
COMMAND @echo "Group: Applications/Ilc" >> ${ILCMDCSPECFILE}
COMMAND @echo "Prefix: /opt/%{name}" >> ${ILCMDCSPECFILE}
COMMAND @echo "BuildRoot: %{_tmppath}/%{name}-root" >> ${ILCMDCSPECFILE}
COMMAND @echo "" >> ${ILCMDCSPECFILE}
COMMAND @echo '\# automatic dependencies' >> ${ILCMDCSPECFILE}
COMMAND @echo "AutoReqProv: yes" >> ${ILCMDCSPECFILE}
COMMAND @echo "" >> ${ILCMDCSPECFILE}
COMMAND @echo '\# list here required RPM packages for runtime' >> ${ILCMDCSPECFILE}
COMMAND @echo "Requires: glibc" >> ${ILCMDCSPECFILE}
COMMAND @echo "" >> ${ILCMDCSPECFILE}
COMMAND @echo "Provides: ilcmdc" >> ${ILCMDCSPECFILE}
COMMAND @echo "" >> ${ILCMDCSPECFILE}
COMMAND @echo '\# description of the package' >> ${ILCMDCSPECFILE}
COMMAND @echo "%description" >> ${ILCMDCSPECFILE}
COMMAND @echo "Package contains both ROOT and IlcRoot" >> ${ILCMDCSPECFILE}
COMMAND @echo "static libs needed by mStreamRecorder" >> ${ILCMDCSPECFILE}
COMMAND @echo "in order to ROOT-ify the incoming raw" >> ${ILCMDCSPECFILE}
COMMAND @echo "data. The package version correspond to" >> ${ILCMDCSPECFILE}
COMMAND @echo "the IlcRoot one." >> ${ILCMDCSPECFILE}
COMMAND @echo "" >> ${ILCMDCSPECFILE}
COMMAND @echo '\# list of files to be installed' >> ${ILCMDCSPECFILE}
COMMAND @echo "%files" >> ${ILCMDCSPECFILE}
COMMAND @echo '%defattr (-,root,root)' >> ${ILCMDCSPECFILE}
COMMAND @echo "%{prefix}/lib/libIlcMDC.a" >> ${ILCMDCSPECFILE}
COMMAND @echo "%{prefix}/lib/libRoot.a" >> ${ILCMDCSPECFILE}
COMMAND @echo "%{prefix}/lib/libpcre.a" >> ${ILCMDCSPECFILE}
COMMAND @echo "%{prefix}/lib/liblzma.a" >> ${ILCMDCSPECFILE}
COMMAND @echo "%{prefix}/lib/libfreetype.a" >> ${ILCMDCSPECFILE}
COMMAND @echo "%{prefix}/include/mdc.h" >> ${ILCMDCSPECFILE}
)


add_custom_target( ilcmdc-static
DEPENDS ${LIBPATH} ${BINPATH} RAW-all-static STEER-all-static )

add_custom_command( 
TARGET ilcmdc-static
POST_BUILD
COMMAND ar r ${LIBPATH}/ilcmdccxxo.a ${CMAKE_BINARY_DIR}/RAW/CMakeFiles/ilcmdc.dir/ilcmdc_main.cxx.o
COMMAND rm -rf ${LIBPATH}/libIlcMDC.a
COMMAND ar x ${RAWDatabaseALIB}
COMMAND ar x ${MDCALIB}
COMMAND ar x ${ESDALIB}
COMMAND ar x ${STEERBaseALIB}
COMMAND ar r ${LIBPATH}/libIlcMDC.a *.o
COMMAND ar t ${RAWDatabaseALIB} | xargs rm 
COMMAND ar t ${MDCALIB} | xargs rm 
COMMAND ar t ${ESDALIB} | xargs rm 
COMMAND ar t ${STEERBaseALIB} | xargs rm 
COMMAND ${CMAKE_CXX_COMPILER} ${LDFLAGS} -o ${BINPATH}/ilcmdca ${LIBPATH}/ilcmdccxxo.a ${LIBPATH}/libIlcMDC.a ${ROOTALIBDIR}/libRoot.a ${ROOTALIBDIR}/libfreetype.a ${ROOTALIBDIR}/libpcre.a ${ROOTALIBDIR}/liblzma.a -pthread -ldl -lz
COMMAND @echo "Target ilcmdc-static made"
)

add_custom_command(
TARGET ${LIBPATH}
COMMAND mkdir -p ${LIBPATH}
)


#include (MakefileDA) 
include (InstallRequiredSystemLibraries)
include (CPack)

# Install data
# ------------------------------
install ( DIRECTORY data 
          DESTINATION . )
install ( DIRECTORY macros 
          DESTINATION . )
install ( DIRECTORY OCDB 
	  DESTINATION . 
	  PATTERN ".svn" EXCLUDE)

#
# EOF
#

