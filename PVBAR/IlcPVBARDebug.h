#ifndef ILCPVBARV1_H
#define ILCPVBARV1_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//_________________________________________________________________________
// Implementation version v1 of PVBAR Manager class 
//*--                  
//*-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---
#include "TClonesArray.h"

class TFile;

// --- IlcRoot header files ---
#include "IlcPVBARv0.h"
#include "IlcPVBARGeometry.h"
#include "IlcPVBARReconstructor.h"
#include "IlcPVBARTrackSegmentMaker.h"
#include "IlcPVBARPID.h"
#include "IlcPVBARCPVDigit.h"

class IlcPVBARv1 : public IlcPVBARv0 {

public:

  IlcPVBARv1(void) ;
  IlcPVBARv1(const char *name, const char *title="") ;
  IlcPVBARv1(IlcPVBARReconstructor * Reconstructioner, const char *name, const char *title="") ;
  IlcPVBARv1(const IlcPVBARv1 & PVBAR) {
    // cpy ctor: no implementation yet
    // requested by the Coding Convention
    assert(0==1) ; 
  }
  virtual ~IlcPVBARv1(void) ;

  virtual void   AddHit( Int_t shunt, Int_t primary, Int_t track, Int_t id, Float_t *hits, Int_t pid, TLorentzVector p, Float_t *pos) ; 
  Float_t        Calibrate(Int_t amp){ return (amp - fDigitizeA)/fDigitizeB ; }
  Int_t          Digitize(Float_t Energy){ return (Int_t ) (fDigitizeA + Energy*fDigitizeB); }
  virtual void   Hits2SDigits() ;
  virtual void   MakeBranch(Option_t* opt, char *file=0 ) ;
  void           Reconstruction(IlcPVBARReconstructor * Reconstructioner) ;
  void           ResetClusters(){} ;
  virtual void   SDigits2Digits() ;  
  virtual Int_t  IsVersion(void) const {
    // Gives the version number 
    return 1 ; 
  }

  virtual void   ResetReconstruction() ; // Reset reconstructed objects
  void           SetReconstructioner(IlcPVBARReconstructor& Reconstructioner) {
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

  IlcPVBARv1 & operator = (const IlcPVBARv1 & rvalue)  {
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
  IlcPVBARReconstructor  * fReconstructioner ;  // Clusterization and subtracking procedures
  IlcPVBARTrackSegmentMaker * fTrackSegmentMaker ; // Reconstructioner of the PVBAR track segment: 2 x PPSD + 1 x EMC

  ClassDef(IlcPVBARv1,1)  // Implementation of PVBAR manager class for layout EMC+PPSD

};

#endif // IlcPVBARV1_H
