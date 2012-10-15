#ifndef ILCTARGETpIDESD_H
#define ILCTARGETpIDESD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------
//                    TARGET PID class
// Base class:
// See the implementations IlcTARGETpidESD1 and IlcTARGETpidESD2
//-------------------------------------------------------
//#include <Rtypes.h>
#include <TObject.h>

class IlcESDEvent;

class IlcTARGETpidESD : public TObject {
public:
  IlcTARGETpidESD();
  virtual ~IlcTARGETpidESD() {}
  virtual Int_t MakePID(IlcESDEvent *event) =0;
  static Double_t Bethe(Double_t bg);
private:
  ClassDef(IlcTARGETpidESD,1)   // TARGET PID class
};

#endif


