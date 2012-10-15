#ifndef ILCDIGITNEW_H
#define ILCDIGITNEW_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDigitNew.h 50615 2011-07-16 23:19:19Z hristov $ */

////////////////////////////////////////////////
//  Base class for Ilc Digits               //
////////////////////////////////////////////////

#include "TObject.h"
class TClonesArray;

typedef TClonesArray DigitsList ;       

class IlcDigitNew : public TObject {

 public: 
  IlcDigitNew();   
  ~IlcDigitNew() {}
  Int_t   GetAmp() const { return fAmp  ; } 
  Int_t   GetId() const  { return fId ; }      
  Int_t   GetIndexInList() const { return fIndexInList ; } 
  void    SetIndexInList(Int_t val) { fIndexInList = val ; } 

 protected:
 
  Int_t fAmp ;               // digitalized energy
  Int_t fId ;                // absolute id
  Int_t fIndexInList ;       // the index of this digit in the list stored in TreeD
   
  ClassDef(IlcDigitNew,1)  //Base class for all Ilc digits

} ;
#endif // ILCDIGITNEW_H
