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

/////////////////////////////////////////////////////////////////////////
//
// Task to Copy ESDs
//
////////////////////////////////////////////////////////////////////////


#include <TTree.h>

#include "IlcAnalysisTaskFilter.h"
#include "IlcAnalysisTaskCopyESD.h"
#include "IlcESDEvent.h"
#include "IlcLog.h"
#include "IlcESDfriend.h"

ClassImp(IlcAnalysisTaskCopyESD)

////////////////////////////////////////////////////////////////////////

IlcAnalysisTaskCopyESD::IlcAnalysisTaskCopyESD():
	IlcAnalysisTaskFilter(),
	fESDEvent(0x0),
	fESDfriend(0x0)
{
	// Default constructor
	//DefineInput(0, TChain::Class());
	// Output slot #0 writes into a TTree
	//DefineOutput(0,TTree::Class());  //My private output
}
//-------------------------------------------------------------------------------
IlcAnalysisTaskCopyESD::IlcAnalysisTaskCopyESD(const char* name):
	IlcAnalysisTaskFilter(name),
	fESDEvent(0x0),
	fESDfriend(0x0)
{
	// Constructor
}

//-------------------------------------------------------------------------------
void IlcAnalysisTaskCopyESD::UserCreateOutputObjects()
{
	// Create the output container
	IlcInfo("In UserCreateOuptputObject");
}

//-------------------------------------------------------------------------------
void IlcAnalysisTaskCopyESD::Init()
{
	// Initialization
	if (fDebug > 1) IlcInfo("Init() \n");
}


//-------------------------------------------------------------------------------
void IlcAnalysisTaskCopyESD::UserExec(Option_t */*option*/)
{
	// Execute analysis for current event
	//
	
	IlcInfo("Copying event");	
	IlcESDEvent* esd = dynamic_cast<IlcESDEvent*>(InputEvent());
	fESDEvent = ESDEvent(); // get the output ESD
	fESDfriend = ESDfriend();  // get the output friend
	esd->Copy(*fESDEvent);

	// releasing tracks to avoid copying them 
	//	for (Int_t i = 0; i<fESDEvent->GetNumberOfTracks(); i++){
	//	fESDEvent->GetTrack(i)->ReleaseESDfriendTrack();
	//}
}


//-------------------------------------------------------------------------------
void IlcAnalysisTaskCopyESD::Terminate(Option_t */*option*/)
{
	// Terminate analysis
	//
	if (fDebug > 1) printf("AnalysisCopyESD: Terminate() \n");
}

