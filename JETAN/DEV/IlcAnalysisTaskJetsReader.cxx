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

//----------------------------------------------------------------
// Analysis task for interfacing the jet reader with the analysis framework
//
// Author: magali.estienne@subatech.in2p3.fr 
//         alexandre.shabetai@cern.ch
//----------------------------------------------------------------

#include <Riostream.h> 
#include <TROOT.h>
#include <TInterpreter.h>
#include <TTree.h>

#include "IlcAnalysisTaskJetsReader.h"
#include "IlcAnalysisManager.h"
#include "IlcJetReader.h"
#include "IlcAODEvent.h"
#include "IlcMCEvent.h"
#include "IlcJetCalTrk.h"

ClassImp(IlcAnalysisTaskJetsReader)

////////////////////////////////////////////////////////////////////////

IlcAnalysisTaskJetsReader::IlcAnalysisTaskJetsReader():
  IlcAnalysisTaskSE(),
  fConfigFile("ConfigJetReaderAnalysis.C"),
  fJetReader(0x0),
  fReadAODFromOutput(0),
  fReaderEvent(0x0),
  fExchangeTree(0x0)
{
  // Default constructor
}

//----------------------------------------------------------------
IlcAnalysisTaskJetsReader::IlcAnalysisTaskJetsReader(const char* name):
  IlcAnalysisTaskSE(name),
  fConfigFile("ConfigJetReaderAnalysis.C"),
  fJetReader(0x0),
  fReadAODFromOutput(0),
  fReaderEvent(0x0),
  fExchangeTree(0x0)
{
  // Default constructor
  DefineOutput(1, TTree::Class());

}

//----------------------------------------------------------------
IlcAnalysisTaskJetsReader::~IlcAnalysisTaskJetsReader()
{
  // Destructor

  if (fExchangeTree)
   delete fExchangeTree;

}

//----------------------------------------------------------------
void IlcAnalysisTaskJetsReader::UserCreateOutputObjects()
{
  // Create the TTree to be exchanged between the reader and finder
  //
   
  if (fDebug > 1) printf("AnalysisTaskJetsReader::CreateOutPutData() \n");

  fExchangeTree = new TTree("jets_ExchangeContainer","ExchangeTree");

  fJetReader->InitTasks();

  fReaderEvent = fJetReader->GetCalTrkEvent();

  fExchangeTree->Branch("IlcJetCalTrkEvent", &fReaderEvent);

  PostData(1, fExchangeTree);

}

//----------------------------------------------------------------
void IlcAnalysisTaskJetsReader::Init()
{
  // Initialization
  if (fDebug > 1) printf("AnalysisTaskJets::Init() \n");
 
  // Call configuration file
  if (fConfigFile.Length()) {
    gROOT->LoadMacro(fConfigFile);
    fJetReader = (IlcJetReader*) gInterpreter->ProcessLine("ConfigJetReaderAnalysis()");
  }

}

//----------------------------------------------------------------
void IlcAnalysisTaskJetsReader::UserExec(Option_t */*option*/)
{

  // Execute analysis for current event
  //

  // Clear current CalTrkEvent 
  fReaderEvent->Clear();

  fExchangeTree->Reset();

  // Give InputEvent to the reader
  if (dynamic_cast<IlcAODEvent*>(InputEvent()) !=  0 && !fReadAODFromOutput) {
    // AOD is input event..........................................V                                       
    fJetReader->SetInputEvent(InputEvent(), InputEvent(), MCEvent());
  } else {
    // AOD is read from output ....................................V     
    fJetReader->SetInputEvent(InputEvent(), AODEvent(), MCEvent());
  }

  // Process current event
  fJetReader->ProcessEvent();

  // Fill object to be exchanged between reader and finder tasks
  fExchangeTree->Fill();

  // Post the data
  PostData(1, fExchangeTree);

  return;

}

//----------------------------------------------------------------
void IlcAnalysisTaskJetsReader::Terminate(Option_t */*option*/)
{
  // Terminate analysis
  if (fDebug > 1) printf("AnalysisJets: Terminate() \n");

}

