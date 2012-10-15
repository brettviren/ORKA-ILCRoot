#ifndef ILCPVBARTRIGGERRAWDIGIT_H
#define ILCPVBARTRIGGERRAWDIGIT_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

#include "IlcDigitNew.h"

class IlcPVBARTriggerRawDigit : public IlcDigitNew 
{
 public:

  IlcPVBARTriggerRawDigit();
  IlcPVBARTriggerRawDigit(Int_t module, Int_t xIdx, Int_t zIdx, Int_t TRURow, Int_t branch, Int_t amp);
  IlcPVBARTriggerRawDigit(const IlcPVBARTriggerRawDigit & tdigit);
  IlcPVBARTriggerRawDigit& operator=(const IlcPVBARTriggerRawDigit & tdigit);
  virtual ~IlcPVBARTriggerRawDigit() {}

  void Get4x4Position(Int_t& module, Int_t& xIdx, Int_t& zIdx, Int_t& TRURow, Int_t& branch) 
  {module = fMod; xIdx = fXIdx; zIdx = fZIdx; TRURow = fTRURow; branch = fBranch; }

  void GetModXZ(Int_t& mod, Int_t& modX, Int_t& modZ);

 private:

  Int_t fMod;  // module
  Int_t fXIdx; // 4x4 X 
  Int_t fZIdx; // 4x4 Z
  Int_t fTRURow; // TRU row
  Int_t fBranch; // branch

  ClassDef(IlcPVBARTriggerRawDigit,1)
};

#endif
