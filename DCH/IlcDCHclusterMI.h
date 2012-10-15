#ifndef ILCDCHCLUSTERMI_H
#define ILCDCHCLUSTERMI_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHclusterMI.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */


#include "IlcDCHcluster.h"  
#include "TMath.h"  

class IlcDCHrecPoint;

class IlcDCHclusterMI : public IlcDCHcluster {

 public:
  IlcDCHclusterMI();
  IlcDCHclusterMI(IlcDCHcluster&cl);
  IlcDCHclusterMI(const IlcDCHrecPoint &p);
  void SetRelPos(Float_t pos){fRelPos = TMath::Nint(pos*128.);}
  Float_t GetRelPos(){return float(fRelPos)/128.;}
  void SetNPads(Int_t npads){fNPads = npads;}
  Char_t GetNPads(){return fNPads;}
  Float_t fRmsY;
 protected:
  Char_t fNPads;
  Char_t fRelPos;		       	 
  ClassDef(IlcDCHclusterMI,1) // ClusterMI for the DCH
 
};

#endif
