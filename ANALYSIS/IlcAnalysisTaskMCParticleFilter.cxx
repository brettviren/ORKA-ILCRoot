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

//
//  Analysis task for Kinematic filtering
//  Fill AODtrackMC tracks from Kinematic stack
//
 
#include <TChain.h>
#include <TFile.h>
#include "TParticle.h"
#include "TParameter.h"
#include "TString.h"
#include "TList.h"
#include "TProfile.h"
#include "TH1F.h"


#include "IlcAnalysisTaskMCParticleFilter.h"
#include "IlcAnalysisManager.h"
#include "IlcAnalysisFilter.h"
#include "IlcHeader.h"
#include "IlcStack.h"
#include "IlcMCEvent.h"
#include "IlcMCEventHandler.h"
#include "IlcAODEvent.h"
#include "IlcAODHeader.h"
#include "IlcAODMCHeader.h"
#include "IlcAODHandler.h"
#include "IlcAODVertex.h"
#include "IlcAODMCParticle.h"
#include "IlcCollisionGeometry.h"
#include "IlcGenDPMjetEventHeader.h"
#include "IlcGenHijingEventHeader.h"
#include "IlcGenPythiaEventHeader.h"
#include "IlcGenCocktailEventHeader.h"

#include "IlcLog.h"

ClassImp(IlcAnalysisTaskMCParticleFilter)

////////////////////////////////////////////////////////////////////////

//____________________________________________________________________
IlcAnalysisTaskMCParticleFilter::IlcAnalysisTaskMCParticleFilter():
  IlcAnalysisTaskSE(),
  fTrackFilterMother(0x0),
  fAODMcHeader(0x0),
  fAODMcParticles(0x0),
  fHistList(0x0)
{
  // Default constructor
}

Bool_t IlcAnalysisTaskMCParticleFilter::Notify()
{
  //
  // Implemented Notify() to read the cross sections
  // from pyxsec.root
  // 
  TTree *tree = IlcAnalysisManager::GetAnalysisManager()->GetTree();
  Double_t xsection = 0;
  UInt_t   ntrials  = 0;
  if(tree){
    TFile *curfile = tree->GetCurrentFile();
    if (!curfile) {
      Error("Notify","No current file");
      return kFALSE;
    }

    TString fileName(curfile->GetName());
    if(fileName.Contains("IlcESDs.root")){
        fileName.ReplaceAll("IlcESDs.root", "pyxsec.root");
    }
    else if(fileName.Contains("IlcAOD.root")){
        fileName.ReplaceAll("IlcAOD.root", "pyxsec.root");
    }
    else if(fileName.Contains("gilc.root")){
        // for running with gilc and kinematics alone...                      
        fileName.ReplaceAll("gilc.root", "pyxsec.root");
    }


    TFile *fxsec = TFile::Open(fileName.Data());
    if(!fxsec){
      IlcInfo(Form("%s:%d %s not found in the Input",(char*)__FILE__,__LINE__,fileName.Data()));
      // not a severe condition we just do not have the information...
      return kTRUE;
    }
    TTree *xtree = (TTree*)fxsec->Get("Xsection");
    if(!xtree){
      IlcWarning(Form("%s:%d tree not found in the pyxsec.root",(char*)__FILE__,__LINE__));
      return kTRUE;
    }
    xtree->SetBranchAddress("xsection",&xsection);
    xtree->SetBranchAddress("ntrials",&ntrials);
    xtree->GetEntry(0);
    ((TProfile*)(fHistList->FindObject("h1Xsec")))->Fill("<#sigma>",xsection);
  }
  return kTRUE;
}



//____________________________________________________________________
IlcAnalysisTaskMCParticleFilter::IlcAnalysisTaskMCParticleFilter(const char* name):
    IlcAnalysisTaskSE(name),
    fTrackFilterMother(0x0),
    fAODMcHeader(0x0),
    fAODMcParticles(0x0),
    fHistList(0x0)
{
  // Default constructor
  DefineOutput(1, TList::Class());  
}

