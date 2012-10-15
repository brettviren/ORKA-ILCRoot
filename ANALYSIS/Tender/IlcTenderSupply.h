#ifndef ILCTENDERSUPPLY_H
#define ILCTENDERSUPPLY_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcTenderSupply.h 40318 2010-04-12 08:04:15Z agheata $ */
// Author: Andrei Gheata, 01/09/2009

//==============================================================================
//   IlcTenderSupply - Base class for user-defined ESD additions and corrections.
//==============================================================================

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

class IlcTender;

class IlcTenderSupply : public TNamed {

protected:
  const IlcTender          *fTender;         // Tender car
  
public:  
  IlcTenderSupply();
  IlcTenderSupply(const char *name, const IlcTender *tender=NULL);
  IlcTenderSupply(const IlcTenderSupply &other);
  virtual ~IlcTenderSupply();
  IlcTenderSupply& operator=(const IlcTenderSupply &other);

  // Run control
  virtual void              Init() = 0;
  virtual void              ProcessEvent() = 0;
  
  void                      SetTender(const IlcTender *tender) {fTender = tender;}
    
  ClassDef(IlcTenderSupply,1)  // Base class for tender user algorithms
};
#endif
