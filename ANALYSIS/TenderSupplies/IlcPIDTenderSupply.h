#ifndef ILCPIDTENDERSUPPLY_H
#define ILCPIDTENDERSUPPLY_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////
//                                                                    //
//  PID tender, reapply pid on the fly                                //
//                                                                    //
////////////////////////////////////////////////////////////////////////



#include <IlcTenderSupply.h>

class IlcPIDTenderSupply: public IlcTenderSupply {
  
public:
  IlcPIDTenderSupply();
  IlcPIDTenderSupply(const char *name, const IlcTender *tender=NULL);
  
  virtual ~IlcPIDTenderSupply(){;}
  
  virtual void              Init(){;}
  virtual void              ProcessEvent();
  
private:
  
  IlcPIDTenderSupply(const IlcPIDTenderSupply&c);
  IlcPIDTenderSupply& operator= (const IlcPIDTenderSupply&c);
  
  ClassDef(IlcPIDTenderSupply, 1);  // PID tender task
};


#endif

