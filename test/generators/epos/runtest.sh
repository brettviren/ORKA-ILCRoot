#!/bin/sh
# Root
# export ROOTSYS=/afs/cern.ch/ilc/library/root/new
# export PATH=$ROOTSYS/bin:$PATH
# export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
# IlcRoot
# export ILC=/afs/cern.ch/ilc/library
# export ILC_LEVEL=new
# export ILC_ROOT=$ILC/$ILC_LEVEL
# export ILC_TARGET=`$ROOTSYS/bin/root-config --arch`
# export PATH=$ILC_ROOT/bin/tgt_${ILC_TARGET}:$PATH
# export LD_LIBRARY_PATH=$ILC_ROOT/lib/tgt_${ILC_TARGET}:$LD_LIBRARY_PATH
# Geant3
# export LD_LIBRARY_PATH=$ILC/geant3/lib/tgt_${ILC_TARGET}:$LD_LIBRARY_PATH

rm -rf *.root *.dat *.log fort* hlt hough raw* recraw/*.root recraw/*.log
ilcroot -b -q $1sim.C      2>&1 | tee sim.log
mv syswatch.log simwatch.log
ilcroot -b -q $1rec.C      2>&1 | tee rec.log
mv syswatch.log recwatch.log
ilcroot -b -q ${ILC_ROOT}/STEER/CheckESD.C 2>&1 | tee check.log
ilcroot -b -q ${ILC_ROOT}/STEER/CreateAODfromESD.C 2>&1 | tee aod.log



