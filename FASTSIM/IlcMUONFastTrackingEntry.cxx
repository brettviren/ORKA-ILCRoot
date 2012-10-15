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

/* $Id: IlcMUONFastTrackingEntry.cxx 48085 2011-03-05 18:37:52Z morsch $ */

#include "IlcMUONFastTrackingEntry.h"

IlcMUONFastTrackingEntry::IlcMUONFastTrackingEntry():
    fP(0.),
    fTheta(0.),
    fPhi(0.),
    fMeanp(0.),
    fMeantheta(0.),
    fMeanphi(0.),
    fSigmap(0.),
    fSigmatheta(0.),
    fSigmaphi(0.),
    fSigma1p(0.),
    fChi2p(0.),
    fChi2theta(0.),
    fChi2phi(0.),
    fNormG2(0.),
    fMeanG2(0.),
    fSigmaG2(0.)
{ 
// Default constructor
  for (Int_t i=0; i<5; i++) { 
    for (Int_t j=0; j<3; j++) { 
      fAcc[i][j] = 0;
      fEff[i][j] = 0;
    }
  }  
}

ClassImp(IlcMUONFastTrackingEntry)
