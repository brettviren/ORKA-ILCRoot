#ifndef IlcDCHCALGLOBALS_H
#define IlcDCHCALGLOBALS_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHCalGlobals.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for global DCH parameters        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"

class IlcDCHCalGlobals : public TNamed {
  public:
    IlcDCHCalGlobals();
    IlcDCHCalGlobals(const Text_t* name, const Text_t* title);
    virtual ~IlcDCHCalGlobals() {};
    
    void SetSamplingFrequency(Float_t freq)                { fSamplingFrequency = freq; };
    Float_t GetSamplingFrequency()                   const { return fSamplingFrequency; };
    
    void SetNumberOfTimeBins(Int_t value)     { fNumberOfTimeBins = value; };
    Int_t GetNumberOfTimeBins()         const { return fNumberOfTimeBins; };
  
  protected:
    Float_t fSamplingFrequency;                  // Sampling Frequency in MHz
    Int_t fNumberOfTimeBins;                     // Number of timebins  
    
    void Init();
    
    ClassDef(IlcDCHCalGlobals,1)                      //  DCH calibration class for global DCH parameters
};

#endif
