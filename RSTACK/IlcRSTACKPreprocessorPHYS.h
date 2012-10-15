#ifndef ILCRSTACKPREPROCESSORPHYS_H
#define ILCRSTACKPREPROCESSORPHYS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
// Class IlcRSTACKPreprocessorPHYS
///////////////////////////////////////////////////////////////////////////////


#include "IlcPreprocessor.h"

class IlcRSTACKPreprocessorPHYS : public IlcPreprocessor {
 public:

  IlcRSTACKPreprocessorPHYS();
  IlcRSTACKPreprocessorPHYS(IlcShuttleInterface* shuttle);

 protected:

  virtual UInt_t Process(TMap* valueSet);
  Bool_t CalibratePhys();

 private:

  ClassDef(IlcRSTACKPreprocessorPHYS,1)  // Preprocessor for PHYS event
};
#endif
