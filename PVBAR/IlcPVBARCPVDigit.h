#ifndef ILCPVBARCPVDIGIT_H
#define ILCPVBARCPVDIGIT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPVBARCPVDigit.h 15036 2006-09-13 07:31:01Z kharlov $ */

////////////////////////////////////////////////
//  Digit class for CPV                       //
//                                            //
//  Author: Yuri Kharlov, IHEP, Protvino      //
//  e-mail: Yuri.Kharlov@cern.ch              //
//  Last modified: 23 March 2000              //
////////////////////////////////////////////////
 
// --- ROOT system ---
#include <TObject.h> 

class IlcPVBARCPVDigit : public TObject {
  
public:
  virtual ~IlcPVBARCPVDigit() {}
           IlcPVBARCPVDigit();
           IlcPVBARCPVDigit(Int_t x, Int_t y, Float_t q);
  
  void     SetQpad(Float_t q) { fQpad = q;     }
  Int_t    GetXpad()          { return  fXpad; }
  Int_t    GetYpad()          { return  fYpad; }
  Float_t  GetQpad()          { return  fQpad; }

private:
  Int_t    fXpad;       // Digit's pad number in Phi
  Int_t    fYpad;       // Digit's pad number in Z
  Float_t  fQpad;       // Digit's pad amplitude
  
  ClassDef(IlcPVBARCPVDigit,1)  // Digit object in one CPV pad
};
 
#endif
