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

/* $Id: IlcPIDResponseInputHandler 46193 2010-12-21 09:00:14Z wiechula $ */

//-----------------------------------------------------------------
//        Handler to set up the PID response object and
//         initialise it correctly for each event
//
//      Origin:
//        Jens Wiechula (jens.wiechula@cern.ch)
//        Martin Vala (martin.vala@cern.ch)

//-----------------------------------------------------------------


#include <TFile.h>
#include <TPRegexp.h>

#include <IlcLog.h>
#include <IlcVEvent.h>
#include "IlcAnalysisManager.h"
#include "IlcMultiInputEventHandler.h"
#include "IlcPIDResponse.h"

#include "IlcPIDResponseInputHandler.h"


ClassImp(IlcPIDResponseInputHandler)

//_____________________________________________________________________________
IlcPIDResponseInputHandler::IlcPIDResponseInputHandler(const char *name) :
  IlcInputEventHandler(name, name),
  fIsMC(kFALSE),
  fPIDResponse(0x0),
  fRun(0),
  fOldRun(0),
  fRecoPass(0),
  fMCurrentMutliIH(0)
{
//
// Default constructor.
//
   IlcDebug(IlcLog::kDebug + 10, "<-");
   IlcDebug(IlcLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
IlcPIDResponseInputHandler::~IlcPIDResponseInputHandler()
{
//
// Destructor
//
   IlcDebug(IlcLog::kDebug + 10, "<-");
// 	delete fArrPidResponseMaster;
   IlcDebug(IlcLog::kDebug + 10, "->");
}

//_____________________________________________________________________________
Bool_t IlcPIDResponseInputHandler::Init(Option_t *opt)
{
//
// Init() is called for all mix input handlers.
//
   IlcDebug(IlcLog::kDebug + 5, Form("<- opt=%s", opt));

   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcPIDResponseInputHandler::Init(TTree *tree, Option_t *opt)
{
//
// Init(const char*path) is called for all mix input handlers.
// Create event pool if needed
//
   IlcDebug(IlcLog::kDebug + 5, Form("<- %p %s opt=%s", (void *) tree, tree->GetName(), opt));

   if (fParentHandler) {
      TString tmp = "";
      IlcInputEventHandler *ih = 0;
      fMCurrentMutliIH = dynamic_cast<IlcMultiInputEventHandler*>(fParentHandler);
      if (fMCurrentMutliIH) {
         ih = fMCurrentMutliIH->GetFirstInputEventHandler();
         if (ih) {
            //pid response object
            ih->CreatePIDResponse(fIsMC);
            fPIDResponse = ih->GetPIDResponse();
            if (!fPIDResponse) IlcFatal("PIDResponse object was not created");
         }
      }
   }

   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcPIDResponseInputHandler::Notify()
{
//
// Notify() is called for all mix input handlers
//
   IlcDebug(IlcLog::kDebug + 5, Form("<-"));
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcPIDResponseInputHandler::Notify(const char *path)
{
//
// Notify(const char*path) is called for all mix input handlers
//
   IlcDebug(IlcLog::kDebug + 5, Form("<- %s", path));
   IlcDebug(IlcLog::kDebug + 5, "->");
   return kTRUE;
}
//_____________________________________________________________________________
Bool_t IlcPIDResponseInputHandler::BeginEvent(Long64_t entry)
{
//
// BeginEvent(Long64_t entry) is called for all mix input handlers
//
   IlcDebug(IlcLog::kDebug + 5, Form("<- %lld", entry));

   if (fParentHandler) {
      TString tmp = "";
      IlcInputEventHandler *ih = 0;
      fMCurrentMutliIH = dynamic_cast<IlcMultiInputEventHandler*>(fParentHandler);
      if (fMCurrentMutliIH) {
         ih = fMCurrentMutliIH->GetFirstInputEventHandler();
         if (ih) {
            //pid response object
            ih->CreatePIDResponse(fIsMC);
            fPIDResponse = ih->GetPIDResponse();
            if (!fPIDResponse) IlcFatal("PIDResponse object was not created");

            IlcVEvent *event = ih->GetEvent();
            if (!event) return kFALSE;
            fRun = event->GetRunNumber();

            if (fRun != fOldRun) {
              SetRecoInfo();
              fOldRun = fRun;
            }
           fPIDResponse->SetOADBPath(IlcAnalysisManager::GetOADBPath());
           fPIDResponse->InitialiseEvent(event,fRecoPass);
         }
      }
   }
   IlcDebug(IlcLog::kDebug + 5, "->");
   return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcPIDResponseInputHandler::GetEntry()
{
   IlcDebug(IlcLog::kDebug + 5, "<-");
   IlcDebug(IlcLog::kDebug + 5, "->");
   return kTRUE;
}

//_____________________________________________________________________________
Bool_t IlcPIDResponseInputHandler::FinishEvent()
{
   //
   // FinishEvent() is called for all mix input handlers
   //
   IlcDebug(IlcLog::kDebug + 5, Form("<-"));
   IlcDebug(IlcLog::kDebug + 5, Form("->"));
   return kTRUE;
}

//_____________________________________________________________________________
void IlcPIDResponseInputHandler::SetRecoInfo()
{
  //
  // Set reconstruction information
  //
  
  //reset information
  fRecoPass=0;
  
  //Get the current file to check the reconstruction pass (UGLY, but not stored in ESD... )
//   IlcAnalysisManager *mgr=IlcAnalysisManager::GetAnalysisManager();
  IlcVEventHandler *inputHandler=fMCurrentMutliIH->GetFirstInputEventHandler();
  if (!inputHandler) return;
  
  TTree *tree= (TTree*)inputHandler->GetTree();
  TFile *file= (TFile*)tree->GetCurrentFile();
  
  if (!file) {
    IlcError("Current file not found, cannot set reconstruction information");
    return;
  }
  
  //find pass from file name (UGLY, but not stored in ESD... )
  TString fileName(file->GetName());
  if (fileName.Contains("/pass1") || fileName.Contains(".pass1")) {
    fRecoPass=1;
  } else if (fileName.Contains("/pass2") || fileName.Contains(".pass2")) {
    fRecoPass=2;
  } else if (fileName.Contains("/pass3") || fileName.Contains(".pass3")) {
    fRecoPass=3;
  }

  fPIDResponse->SetCurrentFile(fileName.Data());
}
