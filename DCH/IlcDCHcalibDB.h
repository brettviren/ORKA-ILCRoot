#ifndef ILCDCHCILCBDB_H
#define ILCDCHCILCBDB_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Class providing the calibration parameters by accessing the CDB           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

/* $Id: IlcDCHcalibDB.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

#include <iostream>
#include "TObject.h"

#include "IlcLog.h"
#include "IlcDCHgeometry.h"

#include <IlcCDBStorage.h>
#include <IlcCDBEntry.h>

class IlcDCHCalPIDLQ;
class IlcDCHCalMonitoring;

class IlcDCHcalibDB : public TObject
{
public:
  enum { kNplan = 6, kNcham = 5, kNsect = 18, kNdet = 540 };
  
  static IlcDCHcalibDB* Instance();
  static void Terminate();

  void SetRun(Long64_t run);
  Long64_t GetRun() { return fRun; }

  Bool_t GetChamberPos(Int_t det, Float_t* xyz);
  Bool_t GetChamberRot(Int_t det, Float_t* xyz);

  Bool_t GetStackPos(Int_t chamber, Int_t sector, Float_t* xyz);
  Bool_t GetStackRot(Int_t chamber, Int_t sector, Float_t* xyz);

  Bool_t GetSuperModulePos(Int_t sm, Float_t* xyz);
  Bool_t GetSuperModuleRot(Int_t sm, Float_t* xyz);

  Float_t GetVdrift(Int_t det, Int_t col, Int_t row);
  Float_t GetVdriftAverage(Int_t det);

  Float_t GetT0(Int_t det, Int_t col, Int_t row);
  Float_t GetT0Average(Int_t det);

  Float_t GetGainFactor(Int_t det, Int_t col, Int_t row);
  Float_t GetGainFactorAverage(Int_t det);

  Float_t GetPRFWidth(Int_t det, Int_t col, Int_t row);

  Float_t GetSamplingFrequency();
  Int_t GetNumberOfTimeBins();

  Char_t GetPadStatus(Int_t det, Int_t col, Int_t row);
  Char_t GetMCMStatus(Int_t det, Int_t col, Int_t row);
  Char_t GetChamberStatus(Int_t det);
  Char_t GetSuperModuleStatus(Int_t sm);

  Bool_t IsPadMasked(Int_t det, Int_t col, Int_t row);
  Bool_t IsPadBridgedLeft(Int_t det, Int_t col, Int_t row);
  Bool_t IsPadBridgedRight(Int_t det, Int_t col, Int_t row);
  
  Bool_t IsMCMMasked(Int_t det, Int_t col, Int_t row);
  
  Bool_t IsChamberInstalled(Int_t det);
  Bool_t IsChamberMasked(Int_t det);
  
  Bool_t IsSuperModuleInstalled(Int_t det);
  Bool_t IsSuperModuleMasked(Int_t det);

  const IlcDCHCalMonitoring* GetMonitoringObject();
  const IlcDCHCalPIDLQ* GetPIDLQObject();

  //Related functions, these depend on calibration data
  static Float_t GetOmegaTau(Float_t vdrift);
  Int_t PadResponse(Double_t signal, Double_t dist, Int_t plane, Double_t *pad) const;
  
protected:
  // for caching see also implentation of GetCachedCDBObject in the .cxx file
  enum { kCDBCacheSize = 17 };   // Number of cached objects
  enum { kIDVdriftPad = 0, kIDVdriftChamber,
         kIDT0Pad, kIDT0Chamber,
         kIDGainFactorPad, kIDGainFactorChamber,
         kIDPRFWidth,
         kIDGlobals,
         kIDChamberPos, kIDStackPos, kIDSuperModulePos,
         kIDPIDLQ, kIDMonitoringData,
         kIDSuperModuleStatus, kIDChamberStatus, kIDMCMStatus, kIDPadStatus };    // IDs of cached objects

  const TObject* GetCachedCDBObject(Int_t id);
  
  void Invilcdate();
  void SamplePRF();
  
  IlcCDBEntry* GetCDBEntry(const char* cdbPath);
  const TObject* CacheCDBEntry(Int_t id, const char* cdbPath);

  static IlcDCHcalibDB* fgInstance;     // Instance of this class (singleton implementation)
  static Bool_t fgTerminated;               // Defines if this class has already been terminated and therefore does not return instances in GetInstance anymore

  IlcCDBEntry* fCDBEntries[kCDBCacheSize];    // Cache for CDB entries
  TObject* fCDBCache[kCDBCacheSize];          // Cache for calibration objects.

  Long64_t fRun;
  
  struct 
  {
    Float_t             *fPRFsmp;                             //! Sampled pad response
    Int_t                fPRFbin;                             //  Number of bins for the PRF
    Float_t              fPRFlo;                              //  Lower boundary of the PRF
    Float_t              fPRFhi;                              //  Higher boundary of the PRF
    Float_t              fPRFwid;                             //  Bin width of the sampled PRF
    Int_t                fPRFpad;                             //  Distance to next pad in PRF
  } fPadResponse;
  
private:
  IlcDCHcalibDB(); // this is a singleton, constructor is private!  
  virtual ~IlcDCHcalibDB();

  ClassDef(IlcDCHcalibDB, 2)
};

#endif
