#ifndef ILCPVBARCPVCILCBDATA_H
#define ILCPVBARCPVCILCBDATA_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////
//  class for CPV calibration                 //
////////////////////////////////////////////////

#include "TNamed.h"

class IlcPVBARCpvCalibData: public TNamed {

 public:
  IlcPVBARCpvCalibData();
  IlcPVBARCpvCalibData(const char* name);
  IlcPVBARCpvCalibData(const IlcPVBARCpvCalibData &calibda);
  IlcPVBARCpvCalibData& operator= (const IlcPVBARCpvCalibData &calibda);
  virtual ~IlcPVBARCpvCalibData();
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
  ClassDef(IlcPVBARCpvCalibData,1)    // CPV Calibration data

};

#endif
