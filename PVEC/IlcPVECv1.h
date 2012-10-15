#ifndef ILCPVECV1_H
#define ILCPVECV1_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECv1.h 25985 2008-05-20 21:07:31Z kharlov $ */

/* History of cvs commits:
 *
 * $Log$
 * Revision 1.40  2006/09/13 07:31:01  kharlov
 * Effective C++ corrections (T.Pocheptsov)
 *
 * Revision 1.39  2005/05/28 14:19:05  schutz
 * Compilation warnings fixed by T.P.
 *
 */

//_________________________________________________________________________
// Implementation version v1 of PVEC Manager class 
//--                  
//-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---
class TClonesArray ;
class TFile;
#include <TLorentzVector.h>

// --- IlcRoot header files ---
#include "IlcPVECv0.h"

class IlcPVECv1 : public IlcPVECv0 {

public:

  IlcPVECv1(void) ;
  IlcPVECv1(const char *name, const char *title="") ;
  virtual ~IlcPVECv1(void) ;

  using IlcPVECv0::AddHit;
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
  IlcPVECv1(IlcPVECv1 & PVEC);
  IlcPVECv1 & operator = (const IlcPVECv1 & /*rvalue*/);

  ClassDef(IlcPVECv1,5)  // Implementation of PVEC manager class for layout EMC+PPSD

};

#endif // IlcPVECV1_H
