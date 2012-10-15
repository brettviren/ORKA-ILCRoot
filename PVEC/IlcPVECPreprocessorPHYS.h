#ifndef ILCPVECPREPROCESSORPHYS_H
#define ILCPVECPREPROCESSORPHYS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
// Class IlcPVECPreprocessorPHYS
///////////////////////////////////////////////////////////////////////////////


#include "IlcPreprocessor.h"

class IlcPVECPreprocessorPHYS : public IlcPreprocessor {
 public:

  IlcPVECPreprocessorPHYS();
  IlcPVECPreprocessorPHYS(IlcShuttleInterface* shuttle);

 protected:

  virtual UInt_t Process(TMap* valueSet);
  Bool_t CalibratePhys();

 private:

  ClassDef(IlcPVECPreprocessorPHYS,1)  // Preprocessor for PHYS event
};
#endif
