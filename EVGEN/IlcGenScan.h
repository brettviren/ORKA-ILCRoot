#ifndef ILCGENSCAN_H
#define ILCGENSCAN_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenScan.h 8644 2003-11-14 11:01:41Z morsch $ */

// Realisation of IlcGenerator that generates particles with
// vertices on a user defined grid.
// Author: andreas.morsch@cern.ch

#include "IlcGenerator.h"

class IlcGenScan : public IlcGenerator
{
public:
    IlcGenScan();
    IlcGenScan(Int_t npart);
    virtual ~IlcGenScan();
    // Set Scanning Range 
    virtual void SetRange(Int_t nx, Float_t xmin, Float_t xmax,
			  Int_t ny, Float_t ymin, Float_t ymax,
			  Int_t nz, Float_t zmin, Float_t zmax);
   
    // Initialise 
    virtual void Init() {}
    // generate event
    virtual void Generate();
    virtual void SetPart(Int_t part) {fIpart=part;}   
 protected:
    Float_t fXCmin;     // Minimum x on grid
    Float_t fXCmax;     // Maximum x on grid
    Int_t   fNx;        // Number of divisions in x
    Float_t fYCmin;     // Minimum y on grid
    Float_t fYCmax;     // Maximum y on grid
    Int_t   fNy;        // Number of divisions in y
    Float_t fZmin;      // Minimum z on grid
    Float_t fZmax;      // Maximum z on grid
    Int_t   fNz;        // Number of divisions in z
    Int_t   fIpart;     // Particle type
    
   
  ClassDef(IlcGenScan,1) //Partcles on a regular grid
};
#endif






