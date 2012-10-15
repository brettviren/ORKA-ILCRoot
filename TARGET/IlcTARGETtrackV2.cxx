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

///////////////////////////////////////////////////////////////////////////
//                Implementation of the TARGET track class
//
//          Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch
//     dEdx analysis by: Boris Batyunya, JINR, Boris.Batiounia@cern.ch
///////////////////////////////////////////////////////////////////////////
#include <TMath.h>

#include "IlcCluster.h"
#include "IlcTracker.h"
#include "IlcESDtrack.h"
#include "IlcTARGETtrackV2.h"
#include "TClass.h"

ClassImp(IlcTARGETtrackV2)

const Int_t kWARN=-1;

//____________________________________________________________________________
IlcTARGETtrackV2::IlcTARGETtrackV2() : IlcKalmanTrack(),
  fdEdx(0),
  fESDtrack(0)
{
    for(Int_t i=0; i<2*kMaxClusters; i++) fIndex[i]=-1;
    for(Int_t i=0; i<kMaxClusters; i++) fdEdxSample[i]=0;
}


//____________________________________________________________________________
IlcTARGETtrackV2::IlcTARGETtrackV2(IlcESDtrack& t,Bool_t c) throw (const Char_t *) :
  IlcKalmanTrack(),
  fdEdx(t.GetVXDsignal()),
  fESDtrack(&t)
{
  //------------------------------------------------------------------
  // Conversion ESD track -> TARGET track.
  // If c==kTRUE, create the TARGET track out of the constrained params.
  //------------------------------------------------------------------
  const IlcExternalTrackParam *par=&t;
  if (c) {
    par=t.GetConstrainedParam();
    if (!par) throw "IlcTARGETtrackV2: conversion failed !\n";
  }
  Set(par);

  //if (!Invariant()) throw "IlcTARGETtrackV2: conversion failed !\n";

  SetLabel(t.GetLabel());
  SetMass(t.GetMass());
  SetNumberOfClusters(t.GetVXDclusters(fIndex));

  if (t.GetStatus()&IlcESDtrack::kTIME) {
    StartTimeIntegral();
    Double_t times[10]; t.GetIntegratedTimes(times); SetIntegratedTimes(times);
    SetIntegratedLength(t.GetIntegratedLength());
  }

  for(Int_t i=0; i<kMaxClusters; i++) fdEdxSample[i]=0;
}

void IlcTARGETtrackV2::UpdateESDtrack(ULong_t flags) const {
  fESDtrack->UpdateTrackParams(this,flags);
}

//____________________________________________________________________________
IlcTARGETtrackV2::IlcTARGETtrackV2(const IlcTARGETtrackV2& t) : 
  IlcKalmanTrack(t),
  fdEdx(t.fdEdx),
  fESDtrack(t.fESDtrack) 
{
  //------------------------------------------------------------------
  //Copy constructor
  //------------------------------------------------------------------
  Int_t i;
  for (i=0; i<kMaxClusters; i++) fdEdxSample[i]=t.fdEdxSample[i];
  for (i=0; i<2*kMaxClusters; i++) fIndex[i]=t.fIndex[i];
}

//_____________________________________________________________________________
Int_t IlcTARGETtrackV2::Compare(const TObject *o) const {
  //-----------------------------------------------------------------
  // This function compares tracks according to the their curvature
  //-----------------------------------------------------------------
  IlcTARGETtrackV2 *t=(IlcTARGETtrackV2*)o;
  //Double_t co=TMath::Abs(t->Get1Pt());
  //Double_t c =TMath::Abs(Get1Pt());
  Double_t co=t->GetSigmaY2()*t->GetSigmaZ2();
  Double_t c =GetSigmaY2()*GetSigmaZ2();
  if (c>co) return 1;
  else if (c<co) return -1;
  return 0;
}

//____________________________________________________________________________
Bool_t 
IlcTARGETtrackV2::PropagateToVertex(const IlcESDVertex *v,Double_t d,Double_t x0) 
{
  //------------------------------------------------------------------
  //This function propagates a track to the minimal distance from the origin
  //------------------------------------------------------------------  
  Double_t bz=GetBz();
  if (PropagateToDCA(v,bz,kVeryBig))
   if (IlcExternalTrackParam::CorrectForMaterial(d,x0,GetMass())) return kTRUE;
  return kFALSE;
}

