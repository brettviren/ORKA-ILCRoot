#!/bin/sh
rm -rf */*.root */*.log */*.dat
cd ./gen
ilcroot -b -q rungen.C\(5\) 2>&1 | tee gen.log
chmod a-w *.root
cd ../sim
ilcroot -b -q sim.C\(5\) 2>&1 | tee sim.log
ilcroot -b -q rec.C      2>&1 | tee rec.log
ilcroot -b -q ${ILC_ROOT}/STEER/CheckESD.C 2>&1 | tee check.log
ilcroot -b -q ${ILC_ROOT}/STEER/CreateAODfromESD.C 2>&1 | tee aod.log


