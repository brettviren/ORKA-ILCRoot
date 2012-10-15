#ifndef ILCHMPIDTENDERSUPPLY_H
#define ILCHMPIDTENDERSUPPLY_H

/* 2012-2013 Infrastructure for Large Collider Experiment, All rights reserved. *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////
//                                                                    //
//  HMPID tender, reapply pid on the fly                                //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <IlcTenderSupply.h>
#include <IlcLog.h>
#include <IlcESDpid.h>

class IlcESDpid;

class IlcHMPIDTenderSupply: public IlcTenderSupply {

public:
  IlcHMPIDTenderSupply();
  IlcHMPIDTenderSupply(const char *name, const IlcTender *tender=NULL);

  virtual ~IlcHMPIDTenderSupply(){;}

  virtual void              Init();
  virtual void              ProcessEvent();


private:

  IlcHMPIDTenderSupply(const IlcHMPIDTenderSupply&c);
  IlcHMPIDTenderSupply& operator= (const IlcHMPIDTenderSupply&c);

  ClassDef(IlcHMPIDTenderSupply, 1);
};


#endif 
