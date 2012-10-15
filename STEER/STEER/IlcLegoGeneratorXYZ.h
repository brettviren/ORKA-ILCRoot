#ifndef ILCLEGOGENERATORXYZ_H
#define ILCLEGOGENERATORXYZ_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcLegoGeneratorXYZ.h 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                                                           //
//    Utility class to compute and draw Radiation Length Map                 //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcLegoGenerator.h"

class IlcLegoGeneratorXYZ : public IlcLegoGenerator
{

 public:
    IlcLegoGeneratorXYZ();
    IlcLegoGeneratorXYZ(char* axis);
    IlcLegoGeneratorXYZ(Int_t nc1, Float_t c1min, Float_t c1max,
			Int_t nc2, Float_t c2min, Float_t c2max,
			Float_t rmin, Float_t rmax, Float_t zmax);
    virtual ~IlcLegoGeneratorXYZ() {}
    virtual void    Generate();
 protected:
    Float_t fDir1[3];  // 1st unit vector spanning the scanning plane
    Float_t fDir2[3];  // 2nd unit vector spanning the scanning plane
    Float_t fDir3[3];  // Direction of flight for geantinos
    
    ClassDef(IlcLegoGeneratorXYZ,1) //Lego GeneratorXYZ
};

#endif








