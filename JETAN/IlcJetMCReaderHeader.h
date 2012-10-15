#ifndef ILCJETMCREADERHEADER_H
#define ILCJETMCREADERHEADER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
// Jet MC Reader Header
// Header for the MC reader in the jet analysis
// Author: Mercedes Lopez Noriega (mercedes.lopez.noriega@cern.ch)

#include "IlcJetESDReaderHeader.h"
 
class IlcJetMCReaderHeader : public IlcJetESDReaderHeader
{

 public:
  IlcJetMCReaderHeader();
  virtual ~IlcJetMCReaderHeader();
 protected:
  ClassDef(IlcJetMCReaderHeader,1);
};
 
#endif
