REM  Setup for defining the necessary environmental variables 
REM  for Geant4 development on an Windows machine with VC++.NET

REM Define the OS and Compiler
REM set G4VMCSYSTEM=WIN32-VC

REM geant4_vmc.2.12 is compatible with Geant4 9.4.x

set G4VMCINSTALL=D:\geant4_vmc.2.13
set G4VMCLIB=%G4VMCINSTALL%\lib
set G4VMCDLL=%G4VMCINSTALL%\dll
set G4VMCTMP=%G4VMCINSTALL%\tmp
set G4VMCBIN=%G4VMCINSTALL%\bin
set G4VMCWORKDIR=%G4VMCINSTALL%\work

set G4VMC_INCLUDE=%G4VMCINSTALL%\include
set G4VMC_LIB=%G4VMCINSTALL%\lib\

 
