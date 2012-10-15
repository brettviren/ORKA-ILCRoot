#ifndef ILCRSTACKCILCBDATA_H
#define ILCRSTACKCILCBDATA_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRSTACKCalibData.h 50875 2011-08-01 09:01:23Z policheh $ */

////////////////////////////////////////////////
//  class for RSTACK calibration                //
////////////////////////////////////////////////

#include "TNamed.h"
#include "TString.h"

class IlcRSTACKEmcCalibData;
class IlcRSTACKCpvCalibData;
class IlcRSTACKEmcBadChannelsMap;
class IlcCDBMetaData;

class IlcRSTACKCalibData: public TNamed {

 public:
  IlcRSTACKCalibData();
  IlcRSTACKCalibData(Int_t runNumber);
  IlcRSTACKCalibData(IlcRSTACKCalibData & RSTACKCDB);
  virtual ~IlcRSTACKCalibData();

  IlcRSTACKCalibData & operator = (const IlcRSTACKCalibData & rhs);

  void Reset();
  virtual void Print(Option_t *option = "") const; 

  IlcRSTACKEmcCalibData *GetCalibDataEmc() const {return fCalibDataEmc;}
  IlcRSTACKCpvCalibData *GetCalibDataCpv() const {return fCalibDataCpv;}
  
  void CreateNew();
  void RandomEmc(Float_t ccMin=0.5   , Float_t ccMax=1.5);
  void RandomCpv(Float_t ccMin=0.0009, Float_t ccMax=0.0015);

  //----First EMC parameters---------
  Float_t GetADCchannelEmc(Int_t module, Int_t column, Int_t row) const;
  void    SetADCchannelEmc(Int_t module, Int_t column, Int_t row, Float_t value);

  Float_t GetADCpedestalEmc(Int_t module, Int_t column, Int_t row) const;
  void    SetADCpedestalEmc(Int_t module, Int_t column, Int_t row, Float_t value);

  Float_t GetHighLowRatioEmc(Int_t module, Int_t column, Int_t row) const ;
  void    SetHighLowRatioEmc(Int_t module, Int_t column, Int_t row, Float_t value) ;
  
  Float_t GetTimeShiftEmc(Int_t module, Int_t column, Int_t row) const;
  void    SetTimeShiftEmc(Int_t module, Int_t column, Int_t row, Float_t value) ;

  Int_t  GetAltroOffsetEmc(Int_t module, Int_t column, Int_t row) const;
  void   SetAltroOffsetEmc(Int_t module, Int_t column, Int_t row, Int_t value) ;

  Float_t GetSampleTimeStep() const ;
  void    SetSampleTimeStep(Float_t step) ;

  //----Now CPV parameters-----------
  Float_t GetADCchannelCpv(Int_t module, Int_t column, Int_t row) const;
  void    SetADCchannelCpv(Int_t module, Int_t column, Int_t row, Float_t value);

  Float_t GetADCpedestalCpv(Int_t module, Int_t column, Int_t row) const;
  void    SetADCpedestalCpv(Int_t module, Int_t column, Int_t row, Float_t value);

  //----Bad channels map-------------
  Int_t  GetNumOfEmcBadChannels() const;
  Bool_t IsBadChannelEmc(Int_t module, Int_t col, Int_t row) const; 
  void   EmcBadChannelIds(Int_t *badIds=0); 

  void SetEmcDataPath(const char* emcPath) {fEmcDataPath=emcPath;}
  void SetCpvDataPath(const char* cpvPath) {fCpvDataPath=cpvPath;}

  Bool_t WriteEmc(Int_t firstRun, Int_t lastRun, IlcCDBMetaData *md);
  Bool_t WriteCpv(Int_t firstRun, Int_t lastRun, IlcCDBMetaData *md);
  Bool_t WriteEmcBadChannelsMap(Int_t firstRun, Int_t lastRun, IlcCDBMetaData *md);

  //----Decalibration factors for simulation-------------
  Float_t GetADCchannelEmcDecalib(Int_t module, Int_t column, Int_t row) const;
  void    SetADCchannelEmcDecalib(Int_t module, Int_t column, Int_t row, Float_t value);  
  
 private:

  IlcRSTACKEmcCalibData* fCalibDataEmc; // EMC calibration data
  IlcRSTACKCpvCalibData* fCalibDataCpv; // CPV calibration data
  IlcRSTACKEmcBadChannelsMap* fEmcBadChannelsMap; // EMC bad channels map
  
  TString fEmcDataPath; // path to EMC calibration data
  TString fCpvDataPath; // path to CPV calibration data
  TString fEmcBadChannelsMapPath; // path to bad channels map

  ClassDef(IlcRSTACKCalibData,6)    // RSTACK Calibration data
};

#endif
