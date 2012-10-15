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

/* $Id: IlcMCEventHandler.cxx 54863 2012-02-28 12:10:29Z hristov $ */
//---------------------------------------------------------------------------------
//                          Class IlcMCEventHandler
// This class gives access to MC truth during the analysis.
// Monte Carlo truth is containe in the kinematics tree (produced particles) and 
// the tree of reference hits.
//      
// Origin: Andreas Morsch, CERN, andreas.morsch@cern.ch 
//---------------------------------------------------------------------------------



#include "IlcMCEventHandler.h"
#include "IlcMCEvent.h"
#include "IlcMCParticle.h"
#include "IlcPDG.h"
#include "IlcTrackReference.h"
#include "IlcHeader.h"
#include "IlcStack.h"
#include "IlcLog.h"

#include <TTree.h>
#include <TFile.h>
#include <TList.h>
#include <TParticle.h>
#include <TString.h>
#include <TClonesArray.h>
#include <TDirectoryFile.h>

ClassImp(IlcMCEventHandler)

IlcMCEventHandler::IlcMCEventHandler() :
    IlcInputEventHandler(),
    fMCEvent(new IlcMCEvent()),
    fFileE(0),
    fFileK(0),
    fFileTR(0),
    fTreeE(0),
    fTreeK(0),
    fTreeTR(0),
    fDirK(0),
    fDirTR(0),
    fParticleSelected(0),
    fLabelMap(0),
    fNEvent(-1),
    fEvent(-1),
    fPathName(new TString("./")),
    fkExtension(""),
    fFileNumber(0),
    fEventsPerFile(0),
    fReadTR(kTRUE),
    fInitOk(kFALSE),
    fSubsidiaryHandlers(0),
    fEventsInContainer(0),
    fPreReadMode(kNoPreRead)
{
  //
  // Default constructor
  //
  // Be sure to add all particles to the PDG database
  IlcPDG::AddParticlesToPdgDataBase();
}

IlcMCEventHandler::IlcMCEventHandler(const char* name, const char* title) :
    IlcInputEventHandler(name, title),
    fMCEvent(new IlcMCEvent()),
    fFileE(0),
    fFileK(0),
    fFileTR(0),
    fTreeE(0),
    fTreeK(0),
    fTreeTR(0),
    fDirK(0),
    fDirTR(0),
    fParticleSelected(0),
    fLabelMap(0),
    fNEvent(-1),
    fEvent(-1),
    fPathName(new TString("./")),
    fkExtension(""),
    fFileNumber(0),
    fEventsPerFile(0),
    fReadTR(kTRUE),
    fInitOk(kFALSE),
    fSubsidiaryHandlers(0),
    fEventsInContainer(0),
    fPreReadMode(kNoPreRead)
{
  //
  // Constructor
  //
  // Be sure to add all particles to the PDG database
  IlcPDG::AddParticlesToPdgDataBase();
}
IlcMCEventHandler::~IlcMCEventHandler()
{ 
    // Destructor
  delete fPathName;
    delete fMCEvent;
    delete fFileE;
    delete fFileK;
    delete fFileTR;
}

