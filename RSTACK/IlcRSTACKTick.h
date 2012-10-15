#ifndef ILCRSTACKTICK_H
#define ILCRSTACKTICK_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKTick.h 7865 2003-07-13 09:26:14Z hristov $ */

//_________________________________________________________________________
//  Class for RSTACK time digitization
//                  
//*-- Author: Dmitri Peressounko (SUBATECH)


// --- ROOT system ---
#include "TObject.h"
// --- Standard library ---

// --- IlcRoot header files ---

class IlcRSTACKTick: public TObject {

public:
  IlcRSTACKTick() ;          
  IlcRSTACKTick(Float_t time, Float_t a, Float_t slope) ;
  virtual ~IlcRSTACKTick(){}

  Int_t   Compare(const TObject * obj) const ;  
  Bool_t  IsSortable() const { return kTRUE ; }

  Float_t CrossingTime(Float_t threshold) const
    //Calculates time, when rizing front of the signal crosses 
    {if(fB) return fTime + (threshold - fA)/fB ;
    else return 1. ;} //return very big time

  Float_t GetTime(void){return fTime ;}

  void operator+=(IlcRSTACKTick const &rValue) ;


private:
  Float_t fTime ;     //!time of the beginning of this tick
  Float_t fA ;        //!constant
  Float_t fB ;        //!slope        

  ClassDef(IlcRSTACKTick,1)  // description 

};

#endif // IlcRSTACKTICK_H
