#ifndef ILCPVBARTRIGGERRAWREADER_H
#define ILCPVBARTRIGGERRAWREADER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
/* $Id$ */

#include "TObject.h"

class IlcCaloRawStreamV3;
class IlcPVBARTRURawReader;

/*
 *  Class for reading the Trigger Data Stream from Raw.
 *  Author: Henrik Qvigstad (henrik.qvigstad@cern.ch)
 */
class IlcPVBARTriggerRawReader : public TObject
{
 public:
  IlcPVBARTriggerRawReader();
 ~IlcPVBARTriggerRawReader();
  
  IlcPVBARTRURawReader* GetTRU(Int_t mod, Int_t truRow, Int_t branch);
  
  void ReadFromStream(IlcCaloRawStreamV3* );
  void Reset();
  
 private:
  IlcPVBARTriggerRawReader(const IlcPVBARTriggerRawReader&); // not implemented
  IlcPVBARTriggerRawReader& operator= (const IlcPVBARTriggerRawReader&); // not implemented

 private:
  // constants
  const static Int_t kNMods = 5; // n. mods
  const static Int_t kNTRURows = 4; // n. tru rows
  const static Int_t kNBranches = 2; // n. branches

  IlcPVBARTRURawReader* fTRUs[kNMods][kNTRURows][kNBranches]; // TRU raw readers [mod][truRow][branch]


  ClassDef(IlcPVBARTriggerRawReader, 0)
};

#endif 
