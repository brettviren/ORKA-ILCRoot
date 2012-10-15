#ifndef ILCDCHPOINTS_H
#define ILCDCHPOINTS_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHpoints.h,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  This class contains the DCH points for the ILC event display.            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcPoints.h"

class IlcDCHpoints : public IlcPoints {

 public:

  IlcDCHpoints();
  IlcDCHpoints(const IlcDCHpoints &p);
//  IlcDCHpoints(Int_t nhitsE, Int_t nhitsT);
  virtual ~IlcDCHpoints();
  IlcDCHpoints &operator=(const IlcDCHpoints &p);

  virtual void           Copy(TObject &p) const;   
  virtual void           Draw(Option_t *option);

  virtual void           Setpoints(Int_t n, Float_t *coor);

 protected:

//  enum { kNTRpoints = 75 };

  Float_t         *fDCHpoints;                  //  The hits DCH
  Int_t            fNDCHpoints;                 //  The number of hits
  TPolyMarker3D   *fDCHpolyMarker;              //! Polymarker to draw the photon hits
  
  ClassDef(IlcDCHpoints,2)                      // Class to draw DCH hits 

};
#endif
