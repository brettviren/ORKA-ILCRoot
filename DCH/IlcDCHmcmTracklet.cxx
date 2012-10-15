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

///////////////////////////////////////////////////////
//                                                   //
//                                                   //
//  Tracklet object created in the local tracking    //
//                                                   //
//                                                   //
///////////////////////////////////////////////////////

#include <TGraph.h>
#include <TMath.h>
#include <TF1.h>

#include "IlcDCHcalibDB.h"
#include "IlcDCHCommonParam.h"
#include "IlcDCHpadPlane.h"
#include "IlcDCHgeometry.h"

#include "IlcDCHmcmTracklet.h"

ClassImp(IlcDCHmcmTracklet)

//_____________________________________________________________________________
IlcDCHmcmTracklet::IlcDCHmcmTracklet() 
{
  //
  // IlcDCHmcmTracklet default constructor
  //

  fDetector = -1;
  fRow      = -1;

  for (Int_t time = 0; time < kNtimeBins; time++) {
    for (Int_t icl = 0; icl < kNclsPads; icl++) {
      fADC[time][icl] = 0;
    }
    for (Int_t it = 0; it < kNdict; it++) {
      fTrack[time][it] = -1;
    }
    fTime[time]   = 0;
    fCol[time]    = 0;
  }

  fNclusters  =  0;
  fN          =  0;
  fTrackLabel = -1;

  fGPos = 0;
  fGAmp = 0;

  fSlope  = 0.0;
  fOffset = 0.0;
  fTime0  = 0.0;
  fRowz   = 0.0;
  fPt     = 0.0;
  fdQdl   = 0.0;

}

//_____________________________________________________________________________
IlcDCHmcmTracklet::IlcDCHmcmTracklet(Int_t det, Int_t row, Int_t n) 
{
  //
  // IlcDCHmcmTracklet default constructor
  //

  fDetector = det;
  fRow      = row;

  for (Int_t time = 0; time < kNtimeBins; time++) {
    for (Int_t icl = 0; icl < kNclsPads; icl++) {
      fADC[time][icl] = 0;
    }
    for (Int_t it = 0; it < kNdict; it++) {
      fTrack[time][it] = -1;
    }
    fTime[time]   = 0;
    fCol[time]    = 0;
  }

  fNclusters = 0;

  fN = n;

  fTrackLabel = -1;

  fGPos = new TGraph(0);
  fGAmp = new TGraph(0);

  fSlope  = 0.0;
  fOffset = 0.0;
  fTime0  = 0.0;
  fRowz   = 0.0;
  fPt     = 0.0;
  fdQdl   = 0.0;

}

//_____________________________________________________________________________
IlcDCHmcmTracklet::~IlcDCHmcmTracklet() 
{
  //
  // IlcDCHmcmTracklet destructor
  //

  if (fGPos != 0) delete fGPos;
  if (fGAmp != 0) delete fGAmp;

}

//_____________________________________________________________________________
IlcDCHmcmTracklet &IlcDCHmcmTracklet::operator=(const IlcDCHmcmTracklet &t)
{
  //
  // assignment operator
  //

  if (this != &t) ((IlcDCHmcmTracklet &) t).Copy(*this); 
  return *this;

}

