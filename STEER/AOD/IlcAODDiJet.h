#ifndef IlcAODDIJet_H
#define IlcAODDIJet_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-------------------------------------------------------------------------
//     AOD di-jet class
//     The present version is for test purposes only
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include <TLorentzVector.h>
#include <TRefArray.h>
#include <TRef.h>
#include "IlcAODJet.h"


class IlcAODDiJet : public IlcAODJet {

 public:
    IlcAODDiJet();
    IlcAODDiJet(Double_t px, Double_t py, Double_t pz, Double_t e);
    IlcAODDiJet(TLorentzVector & p);
    virtual ~IlcAODDiJet();

    void SetJetRefs(IlcAODJet* jet1, IlcAODJet* jet2);
    IlcAODJet* Jet(Int_t i) {return ((IlcAODJet*) (fJetR->At(i)));}
    Float_t    DeltaPhi();
    Float_t    PhiImbalance();

 private:
    IlcAODDiJet(const IlcAODDiJet& jet);
    IlcAODDiJet& operator=(const IlcAODDiJet& jet);

 private:
    TRefArray*  fJetR;  // References to jets
    TRef        fJet1;  // Reference to Jet 1
    TRef        fJet2;  // Reference to Jet 2
    ClassDef(IlcAODDiJet, 1);
};
#endif
