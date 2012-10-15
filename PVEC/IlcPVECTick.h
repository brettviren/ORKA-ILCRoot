#ifndef ILCPVECTICK_H
#define ILCPVECTICK_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVECTick.h 7865 2003-07-13 09:26:14Z hristov $ */

//_________________________________________________________________________
//  Class for PVEC time digitization
//                  
//*-- Author: Dmitri Peressounko (SUBATECH)


// --- ROOT system ---
#include "TObject.h"
// --- Standard library ---

// --- IlcRoot header files ---

class IlcPVECTick: public TObject {

public:
  IlcPVECTick() ;          
  IlcPVECTick(Float_t time, Float_t a, Float_t slope) ;
  virtual ~IlcPVECTick(){}

  Int_t   Compare(const TObject * obj) const ;  
  Bool_t  IsSortable() const { return kTRUE ; }

  Float_t CrossingTime(Float_t threshold) const
    //Calculates time, when rizing front of the signal crosses 
    {if(fB) return fTime + (threshold - fA)/fB ;
    else return 1. ;} //return very big time

  Float_t GetTime(void){return fTime ;}

  void operator+=(IlcPVECTick const &rValue) ;


private:
  Float_t fTime ;     //!time of the beginning of this tick
  Float_t fA ;        //!constant
  Float_t fB ;        //!slope        

  ClassDef(IlcPVECTick,1)  // description 

};

#endif // IlcPVECTICK_H
