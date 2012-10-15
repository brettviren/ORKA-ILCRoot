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
   
/* $Id: IlcJetFinder.cxx 48117 2011-03-06 14:24:54Z morsch $ */

//---------------------------------------------------------------------
// Jet finder base class
// manages the search for jets 
// Authors: jgcn@mda.cinvestav.mx
//          andreas.morsch@cern.ch
//          magali.estienne@subatech.in2p3.fr
//---------------------------------------------------------------------

#include <Riostream.h>
#include <TFile.h>

#include "IlcJetFinder.h"
#include "IlcAODJet.h"
#include "IlcAODEvent.h"
#include "IlcJetUnitArray.h"
#include "IlcJetReaderHeader.h"
#include "IlcJetHeader.h"
#include "IlcJetReader.h"
#include "IlcAODJetEventBackground.h"

ClassImp(IlcJetFinder)

IlcJetFinder::IlcJetFinder():
    fReader(0x0),
    fHeader(0x0),
    fAODjets(0x0),
    fNAODjets(0),
    fAODEvBkg(0),
    fDebug(0)
{
  //
  // Constructor
  //
  fAODjets = 0;
}

////////////////////////////////////////////////////////////////////////
IlcJetFinder::~IlcJetFinder()
{
  //
  // Destructor
  //
}



////////////////////////////////////////////////////////////////////////
void IlcJetFinder::WriteRHeaderToFile()
{
  // write reader header
    IlcJetReaderHeader *rh = fReader->GetReaderHeader();
    rh->Write();
}


////////////////////////////////////////////////////////////////////////
void IlcJetFinder::ConnectTree(TTree* tree, TObject* data)
{
    // Connect the input file
    fReader->ConnectTree(tree, data);
}

////////////////////////////////////////////////////////////////////////
void IlcJetFinder::WriteHeaders()
{
    // Write the Headers
    TFile* f = new TFile("jets_local.root", "recreate");
    WriteRHeaderToFile();
    WriteJHeaderToFile();
    f->Close();
}

////////////////////////////////////////////////////////////////////////
Bool_t IlcJetFinder::ProcessEvent()
{
  //
  // Process one event
  // Charged only jets
  //

  Bool_t ok = fReader->FillMomentumArray();
  if (!ok) return kFALSE;
  // Jets
  FindJets(); // V1
  Reset();  
  return kTRUE;
}

////////////////////////////////////////////////////////////////////////
Bool_t IlcJetFinder::ProcessEvent2()
{
  //
  // Process one event
  // Charged only or charged+neutral jets
  //

  TRefArray* ref = new TRefArray();
  Bool_t procid = kFALSE;
  Bool_t ok = fReader->ExecTasks(procid,ref);

  // Delete reference pointer  
  if (!ok) {delete ref; return kFALSE;}
  // Jets
  FindJets();
  
  Int_t nEntRef = ref->GetEntries();

  for(Int_t i=0; i<nEntRef; i++)
    { 
      // Reset the UnitArray content which were referenced
      ((IlcJetUnitArray*)ref->At(i))->SetUnitTrackID(0);
      ((IlcJetUnitArray*)ref->At(i))->SetUnitEnergy(0.);
      ((IlcJetUnitArray*)ref->At(i))->SetUnitCutFlag(kPtSmaller);
      ((IlcJetUnitArray*)ref->At(i))->SetUnitCutFlag2(kPtSmaller);
      ((IlcJetUnitArray*)ref->At(i))->SetUnitSignalFlag(kBad);
      ((IlcJetUnitArray*)ref->At(i))->SetUnitSignalFlagC(kTRUE,kBad);
      ((IlcJetUnitArray*)ref->At(i))->SetUnitDetectorFlag(kTpc);
      ((IlcJetUnitArray*)ref->At(i))->SetUnitFlag(kOutJet);
      ((IlcJetUnitArray*)ref->At(i))->ClearUnitTrackRef();

      // Reset process ID
      IlcJetUnitArray* uA = (IlcJetUnitArray*)ref->At(i);
      uA->ResetBit(kIsReferenced);
      uA->SetUniqueID(0);     
    }

  // Delete the reference pointer
  ref->Delete();
  delete ref;

  Reset();

  return kTRUE;
}


void IlcJetFinder::AddJet(IlcAODJet p)
{
// Add new jet to the list
  new ((*fAODjets)[fNAODjets++]) IlcAODJet(p);
}

void IlcJetFinder::ConnectAOD(const IlcAODEvent* aod)
{
// Connect to the AOD
    fAODjets = aod->GetJets();
    fAODEvBkg = (IlcAODJetEventBackground*)(aod->FindListObject(IlcAODJetEventBackground::StdBranchName()));
}

////////////////////////////////////////////////////////////////////////
void IlcJetFinder::ConnectAODNonStd(IlcAODEvent* aod,const char *bname)
{

  fAODjets = dynamic_cast<TClonesArray*>(aod->FindListObject(bname));
  fAODEvBkg = (IlcAODJetEventBackground*)(aod->FindListObject(Form("%s_%s",IlcAODJetEventBackground::StdBranchName(),bname)));
  // how is this is reset? Cleared? -> by the UserExec!!
}

