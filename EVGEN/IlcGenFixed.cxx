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

/* $Id: IlcGenFixed.cxx 51126 2011-08-17 13:37:49Z cvetan $ */

// Simple particle gun. 
// Momentum, phi and theta of the partice as well as the particle type can be set.
// If fExplicit is true the user set momentum vector is used,
// otherwise it is calculated.
// andreas.morsch@cern.ch

#include "TPDGCode.h"

#include "IlcGenFixed.h"
#include "IlcRun.h"
  
ClassImp(IlcGenFixed)

//_____________________________________________________________________________
IlcGenFixed::IlcGenFixed()
    :IlcGenerator(), 
     fIpart(0),
     fExplicit(kFALSE)
{
  //
  // Default constructor
  //
    for (Int_t i = 0; i < 3; i++) fP[i] = 0.;
    
}

//_____________________________________________________________________________
IlcGenFixed::IlcGenFixed(Int_t npart)
    :IlcGenerator(npart),
     fIpart(kProton),
     fExplicit(kFALSE)
{
  //
  // Standard constructor
  //
  fName="Fixed";
  fTitle="Fixed Particle Generator";
  for (Int_t i = 0; i < 3; i++) fP[i] = 0.;
}

//_____________________________________________________________________________
void IlcGenFixed::Generate()
{
  //
  // Generate one trigger
  //
  Float_t polar[3]= {0,0,0};
  Float_t random[6];

  if(!fExplicit) {
    fP[0] = fPMin * TMath::Cos(fPhiMin) * TMath::Sin(fThetaMin);
    fP[1] = fPMin * TMath::Sin(fPhiMin) * TMath::Sin(fThetaMin);
    fP[2] = fPMin * TMath::Cos(fThetaMin);
  }
  Int_t i, j, nt;
  //
  Float_t o[3] = {fOrigin[0], fOrigin[1], fOrigin[2]}; 
  Float_t time = fTimeOrigin;
  if(fVertexSmear == kPerEvent) {
      Vertex();
      for (j = 0;j < 3; j++) o[j] = fVertex[j];
      time = fTime;
  }
  if(fVertexSmear==kPerTrack) {
    Rndm(random,6);
    for (j=0;j<3;j++) {
      o[j]=fOrigin[j]+fOsigma[j]*TMath::Cos(2*random[2*j]*TMath::Pi())*
      TMath::Sqrt(-2*TMath::Log(random[2*j+1]));
    }
    
    Rndm(random,2);
    time = fTimeOrigin + fOsigma[2]/TMath::Ccgs()*
    TMath::Cos(2*random[0]*TMath::Pi())*
    TMath::Sqrt(-2*TMath::Log(random[1]));
  }
  
  for(i = 0; i < fNpart; i++) 
    PushTrack(fTrackIt, -1, fIpart, fP, o , polar, time, kPPrimary, nt);
}
  
//_____________________________________________________________________________
void IlcGenFixed::SetSigma(Float_t sx, Float_t sy, Float_t sz)
{
  //
  // Set the interaction point sigma
  //
  fOsigma[0] = sx;
  fOsigma[1] = sy;
  fOsigma[2] = sz;
//   printf("Vertex smearing not implemented for fixed generator\n");
}
