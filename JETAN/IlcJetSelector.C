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
// Jet finder analysis class deriving from TSelector
// and using IlcJetFinder.cxx
// manages the search for jets 
// Author: andreas.morsch@cern.ch
//---------------------------------------------------------------------

#include <Riostream.h>
#include <TTree.h>
#include <TFile.h>
#include "IlcJetSelector.h"
#include "IlcJetESDReaderHeader.h"
#include "IlcJetESDReader.h"
#include "IlcUA1JetHeaderV1.h"
#include "IlcUA1JetFinderV1.h"
#include "IlcJetFinder.h"

IlcJetSelector::IlcJetSelector(TTree*):
    TSelector(),  
    fJetFinder(0)
{
  // Constructor
}

////////////////////////////////////////////////////////////////////////

IlcJetSelector::~IlcJetSelector()
{
  // destructor
    delete fJetFinder;
}

void IlcJetSelector::Config()
{
    //
    // Configuration goes here
    // 
    printf("JetSelector::Config\n");
    IlcJetESDReaderHeader *jrh = new IlcJetESDReaderHeader();
    jrh->SetComment("Testing");
    jrh->SetDirectory("/home/morsch/analysis/AliEn/PDC06/");
    jrh->SetPattern("00");
    jrh->SetFirstEvent(0);
    jrh->SetLastEvent(1000);
    jrh->SetPtCut(0.);
    jrh->SetReadSignalOnly(kFALSE);
    // Define reader and set its header
    IlcJetESDReader *er = new IlcJetESDReader();
    er->SetReaderHeader(jrh);
   
 
    // Define jet header
    IlcUA1JetHeaderV1 *jh=new IlcUA1JetHeaderV1();
    jh->SetComment("UA1 jet code with default parameters");
    jh->BackgMode(0);
    jh->SetRadius(1.0);
    jh->SetEtSeed(2.);
    jh->SetLegoNbinPhi(420.);
    jh->SetLegoNbinEta(120.);
    jh->SetLegoEtaMin(-1.9);
    jh->SetLegoEtaMax(+1.9);  
    jh->SetMinJetEt(5.);
    
  // Define jet finder. Set its header and reader
    fJetFinder = new IlcUA1JetFinderV1();
    fJetFinder->SetJetHeader(jh);
    fJetFinder->SetJetReader(er);
    fJetFinder->SetPlotMode(kTRUE);
    fJetFinder->SetOutputFile("jets.root");
}


void IlcJetSelector::Begin(TTree*)
{
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).
    printf("JetSelector::Begin \n");
    TString option = GetOption();
}

void   IlcJetSelector::SlaveBegin(TTree* tree)
{
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).
    printf("JetSelector::SlaveBegin \n");
    // TSelector::Init
    Init(tree);
    // Configuration
    Config();
    // Initialize Jet Finder
    fJetFinder->Init();
    // Connect Tree
    fJetFinder->ConnectTree(tree);
    // Write the headers
    fJetFinder->WriteHeaders();
}

void   IlcJetSelector::Init(TTree* tree)
{
    // 
    // Here the chain has to be assigned to the reader
    // 
    printf("JetSelector::Init \n");
}

Bool_t IlcJetSelector::Notify()
{
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. Typically here the branch pointers
    // will be retrieved. It is normaly not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed.
    printf("JetSelector::Notify \n");
    return kTRUE;
}

Bool_t IlcJetSelector::Process(Long64_t entry)
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
    //  Assuming that fChain is the pointer to the TChain being processed,
    //  use fChain->GetEntry(entry).
    printf("JetSelector::Process \n");
    return (fJetFinder->ProcessEvent(entry));
}

void   IlcJetSelector::SlaveTerminate()
{
    // Finish the run
    fJetFinder->FinishRun();
}

void   IlcJetSelector::Terminate()
{
}

