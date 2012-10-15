#ifndef ILCPVECV1_H
#define ILCPVECV1_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//_________________________________________________________________________
// Implementation version v1 of PVEC Manager class 
// Layout EMC + CPV  has name IHEP
//*--                  
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---
#include "TClonesArray.h"

class TFile;

// --- IlcRoot header files ---
#include "IlcPVECv0.h"
#include "IlcPVECGeometry.h"
#include "IlcPVECReconstructor.h"
#include "IlcPVECTrackSegmentMaker.h"
#include "IlcPVECPID.h"
#include "IlcPVECCPVDigit.h"

class IlcPVECv1 : public IlcPVECv0 {

public:

  IlcPVECv1(void) ;
  IlcPVECv1(const char *name, const char *title="") ;
  IlcPVECv1(IlcPVECReconstructor * Reconstructioner, const char *name, const char *title="") ;
  IlcPVECv1(const IlcPVECv1 & PVEC) {
    // cpy ctor: no implementation yet
    // requested by the Coding Convention
    assert(0==1) ; 
  }
  virtual ~IlcPVECv1(void) ;

  virtual void   AddHit( Int_t shunt, Int_t primary, Int_t track, Int_t id, Float_t *hits, Int_t pid, TLorentzVector p, Float_t *pos) ; 
  Float_t        Calibrate(Int_t amp){ return (amp - fDigitizeA)/fDigitizeB ; }
  Int_t          Digitize(Float_t Energy){ return (Int_t ) (fDigitizeA + Energy*fDigitizeB); }
  virtual void   Hits2SDigits() ;
  virtual void   MakeBranch(Option_t* opt, char *file=0 ) ;
  void           Reconstruction(IlcPVECReconstructor * Reconstructioner) ;
  void           ResetClusters(){} ;
  virtual void   SDigits2Digits() ;  
  virtual Int_t  IsVersion(void) const {
    // Gives the version number 
    return 1 ; 
  }

  virtual void   ResetReconstruction() ; // Reset reconstructed objects
  void           SetReconstructioner(IlcPVECReconstructor& Reconstructioner) {
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

  IlcPVECv1 & operator = (const IlcPVECv1 & rvalue)  {
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
  IlcPVECReconstructor  * fReconstructioner ;  // Clusterization and subtracking procedures
  IlcPVECTrackSegmentMaker * fTrackSegmentMaker ; // Reconstructioner of the PVEC track segment: 2 x PPSD + 1 x EMC

  ClassDef(IlcPVECv1,1)  // Implementation of PVEC manager class for layout EMC+PPSD

};

#endif // IlcPVECV1_H
