#ifndef ILCGENEPEMV1_H
#define ILCGENEPEMV1_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Copyright(c) 1997, 1998, 2002, Adrian Alscher and Kai Hencken          *
 * Copyright(c) 2002 Kai Hencken, Yuri Kharlov, Serguei Sadovsky          *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcGenEpEmv1.h 43681 2010-09-20 08:55:06Z morsch $ */
// Event generator of single e+e- pair production in ultraperipheral PbPb collisions
// at 5.5 TeV/nucleon.
// Author: Yuri.Kharlov@cern.ch
// 9 October 2002

#include "IlcGenMC.h"
#include "IlcGenEventHeader.h"

class TEpEmGen;

//-------------------------------------------------------------
class IlcGenEpEmv1 : public IlcGenMC
{
public:
  IlcGenEpEmv1();
  
  virtual ~IlcGenEpEmv1();
  void Generate();
  void Init();
  void SetDebug(Int_t debug) {fDebug=debug;}
  
 protected:
  IlcGenEpEmv1(const IlcGenEpEmv1 & gen);
  IlcGenEpEmv1 & operator=(const IlcGenEpEmv1 & gen);

  Float_t    fMass;    // electron mass
  TEpEmGen * fEpEmGen; // e+e- generator
  Int_t      fDebug;   // debug level
  Int_t      fEvent;   // internal event number
  IlcGenEventHeader fHeader; // Event Header
ClassDef(IlcGenEpEmv1,1) // Generator of single e+e- pair production in PbPb ultra-peripheral collisions
};
#endif
