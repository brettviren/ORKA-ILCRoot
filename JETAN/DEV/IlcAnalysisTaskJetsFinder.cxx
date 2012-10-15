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

//----------------------------------------------------------------
// Analysis task for interfacing the jet finders with the analysis framework
//
// Author: magali.estienne@subatech.in2p3.fr
//	   alexandre.shabetai@cern.ch
//          
//----------------------------------------------------------------

#include <Riostream.h> 
#include <TROOT.h>
#include <TSystem.h>
#include <TInterpreter.h>
#include <TFile.h>
#include <TList.h>
#include <TH1.h>

#include "IlcAnalysisTaskJetsFinder.h"
#include "IlcAnalysisManager.h"
#include "IlcJetFinder.h"
#include "IlcJetHeader.h"
#include "IlcJetHistos.h"
#include "IlcAODEvent.h"
#include "IlcAODJetEventBackground.h"
#include "IlcAODHandler.h"
#include "IlcAODExtension.h"

ClassImp(IlcAnalysisTaskJetsFinder)

////////////////////////////////////////////////////////////////////////

IlcAnalysisTaskJetsFinder::IlcAnalysisTaskJetsFinder():
  IlcAnalysisTaskSE(),
  fConfigFile("ConfigJetAnalysis.C"),
  fNonStdBranch(""), 
  fNonStdFile(""),
  fJetFinder(0x0),
  fHistos(0x0),
  fAODExtension(0x0),
  fListOfHistos(0x0),
  fTreeI(0x0),
  fEvent(0x0),
  fUseAODBackground(kFALSE),
  fFilterPt(0.)
{
  // Default constructor
}

//----------------------------------------------------------------
IlcAnalysisTaskJetsFinder::IlcAnalysisTaskJetsFinder(const char* name):
  IlcAnalysisTaskSE(name),
  fConfigFile("ConfigJetAnalysis.C"),
  fNonStdBranch(""),
  fNonStdFile(""),
  fJetFinder(0x0),
  fHistos(0x0),
  fAODExtension(0x0),
  fListOfHistos(0x0),
  fTreeI(0x0),
  fEvent(0x0),
  fUseAODBackground(kFALSE),
  fFilterPt(0.)
{
  // Constructor 2
  DefineInput(1, TTree::Class());
  DefineOutput(1, TList::Class());

}

