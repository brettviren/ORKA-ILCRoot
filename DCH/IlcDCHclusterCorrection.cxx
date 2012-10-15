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
//  DCH clusterCorrection                                                    //
//  marian.ivanov@cern.ch                                                    //
//                                                                           //
/////////////////////////////////////////////////////////////////////////////// 

#include "IlcDCHclusterCorrection.h"
#include "TFile.h"

ClassImp(IlcDCHclusterCorrection)

IlcDCHclusterCorrection * gIlcDCHclusterCorrection=0;

//_____________________________________________________________________________
IlcDCHclusterCorrection::IlcDCHclusterCorrection()
{
  //
  //default constructor for correction
  //

  for (Int_t iplane=0;iplane<6;iplane++) {
    for (Int_t itime=0;itime<30;itime++) {
      for(Int_t iangle=0;iangle<20;iangle++){	
	fCorrections[iplane][itime][iangle][0]=0;
	fCorrections[iplane][itime][iangle][1]=0;
      }
    }
  }

  fOffsetAngle =0;

}

//_____________________________________________________________________________
void IlcDCHclusterCorrection::SetCorrection(Int_t plane,Int_t timebin, Float_t angle, 
					    Float_t value, Float_t sigma)
{
  //
  // Set the correction factors
  //

  Int_t iangle = int( (angle-fOffsetAngle+1.)*10.+0.5);
  if (iangle<0) return;
  if (iangle>=20) return;
  fCorrections[plane][timebin][iangle][0] = value;
  fCorrections[plane][timebin][iangle][1] = sigma;

}

//_____________________________________________________________________________
Float_t IlcDCHclusterCorrection::GetCorrection(Int_t plane, Int_t timebin, Float_t angle) const
{
  //
  // Get the correction factors
  //

  Int_t iangle = int( (angle-fOffsetAngle+1.)*10.+0.5);
  if (iangle<0) return 0.;
  if (iangle>=20) return 0.;
  return fCorrections[plane][timebin][iangle][0];

}

//_____________________________________________________________________________
Float_t IlcDCHclusterCorrection::GetSigma(Int_t plane, Int_t timebin, Float_t angle) const
{
  //
  // Returns the sigma
  //

  Int_t iangle = int( (angle-fOffsetAngle+1.)*10.+0.5);
  if (iangle<0) return 1.;
  if (iangle>=20) return 1.;
  return fCorrections[plane][timebin][iangle][1];

}

//_____________________________________________________________________________
IlcDCHclusterCorrection *  IlcDCHclusterCorrection::GetCorrection()
{
  //
  // Return an instance of IlcDCHclusterCorrection
  //

  if (gIlcDCHclusterCorrection!=0) return gIlcDCHclusterCorrection;
  //
  TFile * f  = new TFile("$ILC_ROOT/DCH/DCHcorrection.root");
  if (!f){
    ////
    gIlcDCHclusterCorrection = new IlcDCHclusterCorrection();
    return gIlcDCHclusterCorrection;
  }
  gIlcDCHclusterCorrection = (IlcDCHclusterCorrection*) f->Get("DCHcorrection");
  if (gIlcDCHclusterCorrection==0)  
    gIlcDCHclusterCorrection = new IlcDCHclusterCorrection();

  return gIlcDCHclusterCorrection;
  
}
