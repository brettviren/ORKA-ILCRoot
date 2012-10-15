#ifndef ILCRSTACKCPVPREPROCESSOR_H
#define ILCRSTACKCPVPREPROCESSOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
// Class IlcRSTACKCpvPreprocessor - CPV preprocessor.
///////////////////////////////////////////////////////////////////////////////


#include "IlcPreprocessor.h"

class IlcRSTACKCpvPreprocessor : public IlcPreprocessor {
public:

  IlcRSTACKCpvPreprocessor();
  IlcRSTACKCpvPreprocessor(IlcShuttleInterface* shuttle);

protected:

  virtual UInt_t Process(TMap* valueSet);

  ClassDef(IlcRSTACKCpvPreprocessor,1);

};

#endif
