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
//  DCH trigger parameters class                                             //
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHgeometry.h"

#include "IlcDCHtrigParam.h"

ClassImp(IlcDCHtrigParam)

//_____________________________________________________________________________
IlcDCHtrigParam::IlcDCHtrigParam():TNamed()
{
  //
  // IlcDCHtrigParam default constructor
  //

  fDebug      = 0;
  fTime1      = 0;
  fTime2      = 0;
  fClusThr    = 0;
  fPadThr     = 0;
  fSum10      = 0;
  fSum12      = 0;
  fTCOn       = 0;
  fTCnexp     = 0;
  fFilterType = 0;
  fR1         = 0;
  fR2         = 0;
  fC1         = 0;
  fC2         = 0;
  fPedestal   = 0;
  fDeltaY     = 0.0;
  fDeltaS     = 0.0;
  fXprojPlane = 0.0;
  fField      = 0.0;
  fLtuPtCut   = 0.0;
  fGtuPtCut   = 0.0;
  fHighPt     = 0.0;

  fNPartJetLow  = 0;
  fNPartJetHigh = 0;
  fJetLowPt     = 0.0;
  fJetHighPt    = 0.0;

}

//_____________________________________________________________________________
IlcDCHtrigParam::IlcDCHtrigParam(const Text_t *name, const Text_t *title)
                :TNamed(name,title)
{
  //
  // IlcDCHtrigParam constructor
  //

  fDebug      =    0;
  fTime1      =    2;
  fTime2      =   22;
  fClusThr    = 10.0;
  fPadThr     =    1;
  fSum10      =    2;
  fSum12      =   10;
  fTCOn       =    1;
  fTCnexp     =    1;
  fFilterType =    0;
  fR1         =  0.0;
  fR2         =  0.0;
  fC1         =  0.0;
  fC2         =  0.0;
  fPedestal   =  0.0;
  fDeltaY     =  2.0;
  fDeltaS     =  2.5;
  fXprojPlane =  0.0;
  fField      =  0.0;
  fLtuPtCut   =  2.3;
  fGtuPtCut   =  3.0;
  fHighPt     = 10.0;

  fNPartJetLow  = 5;
  fNPartJetHigh = 3;
  fJetLowPt     = 3.0;
  fJetHighPt    = 5.0;

  // PASA.v.4

  if (fTCnexp == 1) {
    fR1         = 1.1563;
    fR2         = 0.1299;
    fC1         = 0.0657;
    fC2         = 0.0000;
  }

  if (fTCnexp == 2) {
    fR1         = 1.1563;
    fR2         = 0.1299;
    fC1         = 0.1141;
    fC2         = 0.6241;
  }

}

//_____________________________________________________________________________
IlcDCHtrigParam::IlcDCHtrigParam(const IlcDCHtrigParam &p):TNamed(p)
{
  //
  // IlcDCHtrigParam copy constructor
  //

  ((IlcDCHtrigParam &) p).Copy(*this);

}

//_____________________________________________________________________________
IlcDCHtrigParam::~IlcDCHtrigParam()
{
  //
  // IlcDCHtrigParam destructor
  //
}

//_____________________________________________________________________________
IlcDCHtrigParam &IlcDCHtrigParam::operator=(const IlcDCHtrigParam &p)
{
  //
  // Assignment operator
  //

  if (this != &p) ((IlcDCHtrigParam &) p).Copy(*this);
  return *this;

}

//_____________________________________________________________________________
void IlcDCHtrigParam::Copy(TObject &p) const
{
  //
  // Copy function
  //

  ((IlcDCHtrigParam &) p).fDebug      = fDebug;
  ((IlcDCHtrigParam &) p).fTime1      = fTime1;
  ((IlcDCHtrigParam &) p).fTime2      = fTime2;
  ((IlcDCHtrigParam &) p).fClusThr    = fClusThr;
  ((IlcDCHtrigParam &) p).fPadThr     = fPadThr;
  ((IlcDCHtrigParam &) p).fSum10      = fSum10;
  ((IlcDCHtrigParam &) p).fSum12      = fSum12;
  ((IlcDCHtrigParam &) p).fTCOn       = fTCOn;
  ((IlcDCHtrigParam &) p).fTCnexp     = fTCnexp;
  ((IlcDCHtrigParam &) p).fFilterType = fFilterType;
  ((IlcDCHtrigParam &) p).fR1         = fR1;
  ((IlcDCHtrigParam &) p).fR2         = fR2;
  ((IlcDCHtrigParam &) p).fC1         = fC1;
  ((IlcDCHtrigParam &) p).fC2         = fC2;
  ((IlcDCHtrigParam &) p).fPedestal   = fPedestal;
  ((IlcDCHtrigParam &) p).fADCnoise   = fADCnoise;
  ((IlcDCHtrigParam &) p).fDeltaY     = fDeltaY;
  ((IlcDCHtrigParam &) p).fDeltaS     = fDeltaS;
  ((IlcDCHtrigParam &) p).fXprojPlane = fXprojPlane;
  ((IlcDCHtrigParam &) p).fField      = fField;
  ((IlcDCHtrigParam &) p).fLtuPtCut   = fLtuPtCut;
  ((IlcDCHtrigParam &) p).fGtuPtCut   = fGtuPtCut;
  ((IlcDCHtrigParam &) p).fHighPt     = fHighPt;
  ((IlcDCHtrigParam &) p).fNPartJetLow  = fNPartJetLow;
  ((IlcDCHtrigParam &) p).fNPartJetHigh = fNPartJetHigh;
  ((IlcDCHtrigParam &) p).fJetLowPt     = fJetLowPt;
  ((IlcDCHtrigParam &) p).fJetHighPt    = fJetHighPt;

}

//_____________________________________________________________________________
void IlcDCHtrigParam::Init()
{
  //
  // Initialize the other parameters
  //

  Float_t fXplane0, fXplane5;

  fXplane0 = IlcDCHgeometry::GetTime0(0) - IlcDCHgeometry::CdrHght() - 0.5*IlcDCHgeometry::CamHght(); 

  fXplane5 = IlcDCHgeometry::GetTime0(5) - IlcDCHgeometry::CdrHght() - 0.5*IlcDCHgeometry::CamHght();

  fXprojPlane = 0.5 * (fXplane0 + fXplane5);

}