//____________________________________________________________________________
Bool_t IlcTARGETtrackV2::
GetGlobalXYZat(Double_t xk, Double_t &x, Double_t &y, Double_t &z) const {
  //------------------------------------------------------------------
  //This function returns a track position in the global system
  //------------------------------------------------------------------
  Double_t r[3];
  Bool_t rc=GetXYZAt(xk, IlcTracker::GetBz(), r, kFALSE);
  x=r[0]; y=r[1]; z=r[2]; 
  return rc;
}

//_____________________________________________________________________________
Double_t IlcTARGETtrackV2::GetPredictedChi2(const IlcCluster *c) const {
  //-----------------------------------------------------------------
  // This function calculates a predicted chi2 increment.
  //-----------------------------------------------------------------
  Double_t p[2]={c->GetY(), c->GetZ()};
  Double_t cov[3]={c->GetSigmaY2(), 0., c->GetSigmaZ2()};
  return IlcExternalTrackParam::GetPredictedChi2(p,cov);
}

//____________________________________________________________________________
Bool_t IlcTARGETtrackV2::PropagateTo(Double_t xk, Double_t d, Double_t x0) {
  //------------------------------------------------------------------
  //This function propagates a track
  //------------------------------------------------------------------
  Double_t oldX=GetX(), oldY=GetY(), oldZ=GetZ();
  
  Double_t bz=GetBz();
  if (!IlcExternalTrackParam::PropagateTo(xk,bz)) return kFALSE;
  if (!IlcExternalTrackParam::CorrectForMaterial(d,x0,GetMass())) return kFALSE;

  Double_t x=GetX(), y=GetY(), z=GetZ();
  if (IsStartedTimeIntegral() && x>oldX) {
    Double_t l2 = (x-oldX)*(x-oldX) + (y-oldY)*(y-oldY) + (z-oldZ)*(z-oldZ);
    AddTimeStep(TMath::Sqrt(l2));
  }

  return kTRUE;
}

//____________________________________________________________________________
Bool_t IlcTARGETtrackV2::Update(const IlcCluster* c, Double_t chi2, Int_t index) 
{
  //------------------------------------------------------------------
  //This function updates track parameters
  //------------------------------------------------------------------
  Double_t p[2]={c->GetY(), c->GetZ()};
  Double_t cov[3]={c->GetSigmaY2(), 0., c->GetSigmaZ2()};

  if (!IlcExternalTrackParam::Update(p,cov)) return kFALSE;

  if (!Invariant()) {
     IlcWarning("Wrong invariant !");
     return kFALSE;
  }

  if (chi2<0) return kTRUE;

  Int_t n=GetNumberOfClusters();
  fIndex[n]=index;
  SetNumberOfClusters(n+1);
  SetChi2(GetChi2()+chi2);

  return kTRUE;
}

