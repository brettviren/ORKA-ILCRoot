#ifndef ILCPVECQACHECKER_H
#define ILCPVECQACHECKER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcPVECQAChecker.h 56633 2012-05-22 22:14:15Z hristov $ */

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
class IlcPVECLoader ; 

class IlcPVECQAChecker: public IlcQACheckerBase {

public:
  IlcPVECQAChecker() : IlcQACheckerBase("PVEC","PVEC Quality Assurance Data Maker") {;}          // ctor
  IlcPVECQAChecker(const IlcPVECQAChecker& qac) : IlcQACheckerBase(qac.GetName(), qac.GetTitle()) {;} // cpy ctor   
  virtual ~IlcPVECQAChecker() {;} // dtor

private:
  virtual void Check(Double_t *rv, IlcQAv1::ILCTASK_t, TObjArray ** list, const IlcDetectorRecoParam * recoParam=0) ; 
  IlcPVECQAChecker & operator = (const IlcPVECQAChecker & /*qac*/);
  
  ClassDef(IlcPVECQAChecker,1)  // description 

};

#endif // IlcPVECQAChecker_H
