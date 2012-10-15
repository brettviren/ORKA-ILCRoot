#ifndef ILCDCHPIDESD_H
#define ILCDCHPIDESD_H 
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */ 
 
//------------------------------------------------------- 
//                    DCH PID class 
// A very naive design...  
//------------------------------------------------------- 
#include <Rtypes.h> 
 
class IlcESD; 
 
class IlcDCHpidESD { 
public: 
  IlcDCHpidESD(Double_t *param); 
  virtual ~IlcDCHpidESD() {} 
  static Int_t MakePID(IlcESD *event); 
  static Double_t Bethe(Double_t bg); 
private: 
  Double_t fMIP;          // dEdx for MIP 
  Double_t fRes;          // relative dEdx resolution 
  Double_t fRange;        // one particle type PID range (in sigmas) 
  ClassDef(IlcDCHpidESD,1)   // DCH PID class 
}; 
 
#endif 
 
 
