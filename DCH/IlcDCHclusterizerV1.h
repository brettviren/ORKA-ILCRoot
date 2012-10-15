#ifndef ILCDCHCLUSTERIZERV1_H
#define ILCDCHCLUSTERIZERV1_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHclusterizerV1.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

#include "IlcDCHclusterizer.h"

///////////////////////////////////////////////////////
//  Finds and handles cluster (slow simulation)      //
///////////////////////////////////////////////////////

class IlcDCHdataArrayI;
class IlcDCHdataArrayF;
class IlcDCHdigitsManager;
class IlcRawReader;

class IlcDCHclusterizerV1 : public IlcDCHclusterizer {

 public:

  IlcDCHclusterizerV1();
  IlcDCHclusterizerV1(const Text_t* name, const Text_t* title);
  IlcDCHclusterizerV1(const IlcDCHclusterizerV1 &c);
  virtual ~IlcDCHclusterizerV1();
  IlcDCHclusterizerV1 &operator=(const IlcDCHclusterizerV1 &c);

  virtual void     Copy(TObject &c) const;
  virtual Bool_t   MakeClusters();
  virtual Bool_t   ReadDigits();
  virtual Bool_t   ReadDigits(IlcRawReader* rawReader);

 protected:

  IlcDCHdigitsManager *fDigitsManager;      //! DCH digits manager

 private:
  void DeConvExp(Double_t *source, Double_t *target, Int_t nTimeTotal, Int_t nexp);
  void Transform(IlcDCHdataArrayI* digitsIn, IlcDCHdataArrayF* digitsOut,
		 Int_t idet, Int_t nRowMax, Int_t nColMax, Int_t nTimeTotal, Float_t ADCthreshold);
  virtual Double_t Unfold(Double_t eps, Int_t plane, Double_t *padSignal);
  Double_t GetCOG(Double_t signal[5]);      // get COG position

  ClassDef(IlcDCHclusterizerV1,5)           // DCH-Cluster finder, slow simulator

};

#endif