Bool_t IlcTARGETtrackV2::Invariant() const {
  //------------------------------------------------------------------
  // This function is for debugging purpose only
  //------------------------------------------------------------------
  Int_t n=GetNumberOfClusters();
  double factor=1;
  if(n<3) factor=10000;

  Double_t sP2=GetParameter()[2];
  if (TMath::Abs(sP2) >= kAlmost1){
    if (n>kWARN) if(IlcDebugLevelClass()>1) IlcDebug(1,Form("Invariant","fP2=%f\n",sP2));
     return kFALSE;
  }
  Double_t sP4=GetParameter()[4];
  if (TMath::Abs(sP4) >= 1./kSmallPt){
    if (n>kWARN) if(IlcDebugLevelClass()>1) IlcDebug(1,Form("Invariant","fP4=%f\n",sP4));
     return kFALSE;
  }
  Double_t sC00=GetCovariance()[0];
  if (sC00<=0 || sC00>9.*16*100*factor) {
     if (n>kWARN) if(IlcDebugLevelClass()>1) IlcDebug(1,Form("Invariant","fC00=%f\n",sC00)); 
     return kFALSE;
  }
  Double_t sC11=GetCovariance()[2];
  if (sC11<=0 || sC11>9.*16*100*factor) {
     if (n>kWARN) if(IlcDebugLevelClass()>1) IlcDebug(1,Form("Invariant","fC11=%f\n",sC11)); 
     return kFALSE;
  }
  Double_t sC22=GetCovariance()[5];
  if (sC22<=0 || sC22>1.*factor) {
     if (n>kWARN) if(IlcDebugLevelClass()>1) IlcDebug(1,Form("Invariant","fC22=%f\n",sC22)); 
     return kFALSE;
  }
  Double_t sC33=GetCovariance()[9];
  if (sC33<=0 || sC33>4.*factor) {
     if (n>kWARN) if(IlcDebugLevelClass()>1) IlcDebug(1,Form("Invariant","fC33=%f\n",sC33)); 
     return kFALSE;
  }
  Double_t sC44=GetCovariance()[14];
  if (sC44<=0 /*|| sC44>6e-5*/) {
     if (n>kWARN) if(IlcDebugLevelClass()>1) IlcDebug(1,Form("Invariant","fC44=%f\n",sC44));
     return kFALSE;
  }

  return kTRUE;
}
#include <iostream>
using namespace std; 
//____________________________________________________________________________
Bool_t IlcTARGETtrackV2::Propagate(Double_t alp,Double_t xk,bool byR) {
  //------------------------------------------------------------------
  //This function propagates a track
  //------------------------------------------------------------------
  Double_t bz=GetBz();
//   if(fabs(xk+9.5056495666503906)<1e-6){
//     Print("param");
//     cout<<alp<<" "<<xk<<" "<<bz<<" "<<byR<<endl;
//   }
  if (!IlcExternalTrackParam::Propagate(alp,xk,bz,byR)) return kFALSE;
  if (!Invariant()) {
     IlcDebug(1,"Wrong invariant !");
     return kFALSE;
  }

  return kTRUE;
}

