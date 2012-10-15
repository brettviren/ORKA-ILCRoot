
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

/*$Id$*/

//-------------------------------------------------------------------------
//
//     Implementation of the Virtual Event Handler Interface for ESD
//
//-------------------------------------------------------------------------


#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TROOT.h>

#include "IlcLog.h"
#include "IlcESDHandler.h"
#include "IlcESDEvent.h"
#include "IlcESDfriend.h"

ClassImp(IlcESDHandler)

//______________________________________________________________________________
IlcESDHandler::IlcESDHandler() :
	IlcVEventHandler(),
	fesdf(NULL),
	fTreeEF(NULL),
	fFileEF(NULL),
	fFileName("IlcESDfriends_v1.root"),
	fIsEventSelectedForFriends(kFALSE)
{

	// default constructor
}

//______________________________________________________________________________
IlcESDHandler::IlcESDHandler(const char* name, const char* title):
	IlcVEventHandler(name, title),
	fesdf(NULL),
	fTreeEF(NULL),
	fFileEF(NULL),
	fFileName("IlcESDfriends_v1.root"),
	fIsEventSelectedForFriends(kFALSE)
{

	// constructor with name and title

}

//______________________________________________________________________________
IlcESDHandler::~IlcESDHandler() 
{
	// Destructor.
	delete fesdf;
	if(fFileEF){
		// is already handled in TerminateIO
		fFileEF->Close();
		delete fFileEF;
	}
	delete fTreeEF;
}

//______________________________________________________________________________
Bool_t IlcESDHandler::Init(Option_t* opt)
{
	//
	// Initialize IO
	//
	
	// File opening according to execution mode
	TString option(opt);
	option.ToLower();
	TDirectory *owd = gDirectory;

	fesdf = new IlcESDfriend();

	// Open the file with friends
	if (option.Contains("proof")) {
		// proof
		// Merging via files. Need to access analysis manager via interpreter.
		gROOT->ProcessLine(Form("IlcAnalysisManager::GetAnalysisManager()->OpenProofFile(\"%s\", \"RECREATE\");", fFileName.Data()));
		gROOT->ProcessLine(Form("IlcAnalysisManager::GetAnalysisManager()->GetCommonOutputContainer()->SetFile((TFile*)0x%p);", gFile));
		fFileEF = gFile;
	} else {
		// local and grid
		fFileEF = new TFile(fFileName.Data(), "RECREATE");
	}

	// Create the friends tree
	fFileEF->cd();
	fTreeEF = new TTree("esdFriendTree", "Tree with ESD friends");
      	fTreeEF->Branch("ESDfriend.","IlcESDfriend", &fesdf);

	owd->cd();
	
	return kTRUE;
}


//______________________________________________________________________________
Bool_t IlcESDHandler::FinishEvent()
{
	//
	// Fill the tree 
	//

	FillTree();
	
	// resetting
	if (fesdf) fesdf->~IlcESDfriend();
	new(fesdf) IlcESDfriend();  
	return kTRUE;
}

//______________________________________________________________________________
Bool_t IlcESDHandler::Terminate()
{
	//
	// Terminate 
	//

	return kTRUE;
}

//______________________________________________________________________________
Bool_t IlcESDHandler::TerminateIO()
{
	//
	// Terminate IO
	//

	if (fFileEF) {
		fFileEF->cd();
		fTreeEF->Write();
		fFileEF->Close();
		delete fFileEF;
		fFileEF = 0;
	}

	return kTRUE;
}

//______________________________________________________________________________
void IlcESDHandler::FillTree()
{
	//
	// Fill the ESD Tree
	//
	if (fIsEventSelectedForFriends){
		IlcDebug(2,Form("number of friend tracks = %d\n",fesdf->GetNumberOfTracks()));
	}
	else {
		fesdf->SetSkipBit(kTRUE);
	}
	IlcDebug(2,Form("friend = %p",fesdf));
	fFileEF->cd();
	fTreeEF->Fill();
}
