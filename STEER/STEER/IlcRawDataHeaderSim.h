#ifndef ILCRAWDATAHEADERSIM_H
#define ILCRAWDATAHEADERSIM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

#include <TError.h>

#include "IlcRawDataHeader.h"
#include "IlcRunLoader.h"
#include "IlcCentralTrigger.h"

class IlcRawDataHeaderSim : public IlcRawDataHeader {

public:
  IlcRawDataHeaderSim() : IlcRawDataHeader() {
    // Takes the trigger mask and
    // stores it in the data header
    IlcRunLoader *runloader = IlcRunLoader::Instance();
    if (runloader) {
      if(!runloader->GetTrigger()) runloader->LoadTrigger();
      if (IlcCentralTrigger *aCTP = runloader->GetTrigger()) {
	ULong64_t mask = aCTP->GetClassMask();
	SetTriggerClass(mask);
      }
      else
	Warning("SetTriggerClass","No trigger can be loaded! Putting empty trigger class into the raw data header !");
    }
    else
      Error("SetTriggerClass","No run loader is available! Putting empty trigger class into the raw data header !");
  }

};

#endif
