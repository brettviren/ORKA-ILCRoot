#ifndef ILCRSTACKV1_H
#define ILCRSTACKV1_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//_________________________________________________________________________
// Implementation version v1 of RSTACK Manager class 
//--                  
//-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---
class TClonesArray ;
class TFile;
#include <TLorentzVector.h>

// --- IlcRoot header files ---
#include "IlcRSTACKv0.h"

class IlcRSTACKv1 : public IlcRSTACKv0 {

public:

  IlcRSTACKv1(void) ;
  IlcRSTACKv1(const char *name, const char *title="") ;
  virtual ~IlcRSTACKv1(void) ;

  using IlcRSTACKv0::AddHit;
  virtual void   AddHit( Int_t shunt, Int_t primary, Int_t id, Float_t *hits) ; 
  virtual void   FinishEvent() ;
  virtual void   FinishPrimary() ;
  virtual Int_t  IsVersion(void) const {
    // Gives the version number 
    return 1 ; 
  }

  virtual void   StepManager(void) ;                              
  virtual const TString Version(void)const { return TString("v1") ;  }

 private:
  IlcRSTACKv1(IlcRSTACKv1 & RSTACK);
  IlcRSTACKv1 & operator = (const IlcRSTACKv1 & /*rvalue*/);

  ClassDef(IlcRSTACKv1,5)  // Implementation of RSTACK manager class for ORKA layout

};

#endif // IlcRSTACKV1_H
