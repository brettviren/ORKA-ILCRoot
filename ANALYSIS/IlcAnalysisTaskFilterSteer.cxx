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

/*$Id$

*/

/////////////////////////////////////////////////////////////
//
//   Filtering task:  
//   Selection of only 1% of the events for which to keep  
//   the ESD friend
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
#include "IlcAnalysisTaskFilterSteer.h"
#include "TRandom.h"


ClassImp(IlcAnalysisTaskFilterSteer)


//________________________________________________________________________
IlcAnalysisTaskFilterSteer::IlcAnalysisTaskFilterSteer():
IlcAnalysisTaskFilter(),
fFraction(0.01),
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
IlcAnalysisTaskFilterSteer::IlcAnalysisTaskFilterSteer(const char* name):
IlcAnalysisTaskFilter(name),
fFraction(0.01),
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
IlcAnalysisTaskFilterSteer::~IlcAnalysisTaskFilterSteer()
{

	// dtor

}  

//________________________________________________________________________
void IlcAnalysisTaskFilterSteer::Init()
{
	// Initialization
	
	return;
}

//________________________________________________________________________
void IlcAnalysisTaskFilterSteer::UserCreateOutputObjects()
{
	//
	// Create the output container
	//
	
	return;
}

//________________________________________________________________________
void IlcAnalysisTaskFilterSteer::UserExec(Option_t */*option*/)
{

	// 
	// Applying random selection of the events

	fESDInput = dynamic_cast<IlcESDEvent*>(InputEvent()); // get the input ESD
	fESDfriendInput = InputFriend();  // get the input friend
	if(!fESDInput) {
		printf("IlcAnalysisTaskFilterSteer::Exec(): no ESD \n");
		return;
	} 
	if(!fESDfriendInput) {
		printf("IlcAnalysisTaskFilterSteer::Exec(): no ESDfriend \n");
		return;
	} 

	// attach ESDfriend
	
	IlcESDfriend* esdFriendOutput = (IlcESDfriend*)ESDfriend();  
	IlcDebug(3,Form("Number of ESD tracks in input = %d ",fESDInput->GetNumberOfTracks()));
	IlcDebug(3,Form("Number of tracks in input friends = %d ",fESDfriendInput->GetNumberOfTracks()));
	IlcDebug(3,Form("Number of tracks in output friendsNew before filtering = %d ",esdFriendOutput->GetNumberOfTracks()));
	
	//
	//  keeping all the tracks for the randomly "fFraction" of the total number of events
	//

	for (Int_t i = 0; i< fESDInput->GetNumberOfTracks(); i++){
		IlcESDfriendTrack* tOld = (IlcESDfriendTrack*)fESDfriendInput->GetTrack(i);
		AddFriendTrackAt(tOld,i);
	}			 
	IlcDebug(2,Form("Number of tracks in output friendsNew after filtering = %d ",esdFriendOutput->GetNumberOfTracks()));
	IlcDebug(2,Form("Number of tracks in output friendsNew after filtering with GetEntries() = %d ",esdFriendOutput->GetEntriesInTracks()));

	return;
}

//________________________________________________________________________
void IlcAnalysisTaskFilterSteer::Terminate(Option_t */*option*/)
{
	// Terminate analysis
	//
	IlcDebug(2,"IlcAnalysisTaskFilterSteer: Terminate() \n");
	
	return;
}
//________________________________________________________________________
Bool_t IlcAnalysisTaskFilterSteer::UserSelectESDfriendForCurrentEvent()
{
	// 
	// Selecting or discarding current event
	//
	Double_t number = gRandom->Rndm();
	if (number<fFraction){
		// keeping event
		IlcDebug(2,Form("*****************Selecting event (number = %f)",number));
		return kTRUE;	
	}
	IlcDebug(2,Form("*****************Skipping event (number = %f)",number));
	return kFALSE;
}
