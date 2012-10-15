#ifndef ILCMAG_H
#define ILCMAG_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcMAG.h 47909 2011-02-28 15:05:53Z fca $ */

////////////////////////////////////////////////
//  Manager class for detector: MAG           //
////////////////////////////////////////////////
 
#include "IlcModule.h"
 
 
class IlcMAG : public IlcModule {
 
public:
   IlcMAG();
   IlcMAG(const char *name, const char *title);
   virtual      ~IlcMAG() {}
   virtual void  CreateGeometry();
   virtual void  CreateMaterials();
   virtual void  Init();
   virtual Int_t IsVersion() const {return 0;}
 
   ClassDef(IlcMAG,1)  //Class manager for detector:MAG
};

#endif
