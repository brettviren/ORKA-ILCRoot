#ifndef ILCRSTACKQACHECKER_H
#define ILCRSTACKQACHECKER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcRSTACKQAChecker.h 56633 2012-05-22 22:14:15Z hristov $ */

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
class IlcRSTACKLoader ; 

class IlcRSTACKQAChecker: public IlcQACheckerBase {

public:
  IlcRSTACKQAChecker() : IlcQACheckerBase("RSTACK","RSTACK Quality Assurance Data Maker") {;}          // ctor
  IlcRSTACKQAChecker(const IlcRSTACKQAChecker& qac) : IlcQACheckerBase(qac.GetName(), qac.GetTitle()) {;} // cpy ctor   
  virtual ~IlcRSTACKQAChecker() {;} // dtor

private:
  virtual void Check(Double_t *rv, IlcQAv1::ILCTASK_t, TObjArray ** list, const IlcDetectorRecoParam * recoParam=0) ; 
  IlcRSTACKQAChecker & operator = (const IlcRSTACKQAChecker & /*qac*/);
  
  ClassDef(IlcRSTACKQAChecker,1)  // description 

};

#endif // IlcRSTACKQAChecker_H
