#ifndef ILCPVBARQACHECKER_H
#define ILCPVBARQACHECKER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcPVBARQAChecker.h 56633 2012-05-22 22:14:15Z hristov $ */

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
class IlcPVBARLoader ; 

class IlcPVBARQAChecker: public IlcQACheckerBase {

public:
  IlcPVBARQAChecker() : IlcQACheckerBase("PVBAR","PVBAR Quality Assurance Data Maker") {;}          // ctor
  IlcPVBARQAChecker(const IlcPVBARQAChecker& qac) : IlcQACheckerBase(qac.GetName(), qac.GetTitle()) {;} // cpy ctor   
  virtual ~IlcPVBARQAChecker() {;} // dtor

private:
  virtual void Check(Double_t *rv, IlcQAv1::ILCTASK_t, TObjArray ** list, const IlcDetectorRecoParam * recoParam=0) ; 
  IlcPVBARQAChecker & operator = (const IlcPVBARQAChecker & /*qac*/);
  
  ClassDef(IlcPVBARQAChecker,1)  // description 

};

#endif // IlcPVBARQAChecker_H
