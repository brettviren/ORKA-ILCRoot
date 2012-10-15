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

//*****************************************************
//   Class IlcVZEROEPSelectionTask
//   author: Cvetan Cheshkov
//   30/01/2012
//   This analysis task reads the OADB and
//   provides the parameters needed to flatten
//   the VZERO event plane in IlcEventplane
//*****************************************************

#include "IlcVZEROEPSelectionTask.h"

#include <TList.h>
#include <TProfile.h>
#include <TFile.h>
#include <TString.h>
#include <TDirectory.h>

#include "IlcLog.h"
#include "IlcVEvent.h"
#include "IlcAnalysisManager.h"
#include "IlcOADBContainer.h"
#include "IlcEventplane.h"
#include "IlcCentrality.h"

ClassImp(IlcVZEROEPSelectionTask)

//________________________________________________________________________
IlcVZEROEPSelectionTask::IlcVZEROEPSelectionTask():
IlcAnalysisTaskSE(),
  fRunNumber(-1),
  fUserParams(kFALSE),
  fUseVZEROCentrality(kFALSE),
  fVZEROEPContainer(0)
{   
  // Default constructor
  // Initialize pointers
  IlcInfo("VZERO Event Plane Selection enabled.");
  for(Int_t i = 0; i < 8; ++i) fX2In[i] = fY2In[i] = fX2Y2In[i] = fCos8PsiIn[i] = NULL;
}   

//________________________________________________________________________
IlcVZEROEPSelectionTask::IlcVZEROEPSelectionTask(const char *name):
  IlcAnalysisTaskSE(name),
  fRunNumber(-1),
  fUserParams(kFALSE),
  fUseVZEROCentrality(kFALSE),
  fVZEROEPContainer(0)
{
  // Default constructor
  // Initialize pointers
  IlcInfo("Event Plane Selection enabled.");
  for(Int_t i = 0; i < 8; ++i) fX2In[i] = fY2In[i] = fX2Y2In[i] = fCos8PsiIn[i] = NULL;
}
 
//________________________________________________________________________
IlcVZEROEPSelectionTask::~IlcVZEROEPSelectionTask()
{
  // Destructor
  // ...
  if (fUserParams) {
    for(Int_t i = 0; i < 8; ++i) {
      delete fX2In[i];
      fX2In[i] = NULL;
      delete fY2In[i];
      fY2In[i] = NULL;
      delete fX2Y2In[i];
      fX2Y2In[i] = NULL;
      delete fCos8PsiIn[i];
      fCos8PsiIn[i] = NULL;
    }
  }
  if (fVZEROEPContainer){
    delete fVZEROEPContainer;
    fVZEROEPContainer = NULL;
  }
}  

//________________________________________________________________________
void IlcVZEROEPSelectionTask::UserCreateOutputObjects()
{  
  // Create the output containers (none in this case)
  // Open the OADB file
  
  if(!fUserParams) {
    TString oadbFileName = Form("%s/COMMON/EVENTPLANE/data/vzero.root", IlcAnalysisManager::GetOADBPath());
    TFile *fOADB = TFile::Open(oadbFileName); 
    if(!fOADB->IsOpen()) IlcFatal(Form("Cannot open OADB file %s", oadbFileName.Data()));

    IlcInfo("Using Standard OADB");
    IlcOADBContainer *cont = (IlcOADBContainer*)fOADB->Get("vzeroEP");
    if (!cont) IlcFatal("Cannot fetch OADB container for VZERO EP selection");
    fVZEROEPContainer = new IlcOADBContainer(*cont);
    fOADB->Close();
    delete fOADB;
  }
}

//________________________________________________________________________
void IlcVZEROEPSelectionTask::UserExec(Option_t */*option*/)
{ 
  // Execute analysis for current event:
  // Fill the flatenning parameters in
  // IlcEventplane object

  IlcVEvent* event = InputEvent();
  if (!(fRunNumber == event->GetRunNumber())) {
    fRunNumber = event->GetRunNumber();
    SetParamsFromOADB();
  }

  IlcCentrality *centrality = event->GetCentrality();
  Float_t percentile = (fUseVZEROCentrality) ? centrality->GetCentralityPercentile("V0M") : centrality->GetCentralityPercentile("CL1");
  IlcEventplane *esdEP = event->GetEventplane();
  SetEventplaneParams(esdEP,percentile);
}

//________________________________________________________________________
void IlcVZEROEPSelectionTask::Terminate(Option_t */*option*/)
{
  // Terminate analysis
  // Nothing here
}

