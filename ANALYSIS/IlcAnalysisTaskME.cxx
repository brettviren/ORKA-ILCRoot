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
 
#include <TROOT.h>
#include <TSystem.h>
#include <TInterpreter.h>
#include <TChain.h>
#include <TFile.h>
#include <TList.h>

#include "IlcAnalysisTaskME.h"
#include "IlcAnalysisManager.h"
#include "IlcAnalysisDataSlot.h"
#include "IlcAODEvent.h"
#include "IlcVEvent.h"
#include "IlcAODHandler.h"
#include "IlcMultiEventInputHandler.h"
#include "IlcLog.h"


ClassImp(IlcAnalysisTaskME)

////////////////////////////////////////////////////////////////////////

IlcAnalysisTaskME::IlcAnalysisTaskME():
    IlcAnalysisTask(),
    fDebug(0),
    fEntry(0),
    fFreshBufferOnly(kFALSE),
    fInputHandler(0x0),
    fOutputAOD(0x0),
    fTreeA(0x0),
    fOfflineTriggerMask(0)
{
  // Default constructor
}

IlcAnalysisTaskME::IlcAnalysisTaskME(const char* name):
    IlcAnalysisTask(name, "AnalysisTaskME"),
    fDebug(0),
    fEntry(0),
    fFreshBufferOnly(kFALSE),
    fInputHandler(0x0),
    fOutputAOD(0x0),
    fTreeA(0x0),
    fOfflineTriggerMask(0)
{
  // Default constructor
    DefineInput (0, TChain::Class());
    DefineOutput(0,  TTree::Class());
}

IlcAnalysisTaskME::IlcAnalysisTaskME(const IlcAnalysisTaskME& obj):
    IlcAnalysisTask(obj),
    fDebug(0),
    fEntry(0),
    fFreshBufferOnly(kFALSE),
    fInputHandler(0x0),
    fOutputAOD(0x0),
    fTreeA(0x0),
    fOfflineTriggerMask(0)
{
// Copy constructor
    fDebug        = obj.fDebug;
    fEntry        = obj.fEntry;
    fInputHandler = obj.fInputHandler;
    fOutputAOD    = obj.fOutputAOD;
    fTreeA        = obj.fTreeA; 
    fOfflineTriggerMask = obj.fOfflineTriggerMask;
}


IlcAnalysisTaskME& IlcAnalysisTaskME::operator=(const IlcAnalysisTaskME& other)
{
// Assignment
    if (this != &other) {
	IlcAnalysisTask::operator=(other);
	fDebug           = other.fDebug;
	fEntry           = other.fEntry;
	fFreshBufferOnly = other.fFreshBufferOnly;
	fInputHandler    = other.fInputHandler;
	fOutputAOD       = other.fOutputAOD;
	fTreeA           = other.fTreeA;    
	fOfflineTriggerMask = other.fOfflineTriggerMask;
    }
    return *this;
}


void IlcAnalysisTaskME::ConnectInputData(Option_t* /*option*/)
{
// Connect the input data
    if (fDebug > 1) printf("AnalysisTaskME::ConnectInputData() \n");
//
//  Multi AOD
//
    fInputHandler = dynamic_cast<IlcMultiEventInputHandler*> 
	((IlcAnalysisManager::GetAnalysisManager())->GetInputEventHandler());
    if (fInputHandler == 0) {
	IlcFatal("Event Handler has to be MultiEventInputHandler !");
    } else {
	// Check that we have an event pool
	if (!fInputHandler->GetEventPool()) {
	    fInputHandler->SetEventPool(IlcAnalysisManager::GetAnalysisManager()->GetEventPool());
	    if (!fInputHandler->GetEventPool()) 
		IlcFatal("MultiEventInputHandler has no EventPool connected !");
	}
    }
}

void IlcAnalysisTaskME::CreateOutputObjects()
{
// Create the output container
//
//  Default AOD
    if (fDebug > 1) printf("AnalysisTaskME::CreateOutPutData() \n");

    IlcAODHandler* handler = (IlcAODHandler*) 
	((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());
    
    if (handler) {
	fOutputAOD   = handler->GetAOD();
	fTreeA = handler->GetTree();
    } else {
	IlcWarning("No AOD Event Handler connected.") ; 
    }
    UserCreateOutputObjects();
}

void IlcAnalysisTaskME::Exec(Option_t* option)
{
//
// Exec analysis of one event

    if (fDebug > 1) IlcInfo("IlcAnalysisTaskME::Exec() \n");
    if( fInputHandler ) 
       fEntry = fInputHandler->GetReadEntry();
    if ( !((Entry()-1)%100) && fDebug > 0) 
         IlcInfo(Form("%s ----> Processing event # %lld", CurrentFileName(), Entry()));

    IlcAODHandler* outputHandler = (IlcAODHandler*) 
	((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());         
//
// Was event selected ? If no event selection mechanism, the event SHOULD be selected (AG)
    UInt_t isSelected = IlcVEvent::kAny;
    if(fInputHandler && fInputHandler->GetEventSelection()) {
      // Get the actual offline trigger mask for the event and AND it with the
      // requested mask. If no mask requested select by default the event.
      if (fOfflineTriggerMask)
	isSelected = fOfflineTriggerMask & fInputHandler->IsEventSelected();
    }
    
    if (!isSelected) { 
	if (fDebug > 1) IlcInfo("Event rejected \n");
	fInputHandler->EventSkipped();
	return;
    }
// Call the user analysis    
    
    if (fInputHandler && fInputHandler->IsBufferReady()) {
	if ((fFreshBufferOnly && fInputHandler->IsFreshBuffer()) || !fFreshBufferOnly)
	{
	    if (outputHandler) outputHandler->SetFillAOD(kTRUE);
	    UserExec(option);
	    // Added protection in case the derived task is not an AOD producer.
	    IlcAnalysisDataSlot *out0 = GetOutputSlot(0);
	    if (out0 && out0->IsConnected()) PostData(0, fTreeA);
	} else {
	    if (outputHandler) outputHandler->SetFillAOD(kFALSE);
	}
    } else {
	IlcInfo(Form("Waiting for buffer to be ready !\n"));
    }
}

const char* IlcAnalysisTaskME::CurrentFileName()
{
// Returns the current file name    
    if(fInputHandler )
	return fInputHandler->GetTree()->GetCurrentFile()->GetName();
    else return "";
}

void IlcAnalysisTaskME::AddAODBranch(const char* cname, void* addobj, const char *fname)
{
    // Add a new branch to the aod tree
    IlcAODHandler* handler = (IlcAODHandler*) 
	((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());
    if (handler) {
	handler->AddBranch(cname, addobj, fname);
    }
}

IlcVEvent*  IlcAnalysisTaskME::GetEvent(Int_t iev)
{
    // Get an event from the input handler
    return (fInputHandler->GetEvent(iev));
}

