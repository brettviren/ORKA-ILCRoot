#ifndef ILCTARGETpIDESD2_H
#define ILCTARGETpIDESD2_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

//-----------------------------------------------------------------------//
// TARGET PID class --- method # 2                                          //
//                                                                       //
//                                                                       //
//The PID is based on the likelihood of all the four TARGET' layers,        //
//without using the truncated mean for the dE/dx. The response           //
//functions for each layer are convoluted Landau-Gaussian functions.     // 
// Origin: Elena Bruna bruna@to.infn.it, Massimo Masera masera@to.infn.it//
//-----------------------------------------------------------------------//
#include "IlcTARGETpidESD.h"

class IlcTARGETtrackerMI;
class IlcTARGETLoader;
class IlcTARGETSteerPid;

class IlcTARGETpidESD2 : public IlcTARGETpidESD {
public:
  IlcTARGETpidESD2();
  IlcTARGETpidESD2(IlcTARGETtrackerMI *tracker,IlcTARGETLoader* loader);
  virtual ~IlcTARGETpidESD2();
  virtual Int_t MakePID(IlcESDEvent *event);
  IlcTARGETpidESD2(const IlcTARGETpidESD2 &ob); // copy constructor
  IlcTARGETpidESD2& operator=(const IlcTARGETpidESD2 & /* source */); // ass. op.

private:
  IlcTARGETtrackerMI *fTracker; //!tracker MI
  IlcTARGETLoader* fLoader;     //!TARGET Loader
  IlcTARGETSteerPid* fSp;       //!pointer to IlcTARGETSteerPid

  ClassDef(IlcTARGETpidESD2,1)   // TARGET PID class
};

#endif

