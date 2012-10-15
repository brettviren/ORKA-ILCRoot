#ifndef ILCGENDPMJET_H
#define ILCGENDPMJET_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

// Generator using DPMJET as an external generator
// The main DPMJET options are accessable for the user through this interface.

#include "IlcGenMC.h"
#include "TDPMjet.h"
#include <TString.h>
#include <TArrayI.h>

class TDPMjet;
class TArrayI;
class TParticle;
class TClonesArray;
class TGraph;
class IlcGenEventHeader;
class IlcStack;
class IlcRunLoader;
class IlcGenDPMjetEventHeader;



class IlcGenDPMjet : public IlcGenMC
{

 public:
    IlcGenDPMjet();
    IlcGenDPMjet(Int_t npart);
    IlcGenDPMjet(const IlcGenDPMjet &Dpmjet);
    virtual ~IlcGenDPMjet(); 
    virtual void    Generate();
    virtual void    Init();
    virtual void    FinishRun();
    virtual void    SetEnergyCMS(Float_t energy = 14000.) {fEnergyCMS = energy; fBeamEn = energy / 2.;}
    virtual void    SetImpactParameterRange(Float_t bmin=0., Float_t bmax=1.)
			{fMinImpactParam=bmin; fMaxImpactParam=bmax;}
    virtual void    SetProcess(DpmProcess_t iproc) {fProcess = iproc;}
    virtual void    SetCentral(Int_t icentr=-2) {fICentr = icentr;}
    virtual void    SetFlavor(Int_t flag=0)           {fFlavor     = flag;}
    virtual void    SetSelectAll(Int_t flag=0)        {fSelectAll  = flag;}
    virtual void    SetSpectators(Int_t spects=1)     {fSpectators = spects;}
    virtual void    SetBoostLHC(Int_t flag=0)         {fLHC        = flag;}
    virtual void    SetPi0Decay(Int_t iPi0)  {fPi0Decay = iPi0;}
    virtual void    SetDecayAll(Int_t iDec)  {fDecayAll = iDec;}
    virtual void    GetImpactParameterRange(Float_t& bmin, Float_t& bmax)
			{bmin = fMinImpactParam; bmax = fMaxImpactParam;}
    virtual Int_t   GetSpectators()        {return fSpectators;}
    virtual Int_t   GetFlavor()            {return fFlavor;}

    virtual void    SetGenImpPar(Float_t bValue) {fGenImpPar=bValue;}
    virtual Float_t GetGenImpPar() {return fGenImpPar;}
    virtual void    SetTriggerChargedMultiplicity(Int_t multiplicity, 
						  Float_t etamax = 0, Float_t ptmin = -1.) 
    {fTriggerMultiplicity = multiplicity; fTriggerMultiplicityEta = etamax; 
      fTriggerMultiplicityPtMin = ptmin;}

    IlcGenDPMjet &  operator=(const IlcGenDPMjet & rhs);
    void     AddHeader(IlcGenEventHeader* header);

   void SetTuneForDiff(Bool_t a=kTRUE) {fkTuneForDiff=a;}

 protected:
    Bool_t SelectFlavor(Int_t pid);
    void   MakeHeader();

 protected:
    Float_t       fBeamEn; 	   // beam energy
    Float_t       fMinImpactParam; // minimum impact parameter
    Float_t       fMaxImpactParam; // maximum impact parameter	
    Int_t	  fICentr;	   // Flag to force central production
    Int_t         fSelectAll;      // Flag to write the full event
    Int_t         fFlavor;         // Selected particle flavor 4: charm+beauty 5: beauty
    Int_t         fTrials;         // Number of trials
    Int_t         fSpectators;     // put spectators on stack
    Int_t 	  fSpecn;	   // Num. of spectator neutrons
    Int_t 	  fSpecp;	   // Num. of spectator protons
    TDPMjet      *fDPMjet;         // DPMjet
    Int_t         fNoGammas;       // Don't write gammas if flag "on"
    Int_t         fLHC;            // Assume LHC as lab frame
    Int_t         fPi0Decay;       // Flag for pi0 decays
    Int_t         fDecayAll;       // Flag to switch on long-lived particle decays
    Float_t	  fGenImpPar;	   // GeneratedImpactParameter
    DpmProcess_t  fProcess;        // Process type
    // Multiplicity Trigger
    Int_t         fTriggerMultiplicity;      // Triggered multiplicity
    Float_t       fTriggerMultiplicityEta;   // Triggered multiplicity eta cut
    Float_t       fTriggerMultiplicityPtMin; // Triggered multiplicity min pt

    Bool_t fkTuneForDiff;    // Phojet tune 
    Int_t  fProcDiff;

 private:
    // adjust the weight from kinematic cuts
    void   AdjustWeights();
    // check seleted daughters
    Bool_t DaughtersSelection(TParticle* iparticle);
    // check if stable
    Bool_t Stable(TParticle*  particle);
 
   Bool_t CheckDiffraction();
   Bool_t GetWeightsDiffraction(Double_t M, Double_t &Mmin, Double_t &Mmax, 
					       Double_t &wSD, Double_t &wDD, Double_t &wND);

    ClassDef(IlcGenDPMjet,4) // IlcGenerator interface to DPMJET
};
#endif





