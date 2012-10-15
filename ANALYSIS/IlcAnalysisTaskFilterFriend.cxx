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
//   Test task
//
// /////////////////////////////////////////////////////////////

#include <TTree.h>
#include <TChain.h>

#include "IlcLog.h"
#include "IlcESDInputHandler.h"
#include "IlcESDtrack.h"
#include "IlcESDEvent.h"
#include "IlcESDfriend.h"
#include "IlcAnalysisTaskFilter.h"
#include "IlcAnalysisManager.h"
#include "IlcAnalysisTaskFilterFriend.h"


ClassImp(IlcAnalysisTaskFilterFriend)


//________________________________________________________________________
IlcAnalysisTaskFilterFriend::IlcAnalysisTaskFilterFriend():
IlcAnalysisTaskFilter(),
fESDInput(0),
fESDfriendInput(0)
{
	// Constructor
	
	// Define input and output slots here
	// Input slot #0 works with a TChain
	DefineInput(0, TChain::Class());
	// Output slot #0 writes into a TTree
	//DefineOutput(0,TTree::Class());  
}

//________________________________________________________________________
IlcAnalysisTaskFilterFriend::IlcAnalysisTaskFilterFriend(const char* name):
IlcAnalysisTaskFilter(name),
fESDInput(0),
fESDfriendInput(0)
{
	// Constructor
	
	// Define input and output slots here
	// Input slot #0 works with a TChain
	DefineInput(0, TChain::Class());
	// Output slot #0 writes into a TTree
	//DefineOutput(0,TTree::Class());  
}

//________________________________________________________________________
IlcAnalysisTaskFilterFriend::~IlcAnalysisTaskFilterFriend()
{

	// dtor

}  

//________________________________________________________________________
void IlcAnalysisTaskFilterFriend::Init()
{
	// Initialization
	
	return;
}

//________________________________________________________________________
void IlcAnalysisTaskFilterFriend::UserCreateOutputObjects()
{
	//
	// Create the output container
	//
	
	return;
}

//________________________________________________________________________
void IlcAnalysisTaskFilterFriend::UserExec(Option_t */*option*/)
{

	//
	// Filtering
	//

	fESDInput = dynamic_cast<IlcESDEvent*>(InputEvent()); // get the input ESD
	fESDfriendInput = InputFriend(); // get the input friend
	if(!fESDInput) {
		printf("IlcAnalysisTaskFilterFriend::Exec(): no ESD \n");
		return;
	} 
	if(!fESDfriendInput) {
		printf("IlcAnalysisTaskFilterFriend::Exec(): no ESDfriend \n");
		return;
	} 
	// attach ESDfriend
	
	IlcESDfriend* esdFriendOutput = (IlcESDfriend*)ESDfriend();  
	IlcDebug(3,Form("Number of ESD tracks in input = %d ",fESDInput->GetNumberOfTracks()));
	IlcDebug(3,Form("Number of tracks in input friends = %d ",fESDfriendInput->GetNumberOfTracks()));
	IlcDebug(3,Form("Number of tracks in output friendsNew before filtering = %d ",esdFriendOutput->GetNumberOfTracks()));
	
	for (Int_t i = 0; i< fESDInput->GetNumberOfTracks(); i++){
		if (i%2 ==0){
			// keep friend
			IlcDebug(2,Form("Keeping %d-th track",i));
			IlcESDfriendTrack* tOld = (IlcESDfriendTrack*)fESDfriendInput->GetTrack(i);
			IlcDebug(3,Form("1P of the %d-th track = %f",i,tOld->Get1P()));
			IlcDebug(3,Form("MaxITScluster %d-th track = %d",i,tOld->GetMaxITScluster()));
			//	tOld->Dump();
			AddFriendTrackAt(tOld,i);
		
		}
		else {
			//discard friend 
			SkipFriendTrackAt(i);
		}
		
	} 
	IlcDebug(2,Form("Number of tracks in output friendsNew after filtering with GetEntries() = %d ",esdFriendOutput->GetEntriesInTracks()));
	return;
}

//________________________________________________________________________
void IlcAnalysisTaskFilterFriend::Terminate(Option_t */*option*/)
{
	// Terminate analysis
	//
	IlcDebug(2,"IlcAnalysisTaskFilterFriend: Terminate() \n");
	
	return;
}
//________________________________________________________________________
Bool_t IlcAnalysisTaskFilterFriend::UserSelectESDfriendForCurrentEvent()
{
	// 
	// Selecting or discarding current event
	//

	
	fESDInput = dynamic_cast<IlcESDEvent*>(InputEvent()); // get the input ESD
	if (!fESDInput){
		IlcError("No ESD Input, discarding event...");
		return kFALSE;
	}
	if ((fESDInput->GetNumberOfTracks())%2 == 0) {
		IlcDebug(2,"******************Selecting event");
		return kTRUE;
	}
	IlcDebug(2,"*******************Discarding event");	
	return kFALSE;
}
