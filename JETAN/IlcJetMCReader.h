#ifndef ILCJETMCREADER_H
#define ILCJETMCREADER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
// Jet MC Reader 
// MC reader for jet analysis
// Author: Mercedes Lopez Noriega (mercedes.lopez.noriega@cern.ch)

#include "IlcJetESDReader.h"

class IlcJetMCReader : public IlcJetESDReader
{
 public: 
    IlcJetMCReader();
    virtual ~IlcJetMCReader();
    Bool_t FillMomentumArray(Int_t event);
    Bool_t FillMomentumArray() {return kTRUE;}
 private:
  IlcJetMCReader(const IlcJetMCReader &det);
  IlcJetMCReader &operator=(const IlcJetMCReader &det);
    
 protected:
    TChain* fChainMC; // Input chain
    ClassDef(IlcJetMCReader,1)
};
 
#endif
