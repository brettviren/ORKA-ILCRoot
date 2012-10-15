#ifndef ILCEVENTSHAPE_H
#define ILCEVENTSHAPE_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
//---------------------------------------------------------------------
// Event shape utility class
// Circularity, Thrust, ... 
// Authors: Antonio Ortiz Velasquez <Antonio.Ortiz.Velasquez@cern.ch>
//          
//---------------------------------------------------------------------

#include <TObject.h>


class IlcMCEvent;
class TParticle;
class TArrayD;

class IlcEventShape : public TObject
{
  public:
    static TArrayD * GetThrustParamMC(IlcMCEvent* mcEvent, Int_t  NSTUDYMIN=3, Double_t ptcutoff=1, Double_t etacutoff=1, Bool_t chom=kFALSE);
    static Double_t GetCircularityMC(IlcMCEvent* mcEvent, Int_t  NSTUDYMIN=3, Double_t ptcutoff=1, Double_t etacutoff=1, Bool_t chom=kFALSE);
  private:
    IlcEventShape(const IlcEventShape&);
    IlcEventShape& operator=(const IlcEventShape&);
    ClassDef(IlcEventShape, 0)
};

#endif

