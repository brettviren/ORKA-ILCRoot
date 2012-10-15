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

//////////////////////////////////////////////////////////////////////////
//
//  Base class for filtering friends
//
//////////////////////////////////////////////////////////////////////////
 
#include <TChain.h>
#include <TFile.h>
#include <TList.h>

#include "IlcAnalysisTaskFilter.h"
#include "IlcAnalysisManager.h"
#include "IlcAnalysisDataSlot.h"
#include "IlcESDEvent.h"
#include "IlcESD.h"
#include "IlcVEvent.h"
#include "IlcESDHandler.h"
#include "IlcInputEventHandler.h"
#include "IlcLog.h"
#include "IlcESDfriend.h"
#include "IlcESDfriendTrack.h"


ClassImp(IlcAnalysisTaskFilter)

////////////////////////////////////////////////////////////////////////

IlcAnalysisTaskFilter::IlcAnalysisTaskFilter():
	IlcAnalysisTask(),
	fDebug(0),
	fEntry(0),
	fInputEvent(0x0),
	fInputHandler(0x0),
	fOutputESDfriend(0x0),
	fTreeEF(0x0),
	fInputESDfriend(0x0)
{
	//
	// Default constructor
	//
}

//______________________________________________________________________

IlcAnalysisTaskFilter::IlcAnalysisTaskFilter(const char* name):
	IlcAnalysisTask(name, "AnalysisTaskFilter"),
	fDebug(0),
	fEntry(0),
	fInputEvent(0x0),
	fInputHandler(0x0),
	fOutputESDfriend(0x0),
	fTreeEF(0x0),
	fInputESDfriend(0x0)
{
	//
	// Default constructor
	//

	DefineInput (0, TChain::Class());
	DefineOutput(0,  TTree::Class());
}

//______________________________________________________________________

IlcAnalysisTaskFilter::IlcAnalysisTaskFilter(const IlcAnalysisTaskFilter& obj):
	IlcAnalysisTask(obj),
	fDebug(0),
	fEntry(0),
	fInputEvent(0x0),
	fInputHandler(0x0),
	fOutputESDfriend(0x0),
	fTreeEF(0x0),
	fInputESDfriend(0x0)
{
	//
	// Copy constructor
	//

	fDebug        = obj.fDebug;
	fEntry        = obj.fEntry;
	fInputEvent   = obj.fInputEvent;
	fInputHandler = obj.fInputHandler;
	fOutputESDfriend = obj.fOutputESDfriend;
	fTreeEF        = obj.fTreeEF;    
	fInputESDfriend = obj.fInputESDfriend;
}


//______________________________________________________________________

IlcAnalysisTaskFilter& IlcAnalysisTaskFilter::operator=(const IlcAnalysisTaskFilter& other)
{
	//
	// Assignment
	//

	if (&other != this) {
		IlcAnalysisTask::operator=(other);
		fDebug        = other.fDebug;
		fEntry        = other.fEntry;
		fInputEvent   = other.fInputEvent;
		fInputHandler = other.fInputHandler;
		fOutputESDfriend = other.fOutputESDfriend;
		fTreeEF        = other.fTreeEF;    
		fInputESDfriend = other.fInputESDfriend;
	}
	return *this;
}


//______________________________________________________________________

void IlcAnalysisTaskFilter::ConnectInputData(Option_t* /*option*/)
{
	//
	// Connect the input data
	//

	if (fDebug > 1) printf("AnalysisTaskFilter::ConnectInputData() \n");
	fInputHandler = (IlcInputEventHandler*) 
		((IlcAnalysisManager::GetAnalysisManager())->GetInputEventHandler());
	if (fInputHandler) {
		fInputEvent = fInputHandler->GetEvent();
		if (fInputEvent){
			fInputESDfriend = (IlcESDfriend*)(fInputEvent->FindListObject("IlcESDfriend"));
			if (!fInputESDfriend){
				IlcError("No friend found");
			}
		}
		else {
			IlcError("No Input Event found, the friend will remain empty");
		}
	} 
	else {
		IlcError("No Input Event Handler connected") ; 
		return ; 
	}
}

