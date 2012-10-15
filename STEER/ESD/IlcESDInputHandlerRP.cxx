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

//-------------------------------------------------------------------------
//     Event handler for ESD input reading the RecPoint Trees in parallel
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------

#include <TTree.h>
#include <TList.h>
#include <TFile.h>
#include <TArchiveFile.h>
#include <TSystemDirectory.h>
#include <TString.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TProcessID.h>
#include <TSystem.h>

#include "IlcESDInputHandlerRP.h"
#include "IlcESDEvent.h"
#include "IlcESD.h"
#include "IlcLog.h"

ClassImp(IlcESDInputHandlerRP)

//______________________________________________________________________________
IlcESDInputHandlerRP::IlcESDInputHandlerRP() :
    IlcESDInputHandler(),
    fRTrees(   new TObjArray()),
    fRDirs (   new TObjArray()),
    fRFiles(   new TList()),
    fDetectors(new TList()),
    fDirR(0),
    fEventNumber(-1),
    fFileNumber(0),
    fEventsPerFile(0),
    fExtension(""),
    fPathName(new TString("./")),
    fIsArchive(kFALSE)
{
  // Default constructor
}


//______________________________________________________________________________
IlcESDInputHandlerRP::IlcESDInputHandlerRP(const char* name, const char* title):
    IlcESDInputHandler(name, title),
    fRTrees(   new TObjArray()),
    fRDirs (   new TObjArray()),
    fRFiles(   new TList()),
    fDetectors(new TList()),
    fDirR(0),
    fEventNumber(-1),
    fFileNumber(0),
    fEventsPerFile(0),
    fExtension(""),
    fPathName(new TString("./")),
    fIsArchive(kFALSE)
{
    // Constructor
}

//______________________________________________________________________________
IlcESDInputHandlerRP::~IlcESDInputHandlerRP() 
{
  // Destructor
}

Bool_t IlcESDInputHandlerRP::Init(Option_t* opt)
{
    //
    // Initialize input
    //
    if (!(strcmp(opt, "proof")) || !(strcmp(opt, "local"))) return kTRUE;
    //
    TIter next(fDetectors);
    TNamed* det;
    TFile* file = 0;
    while ((det = (TNamed*) next()))
    {
	if (!fIsArchive) {
	    file = TFile::Open(Form("%s%s.RecPoints.root", fPathName->Data(), det->GetName()));
	} else {
	    file = TFile::Open(Form("%s#%s.RecPoints.root", fPathName->Data(), det->GetName()));
	}
	if (!file) {
	  IlcError(Form("IlcESDInputHandlerRP: %s.RecPoints.root not found in %s ! \n", det->GetName(), fPathName->Data()));
	  return kFALSE;
	}
	fRFiles->Add(file);
    }

    if (file) {
	fEventsPerFile = file->GetNkeys() - file->GetNProcessIDs();
    } else {
	IlcError(Form("IlcESDInputHandlerRP: No file with RecPoints found in %s ! \n", fPathName->Data()));
	return kFALSE;
    }
    

    // Reset the event number
    fEventNumber      = -1;
    fFileNumber       =  0;
    // Get number of events from esd tree 
    printf("IlcESDInputHandlerRP::Init() %d %d\n",__LINE__, fNEvents);
    //
    IlcESDInputHandler::Init(opt);
    //
    return kTRUE;
}

Bool_t IlcESDInputHandlerRP::BeginEvent(Long64_t entry)
{
    // Begin the next event
    //
    if (entry == -1) {
	fEventNumber++;
	entry = fEventNumber;
    } else {
	fEventNumber = entry;
    }
    
    if (entry >= fNEvents) {
	IlcWarning(Form("IlcESDInputHandlerRP: Event number out of range %5lld %5d\n", entry, fNEvents));
	return kFALSE;
    }
    
    LoadEvent(entry);

    // Delegate to base class
    return IlcESDInputHandler::BeginEvent(entry);

}

Bool_t IlcESDInputHandlerRP::LoadEvent(Int_t iev)
{
    // Load the event number iev
    //
    // Calculate the file number
  if (fEventsPerFile<=0) return kFALSE;
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
    // Tree R
    TIter next(fRFiles);
    TFile* file;
    Int_t idx  = 0;
    
    while ((file = (TFile*) next()))
    {
	file->GetObject(folder, fDirR);
	
	if (!fDirR) {
	    IlcWarning(Form("IlcESDInputHandlerRP: Event #%5d not found\n", iev));
	    return kFALSE;
	}
	TTree* tree = 0;
	fDirR->GetObject("TreeR", tree);
	fRDirs ->AddAt(fDirR, idx  );
	fRTrees->AddAt(tree,  idx++);
    }
    return kTRUE;
}

