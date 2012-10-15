#ifndef ILCTARGETPIDESD1_H
#define ILCTARGETPIDESD1_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------
// PID method # 1
//                    TARGET PID class
// A very naive design... Should be made better by the detector experts...
//   Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//-------------------------------------------------------
#include "IlcTARGETpidESD.h"


class IlcTARGETpidESD1 : public IlcTARGETpidESD {
public:
  IlcTARGETpidESD1();
  IlcTARGETpidESD1(Double_t *param);
  virtual ~IlcTARGETpidESD1() {}
  virtual Int_t MakePID(IlcESDEvent *event);

private:
  Double_t fMIP;          // dEdx for MIP
  Double_t fRes;          // relative dEdx resolution
  Double_t fRange;        // one particle type PID range (in sigmas)
  ClassDef(IlcTARGETpidESD1,1)   // TARGET PID class
};

#endif

