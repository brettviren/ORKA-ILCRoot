#ifndef IlcDCHCalChamberStatus_H
#define IlcDCHCalChamberStatus_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHCalChamberStatus.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for status of supermodules                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"

class IlcDCHCalChamberStatus : public TNamed {
  public:
    enum { kNdet = 540, kNstacks = 90, kNcham = 5, kNsect = 18 };
    enum { kInstalled = 1, kMasked = 2 };
  
    IlcDCHCalChamberStatus();
    IlcDCHCalChamberStatus(const Text_t* name, const Text_t* title);

    Char_t GetStatus(Int_t det) const { return fStatus[det]; };
    void SetStatus(Int_t det, Char_t status) { fStatus[det] = status; };

    Bool_t IsInstalled(Int_t sm) const { return (GetStatus(sm) & kInstalled) ? kTRUE : kFALSE; }
    Bool_t IsMasked(Int_t sm) const { return (GetStatus(sm) & kMasked) ? kTRUE : kFALSE; }

  protected:
    Char_t fStatus[kNdet];                    //  status byte

    ClassDef(IlcDCHCalChamberStatus,1)
};

#endif
