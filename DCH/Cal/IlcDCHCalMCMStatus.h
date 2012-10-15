#ifndef ILCDCHCALMCMSTATUS_H
#define ILCDCHCALMCMSTATUS_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHCalMCMStatus.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for MCM status                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "IlcDCHgeometry.h"
#include "IlcDCHCalSingleChamberStatus.h"

class IlcDCHCalMCMStatus : public TNamed {

 public:
 
  enum { kNplan = 6, kNcham = 5, kNsect = 18, kNdet = 540 };
  enum { kMasked = 2, kMCMTemperatureBit1 = 4, kMBMTemperatureBit2 = 8 };

  IlcDCHCalMCMStatus();
  IlcDCHCalMCMStatus(const Text_t* name, const Text_t* title);
  IlcDCHCalMCMStatus(const IlcDCHCalMCMStatus &c);   
  virtual ~IlcDCHCalMCMStatus();
  IlcDCHCalMCMStatus &operator=(const IlcDCHCalMCMStatus &c);

  virtual void     Copy(TObject &c) const;

  IlcDCHCalSingleChamberStatus *GetCalROC(Int_t d) const { return fROC[d]; };
  IlcDCHCalSingleChamberStatus *GetCalROC(Int_t p, Int_t c, Int_t s) const
                                               { return GetCalROC(IlcDCHgeometry::GetDetector(p,c,s)); };

  Bool_t IsMasked(Int_t d, Int_t col, Int_t row) const { return CheckStatus(d, col, row, kMasked); };
  inline Bool_t CheckStatus(Int_t d, Int_t col, Int_t row, Int_t bitMask) const;

 protected:

  IlcDCHCalSingleChamberStatus *fROC[kNdet];          //  Array of ROC objects which contain the values per pad

  ClassDef(IlcDCHCalMCMStatus,1)                      //  DCH calibration class for MCM status

};

Bool_t IlcDCHCalMCMStatus::CheckStatus(Int_t d, Int_t col, Int_t row, Int_t bitMask) const
{
  IlcDCHCalSingleChamberStatus* roc = GetCalROC(d);
  if (!roc)
    return kFALSE;
    
  return (roc->GetStatus(col, row) & bitMask) ? kTRUE : kFALSE;
}

#endif
