#ifndef ILCPVECTRIGGERRAWREADER_H
#define ILCPVECTRIGGERRAWREADER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
/* $Id$ */

#include "TObject.h"

class IlcCaloRawStreamV3;
class IlcPVECTRURawReader;

/*
 *  Class for reading the Trigger Data Stream from Raw.
 *  Author: Henrik Qvigstad (henrik.qvigstad@cern.ch)
 */
class IlcPVECTriggerRawReader : public TObject
{
 public:
  IlcPVECTriggerRawReader();
 ~IlcPVECTriggerRawReader();
  
  IlcPVECTRURawReader* GetTRU(Int_t mod, Int_t truRow, Int_t branch);
  
  void ReadFromStream(IlcCaloRawStreamV3* );
  void Reset();
  
 private:
  IlcPVECTriggerRawReader(const IlcPVECTriggerRawReader&); // not implemented
  IlcPVECTriggerRawReader& operator= (const IlcPVECTriggerRawReader&); // not implemented

 private:
  // constants
  const static Int_t kNMods = 5; // n. mods
  const static Int_t kNTRURows = 4; // n. tru rows
  const static Int_t kNBranches = 2; // n. branches

  IlcPVECTRURawReader* fTRUs[kNMods][kNTRURows][kNBranches]; // TRU raw readers [mod][truRow][branch]


  ClassDef(IlcPVECTriggerRawReader, 0)
};

#endif 