/*
//____________________________________________________________________
IlcAnalysisTaskMCParticleFilter::IlcAnalysisTaskMCParticleFilter(const IlcAnalysisTaskMCParticleFilter& obj):
    IlcAnalysisTaskSE(obj),
    fTrackFilterMother(obj.fTrackFilterMother)
{
  // Copy constructor
}
*/
//____________________________________________________________________
IlcAnalysisTaskMCParticleFilter::~IlcAnalysisTaskMCParticleFilter()
{

  if(fAODMcHeader){
    delete fAODMcHeader;
  }
  if(fAODMcParticles){
    fAODMcParticles->Delete();
    delete fAODMcParticles;
  }
}

/*
//____________________________________________________________________
IlcAnalysisTaskMCParticleFilter& IlcAnalysisTaskMCParticleFilter::operator=(const IlcAnalysisTaskMCParticleFilter& other)
{
// Assignment
  if(this!=&other) {
    IlcAnalysisTaskSE::operator=(other);
    fTrackFilterMother = other.fTrackFilterMother;
  }
  return *this;
}
*/
//____________________________________________________________________
void IlcAnalysisTaskMCParticleFilter::UserCreateOutputObjects()
{
  // Create the output container
  

    if (OutputTree()&&fTrackFilterMother) 
	OutputTree()->GetUserInfo()->Add(fTrackFilterMother);

    // this part is mainly needed to set the MCEventHandler
    // to the AODHandler, this will not be needed when
    // AODHandler goes to ANALYSISilc
    // setting in the steering macro will not work on proof :(
    // so we have to do it in a task

    // the branch booking can also go into the AODHandler then


    // mcparticles
    fAODMcParticles = new TClonesArray("IlcAODMCParticle", 0);
    fAODMcParticles->SetName(IlcAODMCParticle::StdBranchName());
    AddAODBranch("TClonesArray",&fAODMcParticles);

    // MC header...
    fAODMcHeader = new IlcAODMCHeader();
    fAODMcHeader->SetName(IlcAODMCHeader::StdBranchName());
    AddAODBranch("IlcAODMCHeader",&fAODMcHeader);    

    IlcMCEventHandler *mcH = (IlcMCEventHandler*) ((IlcAnalysisManager::GetAnalysisManager())->GetMCtruthEventHandler()); 
    IlcAODHandler *aodH = dynamic_cast<IlcAODHandler*> ((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());
    if(!aodH){
      IlcWarning("Could not get AODHandler");
      return;
    }
    aodH->SetMCEventHandler(mcH);


    // these are histograms, for averaging and summing
    // do it via histograms to be PROOF-proof
    // which merges the results from different workers
    // histos are not saved reside only in memory
    
    

    fHistList = new TList();
    TProfile *h1Xsec = new TProfile("h1Xsec","xsec from pyxsec.root",1,0,1);
    h1Xsec->GetXaxis()->SetBinLabel(1,"<#sigma>");
    fHistList->Add(h1Xsec);
    TH1F* h1Trials = new TH1F("h1Trials","trials from MC header",1,0,1);
    h1Trials->GetXaxis()->SetBinLabel(1,"#sum{ntrials}");
    fHistList->Add(h1Trials);

    PostData(1,fHistList);
    
}

//____________________________________________________________________
void IlcAnalysisTaskMCParticleFilter::UserExec(Option_t */*option*/)
{
  // Execute analysis for current event
  //
  
  // Fill AOD tracks from Kinematic stack
  PostData(1,fHistList);
  
  // get IlcAOD Event 
  IlcAODEvent* aod = AODEvent();
  if (!aod) {
      IlcWarning("No Output Handler connected, doing nothing !") ;
      return;
  }

  IlcMCEventHandler *mcH = (IlcMCEventHandler*) ((IlcAnalysisManager::GetAnalysisManager())->GetMCtruthEventHandler()); 
  if(!mcH){ 
    IlcWarning("No MC handler Found");
    return;
  }
  
  IlcAODHandler *aodH = dynamic_cast<IlcAODHandler*> ((IlcAnalysisManager::GetAnalysisManager())->GetOutputEventHandler());
  if(!aodH){
    IlcWarning("Could not get AODHandler");
    return;
  }



  // fetch the output 
  // Fill control histos

  // tmp array for holding the mctracks
  // need to set mother and duagther __before__
  // filling in the tree

  IlcMCEvent *mcE = MCEvent();

  if(!mcE){
    IlcWarning("No MC event Found");
    return;
  }

  Int_t np    = mcE->GetNumberOfTracks();
  Int_t nprim = mcE->GetNumberOfPrimaries();
  // TODO ADD MC VERTEX

  // Get the proper MC Collision Geometry
  IlcGenEventHeader* mcEH = mcE->GenEventHeader();

  IlcGenPythiaEventHeader *pyH  = dynamic_cast<IlcGenPythiaEventHeader*>(mcEH);
  IlcGenHijingEventHeader *hiH  = 0;
  IlcCollisionGeometry    *colG = 0;
  IlcGenDPMjetEventHeader *dpmH = 0;
  // it can be only one save some casts
  // assuming PYTHIA and HIJING are the most likely ones...
  if(!pyH){
      hiH = dynamic_cast<IlcGenHijingEventHeader*>(mcEH);
      if(!hiH){
	  dpmH = dynamic_cast<IlcGenDPMjetEventHeader*>(mcEH);
      }
  }
  
  if (hiH || dpmH) colG = dynamic_cast<IlcCollisionGeometry*>(mcEH);

  // fetch the trials on a event by event basis, not from pyxsec.root otherwise 
  // we will get a problem when running on proof since Notify may be called 
  // more than once per file
  // consider storing this information in the AOD output via IlcAODHandler
  Float_t ntrials = 0;
  if (!colG) {
    IlcGenCocktailEventHeader *ccEH = dynamic_cast<IlcGenCocktailEventHeader *>(mcEH);
    if (ccEH) {
      TList *genHeaders = ccEH->GetHeaders();
      for (int imch=0; imch<genHeaders->GetEntries(); imch++) {
	if(!pyH)pyH = dynamic_cast<IlcGenPythiaEventHeader*>(genHeaders->At(imch));
	if(!hiH)hiH = dynamic_cast<IlcGenHijingEventHeader*>(genHeaders->At(imch));
	if(!colG)colG = dynamic_cast<IlcCollisionGeometry *>(genHeaders->At(imch));
	if(!dpmH)dpmH = dynamic_cast<IlcGenDPMjetEventHeader*>(genHeaders->At(imch));
      }
    }
  }

  // take the trials from the p+p event
  if(hiH)ntrials = hiH->Trials();
  if(dpmH)ntrials = dpmH->Trials();
  if(pyH)ntrials = pyH->Trials();
  if(ntrials)((TH1F*)(fHistList->FindObject("h1Trials")))->Fill("#sum{ntrials}",ntrials); 
  



  if (colG) {
    fAODMcHeader->SetReactionPlaneAngle(colG->ReactionPlaneAngle());
    IlcInfo(Form("Found Collision Geometry. Got Reaction Plane %lf\n", colG->ReactionPlaneAngle()));
  }




  Int_t j=0;
  for (Int_t ip = 0; ip < np; ip++){
    IlcMCParticle* mcpart = (IlcMCParticle*) mcE->GetTrack(ip);
    TParticle* part = mcpart->Particle();
    Float_t xv = part->Vx();
    Float_t yv = part->Vy();
    Float_t zv = part->Vz();
    Float_t rv = TMath::Sqrt(xv * xv + yv * yv);
      
    Bool_t write = kFALSE;
    
    if (ip < nprim) {
      // Select the primary event
      write = kTRUE;
    } else if (part->GetUniqueID() == kPDecay) {
      // Particles from decay
      // Check that the decay chain ends at a primary particle
      IlcMCParticle* mother = mcpart;
      Int_t imo = mcpart->GetMother();
      while((imo >= nprim) && (mother->GetUniqueID() == kPDecay)) {
	mother =  (IlcMCParticle*) mcE->GetTrack(imo);
	imo =  mother->GetMother();
      }
      // Select according to pseudorapidity and production point of primary ancestor
      if (imo < nprim)write = kTRUE;         
      // if(!Select(((IlcMCParticle*) mcE->GetTrack(imo))->Particle(), rv, zv))write = kFALSE; // selection on eta and phi of the mother
    } else if (part->GetUniqueID() == kPPair) {
      // Now look for pair production
      Int_t imo = mcpart->GetMother();
      if (imo < nprim) {
	// Select, if the gamma is a primary
	write = kTRUE;
      } else {
	// Check if the gamma comes from the decay chain of a primary particle
	IlcMCParticle* mother =  (IlcMCParticle*) mcE->GetTrack(imo);
	imo = mother->GetMother();
	while((imo >= nprim) && (mother->GetUniqueID() == kPDecay)) {
	  mother =   (IlcMCParticle*) mcE->GetTrack(imo);
	  imo =  mother->GetMother();
	}
	// Select according to pseudorapidity and production point 
	if (imo < nprim && Select(mother->Particle(), rv, zv)) 
	  write = kTRUE;
      }
    }

    if (write) {
      if(mcH)mcH->SelectParticle(ip);
      j++;      
    }
  }

  /*

  // check varibales for charm need all daughters
  static int  iTaken = 0;
  static int  iAll = 0;
  static int  iCharm = 0;

  for (Int_t ip = 0; ip < np; ip++){
    IlcMCParticle* mcpart = (IlcMCParticle*) mcE->GetTrack(ip);
    TParticle* part = mcpart->Particle();
    // debug info to check fro charm daugthers
    if((TMath::Abs(part->GetPdgCode()))==411){
      iCharm++;
      Int_t d0 =  part->GetFirstDaughter();
      Int_t d1 =  part->GetLastDaughter();
      if(d0>0&&d1>0){
	for(int id = d0;id <= d1;id++){
	  iAll++;
	  if(mcH->IsParticleSelected(id)){
	    iTaken++;
	    Printf("> %d/%d Taken",id,nprim);
	    PrintMCParticle( (IlcMCParticle*)mcE->GetTrack(id),id);
	  }
	  else{
	    Printf("> %d/%d NOT Taken",id,nprim);
	    PrintMCParticle( (IlcMCParticle*)mcE->GetTrack(id),id);
	  }
	}
      }
    }// if charm
    IlcInfo(Form("Taken daughters %d/%d of %d charm",iTaken,iAll,iCharm));
  }
  */


  aodH->StoreMCParticles();

  return;
}

void IlcAnalysisTaskMCParticleFilter::Terminate(Option_t */*option*/){
  //
  // Terminate the execution save the PYTHIA x_section to the UserInfo()
  //


  fHistList = (TList*)GetOutputData(1);
  if(!fHistList){
    Printf("%s:%d Output list not found",(char*)__FILE__,__LINE__);
    return;
  }

  TProfile *hXsec = ((TProfile*)(fHistList->FindObject("h1Xsec")));
  TH1F* hTrials  = ((TH1F*)(fHistList->FindObject("h1Trials")));
  if(!hXsec)return;
  if(!hTrials)return;

  Float_t xsec = hXsec->GetBinContent(1);
  Float_t trials = hTrials->Integral();
  IlcInfo(Form("Average -section %.4E and total number of trials %E",xsec,trials));

}

Bool_t IlcAnalysisTaskMCParticleFilter::Select(TParticle* part, Float_t rv, Float_t zv)
{
  // Selection accoring to eta of the mother and production point
  // This has to be refined !!!!!!

  // Esp if we don't have collisison in the central barrel but e.g. beam gas
  //  return kTRUE;

  Float_t eta = part->Eta();

  // central barrel consider everything in the ITS...
  // large eta window for smeared vertex and SPD acceptance (2 at z = 0)
  // larger for V0s in the TPC
  //  if (TMath::Abs(eta) < 2.5 && rv < 250. && TMath::Abs(zv)<255)return kTRUE;

  if (TMath::Abs(eta) < 2.5 && rv < 170)return kTRUE;   

  // Andreas' Cuts
  //  if (TMath::Abs(eta) < 1. && rv < 170)return kTRUE;   



  // Muon arm
  if(eta > -4.2 && eta < -2.3 && zv > -500.)return kTRUE; // Muon arms

  // PMD acceptance 2.3 <= eta < = 3.5
  //  if(eta>2.0&&eta<3.7)return kTRUE; 

  return kFALSE;
 
}

void IlcAnalysisTaskMCParticleFilter::PrintMCParticle(const IlcMCParticle *mcp,Int_t np){
  
  const TParticle *p = mcp->Particle();
  Printf("Nr.%d --- Status %d ---- Mother1 %d Mother2 %d Daughter1 %d Daughte\
r2 %d ",
	 np,p->GetStatusCode(),p->GetMother(0),p->GetMother(1),p->GetDaughter \
	 (0),p->GetDaughter(1));
  Printf("Eta %3.3f Phi %3.3f ",p->Eta(),p->Phi());
  p->Print();
  Printf("---------------------------------------");
}
