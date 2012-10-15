#ifndef ILCTRDTENDERSUPPLY_H
#define ILCTRDTENDERSUPPLY_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////
//                                                                    //
//  TRD tender, reapply pid on the fly                                //
//                                                                    //
////////////////////////////////////////////////////////////////////////



#include <IlcTenderSupply.h>

class IlcTRDpidRecalculator;
class IlcTRDCalDet;
class IlcESDEvent;
class IlcOADBContainer;

class IlcTRDTenderSupply: public IlcTenderSupply {
  
public:
  enum{
    kNNpid = 0,
    k1DLQpid = 1,
    k2DLQpid = 2
  };
  IlcTRDTenderSupply();
  IlcTRDTenderSupply(const char *name, const IlcTender *tender=NULL);
  virtual ~IlcTRDTenderSupply();

  void SetRunByRunCorrection(const char *filename) { fNameRunByRunCorrection = filename; }
  void SetLoadReferencesFromCDB() { fLoadReferences = kTRUE; fLoadReferencesFromCDB = kTRUE; }
  void SetLoadReferencesFromFile() { fLoadReferences = kTRUE; fLoadReferencesFromCDB = kFALSE; }
  void SetLoadDeadChambersFromCDB(){ fLoadDeadChambers = kTRUE;} ;
  void SetPIDmethod(Int_t pidMethod) { fPIDmethod = pidMethod; }
  void SetNormalizationFactor(Double_t norm) { fNormalizationFactor = norm; }
  void SetCalibLowpThreshold(Double_t pmin) { fPthreshold = pmin; };
  void SetGeoFile(const char *filename) { fGeoFile = filename; }
  void SetDebugMode() { fDebugMode = kTRUE; }

  virtual void              Init();
  virtual void              ProcessEvent();
  
  void SwitchOnGainCorrection() { fGainCorrection = kTRUE; }
  void SwitchOffGainCorrection() { fGainCorrection = kFALSE; }
  void SetSlicesForPID(UInt_t min, UInt_t max) { fSlicesForPID[0] = min; fSlicesForPID[1] = max;}
  void AddBadChamber(Int_t chamberID){fBadChamberID[fNBadChambers++] = chamberID;};
  
private:
  enum{
    kNPlanes = 6,
    kNStacks = 5,
    kNChambers = 540
  };

  Bool_t GetTRDchamberID(IlcESDtrack * const track, Int_t *detectors);
  void SetChamberGain();
  void ApplyGainCorrection(IlcESDtrack *track, const Int_t * const detectors);
  void ApplyRunByRunCorrection(IlcESDtrack *const track);
  void MaskChambers(IlcESDtrack * const track, const Int_t * const detectors); 
  void LoadReferences();
  void LoadDeadChambersFromCDB();
  void LoadRunByRunCorrection(const char *filename);
  Bool_t IsBadChamber(Int_t chamberID);
  
  IlcESDEvent           *fESD;       //! the ESD Event
  IlcESDpid             *fESDpid;    //! ESD PID object

  IlcTRDCalDet *fChamberGainOld;     // TRD Chamber Gain Factor used for producing the ESD
  IlcTRDCalDet *fChamberGainNew;     // New TRD Chamber Gain Factor
  IlcTRDCalDet *fChamberVdriftOld;   // Old drift velocity calibration
  IlcTRDCalDet *fChamberVdriftNew;   // New drift velocity calibration
  IlcOADBContainer *fRunByRunCorrection;    // Run by run gain correction

  Int_t fPIDmethod;                  // PID method
  Double_t fNormalizationFactor;     // dE/dx Normalization Factor 
  Double_t fPthreshold;              // Low Momentum threshold for calibration
  Int_t fBadChamberID[kNChambers];   // List of Bad Chambers
  UInt_t fSlicesForPID[2];           // Select range of slices used in the PID response
  UInt_t fNBadChambers;              // Number of bad chambers
  const char *fGeoFile;              // File with geometry.root
  Bool_t fGainCorrection;            // Apply gain correction 
  Bool_t fLoadReferences;            // Tender Load references
  Bool_t fLoadReferencesFromCDB;     // Load References from CDB
  Bool_t fLoadDeadChambers;          // Load dead chambers
  Bool_t fHasReferences;             // has references loaded
  Bool_t fHasNewCalibration;         // has new calibration
  Bool_t fDebugMode;                 // Run in debug mode
  TString fNameRunByRunCorrection;   // filename with the run-by-run gain correction
  
  IlcTRDTenderSupply(const IlcTRDTenderSupply&c);
  IlcTRDTenderSupply& operator= (const IlcTRDTenderSupply&c);
  
  ClassDef(IlcTRDTenderSupply, 1);  // TRD tender task
};
#endif

