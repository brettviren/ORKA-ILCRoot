#ifndef ILCRSTACKTRIGGERRAWREADER_H
#define ILCRSTACKTRIGGERRAWREADER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
/* $Id$ */

#include "TObject.h"

class IlcCaloRawStreamV3;
class IlcRSTACKTRURawReader;

/*
 *  Class for reading the Trigger Data Stream from Raw.
 *  Author: Henrik Qvigstad (henrik.qvigstad@cern.ch)
 */
class IlcRSTACKTriggerRawReader : public TObject
{
 public:
  IlcRSTACKTriggerRawReader();
 ~IlcRSTACKTriggerRawReader();
  
  IlcRSTACKTRURawReader* GetTRU(Int_t mod, Int_t truRow, Int_t branch);
  
  void ReadFromStream(IlcCaloRawStreamV3* );
  void Reset();
  
 private:
  IlcRSTACKTriggerRawReader(const IlcRSTACKTriggerRawReader&); // not implemented
  IlcRSTACKTriggerRawReader& operator= (const IlcRSTACKTriggerRawReader&); // not implemented

 private:
  // constants
  const static Int_t kNMods = 5; // n. mods
  const static Int_t kNTRURows = 4; // n. tru rows
  const static Int_t kNBranches = 2; // n. branches

  IlcRSTACKTRURawReader* fTRUs[kNMods][kNTRURows][kNBranches]; // TRU raw readers [mod][truRow][branch]


  ClassDef(IlcRSTACKTriggerRawReader, 0)
};

#endif 