Bool_t IlcTARGETtrackV2::Improve(Double_t x0,Double_t xyz[3],Double_t ers[3]) {
  //------------------------------------------------------------------
  //This function improves angular track parameters
  //------------------------------------------------------------------
  //Store the initail track parameters

  Double_t par[] = {GetY(),GetZ(),GetSnp(),GetTgl(),GetSigned1Pt()};
  Double_t cov[] = {
    GetSigmaY2(),
    GetSigmaZY(),
    GetSigmaZ2(),
    GetSigmaSnpY(),
    GetSigmaSnpZ(),
    GetSigmaSnp2(),
    GetSigmaTglY(),
    GetSigmaTglZ(),
    GetSigmaTglSnp(),
    GetSigmaTgl2(),
    GetSigma1PtY(),
    GetSigma1PtZ(),
    GetSigma1PtSnp(),
    GetSigma1PtTgl(),
    GetSigma1Pt2()
  }; 

  bool isendcap=IsEndCap();
  if(isendcap) RotateZ(false);
  double zbefore=par[1];
  Double_t cs=TMath::Cos(GetAlpha()), sn=TMath::Sin(GetAlpha());
//Double_t xv = xyz[0]*cs + xyz[1]*sn; // vertex
  Double_t yv =-xyz[0]*sn + xyz[1]*cs; // in the
  Double_t zv = xyz[2];                // local frame

  Double_t dy = par[0] - yv, dz = par[1] - zv;
  Double_t r2=GetX()*GetX() + dy*dy;
  Double_t p2=(1.+ GetTgl()*GetTgl())/(Get1Pt()*Get1Pt());
  Double_t beta2=p2/(p2 + GetMass()*GetMass());
  x0*=TMath::Sqrt((1.+ GetTgl()*GetTgl())/(1.- GetSnp()*GetSnp()));
  Double_t theta2=14.1*14.1/(beta2*p2*1e6)*x0;
  //Double_t theta2=1.0259e-6*14*14/28/(beta2*p2)*x0*9.36*2.33;

  Double_t cnv=GetBz()*kB2C;
  {
    Double_t dummy = 4/r2 - GetC()*GetC();
    if (dummy < 0){  if(isendcap) RotateZ(true); return kFALSE;}
    Double_t parp = 0.5*(GetC()*GetX() + dy*TMath::Sqrt(dummy));
    Double_t sigma2p = theta2*(1.- GetSnp()*GetSnp())*(1. + GetTgl()*GetTgl());
    sigma2p += cov[0]/r2*(1.- dy*dy/r2)*(1.- dy*dy/r2);
    sigma2p += ers[1]*ers[1]/r2;
    sigma2p += 0.25*cov[14]*cnv*cnv*GetX()*GetX();
    Double_t eps2p=sigma2p/(cov[5] + sigma2p);
    par[0] += cov[3]/(cov[5] + sigma2p)*(parp - GetSnp());
    //    Par(1) += Cov(4)/(Cov(5) + sigma2p)*(parp - GetSnp());
    par[2] += (1 - eps2p)*(parp-GetSnp());
    //    Par(3) += Cov(8)/(Cov(5) + sigma2p)*(parp - GetSnp());
    //    Par(4) += Cov(12)/(Cov(5) + sigma2p)*(parp - GetSnp());
    //    Cov(0) *= (1.- Cov(3)/Cov(0)/Cov(5)*(1-eps2p));
    cov[5] *= eps2p;
    Double_t epsp=TMath::Sqrt(eps2p);
    cov[3] *= epsp;
    cov[4] *= epsp;cov[8] *= epsp;cov[12] *= epsp;
  }
  {
    Double_t parl=0.5*GetC()*dz/TMath::ASin(0.5*GetC()*TMath::Sqrt(r2));
    Double_t sigma2l=theta2;
    sigma2l += cov[2]/r2 + cov[0]*dy*dy*dz*dz/(r2*r2*r2);
    sigma2l += ers[2]*ers[2]/r2;
    Double_t eps2l = sigma2l/(cov[9] + sigma2l);
    par[1] += cov[7]/(cov[9] + sigma2l)*(parl - par[3]);
    par[4] += cov[13]/(cov[9] + sigma2l)*(parl - par[3]);
    par[3] += (1-eps2l)*(parl-par[3]);
    Double_t epsl=TMath::Sqrt(eps2l);
    cov[9] *= eps2l;
    cov[6] *= epsl; cov[7]*= epsl; cov[8]*= epsl;
    cov[13]*= epsl;
  }
  if(isendcap) {
    RotateZ(true);
    Propagate(GetAlpha(),zbefore);
  }
  if (!Invariant()) return kFALSE;

  return kTRUE;
}

void IlcTARGETtrackV2::CookdEdx(Double_t low, Double_t up) {
  //-----------------------------------------------------------------
  // This function calculates dE/dX within the "low" and "up" cuts.
  // Origin: Boris Batyunya, JINR, Boris.Batiounia@cern.ch 
  //-----------------------------------------------------------------
  // The clusters order is: SSD-2, SSD-1, SDD-2, SDD-1, SPD-2, SPD-1

  Int_t i;
  Int_t nc=0;
  for (i=0; i<GetNumberOfClusters(); i++) {
    Int_t idx=GetClusterIndex(i);
    idx=(idx&kClusterLayerMask)>>kClusterLayerShift;
    if (idx>1) nc++; // Take only SSD and SDD
  }

  Int_t swap;//stupid sorting
  do {
    swap=0;
    for (i=0; i<nc-1; i++) {
      if (fdEdxSample[i]<=fdEdxSample[i+1]) continue;
      Float_t tmp=fdEdxSample[i];
      fdEdxSample[i]=fdEdxSample[i+1]; fdEdxSample[i+1]=tmp;
      swap++;
    }
  } while (swap);

  Int_t nl=Int_t(low*nc), nu=Int_t(up*nc); //b.b. to take two lowest dEdX
                                           // values from four ones choose
                                           // nu=2
  Float_t dedx=0;
  for (i=nl; i<nu; i++) dedx += fdEdxSample[i];
  if (nu-nl>0) dedx /= (nu-nl);

  SetdEdx(dedx);
}

Double_t IlcTARGETtrackV2::GetBz() const {
  //
  // returns Bz component of the magnetic field (kG)
  //
  if (IlcTracker::UniformField()) return IlcTracker::GetBz();
  Double_t r[3]; GetXYZ(r,kFALSE); 
  return IlcTracker::GetBz(r);
}

