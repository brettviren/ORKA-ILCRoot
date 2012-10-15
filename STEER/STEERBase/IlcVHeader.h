#ifndef IlcVHeader_H
#define IlcVHeader_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcVHeader.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//     Event header base class
//     Author: Markus Oldenburg, CERN
//-------------------------------------------------------------------------

#include <TNamed.h>

class IlcVHeader : public TNamed {

 public :
  IlcVHeader(); 
  virtual ~IlcVHeader() { };
  IlcVHeader(const IlcVHeader& evt); 
  IlcVHeader& operator=(const IlcVHeader& evt);
  
  virtual UShort_t  GetBunchCrossNumber()   const = 0;
  virtual UInt_t    GetOrbitNumber()        const = 0;
  virtual UInt_t    GetPeriodNumber()       const = 0;
  virtual ULong64_t GetTriggerMask()        const = 0;
  virtual UChar_t   GetTriggerCluster()     const = 0;
  virtual UInt_t    GetEventType()          const = 0;
  
  /*
  virtual void SetBunchCrossNumber(UShort_t nBx)   = 0;
  virtual void SetOrbitNumber(UInt_t nOr)          = 0;
  virtual void SetPeriodNumber(UInt_t nPer)        = 0;
  virtual void SetTriggerMask(ULong64_t trigMsk)   = 0;
  virtual void SetTriggerCluster(UChar_t trigClus) = 0;
  virtual void SetEventType(UInt_t evttype)        = 0;
  */

  virtual void Print(Option_t* option = "") const  = 0;
    
  ClassDef(IlcVHeader,1);
};

#endif
