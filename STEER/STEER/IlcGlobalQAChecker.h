#ifndef ILCGLOBALQACHECKER_H
#define ILCGLOBALQACHECKER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcGlobalQAChecker.h 27115 2008-07-04 15:12:14Z hristov $ */

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
class IlcGlobalLoader ; 

class IlcGlobalQAChecker: public IlcQACheckerBase {

public:
  IlcGlobalQAChecker() : IlcQACheckerBase("Global","Global Quality Assurance Data Maker") {}          // ctor
  virtual ~IlcGlobalQAChecker() {} // dtor

private:
  
  ClassDef(IlcGlobalQAChecker,1)  // description 

};

#endif // IlcGLOBALQAChecker_H
