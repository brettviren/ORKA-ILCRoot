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

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                                                                           //
//  DCH chamber local track (LTU, tracklet)                                  //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TMath.h>

#include "IlcDCHltuTracklet.h"

ClassImp(IlcDCHltuTracklet)

//_____________________________________________________________________________
IlcDCHltuTracklet::IlcDCHltuTracklet(Int_t det, 
				     Int_t row, 
				     Float_t rowz,
				     Float_t slope, 
				     Float_t offset, 
				     Float_t time, 
				     Int_t ncl,
				     Int_t label,
				     Float_t q) 
{
  //
  // IlcDCHltuTracklet constructor
  //

  fDetector  = det;
  fRow       = row;
  fRowz      = rowz;
  fSlope     = slope;
  fX         = time;
  fY         = offset;
  fNclusters = ncl;
  fLabel     = label;
  fQ         = q;

}

//_____________________________________________________________________________
IlcDCHltuTracklet::~IlcDCHltuTracklet()
{
  //
  // destructor
  //

}

//_____________________________________________________________________________
Float_t IlcDCHltuTracklet::GetPt(Float_t field) const
{
  // transverse momentum calculation
  // curvature R = (fX*fX + fY*fY) / (2 * sin(alpha))
  // alpha = angle deviation from "infinite momentum"
  //
  // consistent with IlcDCHmcmTracklet::GetPt(...)

  Float_t infSlope = TMath::ATan(fY/fX)/TMath::Pi()*180.0;    
  Float_t alpha = fSlope - infSlope;
  Float_t r = TMath::Sqrt(fX*fX + fY*fY)/(2.0*TMath::Sin(alpha/180.0*TMath::Pi()));
  
  Float_t pt = 0.3 * field * 0.01 * r;
 
  return pt;
 
}

//_____________________________________________________________________________
Int_t IlcDCHltuTracklet::Compare(const TObject * o) const
{
  //
  // compare two LTU tracklets according to the intercept point Y1
  //

  IlcDCHltuTracklet *ltutrk = (IlcDCHltuTracklet*)o;

  if (fRow      != ltutrk->fRow)      return +1;
  if (fDetector != ltutrk->fDetector) return +1;

  if (fY <  ltutrk->fY) return -1;
  if (fY == ltutrk->fY) return  0;

  return 1;

}

//_____________________________________________________________________________
Float_t IlcDCHltuTracklet::GetYproj(Float_t xpl) const
{
  //
  // y-projection (bending plane) onto the median plane
  //

  Float_t yproj;

  yproj = fY + TMath::Tan(fSlope/180.0*TMath::Pi()) * (xpl - fX);

  return yproj;

}

//_____________________________________________________________________________
Float_t IlcDCHltuTracklet::GetZproj(Float_t xpl) const
{
  //
  // z-projection (using pad row center) onto the median plane
  //

  Float_t zproj;

  zproj = fRowz * xpl / fX;

  return zproj;

}