Bool_t IlcMCEventHandler::Init(Option_t* opt)
{ 
    // Initialize input
    //
    if (!(strcmp(opt, "proof")) || !(strcmp(opt, "local"))) return kTRUE;
    //
    fFileE = TFile::Open(Form("%sgilc.root", fPathName->Data()));
    if (!fFileE) {
	IlcError(Form("IlcMCEventHandler:gilc.root not found in directory %s ! \n", fPathName->Data()));
	fInitOk = kFALSE;
	return kFALSE;
    }
    
    //
    // Tree E
    fFileE->GetObject("TE", fTreeE);
    // Connect Tree E to the MCEvent
    fMCEvent->ConnectTreeE(fTreeE);
    fNEvent = fTreeE->GetEntries();
    //
    // Tree K
    fFileK = TFile::Open(Form("%sKinematics%s.root", fPathName->Data(), fkExtension));
    if (!fFileK) {
	IlcError(Form("IlcMCEventHandler:Kinematics.root not found in directory %s ! \n", fPathName->Data()));
	fInitOk = kFALSE;
	return kTRUE;
    }
    
    fEventsPerFile = fFileK->GetNkeys() - fFileK->GetNProcessIDs();
    //
    // Tree TR
    if (fReadTR) {
	fFileTR = TFile::Open(Form("%sTrackRefs%s.root", fPathName->Data(), fkExtension));
	if (!fFileTR) {
	    IlcError(Form("IlcMCEventHandler:TrackRefs.root not found in directory %s ! \n", fPathName->Data()));
	    fInitOk = kFALSE;
	    return kTRUE;
	}
    }
    //
    // Reset the event number
    fEvent      = -1;
    fFileNumber =  0;
    IlcInfo(Form("Number of events in this directory %5d \n", fNEvent));
    fInitOk = kTRUE;


    if (fSubsidiaryHandlers) {
	TIter next(fSubsidiaryHandlers);
	IlcMCEventHandler *handler;
	while((handler = (IlcMCEventHandler*)next())) {
	    handler->Init(opt);
	    handler->SetNumberOfEventsInContainer(fNEvent);
	}
    }

    return kTRUE;
}

Bool_t IlcMCEventHandler::LoadEvent(Int_t iev)
{
    // Load the event number iev
    //
    // Calculate the file number
  if (!fInitOk) return kFALSE;
    
  Int_t inew  = iev / fEventsPerFile;
  if (inew != fFileNumber) {
    fFileNumber = inew;
    if (!OpenFile(fFileNumber)){
      return kFALSE;
    }
  }
  // Folder name
  char folder[20];
  snprintf(folder, 20, "Event%d", iev);
  // TreeE
  fTreeE->GetEntry(iev);
  // Tree K
  fFileK->GetObject(folder, fDirK);
  if (!fDirK) {
    IlcWarning(Form("IlcMCEventHandler: Event #%5d - Cannot get kinematics\n", iev));
    return kFALSE;
  }
    
  fDirK ->GetObject("TreeK", fTreeK);
  if (!fTreeK) {
    IlcError(Form("IlcMCEventHandler: Event #%5d - Cannot get TreeK\n",iev));
    return kFALSE;
  }  
  // Connect TreeK to MCEvent
  fMCEvent->ConnectTreeK(fTreeK);
  //Tree TR 
  if (fFileTR) {
    // Check which format has been read
    fFileTR->GetObject(folder, fDirTR);
    if (!fDirTR) {
      IlcError(Form("IlcMCEventHandler: Event #%5d - Cannot get track references\n",iev));
      return kFALSE;
    }  
     
    fDirTR->GetObject("TreeTR", fTreeTR);
    //
    if (!fTreeTR) {
      IlcError(Form("IlcMCEventHandler: Event #%5d - Cannot get TreeTR\n",iev));
      return kFALSE;
    }  
    // Connect TR to MCEvent
    fMCEvent->ConnectTreeTR(fTreeTR);
  }

  //
  return kTRUE;
}

Bool_t IlcMCEventHandler::OpenFile(Int_t i)
{
    // Open file i
    if (i > 0) {
	fkExtension = Form("%d", i);
    } else {
	fkExtension = "";
    }
    
    
    delete fFileK;
    fFileK = TFile::Open(Form("%sKinematics%s.root", fPathName->Data(), fkExtension));
    if (!fFileK) {
	IlcError(Form("IlcMCEventHandler:Kinematics%s.root not found in directory %s ! \n", fkExtension, fPathName->Data()));
	fInitOk = kFALSE;
	return kFALSE;
    }
    
    if (fReadTR) {
	delete fFileTR;
	fFileTR = TFile::Open(Form("%sTrackRefs%s.root", fPathName->Data(), fkExtension));
	if (!fFileTR) {
	    IlcWarning(Form("IlcMCEventHandler:TrackRefs%s.root not found in directory %s ! \n", fkExtension, fPathName->Data()));
	    fInitOk = kFALSE;
	    return kFALSE;
	}
    }
    
    fInitOk = kTRUE;

    return kTRUE;
}

