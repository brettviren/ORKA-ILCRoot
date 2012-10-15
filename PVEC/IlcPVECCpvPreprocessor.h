#ifndef ILCPVECCPVPREPROCESSOR_H
#define ILCPVECCPVPREPROCESSOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
// Class IlcPVECCpvPreprocessor - CPV preprocessor.
///////////////////////////////////////////////////////////////////////////////


#include "IlcPreprocessor.h"

class IlcPVECCpvPreprocessor : public IlcPreprocessor {
public:

  IlcPVECCpvPreprocessor();
  IlcPVECCpvPreprocessor(IlcShuttleInterface* shuttle);

protected:

  virtual UInt_t Process(TMap* valueSet);

  ClassDef(IlcPVECCpvPreprocessor,1);

};

#endif