//_____________________________________________________________________________
void IlcDCHmcmTracklet::Copy(TObject &t) const
{
  //
  // copy function
  //

  ((IlcDCHmcmTracklet &) t).fDetector    = fDetector;
  ((IlcDCHmcmTracklet &) t).fRow         = fRow;
  ((IlcDCHmcmTracklet &) t).fTrackLabel  = fTrackLabel;
  ((IlcDCHmcmTracklet &) t).fNclusters   = fNclusters;
  ((IlcDCHmcmTracklet &) t).fN           = fN;
  ((IlcDCHmcmTracklet &) t).fGPos        = NULL;
  ((IlcDCHmcmTracklet &) t).fGAmp        = NULL;
  ((IlcDCHmcmTracklet &) t).fTime0       = fTime0;
  ((IlcDCHmcmTracklet &) t).fRowz        = fRowz;
  ((IlcDCHmcmTracklet &) t).fSlope       = fSlope;
  ((IlcDCHmcmTracklet &) t).fOffset      = fOffset;
  ((IlcDCHmcmTracklet &) t).fPt          = fPt;
  ((IlcDCHmcmTracklet &) t).fdQdl        = fdQdl;

  for (Int_t time = 0; time < kNtimeBins; time++) {
    for (Int_t icl = 0; icl < kNclsPads; icl++) {
      ((IlcDCHmcmTracklet &) t).fADC[time][icl] = 0;
    }
    for (Int_t it = 0; it < kNdict; it++) {
      ((IlcDCHmcmTracklet &) t).fTrack[time][it] = -1;
    }
    ((IlcDCHmcmTracklet &) t).fTime[time]   = 0;
    ((IlcDCHmcmTracklet &) t).fCol[time]    = 0;
  }

}

//_____________________________________________________________________________
void IlcDCHmcmTracklet::Reset() 
{
  //
  // Reset the tracklet information
  //

  fDetector = -1;
  fRow      = -1;

  for (Int_t time = 0; time < kNtimeBins; time++) {
    for (Int_t icl = 0; icl < kNclsPads; icl++) {
      fADC[time][icl] = 0;
    }
    for (Int_t it = 0; it < kNdict; it++) {
      fTrack[time][it] = -1;
    }
    fTime[time]   = 0;
    fCol[time]    = 0;
  }

  fNclusters  =  0;
  fN          =  0;
  fTrackLabel = -1;

  fGPos->Set(0);
  fGAmp->Set(0);

  fSlope  = 0.0;
  fOffset = 0.0;
  fTime0  = 0.0;
  fRowz   = 0.0;
  fPt     = 0.0;
  fdQdl   = 0.0;

}

//_____________________________________________________________________________
void IlcDCHmcmTracklet::AddCluster(Int_t icol, Int_t itb, Float_t *adc, Int_t *track) 
{
  //
  // Add a cluster to the tracklet
  //
 
  if (fNclusters >= kNtimeBins) return;

  for (Int_t icl = 0; icl < kNclsPads; icl++) {
    //fADC[fNclusters][icl] = (Int_t)adc[icl]; 
    fADC[fNclusters][icl] = adc[icl]; 
  }

  fTrack[fNclusters][0] = track[0];
  fTrack[fNclusters][1] = track[1];
  fTrack[fNclusters][2] = track[2];
  fTime[fNclusters] = itb;
  fCol[fNclusters] = icol;

  fNclusters++;

}

