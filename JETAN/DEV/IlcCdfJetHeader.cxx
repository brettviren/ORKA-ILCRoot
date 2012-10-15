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
// Jet Finder based on CDF algortihm
// Charged jet evolution and the underlying event in proton-antiproton collisions at 1.8 TeV
// Physical Review D, vol. 65, Issue 9, id. 092002
// http://www.phys.ufl.edu/~rfield/cdf/chgjet/chgjet_intro.html
// Authors : Adrian.Sevcenco@cern.ch (adriansev@spacescience.ro )
//           Daniel.Felea@cern.ch    (dfelea@spacescience.ro)
//           Ciprian.Mihai.Mitu@cern.ch  (mcm@spacescience.ro)
//---------------------------------------------------------------------


/*
  Changelog



*/

#include "IlcCdfJetHeader.h"

ClassImp ( IlcCdfJetHeader )

////////////////////////////////////////////////////////////////////////

IlcCdfJetHeader::IlcCdfJetHeader():
  fMinPartJet(1),
  fJetPtCut(0.),
  fAODwrite(kFALSE),
  fAODtracksWrite(kFALSE),
  fkAnalyseJets(kFALSE)
{
  // Constructor
}
