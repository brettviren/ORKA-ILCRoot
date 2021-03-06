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
 
//---------------------------------------------------------------------
// UA1 Jet header class 
// Stores parameters of UA1 jet algo
// Author: jgcn@mda.cinvestav.mx
//---------------------------------------------------------------------

#include <Riostream.h> 
#include <TMath.h>
#include "IlcUA1JetHeader.h"
ClassImp(IlcUA1JetHeader)
 
////////////////////////////////////////////////////////////////////////

IlcUA1JetHeader::IlcUA1JetHeader():
  IlcJetHeader("IlcUA1JetHeader"),
  fConeRadius(0.3),
  fEtSeed(3.0),
  fMinJetEt(10.),
  fMinCellEt(0.),
  fMode(1),
  fMinMove(0.05),
  fMaxMove(0.15),
  fPrecBg(0.035),
  fLegoNbinEta(36),
  fLegoNbinPhi(124),
  fLegoEtaMin(-0.9),
  fLegoEtaMax(0.9),
  fLegoPhiMin(0.),
  fLegoPhiMax(2.*TMath::Pi()),
  fOnlySignal(kFALSE),
  fOnlyBkgd(kFALSE)
{
  // Default constructor
}
 
////////////////////////////////////////////////////////////////////////
 
void IlcUA1JetHeader::PrintParameters() const
{
  // prints out parameters of jet algorithm

  cout << " UA1 jet algorithm " << endl;
  cout << " * Jet parameters: " << endl;
  cout << "   Cone size: " << fConeRadius<< endl;
  cout << "   Minimum energy for a seed: " << fEtSeed << endl;
  cout << "   Minumum energy for a jet: " << fMinJetEt << endl;
  cout << "   Minumum energy for a cell: " << fMinCellEt << endl;
  cout << " * Background substraction parameters: " << endl;
  cout << "   Substraction mode: " << fMode << endl;
  cout << "   Minimum allowed move: " << fMinMove << endl;
  cout << "   Maximum allowed move: " << fMaxMove << endl;
  cout << "   Precision for background: " << fPrecBg << endl;
  cout << " * Lego parameters: " << endl;
  cout << "   Number of bins in eta: " << fLegoNbinEta<< endl;
  cout << "   Number of bins in phi: " << fLegoNbinPhi<< endl;
  cout << "   Minimum azimuthal angle: " << fLegoPhiMin<< endl;
  cout << "   Maximum azimuthal angle: " << fLegoPhiMax<< endl;
  cout << "   Minimum rapidity angle: " << fLegoEtaMin<< endl;
  cout << "   Maximum rapidity angle: " << fLegoEtaMax<< endl;
}