//_____________________________________________________________________________
void IlcDCHmcmTracklet::MakeTrackletGraph(IlcDCHgeometry *geo, Float_t field) 
{
  //
  // Tracklet graph of positions (global coordinates, rotated [cm])
  //
  
  if (!geo) {
    Error("MakeTrackletGraph","No geometry.");
    return;
  }
  
  IlcDCHCommonParam* commonParam = IlcDCHCommonParam::Instance();
  if (!commonParam)
  {
    Error("MakeTrackletGraph","No common params.");
    return;
  }

  IlcDCHcalibDB* calibration = IlcDCHcalibDB::Instance();
  if (!calibration)
  {
    Error("MakeTrackletGraph","No instance of IlcDCHcalibDB.");
    return;
  }

  Int_t iplan, icham;

  iplan = geo->GetPlane(fDetector);
  icham = geo->GetChamber(fDetector);

  IlcDCHpadPlane *padPlane = commonParam->GetPadPlane(iplan,icham);

  Float_t samplFreq = calibration->GetSamplingFrequency();

  Int_t time, col;
  Float_t amp[3];
  Float_t xpos, ypos, xzero, colSize, timeBinSize;
  Float_t vDrift, omegaTau, lorentzAngle, thetaSlope;
  Float_t tiltingAngle;
  Int_t npg = 0;

  for (Int_t icl = 0; icl < fNclusters; icl++) {

    time   = GetClusterTime(icl);

    amp[0] = GetClusterADC(icl)[0];
    amp[1] = GetClusterADC(icl)[1];
    amp[2] = GetClusterADC(icl)[2];

    col    = GetClusterCol(icl);

    if (amp[0] < 0.0 || amp[1] < 0.0 || amp[2] < 0.0) continue;

    ypos = GetClusY(amp,iplan);

    colSize = padPlane->GetColSize(col);
    vDrift = calibration->GetVdrift(fDetector,col,fRow);
    timeBinSize = vDrift/samplFreq;
    
    // From v4-03-Release to HEAD28Mar06 the sign has changed from "-" to "+" 
    // due to a change in the digitizer
    omegaTau = +TMath::Sign(1.0,(Double_t)field)*GetOmegaTau(vDrift,TMath::Abs(field));
    lorentzAngle = TMath::ATan(omegaTau)*180.0/TMath::Pi();
    
    xpos = (time+0.5) * timeBinSize;
    xpos = geo->GetTime0(iplan) - xpos;

    ypos = padPlane->GetColPos(col) - (ypos + 0.5) * colSize;

    // ExB correction
    xzero = geo->GetTime0(iplan);
    ypos = ypos + (xpos-xzero) * omegaTau;

    // tilted pads correction
    thetaSlope = - padPlane->GetRowPos(fRow)/geo->GetTime0(iplan);
    tiltingAngle = padPlane->GetTiltingAngle()/180.0*TMath::Pi();
    ypos = ypos - (xpos-xzero) * thetaSlope * TMath::Sin(tiltingAngle);

    fGPos->SetPoint(npg,(Double_t)xpos,(Double_t)ypos);
    npg++;

  }

  fGPos->Set(npg);
  
  fTime0 = geo->GetTime0(iplan) - IlcDCHgeometry::CdrHght() - 0.5*IlcDCHgeometry::CamHght();
  fRowz = padPlane->GetRowPos(fRow) - padPlane->GetRowSize(fRow)/2.0;

  Double_t xMin = 0, xMax = 0, x, y;
  fGPos->GetPoint(0    ,x,y); xMax = x + 0.1;
  fGPos->GetPoint(npg-1,x,y); xMin = x - 0.1;
  
  TF1 *line = new TF1("line","[0]+x*[1]",xMin,xMax);
  fGPos->Fit(line,"WRQ0");

  fOffset = line->Eval(fTime0);
  fSlope  = TMath::ATan(line->GetParameter(1))*180.0/TMath::Pi();

  line->Delete();
  
  Float_t fx = fTime0;
  Float_t fy = fOffset;
  
  Float_t infSlope = TMath::ATan(fy/fx)/TMath::Pi()*180.0;    
  Float_t alpha = fSlope - infSlope;
  Float_t r = TMath::Sqrt(fx*fx + fy*fy)/(2.0*TMath::Sin(alpha/180.0*TMath::Pi()));

  fPt = 0.3 * field * 0.01 * r;
  
  return;

}

//_____________________________________________________________________________
void IlcDCHmcmTracklet::MakeClusAmpGraph() 
{
  //
  // Tracklet graph of cluster charges
  //

  Int_t time;
  Float_t amp[3];
  Int_t npg = 0;
  fdQdl = 0.0;
  for (Int_t icl = 0; icl < fNclusters; icl++) {

    time   = GetClusterTime(icl);

    amp[0] = GetClusterADC(icl)[0];
    amp[1] = GetClusterADC(icl)[1];
    amp[2] = GetClusterADC(icl)[2];

    fGAmp->SetPoint(npg,(Double_t)(time+0.5),(Double_t)(amp[0]+amp[1]+amp[2]));
    npg++;

    fdQdl += amp[0]+amp[1]+amp[2];

  }

  fGAmp->Set(npg);

  fdQdl /= (Float_t)npg;

  return;

}

