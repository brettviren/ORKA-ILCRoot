#ifndef ILCBODY_H
#define ILCBODY_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcBODY.h 47909 2011-02-28 15:05:53Z fca $ */

////////////////////////////////////////////////
//  Manager class for detector: BODY          //
//   This is the envelop for Ilc            //
////////////////////////////////////////////////
 
#include "IlcModule.h"
 
 
class IlcBODY : public IlcModule {
 
public:
   IlcBODY();
   IlcBODY(const char *name, const char *title);
   virtual      ~IlcBODY() {}
   virtual void  CreateGeometry();
   virtual void  CreateMaterials();
   virtual Int_t IsVersion() const {return 0;}

   ClassDef(IlcBODY,1)  //Class manager for the ILC body
};

#endif
