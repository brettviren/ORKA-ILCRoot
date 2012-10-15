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

/* $Id: IlcSelector.cxx 50615 2011-07-16 23:19:19Z hristov $ */

// Selector base class for analysis based on ESD
// Please derive your selector-based analysis from this class, if you just want to use
// information from the ESD.
//
// The ESD is available as member fESD
//
// The following methods can be overrriden. Please do not forgot to call the base class function.
//
//    Begin():        called everytime a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Init():         called for each new tree. Enable/Disable branches here.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
//  Author: Jan.Fiete.Grosse-Oetringhaus@cern.ch

#include "IlcSelector.h"

#include <TStyle.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TRegexp.h>
#include <TTime.h>
#include <TFriendElement.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <TTimeStamp.h>

#include "IlcLog.h"
#include "IlcESD.h"

ClassImp(IlcSelector)

IlcSelector::IlcSelector() :
  TSelector(),
  fTree(0),
  fESD(0),
  fCountFiles(0)
{
  //
  // Constructor. Initialization of pointers
  //
}

IlcSelector::~IlcSelector()
{
  //
  // Destructor
  //

 if (fTree)
   fTree->ResetBranchAddresses();

 if (fESD)
 {
   delete fESD;
   fESD = 0;
 }
}

void IlcSelector::CheckOptions()
{
  // checks the option string for the debug flag

  IlcLog::SetClassDebugLevel(ClassName(), IlcLog::kInfo);

  TString option = GetOption();

  if (option.Contains("moredebug"))
  {
    printf("Enabling verbose debug mode for %s\n", ClassName());
    IlcLog::SetClassDebugLevel(ClassName(), IlcLog::kDebug+1);
    IlcInfo(Form("Called with option %s.", option.Data()));
  }
  else if (option.Contains("debug"))
  {
    printf("Enabling debug mode for %s\n", ClassName());
    IlcLog::SetClassDebugLevel(ClassName(), IlcLog::kDebug);
    IlcInfo(Form("Called with option %s.", option.Data()));
  }
}

void IlcSelector::Begin(TTree*)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  CheckOptions();

  IlcDebug(IlcLog::kDebug, "============BEGIN===========");
}

void IlcSelector::SlaveBegin(TTree* tree)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  CheckOptions();

  IlcDebug(IlcLog::kDebug, "=======SLAVEBEGIN========");
  IlcDebug(IlcLog::kDebug, Form("Hostname: %s", gSystem->HostName()));
  IlcDebug(IlcLog::kDebug, Form("Time: %s", gSystem->Now().AsString()));

  if (tree != 0)
    Init(tree);
}

void IlcSelector::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses of the tree
  // will be set. It is normaly not necessary to make changes to the
  // generated code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running with PROOF.

  IlcDebug(IlcLog::kDebug, "=========Init==========");

  fTree = tree;

  if (fTree == 0)
  {
    IlcDebug(IlcLog::kError, "ERROR: tree argument is 0.");
    return;
  }

  // Set branch address
  fTree->SetBranchAddress("ESD", &fESD);
  if (fESD != 0)
    IlcDebug(IlcLog::kInfo, "INFO: Found ESD branch in chain.");
}

Bool_t IlcSelector::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. Typically here the branch pointers
  // will be retrieved. It is normaly not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed.

  IlcDebug(IlcLog::kDebug, "=========NOTIFY==========");
  IlcDebug(IlcLog::kDebug, Form("Hostname: %s", gSystem->HostName()));
  IlcDebug(IlcLog::kDebug, Form("Time: %s", TTimeStamp(time(0)).AsString()));

  ++fCountFiles;
  if (fTree)
  {
    TFile *f = fTree->GetCurrentFile();
    if (f)
    {
      IlcDebug(IlcLog::kInfo, Form("Processing %d. file %s", fCountFiles, f->GetName()));
    }
    else
      IlcDebug(IlcLog::kError, "fTree->GetCurrentFile() is 0");
  }
  else
  {
    IlcDebug(IlcLog::kError, "fTree not available");
  }

  return kTRUE;
}

Bool_t IlcSelector::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // It can be passed to either TTree::GetEntry() or TBranch::GetEntry()
  // to read either all or the required parts of the data. When processing
  // keyed objects with PROOF, the object is already loaded and is available
  // via the fObject pointer.
  //
  // This function should contain the "body" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.

  // WARNING when a selector is used with a TChain, you must use
  //  the pointer to the current TTree to call GetEntry(entry).
  //  The entry is always the local entry number in the current tree.
  //  Assuming that fTree is the pointer to the TChain being processed,
  //  use fTree->GetTree()->GetEntry(entry).

  IlcDebug(IlcLog::kDebug, Form("=========PROCESS========== Entry %lld", entry));

  if (!fTree)
  {
    IlcDebug(IlcLog::kError, "ERROR: fTree is 0.");
    return kFALSE;
  }

  fTree->GetTree()->GetEntry(entry);

  if (fESD)
    IlcDebug(IlcLog::kDebug, Form("ESD: We have %d tracks.", fESD->GetNumberOfTracks()));

  return kTRUE;
}

void IlcSelector::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

  IlcDebug(IlcLog::kDebug, "=======SLAVETERMINATE=======");
}

void IlcSelector::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

  IlcDebug(IlcLog::kDebug, "=========TERMINATE==========");
}
