#ifndef ILCPVECSDigitizer_H
#define ILCPVECSDigitizer_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcPVECSDigitizer.h 52261 2011-10-23 15:46:57Z hristov $ */


//_________________________________________________________________________
//  Class for making SDigits in PVEC      
// A Summable Digits is the sum of all hits originating 
// from one primary in one active cell
//*--
//*-- Author: Dmitri Peressounko(SUBATECH & KI)


// --- ROOT system ---
#include "TNamed.h"
#include "IlcConfig.h"
class TFile ;


// --- Standard library ---

// --- IlcRoot header files ---
//class IlcPVECQADataMaker ; 

class IlcPVECSDigitizer: public TNamed {

public:
  IlcPVECSDigitizer() ;          // ctor
  IlcPVECSDigitizer(const char * ilcrunFileName, const char * eventFolderName = IlcConfig::GetDefaultEventFolderName()) ; 
  IlcPVECSDigitizer(const IlcPVECSDigitizer& sd) ;   
  IlcPVECSDigitizer& operator = (const IlcPVECSDigitizer& sd) ;

  virtual ~IlcPVECSDigitizer(); // dtor

  virtual void   Digitize(Option_t *option); 
  Int_t          GetSDigitsInRun() const {return fSDigitsInRun ;}  
  virtual void   Print(const Option_t * = "") const ;
  void           SetEventFolderName(TString name) { fEventFolderName = name ; }
  void           SetEventRange(Int_t first=0, Int_t last=-1) {fFirstEvent=first; fLastEvent=last; }

  Bool_t operator == (const IlcPVECSDigitizer & sd) const ;

 
private:

  void     Init() ;
  void     InitParameters() ;
  void     PrintSDigits(Option_t * option) ;
  void     Unload() const ;


private:
  Float_t fPrimThreshold ;  // To store primari if Elos > threshold
  Bool_t  fDefaultInit;     //! Says if the task was created by defaut ctor (only parameters are initialized)
  TString fEventFolderName; // event folder name
  Bool_t  fInit ;           //! tells if initialisation wennt OK, will revent exec if not
  Int_t   fSDigitsInRun ;   //! Total number of sdigits in one run
  Int_t   fFirstEvent;      // first event to process
  Int_t   fLastEvent;       // last  event to process

  ClassDef(IlcPVECSDigitizer,6)  // description 

};

#endif // IlcPVECSDigitizer_H
