#ifndef ILCTARGETBEAMTESTDIGSPD_H
#define ILCTARGETBEAMTESTDIGSPD_H

////////////////////////////////////////////////////
//  Class to define                               //
//  SPD beam test raw 2 dig conv.                 //
//
//  Origin: Jan Conrad Jan.Conrad@cern.ch         //
////////////////////////////////////////////////////

#include "IlcTARGETBeamTestDig.h"


class IlcTARGETBeamTestDigSPD: public IlcTARGETBeamTestDig {
 
 public:

 
  IlcTARGETBeamTestDigSPD();
  IlcTARGETBeamTestDigSPD(const Text_t* name, const Text_t* title);
  virtual ~IlcTARGETBeamTestDigSPD();

  virtual void Exec(Option_t* opt);
  
  
 protected:      
  
   
  Bool_t fFlagHeader; // flag for the event header



 ClassDef(IlcTARGETBeamTestDigSPD,0)  // An Ilc SPD beam test run

 };


#endif

    
