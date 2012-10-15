#ifndef ILCFRAME_H
#define ILCFRAME_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcFRAME.h 27058 2008-07-02 19:55:01Z morsch $ */

////////////////////////////////////////////////
//  Manager class for detector: FRAME         //
////////////////////////////////////////////////
 
#include "IlcModule.h"


class IlcFRAME : public IlcModule {
  
public:
  IlcFRAME();
  IlcFRAME(const char *name, const char *title);
  virtual      ~IlcFRAME() {}
  virtual void   Init() {}
  virtual Int_t IsVersion() const =0;
 protected:
  Int_t fRefVolumeId1;    // Id of the reference volume
  Int_t fRefVolumeId2;    // Id of the reference volume
   ClassDef(IlcFRAME,2)  //Class for Space Frame
};

#endif



