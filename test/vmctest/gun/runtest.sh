#!/bin/sh
# $Id: runtest.sh 48270 2011-03-10 09:20:51Z ivana $

# Before running this script, you should run rungen.sh first.

NEVENTS=20
G3CONFIG="$ILC_ROOT/test/vmctest/gun/g3Config.C" 
G4CONFIG="$ILC_ROOT/test/vmctest/gun/g4Config.C" 
G3OUTDIR=g3
G4OUTDIR=g4

RUNG3=1
RUNG4=1

if [ "$RUNG3" = "1" ]; then 
  rm -rf *.root *.dat *.log fort* hlt hough raw* recraw/*.root recraw/*.log
  ilcroot -b -q  sim.C\($NEVENTS,\""$G3CONFIG"\"\)  2>&1 | tee sim.log
  ilcroot -b -q rec.C      2>&1 | tee rec.log
  rm -fr $G3OUTDIR
  mkdir $G3OUTDIR
  mv *.root *.log GRP $G3OUTDIR
  cp g3Config.C $G3OUTDIR
fi

if [ "$RUNG4" = "1" ]; then 
  rm -rf *.root *.dat *.log fort* hlt hough raw* recraw/*.root recraw/*.log
  ilcroot -b -q  sim.C\($NEVENTS,\""$G4CONFIG"\"\)  2>&1 | tee sim.log
  ilcroot -b -q rec.C      2>&1 | tee rec.log
  rm -fr $G4OUTDIR
  mkdir $G4OUTDIR
  mv *.root *.log GRP $G4OUTDIR
  cp g4Config.C $G4OUTDIR
fi