Bool_t IlcESDInputHandlerRP::OpenFile(Int_t i)
{
    // Open file i
    Bool_t ok = kTRUE;
    if (i > 0) {
	fExtension = Form("%d", i);
    } else {
	fExtension = "";
    }
    
    fRFiles->Delete();
    TIter next(fDetectors);
    TNamed* det;
    TFile* file;
    while ((det = (TNamed*) next()))
    {
	if (!fIsArchive) {
	    file = TFile::Open(Form("%s%s.RecPoints%s.root", fPathName->Data(), det->GetName(), fExtension));
	} else {
	    file = TFile::Open(Form("%s#%s.RecPoints%s.root", fPathName->Data(), det->GetName(), fExtension));
	}
	if (!file) IlcFatal(Form("IlcESDInputHandlerRP: RecPoints.root not found in %s ! \n", fPathName->Data()));
	fRFiles->Add(file);
    }
    return ok;
}

Bool_t IlcESDInputHandlerRP::Notify(const char *path)
{
  // Notify about directory change
  // The directory is taken from the 'path' argument
  // 

    // Get path to directory
    TString fileName(path);
    if (fileName.IsNull()) return kFALSE;
    IlcInfo(Form("Directory change %s \n", path));

    TString esdname = gSystem->BaseName(fileName);
    Int_t index = esdname.Index("#")+1;
    if (index) esdname.Remove(0,index);

    if(fileName.Contains("#")){
    // If this is an archive it will contain a # 
      fIsArchive = kTRUE;
    } else  if(fileName.Contains(esdname)){
      fileName.ReplaceAll(esdname, "");
    }

    //
    // At this point we have a path to the directory or to the archive anchor
    *fPathName = fileName;
    //
    // Now filter the files containing RecPoints *.RecPoints.*

    TSeqCollection* members;

    
    if (fIsArchive) {
	// Archive
      TFile* file = TFile::Open(fPathName->Data());
      TArchiveFile* arch = file->GetArchive();
      members = arch->GetMembers();
      fPathName->ReplaceAll("#", "");
      fPathName->ReplaceAll(esdname, "");
    } else {
	// Directory or alien archive
      if (fileName.BeginsWith("alien:")) {
        TFile* file = TFile::Open(Form("%s/root_archive.zip", fPathName->Data()));
        TArchiveFile* arch = file->GetArchive();
        members = arch->GetMembers();
      } else {  
        TString wd = gSystem->WorkingDirectory();
        TSystemDirectory dir(".", fPathName->Data());
        members = dir.GetListOfFiles();
        gSystem->cd(wd);
      }  
    }

    TIter next(members);
    TFile* entry;
    Int_t ien = 0;
    fDetectors->Delete();
    
    while ( (entry = (TFile*) next()) )
    {
	TString name(entry->GetName());
	TObjArray* tokens = name.Tokenize(".");
	Int_t ntok = 0;
	if (tokens) {
	  ntok = tokens->GetEntries();
	} else {
	  continue;
	}
	if (ntok <= 1) continue;
	TString str = ((TObjString*) tokens->At(1))->GetString();
	if (!(strcmp(str.Data(), "RecPoints"))){
	    TString det = ((TObjString*) tokens->At(0))->GetString();
	    printf("Found file with RecPoints for %s \n", det.Data());
	    TNamed* ent = new TNamed(det.Data(), det.Data());
	    fRTrees->AddAt(0, ien);
	    ent->SetUniqueID(ien++);
	    fDetectors->Add(ent);
	}
	if(tokens) delete tokens;
    } // loop over files


    // Now we have the path and the list of detectors
    
    printf("IlcESDInputHandlerRP::Notify() Path: %s\n", fPathName->Data());
    //
    ResetIO();
    InitIO("");
    // Some clean-up
    if (members) members->Delete();

    IlcESDInputHandler::Notify(path);
    
    return kTRUE;
}

Bool_t IlcESDInputHandlerRP::FinishEvent()
{
    // Clean-up after each event
    fRDirs->Delete();
    IlcESDInputHandler::FinishEvent();
    return kTRUE;
}

void IlcESDInputHandlerRP::ResetIO()
{
// Delete trees and files
    fRFiles->Clear("nodelete");
    fExtension="";
}

TTree* IlcESDInputHandlerRP::GetTreeR(const char* det)
{
// Return pointer to RecPoint tree for detector det
    TNamed* entry = (TNamed*) (fDetectors->FindObject(det));
    if (!entry) {
	IlcWarning(Form("IlcESDInputHandlerRP: No RecPoints for detector %s available \n", det));
	return 0;
    } else {
	Int_t ien = entry->GetUniqueID();
	return ((TTree*) (fRTrees->At(ien)));
    }
}
