#ifndef ILCDCHCALSUPERMODULESTATUS_H
#define ILCDCHCALSUPERMODULESTATUS_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for status of supermodules                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"

class IlcDCHCalSuperModuleStatus : public TNamed {
  public:
    enum { kNdet = 540, kNstacks = 90, kNcham = 5, kNsect = 18 };
    enum { kInstalled = 1, kMasked = 2 };
  
    IlcDCHCalSuperModuleStatus();
    IlcDCHCalSuperModuleStatus(const Text_t* name, const Text_t* title);

    Char_t GetStatus(Int_t sm) const { return fStatus[sm]; };
    void SetStatus(Int_t sm, Char_t status) { fStatus[sm] = status; };

    Bool_t IsInstalled(Int_t sm) const { return (GetStatus(sm) & kInstalled) ? kTRUE : kFALSE; }
    Bool_t IsMasked(Int_t sm) const { return (GetStatus(sm) & kMasked) ? kTRUE : kFALSE; }

  protected:
    Char_t fStatus[kNsect];                    //  status byte

    ClassDef(IlcDCHCalSuperModuleStatus,1)
};

#endif
