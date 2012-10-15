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

//-------------------------------------------------------------------------
//     Event handler for reconstruction
//     Author: Andrei Gheata, CERN
//-------------------------------------------------------------------------

#include "IlcRecoInputHandler.h"
#include "IlcVCuts.h"

ClassImp(IlcRecoInputHandler)

//______________________________________________________________________________
IlcRecoInputHandler::IlcRecoInputHandler(const char* name, const char* title) 
  : IlcESDInputHandler(name,title)
{
// Named constructor
}

//______________________________________________________________________________
Bool_t IlcRecoInputHandler::Init(TTree* tree,  Option_t* opt)
{
// Initialisation necessary for each new tree. In reco case this is once.
   fAnalysisType = opt;
   fTree = tree;
   if (!fTree) return kFALSE;
   fNEvents = fTree->GetEntries();
   return kTRUE;
}  
//______________________________________________________________________________
Bool_t IlcRecoInputHandler::BeginEvent(Long64_t)
{
// Called at the beginning of every event   
  static Bool_t called = kFALSE;
  if (!called && fEventCuts && IsUserCallSelectionMask())
     IlcInfo(Form("The ESD input handler expects that the first task calls IlcESDInputHandler::CheckSelectionMask() %s", fEventCuts->ClassName()));
  fNewEvent = kTRUE;
  called = kTRUE;
  return kTRUE;
}     
