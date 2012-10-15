#ifndef IlcDCHCALSTACKPOS_H
#define IlcDCHCALSTACKPOS_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  DCH calibration class for position parameters of the stacks and chambers //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"

class IlcDCHCalStackPos : public TNamed {
  public:
    enum { kNdet = 540, kNstacks = 90, kNcham = 5, kNsect = 18 };
  
    IlcDCHCalStackPos();
    IlcDCHCalStackPos(const Text_t* name, const Text_t* title);
  
    const Float_t* GetPos(Int_t chamber, Int_t sector) const { return fStackPos[GetStackNumber(chamber, sector)]; };
    const Float_t* GetRot(Int_t chamber, Int_t sector) const { return fStackPos[GetStackNumber(chamber, sector)]; };

    inline void SetPos(Int_t chamber, Int_t sector, Float_t x, Float_t y, Float_t z);
    void SetPos(Int_t chamber, Int_t sector, Float_t* xyz) { SetPos(chamber, sector, xyz[0], xyz[1], xyz[2]); };
  
    inline void SetRot(Int_t chamber, Int_t sector, Float_t x, Float_t y, Float_t z);
    void SetRot(Int_t chamber, Int_t sector, Float_t* xyz) { SetRot(chamber, sector, xyz[0], xyz[1], xyz[2]); };
  
  protected:
    static Int_t GetStackNumber(Int_t chamber, Int_t sector) { return chamber+sector*kNcham; };

    Float_t fStackPos[kNstacks][3];                    //  Deviations of the positions of the stacks from the ideal position
    Float_t fStackRot[kNstacks][3];                    //  Rotation of the stacks in respect to the ideal position
    
    ClassDef(IlcDCHCalStackPos,1)                     
};
    
void IlcDCHCalStackPos::SetPos(Int_t chamber, Int_t sector, Float_t x, Float_t y, Float_t z) 
{ 
  Int_t stack = GetStackNumber(chamber, sector); 
  fStackPos[stack][0] = x; 
  fStackPos[stack][1] = y; 
  fStackPos[stack][2] = z; 
}

void IlcDCHCalStackPos::SetRot(Int_t chamber, Int_t sector, Float_t x, Float_t y, Float_t z) 
{ 
  Int_t stack = GetStackNumber(chamber, sector); 
  fStackRot[stack][0] = x; 
  fStackRot[stack][1] = y; 
  fStackRot[stack][2] = z; 
}

#endif
