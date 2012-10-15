/* $Id$ */


// Simple task to test the collision normalization. Can be called for
// both MC and data and shows how to fill the collision normalization
// class
//
// Author: Michele Floris
//         CERN


#include "IlcCollisionNormalizationTask.h"

#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

#include <IlcLog.h>
#include <IlcESDEvent.h>
#include <IlcHeader.h>

#include "IlcCollisionNormalization.h"
#include "IlcAnalysisManager.h"
#include "IlcInputEventHandler.h"

//#include "IlcBackgroundSelection.h"
#include "IlcMultiplicity.h"
#include "IlcMCEvent.h"

ClassImp(IlcCollisionNormalizationTask)

IlcCollisionNormalizationTask::IlcCollisionNormalizationTask() :
  IlcAnalysisTaskSE("IlcCollisionNormalizationTask"),
  fOutput(0),
  fIsMC(0),
  fCollisionNormalization(0)
{
  //
  // Default event handler
  //

  // Define input and output slots here
  DefineOutput(1, TList::Class());
  
}

IlcCollisionNormalizationTask::IlcCollisionNormalizationTask(const char* name) :
  IlcAnalysisTaskSE(name),
  fOutput(0),
  fIsMC(0),
  fCollisionNormalization(new IlcCollisionNormalization())
{
  //
  // Constructor. Initialization of pointers
  //

  // Define input and output slots here
  DefineOutput(1, TList::Class());
  
  //  IlcLog::SetClassDebugLevel("IlcCollisionNormalizationTask", IlcLog::kWarning);
}

IlcCollisionNormalizationTask::~IlcCollisionNormalizationTask()
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

void IlcCollisionNormalizationTask::UserCreateOutputObjects()
{
  // create result objects and add to output list

  Printf("IlcCollisionNormalizationTask::CreateOutputObjects");

  fOutput = new TList;
  fOutput->SetOwner();
  
  if (!fCollisionNormalization)
    fCollisionNormalization = new IlcCollisionNormalization;
  
  fOutput->Add(fCollisionNormalization);
//   fOutput->Add(fCollisionNormalization->GetVzCorrZeroBin ());
//   fOutput->Add(fCollisionNormalization->GetVzMCGen       ());
//   fOutput->Add(fCollisionNormalization->GetVzMCRec       ());
//   fOutput->Add(fCollisionNormalization->GetVzMCTrg       ());
//   fOutput->Add(fCollisionNormalization->GetVzData        ());
//   fOutput->Add(fCollisionNormalization->GetNEvents       ());
//   fOutput->Add(fCollisionNormalization->GetStatBin0      ());

}

void IlcCollisionNormalizationTask::UserExec(Option_t*)
{
  // process the event


  PostData(1, fOutput);

  // Get the ESD
  IlcESDEvent * aESD = dynamic_cast<IlcESDEvent*>(fInputEvent);
  if(!aESD) {
    IlcFatal("Cannot get ESD");
  }
  if (strcmp(aESD->ClassName(),"IlcESDEvent")) {
    IlcFatal("Not processing ESDs");
  }
  // Get MC event, if needed
  IlcMCEvent* mcEvent = fIsMC ? MCEvent() : 0;
  if (!mcEvent && fIsMC){
    IlcFatal("Running on MC but no MC handler available");
  }

  // Physics selection. At least in the case of MC we cannot use
  // yourTask->SelectCollisionCandidates();, because we also need to
  // fill the "generated" histogram 
  // NB never call IsEventSelected more than once per event
  // (statistics histogram would be altered)

  // FIXME: using only MB events, foresee more events?
  Bool_t isSelected = (((IlcInputEventHandler*)(IlcAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & IlcVEvent::kMB);

  // Get the Multiplicity cut
  const IlcMultiplicity* mult = aESD->GetMultiplicity();
  if (!mult){
    IlcError("Can't get mult object");
    return;
  }

  // Check if the event is "reconstructed" according to some quality
  // cuts. Tipically, we mean "it has a good-eonugh vertex"

  Int_t ntracklet = mult->GetNumberOfTracklets();
  const IlcESDVertex * vtxESD = aESD->GetPrimaryVertexSPD();
  if (IsEventInBinZero()) {
    ntracklet = 0;
    vtxESD    = 0;
  }
  
  if (ntracklet > 0 && !vtxESD) {
    IlcError("No vertex but reconstructed tracklets?");
  }

  // assign vz. For MC we use generated vz
  Float_t vz = 0;
  if (!fIsMC) vz = vtxESD ? vtxESD->GetZ() : 0; // FIXME : is zv used anywhere in Gen?
  else        vz = mcEvent->GetPrimaryVertex()->GetZ();

  if (fIsMC) {
    // Monte Carlo:  we fill 3 histos
    if (!isSelected || !vtxESD) ntracklet = 0; //If the event does not pass the physics selection or is not rec, it goes in the bin0
    fCollisionNormalization->FillVzMCGen(vz, ntracklet, mcEvent);      
    // If triggered == passing the physics selection
    if (isSelected) {
      fCollisionNormalization->FillVzMCTrg(vz, ntracklet, mcEvent);
      // If reconstructer == good enough vertex
      if (vtxESD) fCollisionNormalization->FillVzMCRec(vz, ntracklet, mcEvent);    
    }
  } else {
    if (isSelected) {
      // Passing the trigger
      fCollisionNormalization->FillVzData(vz,ntracklet);
    }
  }

}

void IlcCollisionNormalizationTask::Terminate(Option_t *)
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

  fOutput = dynamic_cast<TList*> (GetOutputData(1));
  if (!fOutput)
    Printf("ERROR: fOutput not available");


}

Bool_t IlcCollisionNormalizationTask::IsEventInBinZero() {

  // Returns true if an event is to be assigned to the zero bin
  //
  // You should have your own version of this method in the class: in
  // general, the definition of "reconstructed" event is subjective.

  Bool_t isZeroBin = kTRUE;
  const IlcESDEvent* esd= dynamic_cast<IlcESDEvent*>(fInputEvent);
  if (!esd){ 
    Printf("IlcCollisionNormalizationTask::IsEventInBinZero: Can't get ESD");
    return kFALSE;   
  }
  const IlcMultiplicity* mult = esd->GetMultiplicity();
  if (!mult){
    Printf("IlcCollisionNormalizationTask::IsEventInBinZero: Can't get mult object");
    return kFALSE;
  }
  Int_t ntracklet = mult->GetNumberOfTracklets();
  const IlcESDVertex * vtxESD = esd->GetPrimaryVertexSPD();
  if(vtxESD) {
    // If there is a vertex from vertexer z with delta phi > 0.02 we
    // don't consider it rec (we keep the event in bin0). If quality
    // is good eneough we check the number of tracklets
    // if the vertex is more than 15 cm away, this is autamatically bin0
    if( TMath::Abs(vtxESD->GetZ()) <= 15 ) {
      if (vtxESD->IsFromVertexerZ()) {
	if (vtxESD->GetDispersion()<=0.02 ) {
	  if(ntracklet>0) isZeroBin = kFALSE;
	}
      } else if(ntracklet>0) isZeroBin = kFALSE; // if the event is not from Vz we chek the n of tracklets
    } 
  }
  return isZeroBin;

}
