#ifndef ILCFASTPARTICLE_H
#define ILCFASTPARTICLE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFastParticle.h 7865 2003-07-13 09:26:14Z hristov $ */

#include <TParticle.h>
class IlcFastParticle : public TParticle {
 public:
    IlcFastParticle(){;}
    virtual ~IlcFastParticle(){;}
 protected:
    ClassDef(IlcFastParticle,1) // Base class for fast particle
};

#endif 