//----------------------------------------------------------------
IlcAnalysisTaskJetsFinder::~IlcAnalysisTaskJetsFinder()
{
  // destructor
  if (fHistos && ! IlcAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fHistos;
  if (fListOfHistos &&  ! IlcAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fListOfHistos;

}

//----------------------------------------------------------------
void IlcAnalysisTaskJetsFinder::ConnectInputData(Option_t *)
{
  // Get the exchanged tree and event
  IlcAnalysisTaskSE::ConnectInputData();

  fTreeI = (TTree*)GetInputData(1);

  char **address =(char **)GetBranchAddress(1,"IlcJetCalTrkEvent");
  if (address) {
    fEvent = (IlcJetCalTrkEvent*) (*address);
  }
  else { printf("IlcJetCalTrkEvent address not found, pleae check containers"); }

}

//----------------------------------------------------------------
void IlcAnalysisTaskJetsFinder::UserCreateOutputObjects()
{
  // Create the output container containt
  //
  if (fDebug > 1) printf("AnalysisTaskJets::CreateOutPutData() \n");

  IlcJetHeader *fH = fJetFinder->GetJetHeader();

  if(fNonStdBranch.Length()==0)
    {
      // Connect default AOD to jet finder
      // create a new branch for the background

      if(fUseAODBackground){
        if(!AODEvent()->FindListObject(IlcAODJetEventBackground::StdBranchName())){
	  IlcAODJetEventBackground* evBkg = new IlcAODJetEventBackground();
	  evBkg->SetName(IlcAODJetEventBackground::StdBranchName());
	  AddAODBranch("IlcAODJetEventBackground",&evBkg);
        }
      }
      fJetFinder->ConnectAOD(AODEvent());
    }
  else
    {
      // Create a new branch for jets...
      // how is this reset? -> cleared in the UserExec....
      // Can this be handled by the framework?
      // here we can also have the case that the brnaches are written to a separate file

      TClonesArray *tca = new TClonesArray("IlcAODJet", 0);
      tca->SetName(fNonStdBranch.Data());
      AddAODBranch("TClonesArray",&tca,fNonStdFile.Data());
      if(fUseAODBackground){
        if(!AODEvent() || !AODEvent()->FindListObject(Form("%s_%s",IlcAODJetEventBackground::StdBranchName(),fNonStdBranch.Data()))){
	  IlcAODJetEventBackground* evBkg = new IlcAODJetEventBackground();
	  evBkg->SetName(Form("%s_%s",IlcAODJetEventBackground::StdBranchName(),fNonStdBranch.Data()));
	  AddAODBranch("IlcAODJetEventBackground",&evBkg,fNonStdFile.Data());
        }
      }
      if(fNonStdFile.Length()!=0){
	// 
	// case that we have an AOD extension we need to fetch the jets from the extended output
	// we identifay the extension aod event by looking for the branchname
	IlcAODHandler *aodH = dynamic_cast<IlcAODHandler*>(IlcAnalysisManager::GetAnalysisManager()->GetOutputEventHandler());
        fAODExtension = 0;
        if(aodH){
	  TObjArray* extArray = aodH->GetExtensions();
	  if (extArray) {
	    TIter next(extArray);
	    while ((fAODExtension=(IlcAODExtension*)next())){
	      TObject *obj = fAODExtension->GetAOD()->FindListObject(fNonStdBranch.Data());
	      if(fDebug>10){
	        Printf("%s:%d Dumping..",(char*)__FILE__,__LINE__);
	        fAODExtension->GetAOD()->Dump();
	      }
	      if(obj){
	        if(fDebug>1)Printf("AODExtension found for %s",fNonStdBranch.Data());
	        break;
	      }
	      fAODExtension = 0;
	    }
	  }
        }
	if(fAODExtension)fJetFinder->ConnectAODNonStd(fAODExtension->GetAOD(), fNonStdBranch.Data()); 
      }
      else{
        if (fDebug > 1) printf("Connecting Non Std Branch AOD %p %s \n",AODEvent(),fNonStdBranch.Data());
	fJetFinder->ConnectAODNonStd(AODEvent(), fNonStdBranch.Data()); 
      }
    }

  // do not add the histograms in the directory
  // all handled by the list
  Bool_t oldStatus = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  // Histograms
  fListOfHistos = new TList();
  fListOfHistos->SetOwner();
  fHistos       = new IlcJetHistos();
  fHistos->CreateHistos();
  fHistos->AddHistosToList(fListOfHistos);
  // CDF case
  if(fJetFinder->InheritsFrom("IlcCdfJetFinder")) fJetFinder->CreateOutputObjects(fListOfHistos);

  // Add the JetFinderInformation to the Outputlist
  
  // Compose a characteristic output name
  // with the name of the output branch
  if(fH) {
    if(fNonStdBranch.Length()==0) {
      fH->SetName("IlcJetHeader_jets");
    }
    else {
      fH->SetName(Form("IlcJetHeader_%s",fNonStdBranch.Data()));
    }
  }

  TH1::AddDirectory(oldStatus);
  
  if(!fAODExtension)OutputTree()->GetUserInfo()->Add(fH);
  else fAODExtension->GetTree()->GetUserInfo()->Add(fH);

  // post
  PostData(1, fListOfHistos);

}

//----------------------------------------------------------------
void IlcAnalysisTaskJetsFinder::Init()
{
  // Initialization
  if (fDebug > 1) printf("AnalysisTaskJetsFinder::Init() \n");

  // Call configuration file
  if (fConfigFile.Length()) {
    gROOT->LoadMacro(fConfigFile);
    fJetFinder = (IlcJetFinder*) gInterpreter->ProcessLine("ConfigJetAnalysis()");
  }

  // Initialise Jet Analysis
  fJetFinder->Init();

}


//----------------------------------------------------------------
Bool_t IlcAnalysisTaskJetsFinder::Notify()
{
  if (fDebug > 1) printf("AnalysisTaskJetsFinder::Notify() \n");
  return kTRUE;

}

//----------------------------------------------------------------
void IlcAnalysisTaskJetsFinder::UserExec(Option_t */*option*/)
{

  // Execute analysis for current event

  if (fDebug > 1) printf("AnalysisTaskJetsFinder::UserExec() \n");

  TClonesArray* jarray = 0;
  IlcAODJetEventBackground* evBkg = 0;

  // only need this once
  static IlcAODHandler *aodH = dynamic_cast<IlcAODHandler*>(IlcAnalysisManager::GetAnalysisManager()->GetOutputEventHandler());

  if(fNonStdBranch.Length()==0) {
    jarray = AODEvent()->GetJets();
    if(fUseAODBackground){
      evBkg = (IlcAODJetEventBackground*)(AODEvent()->FindListObject(IlcAODJetEventBackground::StdBranchName()));
      evBkg->Reset();
    }
  }
  else {
    if(AODEvent())jarray = (TClonesArray*)(AODEvent()->FindListObject(fNonStdBranch.Data()));
    if(!jarray)jarray = (TClonesArray*)(fAODExtension->GetAOD()->FindListObject(fNonStdBranch.Data()));
    if(jarray)jarray->Delete();    // this is our responsibility, clear before filling again
    if(fUseAODBackground){
      if(AODEvent())evBkg = (IlcAODJetEventBackground*)(AODEvent()->FindListObject(Form("%s_%s",IlcAODJetEventBackground::StdBranchName(),fNonStdBranch.Data())));
      if(!evBkg)  evBkg = (IlcAODJetEventBackground*)(fAODExtension->GetAOD()->FindListObject(Form("%s_%s",IlcAODJetEventBackground::StdBranchName(),fNonStdBranch.Data())));
      if(evBkg)evBkg->Reset();
    }
  }

  fTreeI->GetEntry(0);

  fJetFinder->SetCalTrkEvent(*fEvent);

  fJetFinder->ProcessEvent(); 

  // Fill control histos
  if(jarray)fHistos->FillHistos(jarray);

  // Store the jet branch in the AOD
  if(jarray&&aodH&&fFilterPt>0){
    if(jarray->GetEntries()>0){
      IlcAODJet *jet = (IlcAODJet*)jarray->At(0);
      if(jet->Pt()>fFilterPt){
        aodH->SetFillAOD(kTRUE);
      }
    }
  }

  // Post the data
  PostData(1, fListOfHistos);
  return;

}

//----------------------------------------------------------------
void IlcAnalysisTaskJetsFinder::Terminate(Option_t */*option*/)
{
  // Terminate analysis
  if (fDebug > 1) printf("AnalysisJets: Terminate() \n");

}

