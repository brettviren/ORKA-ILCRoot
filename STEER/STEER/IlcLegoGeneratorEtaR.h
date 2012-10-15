#ifndef ILCLEGOGENERATORETAR_H
#define ILCLEGOGENERATORETAR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcLegoGeneratorEtaR.h 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                                                           //
//    Utility class to compute and draw Radiation Length Map                 //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcLegoGenerator.h"

class IlcLegoGeneratorEtaR : public IlcLegoGenerator
{

 public:
    IlcLegoGeneratorEtaR() {}
    virtual ~IlcLegoGeneratorEtaR() {}
    virtual void    Generate();
    ClassDef(IlcLegoGeneratorEtaR, 1) // Lego GeneratorEtaR
};

#endif








