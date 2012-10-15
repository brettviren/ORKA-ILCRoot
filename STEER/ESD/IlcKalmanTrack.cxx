/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
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

/* $Id: IlcKalmanTrack.cxx 57390 2012-06-22 15:30:01Z hristov $ */

//-------------------------------------------------------------------------
//                Implementation of the IlcKalmanTrack class
//   that is the base for IlcTPCtrack, IlcITStrackV2 and IlcTRDtrack
//        Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch
//-------------------------------------------------------------------------
#include <TGeoManager.h>

#include "IlcKalmanTrack.h"

ClassImp(IlcKalmanTrack)

//_______________________________________________________________________
  IlcKalmanTrack::IlcKalmanTrack():IlcExternalTrackParam(),
  fFakeRatio(0),
  fChi2(0),
  fMass(IlcPID::ParticleMass(IlcPID::kPion)),
  fLab(-3141593),
  fN(0),
  fStartTimeIntegral(kFALSE),
  fIntegratedLength(0)
{
  //
  // Default constructor
  //

  for(Int_t i=0; i<IlcPID::kSPECIES; i++) fIntegratedTime[i] = 0;
}

IlcKalmanTrack::IlcKalmanTrack(const IlcKalmanTrack &t):
  IlcExternalTrackParam(t),
  fFakeRatio(t.fFakeRatio),
  fChi2(t.fChi2),
  fMass(t.fMass),
  fLab(t.fLab),
  fN(t.fN),
  fStartTimeIntegral(t.fStartTimeIntegral),
  fIntegratedLength(t.fIntegratedLength)
{
  //
  // Copy constructor
  //
  
  for (Int_t i=0; i<IlcPID::kSPECIES; i++)
      fIntegratedTime[i] = t.fIntegratedTime[i];
}

IlcKalmanTrack& IlcKalmanTrack::operator=(const IlcKalmanTrack&o){
  if(this!=&o){
    IlcExternalTrackParam::operator=(o);
    fLab = o.fLab;
    fFakeRatio = o.fFakeRatio;
    fChi2 = o.fChi2;
    fMass = o.fMass;
    fN = o.fN;
    fStartTimeIntegral = o.fStartTimeIntegral;
    for(Int_t i = 0;i<IlcPID::kSPECIES;++i)fIntegratedTime[i] = o.fIntegratedTime[i];
    fIntegratedLength = o.fIntegratedLength;
  }
  return *this;
}

//_______________________________________________________________________
void IlcKalmanTrack::StartTimeIntegral() 
{
  // Sylwester Radomski, GSI
  // S.Radomski@gsi.de
  //
  // Start time integration
  // To be called at Vertex by ITS tracker
  //
  
  //if (fStartTimeIntegral) 
  //  IlcWarning("Reseting Recorded Time.");

  fStartTimeIntegral = kTRUE;
  for(Int_t i=0; i<IlcPID::kSPECIES; i++) fIntegratedTime[i] = 0;  
  fIntegratedLength = 0;
}

//_______________________________________________________________________
void IlcKalmanTrack:: AddTimeStep(Double_t length) 
{
  // 
  // Add step to integrated time
  // this method should be called by a sublasses at the end
  // of the PropagateTo function or by a tracker
  // each time step is made.
  //
  // If integration not started function does nothing
  //
  // Formula
  // dt = dl * sqrt(p^2 + m^2) / p
  // p = pT * (1 + tg^2 (lambda) )
  //
  // pt = 1/external parameter [4]
  // tg lambda = external parameter [3]
  //
  //
  // Sylwester Radomski, GSI
  // S.Radomski@gsi.de
  // 
  
  static const Double_t kcc = 2.99792458e-2;

  if (!fStartTimeIntegral) return;
  
  fIntegratedLength += length;

  Double_t xr, param[5];
  Double_t pt, tgl;
  
  GetExternalParameters(xr, param);
  pt =  1/param[4] ;
  tgl = param[3];

  Double_t p = TMath::Abs(pt * TMath::Sqrt(1+tgl*tgl));

  //  if (length > 100) return;

  for (Int_t i=0; i<IlcPID::kSPECIES; i++) {
    
    Double_t mass = IlcPID::ParticleMass(i);
    Double_t correction = TMath::Sqrt( pt*pt * (1 + tgl*tgl) + mass * mass ) / p;
    Double_t time = length * correction / kcc;

    fIntegratedTime[i] += time;
  }
}

//_______________________________________________________________________
Double_t IlcKalmanTrack::GetIntegratedTime(Int_t pdg) const 
{
  // Sylwester Radomski, GSI
  // S.Radomski@gsi.de
  //
  // Return integrated time hypothesis for a given particle
  // type assumption.
  //
  // Input parameter:
  // pdg - Pdg code of a particle type
  //


  if (!fStartTimeIntegral) {
    IlcWarning("Time integration not started");
    return 0.;
  }

  for (Int_t i=0; i<IlcPID::kSPECIES; i++)
    if (IlcPID::ParticleCode(i) == TMath::Abs(pdg)) return fIntegratedTime[i];

  IlcWarning(Form("Particle type [%d] not found", pdg));
  return 0;
}

void IlcKalmanTrack::GetIntegratedTimes(Double_t *times) const {
  for (Int_t i=0; i<IlcPID::kSPECIES; i++) times[i]=fIntegratedTime[i];
}

void IlcKalmanTrack::SetIntegratedTimes(const Double_t *times) {
  for (Int_t i=0; i<IlcPID::kSPECIES; i++) fIntegratedTime[i]=times[i];
}

