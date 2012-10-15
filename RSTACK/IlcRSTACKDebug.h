#ifndef ILCRSTACKV1_H
#define ILCRSTACKV1_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//_________________________________________________________________________
// Implementation version v1 of RSTACK Manager class 
//*--                  
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---
#include "TClonesArray.h"

class TFile;

// --- IlcRoot header files ---
#include "IlcRSTACKv0.h"
#include "IlcRSTACKGeometry.h"
#include "IlcRSTACKReconstructor.h"
#include "IlcRSTACKTrackSegmentMaker.h"
#include "IlcRSTACKPID.h"
#include "IlcRSTACKCPVDigit.h"

class IlcRSTACKv1 : public IlcRSTACKv0 {

public:

  IlcRSTACKv1(void) ;
  IlcRSTACKv1(const char *name, const char *title="") ;
  IlcRSTACKv1(IlcRSTACKReconstructor * Reconstructioner, const char *name, const char *title="") ;
  IlcRSTACKv1(const IlcRSTACKv1 & RSTACK) {
    // cpy ctor: no implementation yet
    // requested by the Coding Convention
    assert(0==1) ; 
  }
  virtual ~IlcRSTACKv1(void) ;

  virtual void   AddHit( Int_t shunt, Int_t primary, Int_t track, Int_t id, Float_t *hits, Int_t pid, TLorentzVector p, Float_t *pos) ; 
  Float_t        Calibrate(Int_t amp){ return (amp - fDigitizeA)/fDigitizeB ; }
  Int_t          Digitize(Float_t Energy){ return (Int_t ) (fDigitizeA + Energy*fDigitizeB); }
  virtual void   Hits2SDigits() ;
  virtual void   MakeBranch(Option_t* opt, char *file=0 ) ;
  void           Reconstruction(IlcRSTACKReconstructor * Reconstructioner) ;
  void           ResetClusters(){} ;
  virtual void   SDigits2Digits() ;  
  virtual Int_t  IsVersion(void) const {
    // Gives the version number 
    return 1 ; 
  }

  virtual void   ResetReconstruction() ; // Reset reconstructed objects
  void           SetReconstructioner(IlcRSTACKReconstructor& Reconstructioner) {
    // sets the reconstructionner object to be used
    fReconstructioner = &Reconstructioner ;
  }  
  void           SetDigitThreshold(Float_t th) { fDigitThreshold = th ; } 
  void           SetPpsdEnergyThreshold(Float_t enth)      { fPpsdEnergyThreshold = enth ; } 
  void           SetCpvEnergyThreshold(Float_t enth)       { fCpvEnergyThreshold = enth ; } 

  virtual void   StepManager(void) ;                              
  virtual TString Version(void){ 
    // returns the version number 
    return TString("v1") ; 
  }

  IlcRSTACKv1 & operator = (const IlcRSTACKv1 & rvalue)  {
    // assignement operator requested by coding convention but not needed
    assert(0==1) ;
    return *this ; 
  }

  void       CPVDigitize (TLorentzVector p, Float_t *xy, Int_t moduleNumber, TClonesArray *digits) ;
  Float_t    CPVPadResponseFunction(Float_t qhit, Float_t zg, Float_t xg) ;
  Double_t   CPVCumulPadResponse(Double_t x, Double_t y) ;

protected:

  Float_t fDigitThreshold ;                       // Threshold for the digit registration 
  Float_t fPpsdEnergyThreshold;                   //PPSD
  Float_t fCpvEnergyThreshold;                    //CPV  
  Float_t fPinElectronicNoise  ;                  // Electronic Noise in the PIN
  Float_t fDigitizeA ;                            //Parameters of the 
  Float_t fDigitizeB ;                            //digitization 
  Int_t   fnSdigits ; 
  IlcRSTACKReconstructor  * fReconstructioner ;  // Clusterization and subtracking procedures
  IlcRSTACKTrackSegmentMaker * fTrackSegmentMaker ; // Reconstructioner of the RSTACK track segment: 2 x PPSD + 1 x EMC

  ClassDef(IlcRSTACKv1,1)  // Implementation of RSTACK manager class for layout EMC+PPSD

};

#endif // IlcRSTACKV1_H
