#ifndef ILCDETECTORRECOPARAM_H
#define ILCDETECTORRECOPARAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Base Class for Detector reconstruction parameters                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


#include "TNamed.h"

#include "IlcRecoParam.h"

class IlcDetectorRecoParam : public TNamed
{
  
 public: 
  IlcDetectorRecoParam();
  virtual ~IlcDetectorRecoParam();
  void  Print(Option_t */*option*/) const {Dump();}

  Int_t          GetEventSpecie() const { return fEventSpecie; }
  void           SetEventSpecie(IlcRecoParam::EventSpecie_t specie) { fEventSpecie = specie; }
  void           SetAsDefault() { fEventSpecie |= IlcRecoParam::kDefault; }
  Bool_t         IsDefault() const { return (fEventSpecie & IlcRecoParam::kDefault); }

private:

  Int_t fEventSpecie; // Event specie for which the reco-param object is valid
  
  ClassDef(IlcDetectorRecoParam, 3)
};


#endif