//________________________________________________________________________
void IlcVZEROEPSelectionTask::SetEventplaneParams(IlcEventplane *esdEP,Float_t percentile)
{
  // Read the OADB histograms and
  // prepare parameters used in order to
  // flatten the event-plane
  if(!esdEP)
    IlcFatal("No event plane received");

  if (percentile < 0 || percentile > 100) {
    for(Int_t ring = 0; ring < 8; ++ring) esdEP->SetVZEROEPParams(ring,0.,0.,1.,1.,0.,0.,0.);
    return;
  }

  for(Int_t ring = 0; ring < 8; ++ring) {
    Int_t ibin = fX2In[ring]->FindBin(percentile);
    if (fX2In[ring]->GetBinEntries(ibin) == 0) {
      esdEP->SetVZEROEPParams(ring,0.,0.,1.,1.,0.,0.,0.);
      continue;
    }
    Double_t meanX2 = fX2In[ring]->GetBinContent(ibin);
    Double_t meanY2 = fY2In[ring]->GetBinContent(ibin);
    Double_t sigmaX2 = fX2In[ring]->GetBinError(ibin);
    Double_t sigmaY2 = fY2In[ring]->GetBinError(ibin);
    Double_t rho = (fX2Y2In[ring]->GetBinContent(ibin)-meanX2*meanY2)/sigmaX2/sigmaY2;
  
    Double_t b = rho*sigmaX2*sigmaY2*
      TMath::Sqrt(2.*(sigmaX2*sigmaX2+sigmaY2*sigmaY2-2.*sigmaX2*sigmaY2*TMath::Sqrt(1.-rho*rho))/
		  ((sigmaX2*sigmaX2-sigmaY2*sigmaY2)*(sigmaX2*sigmaX2-sigmaY2*sigmaY2)+
		   4.*sigmaX2*sigmaX2*sigmaY2*sigmaY2*rho*rho));
    Double_t aPlus = TMath::Sqrt(2.*sigmaX2*sigmaX2-b*b);
    Double_t aMinus= TMath::Sqrt(2.*sigmaY2*sigmaY2-b*b);

    Double_t lambdaPlus = b/aPlus;
    Double_t lambdaMinus = b/aMinus;

    Double_t cos8Psi = fCos8PsiIn[ring]->GetBinContent(ibin);
    esdEP->SetVZEROEPParams(ring,meanX2,meanY2,aPlus,aMinus,lambdaPlus,lambdaMinus,cos8Psi);
  }
}

//__________________________________________________________________________
void IlcVZEROEPSelectionTask::SetParamsFromOADB() 
{
  if(!fUserParams) {
    TList *list = (TList*)fVZEROEPContainer->GetObject(fRunNumber, "Default");
    if (!list) IlcFatal(Form("Cannot find VZERO OADB list for run %d", fRunNumber));
    SetHistograms(list);
  }
  else
    IlcInfo("Using custom VZERO event-plane params");
}

//__________________________________________________________________________
void IlcVZEROEPSelectionTask::SetUserParams(const char* inFileName, const char* listName)
{
  
  fUserParams = kTRUE;
  
  TFile f(inFileName);
  TList* list = (TList*)f.Get(listName);
  if (!list) IlcFatal(Form("Cannot find list %s in file %s", listName, inFileName));
  SetHistograms(list);
  f.Close();
} 

//__________________________________________________________________________
void IlcVZEROEPSelectionTask::SetHistograms(TList *list)
{
  // Set the flatenning parameters
  // histograms from a given list

  for(Int_t i = 0; i < 8; ++i) {
    fX2In[i] = (TProfile*)list->FindObject(Form("fX2_%d",i))->Clone(Form("fX2In_%d",i));
    fX2In[i]->SetDirectory(0);
    fY2In[i] = (TProfile*)list->FindObject(Form("fY2_%d",i))->Clone(Form("fY2In_%d",i));
    fY2In[i]->SetDirectory(0);
    fX2Y2In[i] = (TProfile*)list->FindObject(Form("fX2Y2_%d",i))->Clone(Form("fX2Y2In_%d",i));
    fX2Y2In[i]->SetDirectory(0);
    fCos8PsiIn[i] = (TProfile*)list->FindObject(Form("fCos8Psi_%d",i))->Clone(Form("fCos8PsiIn_%d",i));
    fCos8PsiIn[i]->SetDirectory(0);
  }
}