Bool_t IlcMCEventHandler::BeginEvent(Long64_t entry)
{ 
    // Begin event
    fParticleSelected.Delete();
    fLabelMap.Delete();
    // Read the next event

    if (fEventsInContainer != 0) {
	entry = (Long64_t) ( entry * Float_t(fNEvent) / Float_t (fEventsInContainer));
    }


    if (entry == -1) {
	fEvent++;
	entry = fEvent;
    } else {
	fEvent = entry;
    }

    if (entry >= fNEvent) {
	IlcWarning(Form("IlcMCEventHandler: Event number out of range %5lld %5d\n", entry, fNEvent));
	return kFALSE;
    }
    
    Bool_t result = LoadEvent(entry);

    if (fSubsidiaryHandlers) {
	TIter next(fSubsidiaryHandlers);
	IlcMCEventHandler *handler;
	while((handler = (IlcMCEventHandler*)next())) {
	    handler->BeginEvent(entry);
	}
	next.Reset();
	while((handler = (IlcMCEventHandler*)next())) {
	    fMCEvent->AddSubsidiaryEvent(handler->MCEvent());
	}
	fMCEvent->InitEvent();
    }
    
    if (fPreReadMode == kLmPreRead) {
	fMCEvent->PreReadAll();
    }

    return result;
    
}

void IlcMCEventHandler::SelectParticle(Int_t i){
  // taking the absolute values here, need to take care 
  // of negative daughter and mother
  // IDs when setting!
    if (TMath::Abs(i) >= IlcMCEvent::BgLabelOffset()) i =  fMCEvent->BgLabelToIndex(TMath::Abs(i));
    if(!IsParticleSelected(TMath::Abs(i)))fParticleSelected.Add(TMath::Abs(i),1);
}

Bool_t IlcMCEventHandler::IsParticleSelected(Int_t i)  {
  // taking the absolute values here, need to take 
  // care with negative daughter and mother
  // IDs when setting!
  return (fParticleSelected.GetValue(TMath::Abs(i))==1);
}


void IlcMCEventHandler::CreateLabelMap(){

  //
  // this should be called once all selections where done 
  //

  fLabelMap.Delete();
  if(!fMCEvent){
    fParticleSelected.Delete();
    return;
  }

  VerifySelectedParticles();

  Int_t iNew = 0;
  for(int i = 0;i < fMCEvent->GetNumberOfTracks();++i){
    if(IsParticleSelected(i)){
      fLabelMap.Add(i,iNew);
      iNew++;
    }
  }
}

Int_t IlcMCEventHandler::GetNewLabel(Int_t i) {
  // Gets the label from the new created Map
  // Call CreatLabelMap before
  // otherwise only 0 returned
  return fLabelMap.GetValue(TMath::Abs(i));
}

void  IlcMCEventHandler::VerifySelectedParticles(){

  //  
  // Make sure that each particle has at least it's predecessors
  // selected so that we have the complete ancestry tree
  // Private, should be only called by CreateLabelMap

  if(!fMCEvent){
      fParticleSelected.Delete();
      return;
  }

  Int_t nprim = fMCEvent->GetNumberOfPrimaries();

  for(int i = 0;i < fMCEvent->GetNumberOfTracks(); ++i){
      if(i < nprim){
	  SelectParticle(i);// take all primaries
	  continue;
      }

      if(!IsParticleSelected(i))continue;

      IlcMCParticle* mcpart = (IlcMCParticle*) fMCEvent->GetTrack(i);
      Int_t imo = mcpart->GetMother();
      while((imo >= nprim)&&!IsParticleSelected(imo)){
	  // Mother not yet selected
	  SelectParticle(imo);
	  mcpart = (IlcMCParticle*) fMCEvent->GetTrack(imo);
	  imo = mcpart->GetMother();
      }
    // after last step we may have an unselected primary
    // mother
    if(imo>=0){
      if(!IsParticleSelected(imo))
	SelectParticle(imo);
    } 
  }// loop over all tracks
}

