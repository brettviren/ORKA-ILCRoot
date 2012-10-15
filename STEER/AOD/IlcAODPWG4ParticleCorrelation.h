#ifndef ILCAODPWG4PARTICLECORRELATION_H
#define ILCAODPWG4PARTICLECORRELATION_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAODPWG4ParticleCorrelation.h  $ */

//-------------------------------------------------------------------------
//     Copy of AOD photon class, adapted for particle identification
//     and correlations analysis
//     Author: Yves Schutz, CERN, Gustavo Conesa, INFN
//-------------------------------------------------------------------------

//-- ROOT system --
#include "TList.h"
#include "IlcAODJet.h"

//-- Analysis system


#include "IlcAODPWG4Particle.h"

class IlcAODPWG4ParticleCorrelation : public IlcAODPWG4Particle {

 public:
  IlcAODPWG4ParticleCorrelation();
  IlcAODPWG4ParticleCorrelation(Double_t px, Double_t py, Double_t pz, Double_t e);
  IlcAODPWG4ParticleCorrelation(TLorentzVector & p);  
  IlcAODPWG4ParticleCorrelation(IlcAODPWG4Particle & p);  
  
  virtual ~IlcAODPWG4ParticleCorrelation();
  virtual void Clear(const Option_t* /*opt*/);
  
  IlcAODPWG4ParticleCorrelation(const IlcAODPWG4ParticleCorrelation& photon); 
private:
  IlcAODPWG4ParticleCorrelation& operator=(const IlcAODPWG4ParticleCorrelation& photon);
  
public:
  virtual TObjArray* GetObjArray(TString refname)  const { if(fListOfObjArrays) return (TObjArray*) fListOfObjArrays->FindObject(refname); 
                                                           else return 0x0;} 
  virtual TList*     GetObjArrayList()             const { return  fListOfObjArrays; } 
  virtual void       AddObjArray(TObjArray * refarray)  { fListOfObjArrays->Add(refarray); }

  virtual TString GetLeadingDetector()   const {return fLeadingDetector ; }
  virtual void SetLeadingDetector(TString d)   {fLeadingDetector = d ; }
  
  virtual TLorentzVector  GetLeading()               const { return  fLeading;}
  virtual void  SetLeading(TLorentzVector lead) {fLeading = lead;}
  
  virtual TLorentzVector  GetCorrelatedJet()         const { return  fCorrJet;}
  virtual void  SetCorrelatedJet(TLorentzVector jet) {fCorrJet = jet;}
  
  virtual TLorentzVector  GetCorrelatedBackground()  const { return  fCorrBkg;}
  virtual void  SetCorrelatedBackground(TLorentzVector bkg) {fCorrBkg = bkg;}
  
  virtual void SetRefJet(IlcAODJet* jet)  { fRefJet = jet;}
  virtual      IlcAODJet* GetJet() const {return ((IlcAODJet*) fRefJet.GetObject());}
  virtual TRef GetRefJet()         const {return fRefJet;}
  
  virtual Bool_t IsIsolated() const      { return fIsolated ; }
  virtual void   SetIsolated(Bool_t iso) { fIsolated = iso ; }

  virtual Bool_t IsLeadingParticle() const                { return fLeadingParticle ; }
  virtual void   SetLeadingParticle(Bool_t leadPart)      { fLeadingParticle = leadPart ; }
  virtual void   Print(Option_t* /*option*/) const;
  
 private:
  Bool_t         fIsolated ;         //Particle is isolated or not 
  Bool_t         fLeadingParticle ; //Particle is leading or not 
  TString        fLeadingDetector;  // Detector where leading particle was measured.
  TLorentzVector fLeading;          // Leading Particle 4-momentum vector
  TLorentzVector fCorrJet;          // Jet  4-momentum vector
  TLorentzVector fCorrBkg;          // Background  4-momentum vector
  TRef           fRefJet;           // Reference to jet found with JETAN and correlated with particle
  TList   *      fListOfObjArrays ; // List with correlation reference arrays
  
  ClassDef(IlcAODPWG4ParticleCorrelation, 4);
};


#endif //ILCAODPWG4PARTICLECORRELATION_H
