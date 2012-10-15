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

/////////////////////////////////////////////////////////////
//
//   Test task to add an object to the new IlcESDfriends file
//
// /////////////////////////////////////////////////////////////

#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>

#include "IlcLog.h"
#include "IlcESDInputHandler.h"
#include "IlcESDtrack.h"
#include "IlcESDEvent.h"
#include "IlcESDfriend.h"
#include "IlcAnalysisTask.h"
#include "IlcAnalysisManager.h"
#include "IlcAnalysisTaskAddObject.h"


ClassImp(IlcAnalysisTaskAddObject)


//________________________________________________________________________
IlcAnalysisTaskAddObject::IlcAnalysisTaskAddObject():
IlcAnalysisTask(),
fESDInput(0x0),
fESDfriendInput(0x0),
fESDhandler(0x0),
fh(0x0)
{
	// Dummy Constructor
	
}

//________________________________________________________________________
IlcAnalysisTaskAddObject::IlcAnalysisTaskAddObject(const char* name):
IlcAnalysisTask(name,"Adding an object"),
fESDInput(0),
fESDfriendInput(0),
fESDhandler(0x0),
fh(0x0)
{
	// Constructor
	
	// Define input and output slots here
	// Input slot #0 works with a TChain
	DefineInput(0, TChain::Class());
	// Output slot #0 writes into a TTree
	//	DefineOutput(0,TTree::Class());  
	// Output slot #1 writes into a TH1D
	DefineOutput(0,TH1D::Class());  
}

//________________________________________________________________________
IlcAnalysisTaskAddObject::~IlcAnalysisTaskAddObject()
{

	// dtor
	if (fh){
		delete fh;
		fh = 0x0;
	}
}  

//______________________________________________________________________________
void IlcAnalysisTaskAddObject::ConnectInputData(Option_t* /*option*/)
{
	//
	// Connect the input data
	//

	printf("IlcAnalysisTaskAddObject::ConnectInputData()\n");
	IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
	if (!mgr) IlcFatal("No analysis manager available");
	fESDhandler = dynamic_cast<IlcESDInputHandler *>(mgr->GetInputEventHandler());
    
	if (fESDhandler) {
		fESDInput = fESDhandler->GetEvent();
	} else {
		IlcFatal("No ESD input event handler connected") ; 
	}
}
//________________________________________________________________________
void IlcAnalysisTaskAddObject::CreateOutputObjects()
{
	//
	// Create the output container
	//
	//OpenFile(0,"UPDATE");
	fh = new TH1D("fh1","Integrated Length",100,0,1000);
	return;
}

//________________________________________________________________________
void IlcAnalysisTaskAddObject::Exec(Option_t */*option*/)
{

	//	if (fDebug > 1) {
	Long64_t entry = fESDhandler->GetReadEntry();
	IlcDebug(2,Form("IlcAnalysisTaskAddObject::Exec() %s ==> processing event %lld", fESDhandler->GetTree()->GetCurrentFile()->GetName(),entry));
	//}  
	fESDInput = fESDhandler->GetEvent();
	if(!fESDInput) {
		printf("IlcAnalysisTaskAddObject::Exec(): no ESD \n");
		return;
	} 
	for (Int_t i = 0; i< fESDInput->GetNumberOfTracks(); i++){
		IlcESDtrack* t = fESDInput->GetTrack(i);
		Double_t l = t->GetIntegratedLength();
		fh->Fill(l);
	}
	PostData(0,fh);
	return;
}

//________________________________________________________________________
void IlcAnalysisTaskAddObject::Terminate(Option_t */*option*/)
{
	// Terminate analysis
	//
	IlcDebug(2,"IlcAnalysisTaskAddObject: Terminate() \n");
	
	return;
}
