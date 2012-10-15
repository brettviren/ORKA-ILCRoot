/* $Id$ */

#include "IlcPhysicsSelectionTask.h"

#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

#include <IlcLog.h>
#include <IlcESDEvent.h>
#include <IlcHeader.h>

#include "IlcPhysicsSelection.h"
#include "IlcAnalysisManager.h"
#include "IlcInputEventHandler.h"

//#include "IlcBackgroundSelection.h"

ClassImp(IlcPhysicsSelectionTask)

IlcPhysicsSelectionTask::IlcPhysicsSelectionTask() :
  IlcAnalysisTaskSE("IlcPhysicsSelectionTask"),
  fOutput(0),
  fOption(""),
  fPhysicsSelection(0)
{
  //
  // Default event handler
  //
}

IlcPhysicsSelectionTask::IlcPhysicsSelectionTask(const char* opt) :
  IlcAnalysisTaskSE("IlcPhysicsSelectionTask"),
  fOutput(0),
  fOption(opt),
  fPhysicsSelection(new IlcPhysicsSelection())
{
  //
  // Constructor. Initialization of pointers
  //
  IlcInputEventHandler* handler = dynamic_cast<IlcInputEventHandler*> (IlcAnalysisManager::GetAnalysisManager()->GetInputEventHandler());
  if (handler) {
    handler->SetEventSelection(fPhysicsSelection);
    IlcInfo("Physics Event Selection enabled.");
  } else {
    IlcError("No input event handler connected to analysis manager. No Physics Event Selection.");
  }
  // Define input and output slots here
  DefineOutput(1, TList::Class());
  fBranchNames = "ESD:IlcESDRun.,IlcESDHeader.,IlcMultiplicity.,IlcESDVZERO.,"
                 "IlcESDZDC.,SPDVertex.,PrimaryVertex.,TPCVertex.,Tracks,SPDPileupVertices";
  
  IlcLog::SetClassDebugLevel("IlcPhysicsSelectionTask", IlcLog::kWarning);
}

IlcPhysicsSelectionTask::~IlcPhysicsSelectionTask()
{
  //
  // Destructor
  //

  // histograms are in the output list and deleted when the output
  // list is deleted by the TSelector dtor

  if (fOutput && !IlcAnalysisManager::GetAnalysisManager()->IsProofMode()) {
    delete fOutput;
    fOutput = 0;
  }
}

void IlcPhysicsSelectionTask::UserCreateOutputObjects()
{
  // create result objects and add to output list

  Printf("IlcPhysicsSelectionTask::CreateOutputObjects");

  fOutput = new TList;
  fOutput->SetOwner();
  
  if (!fPhysicsSelection)
    fPhysicsSelection = new IlcPhysicsSelection;
  
  fOutput->Add(fPhysicsSelection);
  // All tasks must post data once for all outputs (AG)
  PostData(1, fOutput);
}

void IlcPhysicsSelectionTask::UserExec(Option_t*)
{
  // process the event

  // IlcPhysicsSelection::IsCollisionCandidate is called from the event handler
  // post the data here anyway!
  PostData(1, fOutput);
}

void IlcPhysicsSelectionTask::FinishTaskOutput()
{
// This gets called at the end of the processing on the worker. It allows dumping
// statistics printed by the physics selection object to the statistics message
// handled by the analysis manager.
   if (fPhysicsSelection) fPhysicsSelection->Print("STAT");
}

void IlcPhysicsSelectionTask::Terminate(Option_t *)
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

  fOutput = dynamic_cast<TList*> (GetOutputData(1));
  if (!fOutput)
    Printf("ERROR: fOutput not available");
    
  if (fOutput)
  {
    fPhysicsSelection = dynamic_cast<IlcPhysicsSelection*> (fOutput->FindObject("IlcPhysicsSelection"));
  }

  TFile* fout = new TFile("event_stat.root", "RECREATE");

  if (fPhysicsSelection)
  {
    fPhysicsSelection->Print();
    fPhysicsSelection->SaveHistograms();
  }
    
  fout->Write();
  fout->Close();
  
  Printf("Writting result to event_stat.root");
}
