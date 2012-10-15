#ifndef ILCGENDOUBLESCAN_H
#define ILCGENDOUBLESCAN_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenDoubleScan.h 2661 2000-12-21 16:24:06Z morsch $ */

// As IlcGenScan,  generation of particles on a 3-dim grid
// but here double hits with a predefined distance are generated.
// Author: andreas.morsch@cern.ch

#include "IlcGenScan.h"

class IlcGenDoubleScan : public IlcGenScan
{
 public:
    IlcGenDoubleScan();
    IlcGenDoubleScan(Int_t npart);
    virtual ~IlcGenDoubleScan();
    virtual void Generate();
    virtual void SetDistance(Float_t d) {fDistance=d;}
 private:
    Float_t fDistance;           // Distance between particles
    ClassDef(IlcGenDoubleScan,1) // Generation of particles (double hits) on a grid
};
#endif






