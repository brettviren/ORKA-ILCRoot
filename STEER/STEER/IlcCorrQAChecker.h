#ifndef ILCCORRQACHECKER_H
#define ILCCORRQACHECKER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcCorrQAChecker.h 27115 2008-07-04 15:12:14Z hristov $ */

/*
  Checks the quality assurance. 
  By comparing with reference data
  Y. Schutz CERN July 2007
*/


// --- ROOT system ---
class TFile ; 
class TH1F ; 
class TH1I ; 

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcQACheckerBase.h"
class IlcCorrLoader ; 

class IlcCorrQAChecker: public IlcQACheckerBase {

public:
  IlcCorrQAChecker() : IlcQACheckerBase("Corr","Corr Quality Assurance Data Maker") {;}          // ctor
  virtual ~IlcCorrQAChecker() {;} // dtor

  virtual void   Run(IlcQAv1::ILCTASK_t /*tsk*/, TNtupleD ** /*nt*/, IlcDetectorRecoParam * /*recoParam*/) ;


private:
  IlcCorrQAChecker(const IlcCorrQAChecker& qac); // Not implemented
  IlcCorrQAChecker& operator=(const IlcCorrQAChecker& qac); // Not implemented
  Double_t * CheckN(IlcQAv1::ILCTASK_t index, TNtupleD ** nData, IlcDetectorRecoParam * recoParam) ; 

  ClassDef(IlcCorrQAChecker,1)  // description 

};

#endif // IlcCORRQAChecker_H
