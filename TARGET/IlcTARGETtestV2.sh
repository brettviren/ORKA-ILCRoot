#!/bin/sh


# run TPC
ilcroot -q -b "$ILC_ROOT/TPC/IlcTPCtest.C"

# run TARGET
ilcroot -q -b "$ILC_ROOT/TARGET/IlcTARGETtestV2.C"


