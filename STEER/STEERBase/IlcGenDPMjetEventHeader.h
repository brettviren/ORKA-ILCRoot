#ifndef ILCGENDPMJETEVENTHEADER_H
#define ILCGENDPMJETEVENTHEADER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

#include <TLorentzVector.h>

#include "IlcGenEventHeader.h"
#include "IlcCollisionGeometry.h"

class IlcGenDPMjetEventHeader : public IlcGenEventHeader, public IlcCollisionGeometry
{
 public:
    IlcGenDPMjetEventHeader(const char* name);
    IlcGenDPMjetEventHeader();
    virtual ~IlcGenDPMjetEventHeader() {}

  // Getters
  Float_t TotalEnergy()  const {return fTotalEnergy;} 
  Int_t   Trials()       const {return fTrials;}
  Int_t   ProcessType()  const {return fProcessType;}
  
	  
  // Setters
  void SetTotalEnergy(Float_t energy)  {fTotalEnergy = energy;}
  void SetTrials(Int_t trials)         {fTrials      = trials;}
  void SetProcessType(Int_t type)      {fProcessType = type;}
	  
protected:
  Float_t fTotalEnergy;              // Total energy of produced particles
  Int_t   fTrials;                   // Number of trials to fulfill trigger condition
  Int_t   fProcessType;              // Process Type 
  
  ClassDef(IlcGenDPMjetEventHeader,1) // Event header for dpmjet event
};

#endif
