#ifndef ILCPVBARCPVPREPROCESSOR_H
#define ILCPVBARCPVPREPROCESSOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
// Class IlcPVBARCpvPreprocessor - CPV preprocessor.
///////////////////////////////////////////////////////////////////////////////


#include "IlcPreprocessor.h"

class IlcPVBARCpvPreprocessor : public IlcPreprocessor {
public:

  IlcPVBARCpvPreprocessor();
  IlcPVBARCpvPreprocessor(IlcShuttleInterface* shuttle);

protected:

  virtual UInt_t Process(TMap* valueSet);

  ClassDef(IlcPVBARCpvPreprocessor,1);

};

#endif
