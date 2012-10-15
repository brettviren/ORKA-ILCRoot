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

/* $Id: IlcAODTracklets.cxx 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//     AOD class to store tracklets
//     Author: Jan Fiete Grosse-Oetringhaus, CERN
//     Class created from IlcMultiplicity
//-------------------------------------------------------------------------

#include "IlcAODTracklets.h"

ClassImp(IlcAODTracklets)

IlcAODTracklets::IlcAODTracklets() : TNamed(), fNTracks(0), fTheta(0), fPhi(0), fDeltaPhi(0), fLabels(0), fLabelsL2(0)
{
  // default constructor
}

IlcAODTracklets::IlcAODTracklets(const char* name, const char* title) : TNamed(name, title), fNTracks(0), fTheta(0), fPhi(0), fDeltaPhi(0), fLabels(0), fLabelsL2(0)
{
  // TNamed constructor
}

IlcAODTracklets::IlcAODTracklets(const IlcAODTracklets& tracklet) :
    TNamed(tracklet),
    fNTracks(tracklet.fNTracks),
    fTheta(0),
    fPhi(0),
    fDeltaPhi(0),
    fLabels(0), 
    fLabelsL2(0)
{
// Copy constructor
    fTheta = new Double32_t[fNTracks];
    fPhi = new Double32_t[fNTracks];
    fDeltaPhi = new Double32_t[fNTracks];
    fLabels = new Int_t[fNTracks];
    fLabelsL2 = new Int_t[fNTracks];
    for (Int_t i = 0; i < fNTracks; i++) {
	fTheta[i]    = tracklet.fTheta[i];
	fPhi[i]      = tracklet.fPhi[i];
	fDeltaPhi[i] = tracklet.fDeltaPhi[i];
	fLabels[i]   = tracklet.fLabels[i];
	fLabelsL2[i]   = tracklet.fLabelsL2[i];
    }
}

IlcAODTracklets& IlcAODTracklets::operator=(const IlcAODTracklets& tracklet)
{
// Assignment operator
    if(&tracklet == this) return *this;
    TNamed::operator=(tracklet);
    if(fNTracks!=tracklet.fNTracks){
      fNTracks = tracklet.fNTracks;
      CreateContainer(fNTracks);
    }
    for (Int_t i = 0; i < fNTracks; i++) {
	fTheta[i]    = tracklet.fTheta[i];
	fPhi[i]      = tracklet.fPhi[i];
	fDeltaPhi[i] = tracklet.fDeltaPhi[i];
	fLabels[i]   = tracklet.fLabels[i];
	fLabelsL2[i]   = tracklet.fLabelsL2[i];
    }
    return *this;
}

void IlcAODTracklets::CreateContainer(Int_t nTracks)
{
  // function that creates container to store tracklets

  DeleteContainer();
  
  fNTracks = nTracks;

  if (fNTracks <= 0) {
    fNTracks = 0;
    return;
  }

  fTheta = new Double32_t[fNTracks];
  fPhi = new Double32_t[fNTracks];
  fDeltaPhi = new Double32_t[fNTracks];
  fLabels = new Int_t[fNTracks];
  fLabelsL2 = new Int_t[fNTracks];
}


IlcAODTracklets::~IlcAODTracklets()
{
  // destructor

  DeleteContainer();
}

void IlcAODTracklets::DeleteContainer()
{
  // deletes allocated memory

  if (fTheta)
  {
    delete[] fTheta;
    fTheta = 0;
  }

  if (fPhi)
  {
    delete[] fPhi;
    fPhi = 0;
  }

  if (fDeltaPhi)
  {
    delete[] fDeltaPhi;
    fDeltaPhi = 0;
  }

  if (fLabels)
  {
    delete[] fLabels;
    fLabels = 0;
  }

  if (fLabelsL2)
  {
    delete[] fLabelsL2;
    fLabelsL2 = 0;
  }

  fNTracks = 0;
}

Bool_t IlcAODTracklets::SetTracklet(Int_t pos, Double32_t theta, Double32_t phi, Double32_t deltaPhi, Int_t labelL1, Int_t labelL2)
{
  // Sets a tracklet at the given position

  if (pos < 0 || pos >= fNTracks)
    return kFALSE;

  fTheta[pos] = theta;
  fPhi[pos] = phi;
  fDeltaPhi[pos] = deltaPhi;
  fLabels[pos] = labelL1;
  fLabelsL2[pos] = labelL2;

  return kTRUE;
}

