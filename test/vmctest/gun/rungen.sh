#!/bin/sh
# $Id: rungen.sh 37513 2009-12-02 16:01:38Z ivana $

NEVENTS=100
GENCONFIG="$ILC_ROOT/test/vmctest/gun/genConfig.C" 
OUTDIR=gen

rm -rf *.root *.dat *.log fort* hlt hough raw* recraw/*.root recraw/*.log
ilcroot -b -q gen.C\($NEVENTS,\""$GENCONFIG"\"\)      2>&1 | tee gen.log
rm -fr $OUTDIR
mkdir $OUTDIR
mv gilc.root Kinematics*.root gen.log $OUTDIR

rm -rf *.root *.dat *.log fort* hlt hough raw* recraw/*.root recraw/*.log
