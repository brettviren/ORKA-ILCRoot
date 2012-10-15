#ifndef ILCPVECCPVCILCBDATA_H
#define ILCPVECCPVCILCBDATA_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////
//  class for CPV calibration                 //
////////////////////////////////////////////////

#include "TNamed.h"

class IlcPVECCpvCalibData: public TNamed {

 public:
  IlcPVECCpvCalibData();
  IlcPVECCpvCalibData(const char* name);
  IlcPVECCpvCalibData(const IlcPVECCpvCalibData &calibda);
  IlcPVECCpvCalibData& operator= (const IlcPVECCpvCalibData &calibda);
  virtual ~IlcPVECCpvCalibData();
  void Reset();
  virtual void Print(Option_t *option = "") const; 
  //
  Float_t GetADCchannelCpv(Int_t module, Int_t column, Int_t row) const;
  Float_t GetADCpedestalCpv(Int_t module, Int_t column, Int_t row) const;
  //
  void SetADCchannelCpv(Int_t module, Int_t column, Int_t row, Float_t value);
  void SetADCpedestalCpv(Int_t module, Int_t column, Int_t row, Float_t value);

 protected:
  Float_t  fADCchannelCpv[5][56][128];  // width of one CPV ADC channel ([mod][col][row])
  Float_t  fADCpedestalCpv[5][56][128]; // value of the CPV ADC pedestal ([mod][col][row])
  //
  ClassDef(IlcPVECCpvCalibData,1)    // CPV Calibration data

};

#endif
