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
// Jet AOD Reader Header
// Header for the AOD reader in the jet analysis
// Author: Davide Perrino (davide.perrino@cern.ch)
//---------------------------------------------------------------------
 
#include "IlcJetAODReaderHeader.h"

ClassImp(IlcJetAODReaderHeader)

//____________________________________________________________________________
IlcJetAODReaderHeader::IlcJetAODReaderHeader():
  IlcJetReaderHeader("IlcJetAODReaderHeader"), 
  fNaod(0),
  fTestFilterMask(0),
  fReadMC(0),
  fNonStdBranch("aodExtraTracks"),
  fReadBranches(kReadStdBranch)
{
  // Default constructor  
}

//____________________________________________________________________________
IlcJetAODReaderHeader::~IlcJetAODReaderHeader()
{
  // destructor
}
