#ifndef ILCRSTACKPREPROCESSOR_H
#define ILCRSTACKPREPROCESSOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKPreprocessor.h 42225 2010-07-12 09:30:07Z policheh $ */

///////////////////////////////////////////////////////////////////////////////
// Class IlcRSTACKPreprocessor
///////////////////////////////////////////////////////////////////////////////


#include "IlcPreprocessor.h"
#include "TFile.h"

class TList;
class IlcRSTACKEmcBadChannelsMap;
class IlcRSTACKEmcCalibData;

class IlcRSTACKPreprocessor : public IlcPreprocessor {
 public:

  IlcRSTACKPreprocessor();
  IlcRSTACKPreprocessor(IlcShuttleInterface* shuttle);

 protected:

  virtual UInt_t Process(TMap* valueSet);
  Bool_t ProcessLEDRun();
  Bool_t FindBadChannelsEmc();
  Bool_t CalibrateEmc();
  Float_t HG2LG(Int_t module, Int_t X, Int_t Z, TFile* f);

 private:

  Bool_t DoCalibrateEmc(Int_t system, TList* sources, const IlcRSTACKEmcBadChannelsMap* badMap, IlcRSTACKEmcCalibData& calibData);
  Bool_t DoFindBadChannelsEmc(Int_t system, TList* sources, IlcRSTACKEmcBadChannelsMap& badMap);
  Bool_t StoreReferenceEmc(Int_t system, TList* list);
  Bool_t StoreReferenceLED(TList* list);

  ClassDef(IlcRSTACKPreprocessor,4);

};

#endif
