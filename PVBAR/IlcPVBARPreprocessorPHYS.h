#ifndef ILCPVBARPREPROCESSORPHYS_H
#define ILCPVBARPREPROCESSORPHYS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
// Class IlcPVBARPreprocessorPHYS
///////////////////////////////////////////////////////////////////////////////


#include "IlcPreprocessor.h"

class IlcPVBARPreprocessorPHYS : public IlcPreprocessor {
 public:

  IlcPVBARPreprocessorPHYS();
  IlcPVBARPreprocessorPHYS(IlcShuttleInterface* shuttle);

 protected:

  virtual UInt_t Process(TMap* valueSet);
  Bool_t CalibratePhys();

 private:

  ClassDef(IlcPVBARPreprocessorPHYS,1)  // Preprocessor for PHYS event
};
#endif
