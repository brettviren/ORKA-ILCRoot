#ifndef ILCRSTACKTRIGGERRAWDIGIPRODUCER_H
#define ILCRSTACKTRIGGERRAWDIGIPRODUCER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

class IlcRSTACKTriggerRawReader;
class IlcRSTACKTriggerParameters;
class IlcRawReader;
class IlcCaloRawStreamV3;

#include "TString.h"
#include "TClonesArray.h"

#include <vector>

class IlcRSTACKTriggerRawDigiProducer
{
 public:
  
  IlcRSTACKTriggerRawDigiProducer();
  IlcRSTACKTriggerRawDigiProducer(IlcRawReader *rawReader);
  
  virtual ~IlcRSTACKTriggerRawDigiProducer();
  
  void ProcessEvent(TClonesArray* tdigits);

  void SetTriggerParameters(IlcRSTACKTriggerParameters* parameters) {fParameters = parameters;}
  void SetAnalyseModule(int mod, bool analyse = true) {fModules[mod] = analyse;}
    
  static int Get2x2Max(IlcRSTACKTriggerRawReader*, IlcRSTACKTriggerParameters*, int mod, int xIdx, int zIdx);
  static int Get2x2Signal(IlcRSTACKTriggerRawReader*, IlcRSTACKTriggerParameters*, int mod, int xIdx, int zIdx, int timeBin);
  static int Get4x4Max(IlcRSTACKTriggerRawReader*, IlcRSTACKTriggerParameters*, int mod, int TRURow, int branch, int xIdx, int zIdx);
  static int Get4x4Signal(IlcRSTACKTriggerRawReader*, IlcRSTACKTriggerParameters*, int mod, int TRURow, int branch, int xIdx, int zIdx, int timeBin);
  
  static bool Is2x2Active(IlcRSTACKTriggerRawReader*, int mod, int xIdx, int zIdx);
  static bool Is2x2Active(IlcRSTACKTriggerRawReader*, int mod, int xIdx, int zIdx, int timeBin);
    
  const static int kNMods = 5; // number of RSTACK modules
  const static int kNTRURows = 4; // number of TRU rows
  const static int kNBranches = 2; // number of branches
  const static int kN2x2X = 32; // (=64/2) Number of 2x2 in X direction
  const static int kN2x2Z = 28; // (=56/2) Number of 2x2 in Z direction
  const static Int_t kN2x2XPrTRURow = 8; // (=64/2/4) Number of 2x2 pr. row
  const static Int_t kN2x2ZPrBranch = 14; // (=56/2/2) Number of 2x2 pr. branch
  const static Int_t kN4x4XPrTRURow = 7; // (=64/2/4 -1) Number of 4x4 pr. row
  const static Int_t kN4x4ZPrBranch = 13; // (=56/2/2) -1 Number of 4x4 pr. branch  
  const static int kNTRUTimeBins = 128; // number of TRU time bins
  const static int kNDefaultNEMCTimeBins = 62;

 private:  
  IlcRSTACKTriggerRawDigiProducer(const IlcRSTACKTriggerRawDigiProducer &tdp); // not implemented
  IlcRSTACKTriggerRawDigiProducer& operator= (const IlcRSTACKTriggerRawDigiProducer &tdp); // not implemented

 protected:
  std::vector<bool> fModules; // , per module: should analyser analyse module
  UShort_t fSaturationThreshold;
  IlcRSTACKTriggerParameters* fParameters;
  
private:
  IlcRawReader            * fRawReader;       //! Raw data reader
  IlcCaloRawStreamV3      * fRawStream;       //! Calorimeter decoder of ALTRO format
  IlcRSTACKTriggerRawReader * fTriggerReader;   //! TriggerRawReader

  ClassDef(IlcRSTACKTriggerRawDigiProducer,1)
};

#endif