//_____________________________________________________________________________
Float_t IlcDCHmcmTracklet::GetClusY(Float_t *adc, Int_t pla) const
{
  //
  // Cluster position in the phi direction in pad units (relative to the pad border)
  //

  Float_t ypos = 0.0;

  Float_t a0 = adc[0];
  Float_t a1 = adc[1];
  Float_t a2 = adc[2];

  Float_t t1, t2, w1 ,w2;

  Float_t w = 1.0;  // pad units

  Float_t sigma = 0.0;

  switch(pla) {
  case 0:
    sigma = 0.515; break;
  case 1:
    sigma = 0.501; break;
  case 2:
    sigma = 0.491; break;
  case 3:
    sigma = 0.481; break;
  case 4:
    sigma = 0.471; break;
  case 5:
    sigma = 0.463; break;
  default:
    Error("GetClusY","Wrong plane number.");
    return 0.0;
  }

  sigma *= w;

  t1 = 0.0;
  w1 = 0.0;
  if( a0 > 0 ) {
    w1 = a0*a0;
    //w1 = a0;
    t1 = w1*((sigma*sigma)/w*TMath::Log(a1/a0)-0.5*w);
  }
  t2 = 0.0;
  w2 = 0.0;
  if( a2 > 0 ) {
    w2 = a2*a2;
    //w2 = a2;
    t2 = w2*((sigma*sigma)/w*TMath::Log(a2/a1)+0.5*w);
  }

  ypos = w*(t1+t2)/(w1+w2);  // range: -0.5*w ... +0.5*w

  return ypos;

}

//_____________________________________________________________________________
void IlcDCHmcmTracklet::CookLabel(Float_t frac) 
{
  //
  // Cook the track label from cluster labels
  //

  const Int_t kMaxTracks = 10;
  Int_t trackLabel[kMaxTracks];
  Int_t trackCount[kMaxTracks];
  for (Int_t it = 0; it < kMaxTracks; it++) {
    trackLabel[it] = -1;
    trackCount[it] = 0;
  }

  Bool_t counted;
  Int_t label, nTracks = 0;
  for (Int_t icl = 0; icl < fNclusters; icl++) {

    for (Int_t id = 0; id < kNdict; id++) {

      if (fTrack[icl][id] == -1) continue;

      label = fTrack[icl][id];

      counted = kFALSE;
      for (Int_t it = 0; it < nTracks; it++) {
	if (label == trackLabel[it]) {
	  trackCount[it]++;
	  counted = kTRUE;
	  break;
	}
      }
      if (!counted) {
	trackLabel[nTracks] = label;
	trackCount[nTracks]++;
	nTracks++;
	if (nTracks == kMaxTracks) {
	  Warning("CookLabel","Too many tracks for this tracklet.");
	  nTracks--;
	  break;
	}
      }

    }

  }

  for (Int_t it = 0; it < kMaxTracks; it++) {
    if (trackCount[it] >= (Int_t)(frac*fNclusters)) {
      fTrackLabel = trackLabel[it];
      break;
    }
  }

}

//_____________________________________________________________________________
Float_t IlcDCHmcmTracklet::GetOmegaTau(Float_t vdrift, Float_t field) const
{
  //
  // Returns omega*tau (tan(Lorentz-angle)) for a given drift velocity <vd> 
  // and a B-field <b> for Xe/CO2 (15%).
  // The values are according to a GARFIELD simulation.
  //

  //
  // Copy of the "IlcDCHcalibDB" function, taking as argument the magnetic field too
  //
  
  const Int_t kNb = 5;
  Float_t p0[kNb] = {  0.004810,  0.007412,  0.010252,  0.013409,  0.016888 };
  Float_t p1[kNb] = {  0.054875,  0.081534,  0.107333,  0.131983,  0.155455 };
  Float_t p2[kNb] = { -0.008682, -0.012896, -0.016987, -0.020880, -0.024623 };
  Float_t p3[kNb] = {  0.000155,  0.000238,  0.000330,  0.000428,  0.000541 };

  Int_t ib = ((Int_t) (10 * (field - 0.15)));
  ib       = TMath::Max(  0,ib);
  ib       = TMath::Min(kNb,ib);

  Float_t alphaL = p0[ib] 
      + p1[ib] * vdrift
      + p2[ib] * vdrift*vdrift
      + p3[ib] * vdrift*vdrift*vdrift;

  return TMath::Tan(alphaL);

}
