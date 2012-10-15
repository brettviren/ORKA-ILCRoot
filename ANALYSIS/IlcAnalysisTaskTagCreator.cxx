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

#include <Riostream.h>

#include <TChain.h>
#include <TTree.h>
#include <TString.h>
#include <TFile.h>
#include <TSystem.h>

#include "IlcAnalysisTaskTagCreator.h"
#include "IlcAnalysisManager.h"
#include "IlcESDEvent.h"
#include "IlcAODEvent.h"
#include "IlcESDInputHandler.h"
#include "IlcAODHandler.h"
#include "IlcRunTag.h"
#include "IlcEventTag.h"
#include "IlcAODTagCreator.h"
#include "IlcLog.h"


using std::cout;
using std::endl;
using std::ofstream;
ClassImp(IlcAnalysisTaskTagCreator)

////////////////////////////////////////////////////////////////////////

IlcAnalysisTaskTagCreator::IlcAnalysisTaskTagCreator():
    IlcAnalysisTaskSE(),
    fCreateTags(kFALSE),
    fFirstFile(kTRUE),
    fRunTag(0), 
    fTreeT(0),
    fTagCreator(0),
    fAODFileName(""),
    fGUID(0)
{
  // Default constructor
}

IlcAnalysisTaskTagCreator::IlcAnalysisTaskTagCreator(const char* name):
    IlcAnalysisTaskSE(name),
    fCreateTags(kFALSE),
    fFirstFile(kTRUE),
    fRunTag(0), 
    fTreeT(0),
    fTagCreator(0),
    fAODFileName(""),
    fGUID(0)
{
  // Constructor
    DefineOutput(1, TTree::Class()); 	
}

void IlcAnalysisTaskTagCreator::UserCreateOutputObjects()
{
// Create the output container
    OpenFile(1);
    fTreeT  = new TTree("T", "AOD Tags");
    fRunTag = new IlcRunTag();
    TBranch * btag = fTreeT->Branch("IlcTAG", "IlcRunTag", &fRunTag);
    btag->SetCompressionLevel(9);
    fTagCreator = new IlcAODTagCreator();
    PostData(1, fTreeT);
}

void IlcAnalysisTaskTagCreator::Init()
{

}

void IlcAnalysisTaskTagCreator::ConnectInputData(Option_t * /*option*/)
{
    // Initialization
    const char* turl = gSystem->Getenv("ALIEN_JDL_OUTPUTDIR");
    TString sturl = turl;
    
    if (sturl.Length() != 0) {
      fAODFileName = "alien://";
      fAODFileName += turl;
      fAODFileName += "/IlcAOD.root";
    }  
}

void IlcAnalysisTaskTagCreator::UserExec(Option_t */*option*/)
{

    // Create Tags for the current event
    IlcEventTag* evtTag = new IlcEventTag();
    fTagCreator->FillEventTag(AODEvent(), evtTag);
    // Reference to the input file
    TString fturl, fturltemp, guid;
    
    TString opt(fInputHandler->GetAnalysisType());
    opt.ToLower();
    
    TFile *file = OutputTree()->GetCurrentFile();
    const TUrl *url = file->GetEndpointUrl();
    guid = file->GetUUID().AsString();
    if (fAODFileName.Length() != 0) {
	fturl = fAODFileName;
	guid  = fGUID;
    } else {
	fturl = url->GetFile();
    }

    if (fRunTag->GetFileId(guid) == -1) {
      IlcFileTag *eftag = new IlcFileTag();

      eftag->SetGUID(guid);
      if(fAODFileName.Length() != 0) {
	eftag->SetMD5("");
	eftag->SetTURL(fturl);
	eftag->SetSize(0);
      }
      else eftag->SetPath(fturl);

      fRunTag->AddFileTag(eftag);
    }
    //
    // Add the event tag
    fRunTag->AddEventTag(*evtTag);
    PostData(1, fTreeT);
}


void IlcAnalysisTaskTagCreator::FinishTaskOutput()
{
// Terminate analysis
//
    if (fInputHandler->GetRunTag()) fRunTag->CopyStandardContent(fInputHandler->GetRunTag());	    
    fTreeT->Fill();
}

Bool_t IlcAnalysisTaskTagCreator::Notify()
{
    // Notify file change
    fInputHandler = (IlcInputEventHandler*) 
      ((IlcAnalysisManager::GetAnalysisManager())->GetInputEventHandler());
    return kTRUE;
}


void IlcAnalysisTaskTagCreator::GetGUID(TString &guid) {
    // Get the guid of the IlcAOD.root file
    ofstream myfile ("guid.txt");
    if (myfile.is_open()) {
	TFile *f = TFile::Open("IlcAOD.root","read");
	if(f->IsOpen()) {
	    guid = f->GetUUID().AsString();
	    myfile << "IlcAOD.root \t"<<f->GetUUID().AsString();
	    cout<<guid.Data()<<endl;
	    myfile.close();
	}
	else cout<<"Input file not found"<<endl;
	f->Close();
    }
    else cout<<"Output file can't be created..."<<endl;
}



void IlcAnalysisTaskTagCreator::Terminate(Option_t */*option*/)
{
// Terminate analysis
//
    if (fDebug > 1) printf("AnalysisTagCreator: Terminate() \n");
}


