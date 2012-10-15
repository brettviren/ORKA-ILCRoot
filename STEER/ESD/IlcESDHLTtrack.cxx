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

//-----------------------------------------------------------------
//           Implementation of the ESD HLT track class
//   ESD = Event Summary Data
//   HLT = High Level Trigger
//   This is the class to deal with during the phisical analysis of data
//-----------------------------------------------------------------

#include "TMath.h"
#include "IlcESDHLTtrack.h"
#include "IlcKalmanTrack.h"

ClassImp(IlcESDHLTtrack)

IlcESDHLTtrack::IlcESDHLTtrack() :
  TObject(),
  fNHits(0),
  fMCid(0),
  fWeight(0),
  fFromMainVertex(kFALSE),
  fSector(0),
  fQ(0),
  fTanl(0),
  fPsi(0),
  fPt(0),
  fPterr(0),
  fPsierr(0),
  fTanlerr(0),
  fBinX(0),
  fBinY(0),
  fSizeX(0),
  fSizeY(0),
  fPID(0)
{
  // Default constructor
  fRowRange[0] = fRowRange[1] = 0;
  fFirstPoint[0] = fFirstPoint[1] = fFirstPoint[2] = 0;
  fLastPoint[0] = fLastPoint[1] = fLastPoint[2] = 0;
}

Double_t IlcESDHLTtrack::GetP() const
{
  // Returns total momentum.  
  return TMath::Abs(GetPt())*sqrt(1. + GetTgl()*GetTgl());
}

Double_t IlcESDHLTtrack::GetPseudoRapidity() const
{
  // Calculates the pseudorapidity
  return 0.5 * TMath::Log((GetP() + GetPz()) / (GetP() - GetPz()));
}

Bool_t IlcESDHLTtrack::UpdateTrackParams(const IlcKalmanTrack *t)
{
  // Updates the track parameters

  fNHits = t->GetNumberOfClusters();
  fMCid = t->GetLabel();

  Double_t alpha = t->GetAlpha();
  fSector = (UShort_t)(alpha/(2*TMath::Pi()/18));
  Double_t x,p[5]; t->GetExternalParameters(x,p);
  if(p[4]<=0)
    fQ = 1;
  else
    fQ = -1;
  fPt = TMath::Abs(1./p[4]);
  fTanl = p[3];
  fPsi = alpha + TMath::ASin(p[2]);

  fFirstPoint[0] = x*TMath::Cos(alpha) - p[0]*TMath::Sin(alpha);
  fFirstPoint[1] = x*TMath::Sin(alpha) + p[0]*TMath::Cos(alpha);
  fFirstPoint[2] = p[1];

  return kTRUE;
}
