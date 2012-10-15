#!/bin/sh
# $Id: build_cuts.sh 36464 2009-11-06 14:18:47Z ivana $

# Script for installation of cuts.cxx program

CURDIR=`pwd`

echo "... Installing cuts program"

g++ -I$ROOTSYS/include \
    `root-config --glibs` -lGeomPainter -lGeom cuts.cxx \
    -o cuts2

echo "... Installing cuts program finished"

cd $CURDIR