Int_t IlcMCEventHandler::GetParticleAndTR(Int_t i, TParticle*& particle, TClonesArray*& trefs)
{
    // Retrieve entry i
    if (!fInitOk) {
	return 0;
    } else {
	return (fMCEvent->GetParticleAndTR(i, particle, trefs));
    }
}

void IlcMCEventHandler::DrawCheck(Int_t i, Int_t search)
{
    // Retrieve entry i and draw momentum vector and hits
    fMCEvent->DrawCheck(i, search);
}

Bool_t IlcMCEventHandler::Notify(const char *path)
{
  // Notify about directory change
  // The directory is taken from the 'path' argument
  // Reconnect trees
    TString fileName(path);
    if(fileName.Contains("IlcESDs.root")){
	fileName.ReplaceAll("IlcESDs.root", "");
    }
    else if(fileName.Contains("IlcESDs_wSDD.root")){
	fileName.ReplaceAll("IlcESDs_wSDD.root", "");
    }
    else if(fileName.Contains("IlcAOD.root")){
	fileName.ReplaceAll("IlcAOD.root", "");
    }
    else if(fileName.Contains("gilc.root")){
	// for running with gilc and kinematics alone...
	fileName.ReplaceAll("gilc.root", "");
    }
    else if (fileName.BeginsWith("root:")) {
      fileName.Append("?ZIP=");
    }

    *fPathName = fileName;
    IlcInfo(Form("Path: -%s-\n", fPathName->Data()));
    
    ResetIO();
    InitIO("");

// Handle subsidiary handlers
    if (fSubsidiaryHandlers) {
	TIter next(fSubsidiaryHandlers);
	IlcMCEventHandler *handler;
	while((handler = (IlcMCEventHandler*) next())) {
	    TString* spath = handler->GetInputPath();
	    if (spath->Contains("merged")) {
		if (! fPathName->IsNull()) {
		    handler->Notify(Form("%s/../.", fPathName->Data()));
		} else {
		    handler->Notify("../");
		}
	    }
	}
    }
    
    return kTRUE;
}

void IlcMCEventHandler::ResetIO()
{
//  Clear header and stack
    
    if (fInitOk) fMCEvent->Clean();
    
// Delete Tree E
    delete fTreeE; fTreeE = 0;
     
// Reset files
    if (fFileE)  {delete fFileE;  fFileE  = 0;}
    if (fFileK)  {delete fFileK;  fFileK  = 0;}
    if (fFileTR) {delete fFileTR; fFileTR = 0;}
    fkExtension="";
    fInitOk = kFALSE;

    if (fSubsidiaryHandlers) {
	TIter next(fSubsidiaryHandlers);
	IlcMCEventHandler *handler;
	while((handler = (IlcMCEventHandler*)next())) {
	    handler->ResetIO();
	}
    }

}

			    
Bool_t IlcMCEventHandler::FinishEvent()
{
    // Clean-up after each event
    delete fDirTR;  fDirTR = 0;
    delete fDirK;   fDirK  = 0;    
    if (fInitOk) fMCEvent->FinishEvent();

    if (fSubsidiaryHandlers) {
	TIter next(fSubsidiaryHandlers);
	IlcMCEventHandler *handler;
	while((handler = (IlcMCEventHandler*)next())) {
	    handler->FinishEvent();
	}
    }

    return kTRUE;
}

Bool_t IlcMCEventHandler::Terminate()
{ 
    // Dummy 
    return kTRUE;
}

Bool_t IlcMCEventHandler::TerminateIO()
{ 
    // Dummy
    return kTRUE;
}
    

void IlcMCEventHandler::SetInputPath(const char* fname)
{
    // Set the input path name
    delete fPathName;
    fPathName = new TString(fname);
}

void IlcMCEventHandler::AddSubsidiaryHandler(IlcMCEventHandler* handler)
{
    // Add a subsidiary handler. For example for background events

    if (!fSubsidiaryHandlers) fSubsidiaryHandlers = new TList();
    fSubsidiaryHandlers->Add(handler);
}
