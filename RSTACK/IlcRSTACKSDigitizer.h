#ifndef ILCRSTACKSDigitizer_H
#define ILCRSTACKSDigitizer_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//_________________________________________________________________________
//  Class for making SDigits in RSTACK      
// A Summable Digits correspond to one hits originated 
// from one primary in one scintillating tile in 1 ps time gate
// At the number of photon it is applyed Poisson smearing, attenuation length
// and geometric collection efficiency
//*--


// --- ROOT system ---
#include "TNamed.h"
#include "IlcConfig.h"
class TFile ;


// --- Standard library ---

// --- IlcRoot header files ---
//class IlcRSTACKQADataMaker ; 

class IlcRSTACKSDigitizer: public TNamed {

public:
  IlcRSTACKSDigitizer() ;          // ctor
  IlcRSTACKSDigitizer(const char * ilcrunFileName, const char * eventFolderName = IlcConfig::GetDefaultEventFolderName()) ; 
  IlcRSTACKSDigitizer(const IlcRSTACKSDigitizer& sd) ;   
  IlcRSTACKSDigitizer& operator = (const IlcRSTACKSDigitizer& sd) ;

  virtual ~IlcRSTACKSDigitizer(); // dtor

  virtual void   Digitize(Option_t *option); 
  Int_t          GetSDigitsInRun() const {return fSDigitsInRun ;}  
  virtual void   Print(const Option_t * = "") const ;
  void           SetEventFolderName(TString name) { fEventFolderName = name ; }
  void           SetEventRange(Int_t first=0, Int_t last=-1) {fFirstEvent=first; fLastEvent=last; }

  Bool_t operator == (const IlcRSTACKSDigitizer & sd) const ;

 
private:

  void     Init() ;
  void     InitParameters() ;
  void     PrintSDigits(Option_t * option) ;
  void     Unload() const ;


private:
  Float_t fPrimThreshold ;  // To store primary if NPhotons > threshold
  Float_t fCollectEff;      //geometric collection efficiency
  Float_t fAttenuationLength; //Scintillator attenuation length
  Float_t fSiPMPDE;           //SiPM Photon Detection Efficiency
  Bool_t  fDefaultInit;     //! Says if the task was created by defaut ctor (only parameters are initialized)
  TString fEventFolderName; // event folder name
  Bool_t  fInit ;           //! tells if initialization went OK, will revent exec if not
  Int_t   fSDigitsInRun ;   //! Total number of sdigits in one run
  Int_t   fFirstEvent;      // first event to process
  Int_t   fLastEvent;       // last  event to process

  ClassDef(IlcRSTACKSDigitizer,6)  // description 

};

#endif // IlcRSTACKSDigitizer_H
