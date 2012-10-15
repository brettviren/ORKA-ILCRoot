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

/* $Id: IlcAnalysisTaskStat.cxx 46299 2011-01-06 11:17:06Z agheata $ */
// Author: Andrei Gheata, 20/12/2010

//==============================================================================
// IlcAnalysisTaskStat - basic task that attaches a IlcAnalysisTaskstatistics 
// object to the analysis manager. Use: IlcAnalysisManager::AddStatisticsTask
// to attach to a train.
//==============================================================================

#include "IlcAnalysisTaskStat.h"

#include <TList.h>
#include "IlcVEvent.h"
#include "IlcAnalysisManager.h"
#include "IlcAnalysisDataContainer.h"
#include "IlcAnalysisStatistics.h"

ClassImp(IlcAnalysisTaskStat)

//______________________________________________________________________________
IlcAnalysisTaskStat::IlcAnalysisTaskStat(const char *name)
                    :IlcAnalysisTaskSE(name),
                     fStatistics(0),
                     fOutputList(0)
{
// Named constructor.
  DefineOutput(1, TList::Class());
  fBranchNames = "ESD:IlcESDHeader. AOD:header";
  fStatistics = new IlcAnalysisStatistics("MgrStat");
}

//______________________________________________________________________________
IlcAnalysisTaskStat::~IlcAnalysisTaskStat()
{
// Destructor.
  if (fOutputList) {
    if (!IlcAnalysisManager::GetAnalysisManager()->IsProofMode()) delete fOutputList;
  } else {
    if (fStatistics) delete fStatistics;
  }
} 
    
//______________________________________________________________________________
IlcAnalysisTaskStat *IlcAnalysisTaskStat::AddToManager(UInt_t offlineMask)
{
// Add this task to the analysis manager. By default it selects MB events.
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    ::Error("IlcAnalysisTaskStat::AddToManager", "You need a manager first");
    return 0;
  }
  IlcAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
  if (!cinput) {
    ::Error("IlcAnalysisTaskStat::AddToManager", "Attach first the input handler");
    return 0;
  }  
  IlcAnalysisDataContainer *coutput = mgr->CreateContainer("MgrStat", TList::Class(), IlcAnalysisManager::kOutputContainer,
                mgr->GetCommonFileName());
  IlcAnalysisTaskStat *taskStatistics = new IlcAnalysisTaskStat("MgrStat");
  mgr->AddTask(taskStatistics);
  IlcAnalysisStatistics *stat = taskStatistics->GetStatistics();
  stat->SetOfflineMask(offlineMask);
  mgr->SetStatistics(stat);
  taskStatistics->SelectCollisionCandidates(offlineMask);
  mgr->ConnectInput(taskStatistics, 0, cinput);
  mgr->ConnectOutput(taskStatistics, 1, coutput);
  return taskStatistics;
}

//______________________________________________________________________________
void  IlcAnalysisTaskStat::UserCreateOutputObjects()
{
// Create the output list.
  if (!fStatistics) {
    Fatal("UserCreateOutputObjects", "You are not allowed to create this task using the dummy constructor. Use the named one.");
  }
  fOutputList = new TList();
  fOutputList->SetOwner();
  if (fStatistics) fOutputList->Add(fStatistics);
  PostData(1, fOutputList);
}   

//______________________________________________________________________________
void  IlcAnalysisTaskStat::UserExec(Option_t *)
{
// Event loop.
  fStatistics->AddAccepted();
}

//______________________________________________________________________________
void  IlcAnalysisTaskStat::Terminate(Option_t *)
{
// Get the statistics from its container and copy to manager.
  fOutputList = dynamic_cast<TList*>(GetOutputData(1));
  if (!fOutputList) {
    Error("Terminate", "Cannot get output list from container");
    return;
  }
  IlcAnalysisStatistics *stat = dynamic_cast<IlcAnalysisStatistics*>(fOutputList->At(0));
  if (!stat) {
    Error("Terminate", "Statistics object not found in list");
    return;
  }
  IlcAnalysisManager *mgr = IlcAnalysisManager::GetAnalysisManager();
  if (stat != fStatistics) {
    // Non-local mode
    fStatistics->AddInput(stat->GetNinput());
    fStatistics->AddProcessed(stat->GetNprocessed());
    fStatistics->AddFailed(stat->GetNfailed());
    fStatistics->AddAccepted(stat->GetNaccepted());
    mgr->SetStatistics(fStatistics);
  }
  fStatistics->Print();
}  
