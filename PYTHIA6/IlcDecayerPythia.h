#ifndef ILCDECAYERPYTHIA_H
#define ILCDECAYERPYTHIA_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDecayerPythia.h 56668 2012-05-24 13:14:33Z hristov $ */

// Implementation of IlcDecayer using Pythia
// Method forwarding to the IlcPythia instance.
// Author: andreas.morsch@cern.ch

#include "IlcDecayer.h"
class IlcPythia;
class TClonesArrray;

class IlcDecayerPythia :
public IlcDecayer
{
 public:
    IlcDecayerPythia();
    IlcDecayerPythia(const IlcDecayerPythia &decayer);
    //    
    virtual         ~IlcDecayerPythia(){;}
    virtual void    Init();
    virtual void    Decay(Int_t idpart, TLorentzVector *p);
    virtual Int_t   ImportParticles(TClonesArray *particles);
    virtual void    SetForceDecay(Decay_t decay) {fDecay=decay;}
    virtual void    SetForceDecay(Int_t decay)
      {SetForceDecay((Decay_t) decay);}
    virtual void    ForceDecay();
    virtual void    SetPatchOmegaDalitz() {fPatchOmegaDalitz = 1;}
    virtual void    HeavyFlavourOff() {fHeavyFlavour = kFALSE;}
    virtual void    DecayLongLivedParticles()  {fLongLived    = kTRUE;}
    virtual Float_t GetPartialBranchingRatio(Int_t ipart);
    virtual Float_t GetLifetime(Int_t kf);
    virtual void    SwitchOffBDecay();
    virtual void    WriteDecayTable();
    virtual void    ReadDecayTable();
    
 private:
    void     DefineParticles();
    void     Lu1Ent(Int_t flag, Int_t idpart, 
		    Double_t mom, Double_t theta, Double_t phi);
    Int_t    CountProducts(Int_t channel, Int_t particle);
    void     ForceParticleDecay(Int_t particle, Int_t product, Int_t mult);
    void     ForceParticleDecay(Int_t particle, const Int_t* products, Int_t* mult, Int_t npart, Bool_t flag = 0);
    void     ForceHadronicD(Int_t optUser4Bodies=1);
    void     ForceOmega();
    void     ForceLambda();
    void     SwitchOffHeavyFlavour();
    void     ForceBeautyUpgrade();
    Float_t  GetBraPart(Int_t kf);
    void     Copy(TObject &decayer) const;

    IlcDecayerPythia &operator=(const IlcDecayerPythia &decayer) 
    {decayer.Copy(*this);return(*this);}
    
    
 private:
    IlcPythia*  fPythia;          //! Pointer to IlcPythia
    Decay_t     fDecay;           //  Forced decay mode
    Float_t     fBraPart[501];    //! Branching ratios
    Bool_t      fHeavyFlavour;    //! Flag for heavy flavors
    Bool_t      fLongLived;       //! Flag for long lived particle decay
    Bool_t      fPatchOmegaDalitz;//! Flag to patch the omega Dalitz decays 
    static Bool_t fgInit;         //! initialization flag 
    
    ClassDef(IlcDecayerPythia, 3) // IlcDecayer implementation using Pythia  
};
#endif







