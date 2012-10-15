/**************************************************************************
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

//----------------------------------------------------------------------------
// Deterministic Annealing Jet header class
// Stores parameters of DA jet algorithm
// Author: Davide Perrino (davide.perrino@ba.infn.it, davide.perrino@cern.ch)
//----------------------------------------------------------------------------

#include "IlcDAJetHeader.h"

ClassImp(IlcDAJetHeader)


//---------------------------------------------------------------------
IlcDAJetHeader::IlcDAJetHeader():
	IlcJetHeader("IlcDAJetHeader"),
	fSelectJets(kTRUE),
	fRadius(0.7),
	fNclustMax(10),
	fFixedCl(kFALSE),
	fEtMin(10.),
	fNeff(0),
	fEtaEff(0.9)
{
    // Constructor
}

//---------------------------------------------------------------------
void IlcDAJetHeader::SetRadius(Float_t radius)
{
    // The radius requested is used to estimate the number of clusters
    // to be found, in order to obtain jets with the expected area.
    // It must not be intended as a sharp limit on the cluster radius
    
  Int_t nclust = (Int_t) (4.*fEtaEff/(radius*radius)) + 1;
  SetNclust(nclust);
}
