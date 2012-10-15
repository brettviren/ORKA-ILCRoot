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

/* $Id: IlcAnalysisTaskPIDResponse.cxx 43811 2010-09-23 14:13:31Z wiechula $ */
#include <TFile.h>
#include <TChain.h>

#include <IlcAnalysisManager.h>
#include <IlcInputEventHandler.h>
#include <IlcVEventHandler.h>
#include <IlcVEvent.h>
#include <IlcVParticle.h>
#include <IlcVTrack.h>
#include <IlcLog.h>
#include <IlcPIDResponse.h>

#include "IlcAnalysisTaskPIDResponse.h"

ClassImp(IlcAnalysisTaskPIDResponse)

//______________________________________________________________________________
IlcAnalysisTaskPIDResponse::IlcAnalysisTaskPIDResponse():
IlcAnalysisTaskSE(),
fIsMC(kFALSE),
fOADBPath(),
fPIDResponse(0x0),
fRun(0),
fOldRun(0),
fRecoPass(0)
{
  //
  // Dummy constructor
  //
}

//______________________________________________________________________________
IlcAnalysisTaskPIDResponse::IlcAnalysisTaskPIDResponse(const char* name):
IlcAnalysisTaskSE(name),
fIsMC(kFALSE),
fOADBPath(),
fPIDResponse(0x0),
fRun(0),
fOldRun(0),
fRecoPass(0)
{
  //
  // Default constructor
  //
  DefineInput(0,TChain::Class());
}

//______________________________________________________________________________
IlcAnalysisTaskPIDResponse::~IlcAnalysisTaskPIDResponse()
{
  //
  // Destructor
  //
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDResponse::UserCreateOutputObjects()
{
  //
  // Create the output QA objects
  //
  
  IlcLog::SetClassDebugLevel("IlcAnalysisTaskPIDResponse",10);
  
  //input hander
  IlcAnalysisManager *man=IlcAnalysisManager::GetAnalysisManager();
  IlcInputEventHandler *inputHandler=dynamic_cast<IlcInputEventHandler*>(man->GetInputEventHandler());
  if (!inputHandler) IlcFatal("Input handler needed");
  
  //pid response object
  inputHandler->CreatePIDResponse(fIsMC);
  fPIDResponse=inputHandler->GetPIDResponse();
  if (!fPIDResponse) IlcFatal("PIDResponse object was not created");

  fPIDResponse->SetOADBPath(IlcAnalysisManager::GetOADBPath());
  if (!fOADBPath.IsNull()) fPIDResponse->SetOADBPath(fOADBPath.Data());
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDResponse::UserExec(Option_t */*option*/)
{
  // Setup the PID response functions and fill the QA histograms
  //
  IlcVEvent *event=InputEvent();
  if (!event) return;
  fRun=event->GetRunNumber();
  
  if (fRun!=fOldRun){
    SetRecoInfo();
    fOldRun=fRun;
  }

  fPIDResponse->InitialiseEvent(event,fRecoPass);
}

//______________________________________________________________________________
void IlcAnalysisTaskPIDResponse::SetRecoInfo()
{
  //
  // Set reconstruction information
  //
  
  //reset information
  fRecoPass=0;
  
  //Get the current file to check the reconstruction pass (UGLY, but not stored in ESD... )
  IlcAnalysisManager *mgr=IlcAnalysisManager::GetAnalysisManager();
  IlcVEventHandler *inputHandler=mgr->GetInputEventHandler();
  if (!inputHandler) return;
  
  TTree *tree= (TTree*)inputHandler->GetTree();
  TFile *file= (TFile*)tree->GetCurrentFile();
  
  if (!file) {
    IlcError("Current file not found, cannot set reconstruction information");
    return;
  }
  
  //find pass from file name (UGLY, but not stored in ESD... )
  TString fileName(file->GetName());
  if (fileName.Contains("pass1") ) {
    fRecoPass=1;
  } else if (fileName.Contains("pass2") ) {
    fRecoPass=2;
  } else if (fileName.Contains("pass3") ) {
    fRecoPass=3;
  }

  fPIDResponse->SetCurrentFile(fileName.Data());
}
