#!/bin/bash

if ! test -e "sig/gilc.root"; then
    mkdir -p sig
    cp makeSDigit.C Config.C sig
    cd sig
    ilcroot -b -q makeSDigit.C'(117048,"/ilc/data/2010/LHC10b/000117048/raw/10000117048041.30.root")'
    cd ..
fi
exit;
if ! test -e "bkg/gilc.root"; then
    cp makeSDigit.C Config.C bkg
    mkdir -p bkg
    cd bkg
    ilcroot -b -q makeSDigit.C'(117048,"/ilc/data/2010/LHC10b/000117048/raw/10000117048041.40.root")'
    cd ..
fi

if ! test -e "merged/gilc.root"; then
    mkdir merged
    cp -a sig/*.root merged
    cp merge.C rec.C merged
    cd merged
    ilcroot -b -q merge.C
    ilcroot -b -q rec.C
    cd ..
fi
