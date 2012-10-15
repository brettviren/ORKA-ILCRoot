#ifndef ILCLEGOGENERATORPHIZ_H
#define ILCLEGOGENERATORPHIZ_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcLegoGeneratorPhiZ.h 50615 2011-07-16 23:19:19Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                                                           //
//    Utility class to compute and draw Radiation Length Map                 //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcLegoGenerator.h"

class IlcLegoGeneratorPhiZ : public IlcLegoGenerator
{

 public:
  IlcLegoGeneratorPhiZ(){;}
  virtual ~IlcLegoGeneratorPhiZ(){;}
    virtual void    Generate();
    ClassDef(IlcLegoGeneratorPhiZ,1) //Lego GeneratorPhiZ
};
#endif








