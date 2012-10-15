#ifndef IlcDCHCALMONITORING_H
#define IlcDCHCALMONITORING_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHCalMonitoring.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for monitoring data                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"

class IlcDCHCalMonitoring : public TNamed {
  public:
    IlcDCHCalMonitoring();
    IlcDCHCalMonitoring(const Text_t* name, const Text_t* title);
    virtual ~IlcDCHCalMonitoring() {};

    //void SetSamplingFrequency(Float_t freq)                { fSamplingFrequency = freq; };
    //Float_t GetSamplingFrequency()                   const { return fSamplingFrequency; };

  protected:
    Int_t fADCTresholds[6700];
    Float_t fDriftVelocity;                      // Drift velocity from the monitor
    TString fGasComposition;			 // Gas composition
    Float_t fEnvironmentTemperature;

    //Float_t fMCMTemperature[6700];

    Float_t fAnodeCurrentsMin[540];
    Float_t fAnodeCurrentsMax[540];
    Float_t fDriftCurrentsMin[540];
    Float_t fDriftCurrentsMax[540];
    Float_t fAnodeVoltagesMin[540];
    Float_t fAnodeVoltagesMax[540];
    Float_t fDriftVoltagesMin[540];
    Float_t fDriftVoltagesMax[540];

    Float_t fLVVoltage[360];
    Float_t fLVCurrent[360];

    void Init();

    ClassDef(IlcDCHCalMonitoring,1)                      //  DCH calibration class for global DCH parameters
};

#endif
