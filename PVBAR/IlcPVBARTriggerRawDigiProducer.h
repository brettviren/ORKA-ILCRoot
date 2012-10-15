#ifndef ILCPVBARTRIGGERRAWDIGIPRODUCER_H
#define ILCPVBARTRIGGERRAWDIGIPRODUCER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

class IlcPVBARTriggerRawReader;
class IlcPVBARTriggerParameters;
class IlcRawReader;
class IlcCaloRawStreamV3;

#include "TString.h"
#include "TClonesArray.h"

#include <vector>

class IlcPVBARTriggerRawDigiProducer
{
 public:
  
  IlcPVBARTriggerRawDigiProducer();
  IlcPVBARTriggerRawDigiProducer(IlcRawReader *rawReader);
  
  virtual ~IlcPVBARTriggerRawDigiProducer();
  
  void ProcessEvent(TClonesArray* tdigits);

  void SetTriggerParameters(IlcPVBARTriggerParameters* parameters) {fParameters = parameters;}
  void SetAnalyseModule(int mod, bool analyse = true) {fModules[mod] = analyse;}
    
  static int Get2x2Max(IlcPVBARTriggerRawReader*, IlcPVBARTriggerParameters*, int mod, int xIdx, int zIdx);
  static int Get2x2Signal(IlcPVBARTriggerRawReader*, IlcPVBARTriggerParameters*, int mod, int xIdx, int zIdx, int timeBin);
  static int Get4x4Max(IlcPVBARTriggerRawReader*, IlcPVBARTriggerParameters*, int mod, int TRURow, int branch, int xIdx, int zIdx);
  static int Get4x4Signal(IlcPVBARTriggerRawReader*, IlcPVBARTriggerParameters*, int mod, int TRURow, int branch, int xIdx, int zIdx, int timeBin);
  
  static bool Is2x2Active(IlcPVBARTriggerRawReader*, int mod, int xIdx, int zIdx);
  static bool Is2x2Active(IlcPVBARTriggerRawReader*, int mod, int xIdx, int zIdx, int timeBin);
    
  const static int kNMods = 5; // number of PVBAR modules
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
  IlcPVBARTriggerRawDigiProducer(const IlcPVBARTriggerRawDigiProducer &tdp); // not implemented
  IlcPVBARTriggerRawDigiProducer& operator= (const IlcPVBARTriggerRawDigiProducer &tdp); // not implemented

 protected:
  std::vector<bool> fModules; // , per module: should analyser analyse module
  UShort_t fSaturationThreshold;
  IlcPVBARTriggerParameters* fParameters;
  
private:
  IlcRawReader            * fRawReader;       //! Raw data reader
  IlcCaloRawStreamV3      * fRawStream;       //! Calorimeter decoder of ALTRO format
  IlcPVBARTriggerRawReader * fTriggerReader;   //! TriggerRawReader

  ClassDef(IlcPVBARTriggerRawDigiProducer,1)
};

#endif