//______________________________________________________________________

void IlcAnalysisTaskFilter::CreateOutputObjects()
{
	//
	// Create the output container
	//

	if (fDebug > 1) printf("AnalysisTaskFilter::CreateOutPutData() \n");

	IlcESDHandler* handler = (IlcESDHandler*) ((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());
    
	if (handler) {
		fTreeEF = handler->GetTree();
	}
	else {
		IlcWarning("No AOD Event Handler connected.") ; 
	}

	UserCreateOutputObjects();
}

//______________________________________________________________________

void IlcAnalysisTaskFilter::Exec(Option_t* option)
{
	//
	// Exec analysis of one event
	//

	if (fDebug > 1) IlcInfo("IlcAnalysisTaskFilter::Exec() \n");

	if( fInputHandler ) {
		fEntry = fInputHandler->GetReadEntry();
	}
    
	   
	if ( !((Entry()-1)%100) && fDebug > 0) {
		IlcInfo(Form("%s ----> Processing event # %lld", CurrentFileName(), Entry()));
	}
	IlcESDHandler* handler = (IlcESDHandler*)((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());
	
	if (UserSelectESDfriendForCurrentEvent()){
		// Call the user analysis only if the event was selected   
		handler->SelectEventForFriends();
		fOutputESDfriend   = handler->GetESDfriend();
		UserExec(option);
		// copy the VZERO friend only if it is not already there
		if (fOutputESDfriend->GetVZEROfriend() == 0x0){
			IlcDebug(2,"Copying VZERO friend object");
			IlcESDVZEROfriend* vZEROfriend = fInputESDfriend->GetVZEROfriend();
			fOutputESDfriend->SetVZEROfriend(vZEROfriend);
		}
	}
	else {
		// Event not selected
		IlcDebug(2,"The event was not selected");
	}

	IlcAnalysisDataSlot *out0 = GetOutputSlot(0);
	if (out0 && out0->IsConnected()) PostData(0, fTreeEF);    
}

//______________________________________________________________________

const char* IlcAnalysisTaskFilter::CurrentFileName()
{
	// Returns the current file name    
	if( fInputHandler ){
		return fInputHandler->GetTree()->GetCurrentFile()->GetName();
	}
	else return "";
}

//______________________________________________________________________

void IlcAnalysisTaskFilter::AddFriendTrackAt(IlcESDfriendTrack* t, Int_t index)
{
	//
	// Adds the friend track at the i-th position in the TClonesArray
	// of the ESD friend tracks
	//

	IlcESDfriendTrack* currentTrack = (IlcESDfriendTrack*)fOutputESDfriend->GetTrack(index);
	if(currentTrack){
		if (currentTrack->TestSkipBit()){
			IlcDebug(2,Form("Friend at index %d already there but dummy - the skip bit will be set to FALSE", index));
			t->SetSkipBit(kFALSE);
		}
		else{
			IlcDebug(2,Form("Friend at index %d already there and not dummy", index));
			return;
		}
	}
	else{
		IlcDebug(2,Form("Track at %d not there yet ",index));
	}
	IlcDebug(2,Form("Adding track at %d",index));
	fOutputESDfriend->AddTrackAt(t,index);
	return;
}

//______________________________________________________________________

void IlcAnalysisTaskFilter::SkipFriendTrackAt(Int_t index)
{
	//
	// Skip the friend track at the i-th position in the TClonesArray
	// of the ESD friend tracks
	//

	IlcESDfriendTrack* currentTrack = (IlcESDfriendTrack*)fOutputESDfriend->GetTrack(index);
	if (currentTrack){
		IlcDebug(2,Form("Track already there (no matter what validity) at %d, keeping it as it is", index));
	}
	else {
		IlcDebug(2,Form("Adding NULL track at %d, and setting skip bit to TRUE",index));
		IlcESDfriendTrack* tNull = new IlcESDfriendTrack();
		tNull->SetSkipBit(kTRUE);
		fOutputESDfriend->AddTrackAt(tNull,index);
	}
	return;
}
