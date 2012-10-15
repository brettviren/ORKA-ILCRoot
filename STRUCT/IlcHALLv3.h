#ifndef ILCHALLV3_H
#define ILCHALLV3_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcHALLv3.h 53956 2012-01-17 13:19:32Z morsch $ */

////////////////////////////////////////////////
//  Manager class for detector: HALL          //
////////////////////////////////////////////////
 
#include "IlcHALL.h"
 
 
class IlcHALLv3 : public IlcHALL {
 
public:
   IlcHALLv3();
   IlcHALLv3(const char *name, const char *title);
   virtual      ~IlcHALLv3() {}
   virtual void  CreateGeometry();
   virtual void  StepManager();
   virtual void  Init();
   virtual void  SetNewShield24() {fNewShield24 = 1;}
   virtual void  SetScoring()     {fScoring     = 1;}
private:
   Bool_t fNewShield24;   // Option for new shielding in PX24 and RB24
   Int_t  fRefVolumeId;   // Volume ID of scoring plane
   Bool_t fScoring;       // Scoring Option
   ClassDef(IlcHALLv3,1)  //Class for ILC experimental hall
};

#endif
