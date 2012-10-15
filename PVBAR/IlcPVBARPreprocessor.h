#ifndef ILCPVBARPREPROCESSOR_H
#define ILCPVBARPREPROCESSOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARPreprocessor.h 42225 2010-07-12 09:30:07Z policheh $ */

///////////////////////////////////////////////////////////////////////////////
// Class IlcPVBARPreprocessor
///////////////////////////////////////////////////////////////////////////////


#include "IlcPreprocessor.h"
#include "TFile.h"

class TList;
class IlcPVBAREmcBadChannelsMap;
class IlcPVBAREmcCalibData;

class IlcPVBARPreprocessor : public IlcPreprocessor {
 public:

  IlcPVBARPreprocessor();
  IlcPVBARPreprocessor(IlcShuttleInterface* shuttle);

 protected:

  virtual UInt_t Process(TMap* valueSet);
  Bool_t ProcessLEDRun();
  Bool_t FindBadChannelsEmc();
  Bool_t CalibrateEmc();
  Float_t HG2LG(Int_t module, Int_t X, Int_t Z, TFile* f);

 private:

  Bool_t DoCalibrateEmc(Int_t system, TList* sources, const IlcPVBAREmcBadChannelsMap* badMap, IlcPVBAREmcCalibData& calibData);
  Bool_t DoFindBadChannelsEmc(Int_t system, TList* sources, IlcPVBAREmcBadChannelsMap& badMap);
  Bool_t StoreReferenceEmc(Int_t system, TList* list);
  Bool_t StoreReferenceLED(TList* list);

  ClassDef(IlcPVBARPreprocessor,4);

};

#endif
