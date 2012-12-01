REM  Setup for defining the necessary environmental variables 
REM  for Geant4 development on an Windows machine with VC++.NET

REM Define the OS and Compiler
set G4SYSTEM=WIN32-VC

set G4INSTALL=D:\GEANT4_9_5_p01
set G4LIB=%G4INSTALL%\lib\%G4SYSTEM%
set G4TMP=%G4INSTALL%\tmp
set G4BIN=%G4INSTALL%\bin
set G4WORKDIR=%G4INSTALL%\work
set G4DLL=%G4INSTALL%\dll\%G4SYSTEM%

set G4LEVELGAMMADATA=%G4INSTALL%\DATA\PhotonEvaporation2.0
set G4LEDATA=%G4INSTALL%\DATA\G4EMLOW6.23
set G4NEUTRONHPDATA=%G4INSTALL%\DATA\G4NDL3.13

set G4_INCLUDE=%G4INSTALL%\include
set G4_LIB=%G4INSTALL%\lib\%G4SYSTEM%

REM Next are for VMC
set G4INCLUDE=%G4_INCLUDE%
set G4LIB=%G4_LIB%

set G4LIB_BUILD_ZLIB=1
set G4LIB_USE_ZLIB=1
set G4LIB_BUILD_STATIC=1

REM CLHEP installation

set CLHEP_BASE_DIR=D:\CLHEP\2.1.1.0
set CLHEP_INCLUDE=%CLHEP_BASE_DIR%\include
set CLHEP_LIB=%CLHEP_BASE_DIR%\lib
 
REM XercesC root for GDML
set XERCESCROOT=D:/XercesC/2.8.0
set XERCESC_INCLUDE=%XERCESCROOT%/include
set XERCESC_LIB=%XERCESCROOT%/lib

REM OpenGL
set G4VIS_BUILD_OPENGLX_DRIVER=1
set G4VIS_USE_OPENGLX=1
set OGLHOME=D:\Program Files\Microsoft SDKs\Windows\v6.0A
set OGLFLAGS="-I$OGLHOME/include"
set OGLLIBS="-LD:\Program Files\Microsoft SDKs\Windows\v6.0A\Lib -lGL -lGLU"

REM OpenGL
set HEPMC_DIR=D:\HepMC-2.06.05\

