#ifndef ILCDCHCALPADSTATUS_H
#define ILCDCHCALPADSTATUS_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHCalPadStatus.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for MCM status                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "IlcDCHgeometry.h"
#include "IlcDCHCalSingleChamberStatus.h"

class IlcDCHCalPadStatus : public TNamed {

 public:
 
  enum { kNplan = 6, kNcham = 5, kNsect = 18, kNdet = 540 };
  enum { kMasked = 2, kPadBridgedLeft = 4,     kPadBridgedRight = 8    };

  IlcDCHCalPadStatus();
  IlcDCHCalPadStatus(const Text_t* name, const Text_t* title);
  IlcDCHCalPadStatus(const IlcDCHCalPadStatus &c);   
  virtual ~IlcDCHCalPadStatus();
  IlcDCHCalPadStatus &operator=(const IlcDCHCalPadStatus &c);

  virtual void     Copy(TObject &c) const;

  IlcDCHCalSingleChamberStatus *GetCalROC(Int_t d) const { return fROC[d]; };
  IlcDCHCalSingleChamberStatus *GetCalROC(Int_t p, Int_t c, Int_t s) const
                                               { return fROC[IlcDCHgeometry::GetDetector(p,c,s)]; };

  Bool_t IsMasked(Int_t d, Int_t col, Int_t row) const { return CheckStatus(d, col, row, kMasked); };
  Bool_t IsBridgedLeft(Int_t d, Int_t col, Int_t row) const { return CheckStatus(d, col, row, kPadBridgedLeft); };
  Bool_t IsBridgedRight(Int_t d, Int_t col, Int_t row) const { return CheckStatus(d, col, row, kPadBridgedRight); };
  inline Bool_t CheckStatus(Int_t d, Int_t col, Int_t row, Int_t bitMask) const;

 protected:

  IlcDCHCalSingleChamberStatus *fROC[kNdet];          //  Array of ROC objects which contain the values per pad

  ClassDef(IlcDCHCalPadStatus,1)                      //  DCH calibration class for MCM status

};

Bool_t IlcDCHCalPadStatus::CheckStatus(Int_t d, Int_t col, Int_t row, Int_t bitMask) const
{
  IlcDCHCalSingleChamberStatus* roc = GetCalROC(d);
  if (!roc)
    return kFALSE;

  return (roc->GetStatus(col, row) & bitMask) ? kTRUE : kFALSE;
}
          
#endif
