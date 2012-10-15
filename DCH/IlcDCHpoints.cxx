/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcDCHpoints.cxx,v 1.1.1.1 2008/03/11 14:53:01 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  This class contains the DCH points for the ILC event display.          //
//  Used to seperately display dEdx and TR photon hits.                      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TPad.h>
#include <TView.h>

#include "IlcRun.h"
#include "IlcDetector.h"

#include "IlcDCHpoints.h"
 
ClassImp(IlcDCHpoints)

//_____________________________________________________________________________
IlcDCHpoints::IlcDCHpoints():IlcPoints()
{
  //
  // Default constructor
  //

  fDCHpoints     = 0x0;
  fNDCHpoints    = 0;
  fDCHpolyMarker = 0;

}

//_____________________________________________________________________________
// IlcDCHpoints::IlcDCHpoints(Int_t nhitsE, Int_t nhitsT):IlcPoints(nhitsE)
// {
//   //
//   // Standard constructor
//   //
// 
//   fNTRpoints    = nhitsT;
//   fTRpolyMarker = 0;
// 
// }
	 
//_____________________________________________________________________________
IlcDCHpoints::IlcDCHpoints(const IlcDCHpoints &p):IlcPoints(p)
{
  //
  // Copy contructor
  //
 
  ((IlcDCHpoints &) p).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHpoints::~IlcDCHpoints()
{
  //
  // Default destructor
  //

  if (fDCHpolyMarker) delete fDCHpolyMarker;

}

//_____________________________________________________________________________
IlcDCHpoints &IlcDCHpoints::operator=(const IlcDCHpoints &p)
{
  //
  // Assignment operator 
  //

  if (this != &p) ((IlcDCHpoints &) p).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHpoints::Copy(TObject &p) const
{
  //
  // Copy function
  //

  ((IlcDCHpoints &) p).fNDCHpoints = fNDCHpoints; 
  for (Int_t i = 0; i < 3*fNDCHpoints; i++) {
    ((IlcDCHpoints &) p).fDCHpoints[i] = fDCHpoints[i];
  }

}

//_____________________________________________________________________________
void IlcDCHpoints::Draw(Option_t *option) 
{
  //
  // Draws a DCH point
  //

  IlcPoints::Draw(option);

  //if (fTRpolyMarker) delete fTRpolyMarker;
  if (fNDCHpoints) {
    fDCHpolyMarker = new TPolyMarker3D(fNDCHpoints,fDCHpoints,29);
    fDCHpolyMarker->SetMarkerColor(2); 
    fDCHpolyMarker->SetMarkerSize(0.8);
    fDCHpolyMarker->Draw(option);
  }

}

//_____________________________________________________________________________
void IlcDCHpoints::Setpoints(Int_t n, Float_t *coor) 
{
  //
  // Sets the number and the coordinates of the photon hits
  //

  fNDCHpoints = n;
  fDCHpoints = new Float_t[3*fNDCHpoints];
  for (Int_t i = 0; i < n; i++) {
    fDCHpoints[i] = coor[i];
  } 

}
