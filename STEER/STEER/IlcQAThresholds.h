#ifndef ILCQATHRESHOLDS_H
#define ILCQATHRESHOLDS_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Container for the parameters that might be needed by the QA.              //
// Each detector has 1 of these objects with the list of parameters.         //
// The parameters are created online and are passed to the offline through   //
// the shuttle to be added to the OCDB in the GRP.                           //
// Created by Barthelemy.von.Haller@cern.ch  30/11/2010                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TObjArray.h"
#include "TParameter.h"

class IlcQAThresholds: public TNamed {

 public:

  IlcQAThresholds(Int_t detId);
  IlcQAThresholds() : TNamed(), fThresholds(), fDetectorId(-1){};
  virtual ~IlcQAThresholds();

  virtual const char* GetName() const;
  virtual const char* GetTitle() const;

  Int_t GetDetectorId();
  void SetDetectorId(Int_t i);
  void AddThreshold(TParameter<long>* item);
  void AddThreshold(TParameter<int>* item);
  void AddThreshold(TParameter<double>* item);
  void AddThreshold(TParameter<float>* item);
  void AddThresholdAt(TParameter<long>* item, Int_t index);
  void AddThresholdAt(TParameter<int>* item, Int_t index);
  void AddThresholdAt(TParameter<double>* item, Int_t index);
  void AddThresholdAt(TParameter<float>* item, Int_t index);
  TObject* GetThreshold(Int_t i);
  Int_t GetSize();

 private:

  TObjArray fThresholds;
  Int_t fDetectorId;       // in the sense of the class IlcDAQ

  ClassDef(IlcQAThresholds, 3)
};

#endif
