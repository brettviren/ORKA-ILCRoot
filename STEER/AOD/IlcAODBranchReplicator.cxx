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

// $Id: IlcAODBranchReplicator.cxx 50615 2011-07-16 23:19:19Z hristov $

//
// Base class of an object used for the replication 
// (and possibly filtering) of one (or several) AOD branches.
//
// Author: L. Aphecetche (Subatech)
//
// Intended usage is to be able to produce, besides the standard AOD (IlcAOD.root)
// some light-weight AODs, by filtering (or skipping completely) the unwanted branches.
// 
// Exemple usage (pseudo-code) :
// 
// IlcAODHandler* aodH = IlcAnalysisManager::GetAnalysisManager()->GetOutputEventHandler();
//
// IlcAODExtension* ext = aodH->AddFilteredAOD("IlcAOD.filtered.root","filtered AOD");
//
// ext->DisableReferences();
//  
// ext->FilterBranch("cascades",0x0); // IlcAOD.filtered.root will *not* contain the cascades branch
//
//  
// IlcAODBranchReplicator* murep = new IlcAODMuonReplicator("MuonReplicator",
//                                                           "remove non muon tracks and non primary or pileup vertices",
//                                                           new IlcAnalysisNonMuonTrackCuts,
//                                                           new IlcAnalysisNonPrimaryVertices);
// ext->FilterBranch("tracks",murep);   // both the tracks and vertices branches 
// ext->FilterBranch("vertices",murep); // will be filtered by the MuonReplicator
//  

#include "IlcAODBranchReplicator.h"

ClassImp(IlcAODBranchReplicator)

//______________________________________________________________________________
IlcAODBranchReplicator::IlcAODBranchReplicator(const char* name, const char* title)
: TNamed(name,title)
{
  // default ctor (nop)
}

//______________________________________________________________________________
IlcAODBranchReplicator::~IlcAODBranchReplicator()
{
  // dtor (nop)
}
