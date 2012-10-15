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
   
/* $Id$ */

//---------------------------------------------------------------------
// Jet finder base class
// manages the search for jets 
// Authors: jgcn@mda.cinvestav.mx
//          andreas.morsch@cern.ch
//          magali.estienne@subatech.in2p3.fr
//          alexandre.shabetai@cern.ch
//---------------------------------------------------------------------

#include <TFile.h>

#include "IlcJetFinder.h"
#include "IlcUA1JetHeaderV1.h"
#include "IlcAODJetEventBackground.h"
#include "IlcAODJet.h"
#include "IlcAODEvent.h"

ClassImp(IlcJetFinder)

///////////////////////////////////////////////////////////////////////

IlcJetFinder::IlcJetFinder():
  fHeader(0x0),
  fAODjets(0x0),
  fNAODjets(0),
  fAODEvBkg(0),
  fDebug(0),
  fCalTrkEvent(0x0)
{
  // Constructor
}

//-----------------------------------------------------------------------
IlcJetFinder::~IlcJetFinder()
{
  // Destructor
}

//-----------------------------------------------------------------------
void IlcJetFinder::WriteHeader()
{
  // Write the Headers
  TFile* f = new TFile("jets_local.root", "recreate");
  WriteHeaderToFile();
  f->Close();

}

//-----------------------------------------------------------------------
void IlcJetFinder::WriteHeaderToFile()
{
  // write reader header
  IlcJetHeader *rh = GetJetHeader();
  rh->Write();

}

//-----------------------------------------------------------------------
Bool_t IlcJetFinder::ProcessEvent()
{
  // Process one event

  // Find jets
  FindJets();

  Reset();
  return kTRUE;

}

//-----------------------------------------------------------------------
void IlcJetFinder::AddJet(IlcAODJet p)
{
  // Add new jet to the list
  if (fAODjets) { new ((*fAODjets)[fNAODjets++]) IlcAODJet(p);}
  else { Warning("IlcJetFinder::AddJet(IlcAODJet p)","fAODjets is null!");}

}

//-----------------------------------------------------------------------
void IlcJetFinder::ConnectAOD(const IlcAODEvent* aod)
{
  // Connect to the AOD
  fAODjets = aod->GetJets();
  fAODEvBkg = (IlcAODJetEventBackground*)(aod->FindListObject(IlcAODJetEventBackground::StdBranchName()));

}

//-----------------------------------------------------------------------
void IlcJetFinder::ConnectAODNonStd(IlcAODEvent* aod,const char *bname)
{
  // Connect non standard AOD jet and jet background branches 
  fAODjets = dynamic_cast<TClonesArray*>(aod->FindListObject(bname));
  fAODEvBkg = (IlcAODJetEventBackground*)(aod->FindListObject(Form("%s_%s",IlcAODJetEventBackground::StdBranchName(),bname)));
  // how is this is reset? Cleared? -> by the UserExec!!

}

