#ifndef ILCOMEGADILCTZ_H
#define ILCOMEGADILCTZ_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//-----------------------------------------------------------------------------
//
// Generate lepton-pair mass distributions for Dalitz decays according
// to the Kroll-Wada parametrization: N. Kroll, W. Wada: Phys. Rev 98(1955)1355
//
// For the electromagnetic form factor the parameterization from
// Lepton-G is used: L.G. Landsberg et al.: Phys. Rep. 128(1985)301
//
//-----------------------------------------------------------------------------


#include "IlcDecayer.h"
#include <TLorentzVector.h>

class TH1F;
class TClonesArray;

class IlcOmegaDalitz : public IlcDecayer
{
 public:
    IlcOmegaDalitz();
    virtual void    Init();
    virtual void    Decay(Int_t idlepton,  TLorentzVector* p);
    virtual Int_t   ImportParticles(TClonesArray *particles);
    virtual void    SetForceDecay(Int_t)                      {;}
    virtual void    ForceDecay()                              {;}
    virtual Float_t GetPartialBranchingRatio(Int_t /*ipart*/) {return -1;}
    virtual Float_t GetLifetime(Int_t /*kf*/)                 {return -1;}
    virtual void    ReadDecayTable()                          {;}
    virtual TH1F*   ElectronPairMassHisto()                   {return  fEPMass;}
    virtual TH1F*   MuonPairMassHisto()                       {return  fMPMass;}
    //
    virtual void    Decay(TClonesArray* array);
    virtual const   TLorentzVector* Products() const {return fProducts;}
    virtual void    Copy(TObject&) const;
    
 protected:
    TH1F*           fEPMass;       // Histogram for electron pair mass
    TH1F*           fMPMass;       // Histogram for muon pair mass
    TLorentzVector  fProducts[3];  // Decay products
    Bool_t          fInit;         // Initialized
    
 private:
    virtual void    Rot(Double_t pin[3], Double_t pout[3],
			Double_t costheta, Double_t sintheta,
			Double_t cosphi, Double_t sinphi) const;
    IlcOmegaDalitz(const IlcOmegaDalitz &dalitz);
    IlcOmegaDalitz & operator=(const IlcOmegaDalitz & rhs);
    
    ClassDef(IlcOmegaDalitz, 1) // IlcDecayer implementation for omega Dalitz
};

#endif
