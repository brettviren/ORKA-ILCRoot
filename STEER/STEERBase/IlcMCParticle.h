#ifndef ILCMCPARTICLE_H
#define ILCMCPARTICLE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcMCParticle.h 54863 2012-02-28 12:10:29Z hristov $ */

//-------------------------------------------------------------------------
//     IlcVParticle realisation for MC Particles
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include <Rtypes.h>
#include <TParticle.h>
#include <TParticlePDG.h>
#include <TObjArray.h>

#include "IlcTrackReference.h"
#include "IlcVParticle.h"

class IlcMCParticle: public IlcVParticle {
public:
    IlcMCParticle();
    IlcMCParticle(TParticle* part, TObjArray* rarray = 0, Int_t label=-1);
    virtual ~IlcMCParticle();
    IlcMCParticle(const IlcMCParticle& mcPart); 
    IlcMCParticle& operator=(const IlcMCParticle& mcPart);
    
    // Kinematics
    virtual Double_t Px()        const;
    virtual Double_t Py()        const;
    virtual Double_t Pz()        const;
    virtual Double_t Pt()        const;
    virtual Double_t P()         const;
    virtual Bool_t   PxPyPz(Double_t p[3]) const;
   
    virtual Double_t OneOverPt() const;
    virtual Double_t Phi()       const;
    virtual Double_t Theta()     const;
    
    virtual Double_t Xv()        const;
    virtual Double_t Yv()        const;
    virtual Double_t Zv()        const;
    virtual Bool_t   XvYvZv(Double_t x[3]) const;  
    virtual Double_t T()         const;

    virtual Double_t E()          const;
    virtual Double_t M()          const;
    
    virtual Double_t Eta()        const;
    virtual Double_t Y()          const;
    
    virtual Short_t Charge()      const;

    virtual Int_t    Label()         const;
    virtual Int_t    GetLabel()      const {return Label();}
    virtual Int_t    PdgCode()       const {return fParticle->GetPdgCode();}
    virtual TParticle* Particle() const {return fParticle;}
    
    // PID
    virtual const Double_t *PID() const {return 0;} // return PID object (to be defined, still)

    // Track References
    Int_t              GetNumberOfTrackReferences() const {return fNTrackRef;}
    IlcTrackReference* GetTrackReference(Int_t i)
      {return dynamic_cast<IlcTrackReference*>((*fTrackReferences)[i]);}

    // "Trackable" criteria
    Float_t  GetTPCTrackLength(Float_t bz, Float_t ptmin, Int_t &counter, Float_t deadWidth);
    // Navigation
    Int_t GetMother()        const {return fMother;}
    Int_t GetFirstDaughter() const {return fFirstDaughter;}
    Int_t GetLastDaughter()  const {return fLastDaughter;}
    void  SetMother(Int_t idx)        {fMother        = idx;}
    void  SetFirstDaughter(Int_t idx) {fFirstDaughter = idx;}
    void  SetLastDaughter(Int_t idx)  {fLastDaughter  = idx;}
    void  SetLabel(Int_t label)       {fLabel         = label;}
	    
 private:
    TParticle *fParticle;             // The wrapped TParticle
    TObjArray *fTrackReferences;      // Array to track references
    Int_t      fNTrackRef;            // Number of track references
    Int_t      fLabel;                // fParticle Label in the Stack
    Int_t      fMother;               // Mother particles
    Int_t      fFirstDaughter;        // First daughter
    Int_t      fLastDaughter;         // LastDaughter
    
  ClassDef(IlcMCParticle,0)  // IlcVParticle realisation for MCParticles
};

inline Double_t IlcMCParticle::Px()        const {return fParticle->Px();}
inline Double_t IlcMCParticle::Py()        const {return fParticle->Py();}
inline Double_t IlcMCParticle::Pz()        const {return fParticle->Pz();}
inline Double_t IlcMCParticle::Pt()        const {return fParticle->Pt();}
inline Double_t IlcMCParticle::P()         const {return fParticle->P(); }
inline Double_t IlcMCParticle::OneOverPt() const {return 1. / fParticle->Pt();}
inline Bool_t   IlcMCParticle::PxPyPz(Double_t p[3]) const { p[0] = Px(); p[1] = Py(); p[2] = Pz(); return kTRUE; }
inline Double_t IlcMCParticle::Phi()       const {return fParticle->Phi();}
inline Double_t IlcMCParticle::Theta()     const {return fParticle->Theta();}
inline Double_t IlcMCParticle::Xv()        const {return fParticle->Vx();}
inline Double_t IlcMCParticle::Yv()        const {return fParticle->Vy();}
inline Double_t IlcMCParticle::Zv()        const {return fParticle->Vz();}
inline Bool_t   IlcMCParticle::XvYvZv(Double_t x[3]) const { x[0] = Xv(); x[1] = Yv(); x[2] = Zv(); return kTRUE; }
inline Double_t IlcMCParticle::T()        const {return fParticle->T();}
inline Double_t IlcMCParticle::E()         const {return fParticle->Energy();}
inline Double_t IlcMCParticle::Eta()       const {return fParticle->Eta();}


inline Double_t IlcMCParticle::M()         const
{
    TParticlePDG* pdg = fParticle->GetPDG();
    if (pdg) {
	return (pdg->Mass());
    } else {
	return (fParticle->GetCalcMass());
    }
}


inline Double_t IlcMCParticle::Y()         const 
{
    Double_t e  = E();
    Double_t pz = Pz();
    
    if (e != TMath::Abs(pz)) { 
	return 0.5*TMath::Log((e+pz)/(e-pz));
    } else { 
	return -999.;
    }
}

inline Short_t IlcMCParticle::Charge()     const
{
    TParticlePDG* pdg = fParticle->GetPDG();
    if (pdg) {
	return (Short_t (pdg->Charge()));
    } else {
	return -99;
    }
}

inline Int_t IlcMCParticle::Label()       const {return fLabel;}

#endif
