#!/bin/bash

rm -f default_geo.root
ilcroot <<"FNORD"
  TFile::Open("$ILC_ROOT/OCDB/GRP/Geometry/Data/Run0_999999999_v0_s0.root")
  gFile->Get("IlcCDBEntry");
  gGeoManager->DefaultColors()
  gGeoManager->Export("default_geo.root", "", "")
  .q
FNORD
