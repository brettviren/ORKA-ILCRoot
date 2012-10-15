#!/bin/sh
# $Id: rungen.sh 39165 2010-02-25 16:12:24Z ivana $

NEVENTS=100
GENCONFIG="$ILC_ROOT/test/vmctest/ppbench/genConfig.C" 
OUTDIR=gen

rm -rf *.root *.dat *.log fort* hlt hough raw* recraw/*.root recraw/*.log
ilcroot -b -q gen.C\($NEVENTS,\""$GENCONFIG"\"\)      2>&1 | tee gen.log
rm -fr $OUTDIR
mkdir $OUTDIR
mv gilc.root Kinematics*.root gen.log $OUTDIR

rm -rf *.root *.dat *.log fort* hlt hough raw* recraw/*.root recraw/*.log
