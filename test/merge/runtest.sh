#!/bin/sh
rm -rf */*.root */*.dat* */*.log */fort* */hough */hlt */raw* */*~
cd ./backgr
ilcroot -b -q sim.C\(2\) 2>&1 | tee sim.log
ilcroot -b -q rec.C      2>&1 | tee rec.log
ilcroot -b -q ${ILC_ROOT}/STEER/CheckESD.C 2>&1 | tee check.log
ilcroot -b -q ${ILC_ROOT}/STEER/CreateAODfromESD.C 2>&1 | tee aod.log
chmod a-w *.root
cd ../signal
ilcroot -b -q sim.C\(6\) 2>&1 | tee sim.log
ilcroot -b -q rec.C      2>&1 | tee rec.log
ilcroot -b -q ${ILC_ROOT}/STEER/CheckESD.C 2>&1 | tee check.log
ilcroot -b -q ${ILC_ROOT}/STEER/CreateAODfromESD.C 2>&1 | tee aod.log


