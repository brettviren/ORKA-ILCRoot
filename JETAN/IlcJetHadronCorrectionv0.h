#ifndef ILCJETHADRONCORRECTIONV0_H
#define ILCJETHADRONCORRECTIONV0_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */

/* $Id: IlcJetHadronCorrectionv0.h 48117 2011-03-06 14:24:54Z morsch $ */

//_________________________________________________________________________
//                  
//*-- Author: Aleksei Pavlinov (WSU)
#include "IlcJetHadronCorrection.h"

class IlcJetHadronCorrectionv0: public IlcJetHadronCorrection {

  public:
  static  IlcJetHadronCorrectionv0* Instance();
  virtual Double_t GetEnergy(Double_t pmom, Double_t eta, Int_t gid); 
  Double_t GetEnergy(Double_t pmom, Double_t eta){return GetEnergy(pmom,eta,7);}
  virtual ~IlcJetHadronCorrectionv0() {}

  protected:
  IlcJetHadronCorrectionv0() {;}
  IlcJetHadronCorrectionv0(const char *name, const char *title);


  private:
    static IlcJetHadronCorrectionv0* fHadrCorr; // pointer to instance

  ClassDef(IlcJetHadronCorrectionv0,1) // Hadron correction for EMC (version for MDC)
};

#endif // ILCJETHADRONCORRECTIONV0_H
