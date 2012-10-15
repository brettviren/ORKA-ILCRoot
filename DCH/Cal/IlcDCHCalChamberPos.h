#ifndef IlcDCHCALCHAMBERPOS_H
#define IlcDCHCALCHAMBERPOS_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHCalChamberPos.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for position parameters chambers //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"

class IlcDCHCalChamberPos : public TNamed {
  public:
    enum { kNdet = 540 };
  
    IlcDCHCalChamberPos();
    IlcDCHCalChamberPos(const Text_t* name, const Text_t* title);
  
    const Float_t* GetPos(Int_t det) const { return fChamberPos[det]; };
    const Float_t* GetRot(Int_t det) const { return fChamberRot[det]; };
    
    void SetPos(Int_t det, Float_t x, Float_t y, Float_t z) { fChamberPos[det][0] = x; fChamberPos[det][1] = y; fChamberPos[det][2] = z; };
    void SetPos(Int_t det, Float_t* xyz) { SetPos(det, xyz[0], xyz[1], xyz[2]); };
  
    void SetRot(Int_t det, Float_t x, Float_t y, Float_t z) { fChamberRot[det][0] = x; fChamberRot[det][1] = y; fChamberRot[det][2] = z; };
    void SetRot(Int_t det, Float_t* xyz) { SetRot(det, xyz[0], xyz[1], xyz[2]); };
  
  protected:
    Float_t fChamberPos[kNdet][3];                    //  Deviations of the positions of the chambers from the ideal position
    Float_t fChamberRot[kNdet][3];                    //  Rotation of the chambers in respect to the ideal position
    
    ClassDef(IlcDCHCalChamberPos,1)                      
};

#endif
