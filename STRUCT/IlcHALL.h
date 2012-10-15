#ifndef ILCHALL_H
#define ILCHALL_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcHALL.h 47909 2011-02-28 15:05:53Z fca $ */

////////////////////////////////////////////////
//  Manager class for detector: HALL          //
////////////////////////////////////////////////
 
#include "IlcModule.h"
 
 
class IlcHALL : public IlcModule {
 
public:
   IlcHALL();
   IlcHALL(const char *name, const char *title);
   virtual      ~IlcHALL() {}
   virtual void  CreateGeometry();
   virtual void  CreateMaterials();
   virtual void  Init();
   virtual Int_t IsVersion() const {return 0;}
 
   ClassDef(IlcHALL,1)  //Class for ILC experimental hall
};

#endif
