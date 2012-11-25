#ifndef ILCPVBARV1_H
#define ILCPVBARV1_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARv1.h 25985 2008-05-20 21:07:31Z kharlov $ */

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
// Implementation version v1 of PVBAR Manager class 
//--                  
//-- Author: Yves Schutz (SUBATECH)

// --- ROOT system ---
class TClonesArray ;
class TFile;
#include <TLorentzVector.h>

// --- IlcRoot header files ---
#include "IlcPVBARv0.h"

class IlcPVBARv1 : public IlcPVBARv0 {

public:

  IlcPVBARv1(void) ;
  IlcPVBARv1(const char *name, const char *title="") ;
  virtual ~IlcPVBARv1(void) ;

  using IlcPVBARv0::AddHit;
  virtual void   AddHit( Int_t shunt, Int_t primary, Int_t id, Float_t *hits) ; 
  virtual void   FinishEvent() ;
  virtual void   FinishPrimary() ;
  virtual Int_t  IsVersion(void) const {
    // Gives the version number 
    return 1 ; 
  }

  virtual void   StepManager(void) ;                              
  virtual const TString Version(void)const { return TString("v1") ;  }

  Float_t Npe_Cerenkov_in_Glass_Ex(Float_t beta, Float_t * uP, Float_t step, Float_t *zDistanceFromWLS, Float_t xyDistanceFromWLS, Int_t lambdabins, Float_t lambdazero, Float_t lambdawidth, Float_t (*Npe_Spectrum)[2], Float_t (*Npe_LG)[2],  Float_t (*Npe_WLS)[2], Float_t *TotalNpe);
  Float_t Npe_Cerenkov(Float_t beta, Float_t * uP, Float_t step, Float_t *zDistanceFromWLS, Float_t xyDistanceFromWLS, Int_t lambdabins, Float_t lambdazero, Float_t lambdawidth);
  Float_t nIndex(Float_t lambda);
  Float_t AttenuationLength_m_LG(Float_t lambda);
  Float_t AttenuationLength_m_WLS(Float_t lambda);
  Float_t QE_Fast_WLS(Float_t lambda_green);
  Float_t QE_PMT(Float_t lambda);
  Float_t QE_SiPM(Float_t lambda);
  Float_t Shifter_WLS(Float_t lambda);

 private:
  IlcPVBARv1(IlcPVBARv1 & PVBAR);
  IlcPVBARv1 & operator = (const IlcPVBARv1 & /*rvalue*/);

  ClassDef(IlcPVBARv1,5)  // Implementation of PVBAR manager class for layout EMC+PPSD

};

#endif // IlcPVBARV1_H
