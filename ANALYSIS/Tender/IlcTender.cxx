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

/* $Id: IlcTender.cxx 48689 2011-03-23 21:35:50Z agheata $ */

#include <TChain.h>
#include <TFile.h>
 
#include "IlcTender.h"
#include "IlcTenderSupply.h"
#include "IlcAnalysisManager.h"
#include "IlcCDBManager.h"
#include "IlcESDEvent.h"
#include "IlcESDInputHandler.h"
#include "IlcLog.h"


ClassImp(IlcTender)

//______________________________________________________________________________
IlcTender::IlcTender():
           IlcAnalysisTaskSE(),
           fRun(0),
           fRunChanged(kFALSE),
           fCDBkey(0),
           fDefaultStorage(),
           fCDB(NULL),
           fESDhandler(NULL),
           fESD(NULL),
           fSupplies(NULL),
           fCDBSettings(NULL)
{
// Dummy constructor
}

//______________________________________________________________________________
IlcTender::IlcTender(const char* name):
           IlcAnalysisTaskSE(name),
           fRun(0),
           fRunChanged(kFALSE),
           fCDBkey(0),
           fDefaultStorage(),
           fCDB(NULL),
           fESDhandler(NULL),
           fESD(NULL),
           fSupplies(NULL),
           fCDBSettings(NULL)
{
// Default constructor
  DefineOutput(1,  IlcESDEvent::Class());
}

//______________________________________________________________________________
IlcTender::~IlcTender()
{
// Destructor
  if (fSupplies) {
    fSupplies->Delete();
    delete fSupplies;
  }
}

//______________________________________________________________________________
void IlcTender::AddSupply(IlcTenderSupply *supply)
{
// Addition of supplies.
  if (!fSupplies) fSupplies = new TObjArray();
  if (fSupplies->FindObject(supply)) {
     Error("AddSupply", "Tender supply %s already connected.", supply->GetName());
     return;
  }   
  fSupplies->Add(supply);
  supply->SetTender(this);
}
   
//______________________________________________________________________________
void IlcTender::ConnectInputData(Option_t* option)
{
// Connect the input data, create CDB manager.
  if (fDebug > 1) Printf("IlcTender::ConnectInputData()\n");
  
  if (!fESDhandler) { 
    IlcAnalysisTaskSE::ConnectInputData(option);
    fESDhandler = dynamic_cast<IlcESDInputHandler *>(fInputHandler);
  }
  
  if (fESDhandler) {
     fESD = fESDhandler->GetEvent();
  } else {
     IlcFatal("No ESD input event handler connected") ; 
  }
  // Create CDB manager
  if (!fDefaultStorage.Length()) IlcFatal("Default CDB storage not set.");
  fCDB = IlcCDBManager::Instance();
  // SetDefault storage. Specific storages must be set by IlcTenderSupply::Init()
  fCDB->SetDefaultStorage(fDefaultStorage);
  Int_t run = IlcAnalysisManager::GetAnalysisManager()->GetRunFromPath();
  // Unlock CDB
  fCDBkey = fCDB->SetLock(kFALSE, fCDBkey);
  if (!run) {
     IlcWarning("IlcTaskCDBconnect: Could not set run from path");
  } else {
     fRun = run;
     fRunChanged = kTRUE;
     printf("IlcTender: #### Setting run to: %d\n", fRun);
     fCDB->SetRun(fRun);
  }   
  TIter next(fSupplies);
  IlcTenderSupply *supply;
  while ((supply=(IlcTenderSupply*)next())) supply->Init();
  // Lock CDB
  fCDBkey = fCDB->SetLock(kTRUE, fCDBkey);
}

//______________________________________________________________________________
void IlcTender::UserCreateOutputObjects()
{
// Nothing for the moment, but we may need ESD event replication here.
  if (fDebug > 1) Printf("IlcTender::CreateOutputObjects()\n");
  fESDhandler = dynamic_cast<IlcESDInputHandler *>(fInputHandler);
  if (fESDhandler && TObject::TestBit(kCheckEventSelection)) {
     fESDhandler->SetUserCallSelectionMask(kTRUE);
     Info("UserCreateOutputObjects","The TENDER will check the event selection. Make sure you add the tender as FIRST wagon!");
  }   
}

//______________________________________________________________________________
void IlcTender::UserExec(Option_t* option)
{
//
// Execute all supplied analysis of one event. Notify run change via RunChanged().
  if (fDebug > 1) {
    Long64_t entry = fESDhandler->GetReadEntry();
    Printf("IlcTender::Exec() %s ==> processing event %lld\n", fESDhandler->GetTree()->GetCurrentFile()->GetName(),entry);
  }  
  fESD = fESDhandler->GetEvent();

// Call the user analysis
  // Unlock CDB
  fCDBkey = fCDB->SetLock(kFALSE, fCDBkey);
  // Intercept when the run number changed
  if (fRun != fESD->GetRunNumber()) {
    fRunChanged = kTRUE;
    fRun = fESD->GetRunNumber();
    fCDB->SetRun(fRun);
  }
  TIter next(fSupplies);
  IlcTenderSupply *supply;
  while ((supply=(IlcTenderSupply*)next())) supply->ProcessEvent();
  fRunChanged = kFALSE;

  if (TObject::TestBit(kCheckEventSelection)) fESDhandler->CheckSelectionMask();

  // Lock CDB
  fCDBkey = fCDB->SetLock(kTRUE, fCDBkey);
  
  TString opt = option;
  if (!opt.Contains("NoPost")) PostData(1, fESD);
}

//______________________________________________________________________________
void IlcTender::SetDefaultCDBStorage(const char *dbString)
{
// Set default CDB storage
   fDefaultStorage = dbString;
}
