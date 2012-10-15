#ifndef ILCPVECEMCCILCBDATA_H
#define ILCPVECEMCCILCBDATA_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////
//  class for EMC calibration                 //
////////////////////////////////////////////////

#include "TNamed.h"

class IlcPVECEmcCalibData: public TNamed {

 public:
  IlcPVECEmcCalibData();
  IlcPVECEmcCalibData(const char* name);
  IlcPVECEmcCalibData(const IlcPVECEmcCalibData &calibda);
  IlcPVECEmcCalibData& operator= (const IlcPVECEmcCalibData &calibda);
  virtual ~IlcPVECEmcCalibData();
  void Reset();
  virtual void Print(Option_t *option = "") const; 
  //
  Float_t GetADCchannelEmc(Int_t module, Int_t column, Int_t row) const;
  Float_t GetADCpedestalEmc(Int_t module, Int_t column, Int_t row) const;
  Float_t GetHighLowRatioEmc(Int_t module, Int_t column, Int_t row) const;
  Float_t GetTimeShiftEmc(Int_t module, Int_t column, Int_t row) const;
  Int_t   GetAltroOffsetEmc(Int_t module, Int_t column, Int_t row) const;
  Float_t GetSampleTimeStep() const ;
  //
  void SetADCchannelEmc(Int_t module, Int_t column, Int_t row, Float_t value);
  void SetADCpedestalEmc(Int_t module, Int_t column, Int_t row, Float_t value);
  void SetHighLowRatioEmc(Int_t module, Int_t column, Int_t row, Float_t value);
  void SetTimeShiftEmc(Int_t module, Int_t column, Int_t row, Float_t value);
  void SetAltroOffsetEmc(Int_t module, Int_t column, Int_t row, Int_t value);
  void SetSampleTimeStep(Float_t step) ;
  //
  Float_t GetADCchannelEmcDecalib(Int_t module, Int_t column, Int_t row) const;
  void    SetADCchannelEmcDecalib(Int_t module, Int_t column, Int_t row, Float_t value);
  
protected:
  Float_t  fSampleTimeStep ;            // Conversion between ALTRO sample time step and seconds
  Float_t  fADCchannelEmc[5][56][64] ;  // width of one EMC ADC channel in GeV ([mod][col][row])
  Float_t  fADCpedestalEmc[5][56][64] ; // value of the EMC ADC pedestal ([mod][col][row])
  Float_t  fHighLowRatioEmc[5][56][64]; // ratio of High/Low gain ([mod][col][row])
  Float_t  fTimeShiftEmc[5][56][64] ;   // channel-by-channel t0 shift ([mod][col][row])
  UInt_t   fAltroOffsets[5][56][64] ;   // channel-by-channel offsets udes in ZS
  Float_t  fDecal[5][56][64] ;          // Random decalibrations O(1). Used in simulation.
  //
  ClassDef(IlcPVECEmcCalibData,5)    // PVEC EMC calibration data
};

#endif
