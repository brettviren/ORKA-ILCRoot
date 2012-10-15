#!/bin/sh
# $Id: runtest.sh 48270 2011-03-10 09:20:51Z ivana $

# Before running this script, you should run rungen.sh first.

NEVENTS=3
G3CONFIG="$ILC_ROOT/test/vmctest/ppbench/g3Config.C" 
G4CONFIG="$ILC_ROOT/test/vmctest/ppbench/g4Config.C" 
G3OUTDIR=g3
G4OUTDIR=g4

RUNG3=1
RUNG4=1

if [ "$RUNG3" = "1" ]; then 
  rm -rf *.root *.dat *.log fort* hlt hough raw* recraw/*.root recraw/*.log
  ilcroot -b -q  sim.C\($NEVENTS,\""$G3CONFIG"\"\)  2>&1 | tee sim.log
  mv syswatch.log simwatch.log
  ilcroot -b -q rec.C      2>&1 | tee rec.log
  mv syswatch.log recwatch.log
  ilcroot -b -q ${ILC_ROOT}/STEER/CheckESD.C 2>&1 | tee check.log
  ilcroot -b -q ${ILC_ROOT}/STEER/CreateAODfromESD.C 2>&1 | tee aod.log
  rm -fr $G3OUTDIR
  mkdir $G3OUTDIR
  mv *.root *.log *.ps GRP $G3OUTDIR
  cp g3Config.C $G3OUTDIR
fi

if [ "$RUNG4" = "1" ]; then 
  rm -rf *.root *.dat *.log fort* hlt hough raw* recraw/*.root recraw/*.log
  ilcroot -b -q  sim.C\($NEVENTS,\""$G4CONFIG"\"\)  2>&1 | tee sim.log
  mv syswatch.log simwatch.log
  ilcroot -b -q rec.C      2>&1 | tee rec.log
  mv syswatch.log recwatch.log
  ilcroot -b -q ${ILC_ROOT}/STEER/CheckESD.C 2>&1 | tee check.log
  ilcroot -b -q ${ILC_ROOT}/STEER/CreateAODfromESD.C 2>&1 | tee aod.log
  rm -fr $G4OUTDIR
  mkdir $G4OUTDIR
  mv *.root *.log *.ps GRP $G4OUTDIR
  cp g4Config.C $G4OUTDIR
fi
