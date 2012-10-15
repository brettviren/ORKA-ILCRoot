#ifndef ILCLEGOGENERATORETA_H
#define ILCLEGOGENERATORETA_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcLegoGeneratorEta.h 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                                                           //
//    Utility class to compute and draw Radiation Length Map                 //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcLegoGenerator.h"

class IlcLegoGeneratorEta : public IlcLegoGenerator
{

 public:
    IlcLegoGeneratorEta() {}
    virtual ~IlcLegoGeneratorEta() {}
    virtual void    Generate();
    ClassDef(IlcLegoGeneratorEta,1) //Lego GeneratorEta
};

#endif








