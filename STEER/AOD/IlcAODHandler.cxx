
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

/* $Id: IlcAODHandler.cxx 58340 2012-08-28 15:56:24Z hristov $ */

//-------------------------------------------------------------------------
//     Implementation of the Virtual Event Handler Interface for AOD
//     Author: Andreas Morsch, CERN
//-------------------------------------------------------------------------


#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TList.h>
#include <TROOT.h>

#include "IlcLog.h"
#include "IlcAODHandler.h"
#include "IlcAODEvent.h"
#include "IlcAODExtension.h"
#include "IlcAODTracklets.h"
#include "IlcStack.h"
#include "IlcAODMCParticle.h"
#include "IlcAODMCHeader.h"
#include "IlcMCEventHandler.h"
#include "IlcMCEvent.h"
#include "IlcGenEventHeader.h"
#include "IlcGenHijingEventHeader.h"
#include "IlcGenDPMjetEventHeader.h"
#include "IlcGenPythiaEventHeader.h"
#include "IlcGenCocktailEventHeader.h"
#include "IlcCodeTimer.h"
#include "IlcAODBranchReplicator.h"
#include "Riostream.h"

ClassImp(IlcAODHandler)

//______________________________________________________________________________
IlcAODHandler::IlcAODHandler() :
    IlcVEventHandler(),
    fIsStandard(kTRUE),
    fFillAOD(kTRUE),
    fFillAODRun(kTRUE),
    fFillExtension(kTRUE),
    fNeedsHeaderReplication(kFALSE),
    fNeedsTracksBranchReplication(kFALSE),
    fNeedsVerticesBranchReplication(kFALSE),
    fNeedsV0sBranchReplication(kFALSE),
    fNeedsCascadesBranchReplication(kFALSE),
    fNeedsTrackletsBranchReplication(kFALSE),
    fNeedsPMDClustersBranchReplication(kFALSE),
    fNeedsJetsBranchReplication(kFALSE),
    fNeedsFMDClustersBranchReplication(kFALSE),
    fNeedsCaloClustersBranchReplication(kFALSE),
	fNeedsCaloTriggerBranchReplication(kFALSE),
    fNeedsMCParticlesBranchReplication(kFALSE),
    fNeedsDimuonsBranchReplication(kFALSE),
    fNeedsHMPIDBranchReplication(kFALSE),
    fAODIsReplicated(kFALSE),
    fAODEvent(NULL),
    fMCEventH(NULL),
    fTreeA(NULL),
    fFileA(NULL),
    fFileName(""),
    fExtensions(NULL),
    fFilters(NULL)
{
  // default constructor
}

//______________________________________________________________________________
IlcAODHandler::IlcAODHandler(const char* name, const char* title):
    IlcVEventHandler(name, title),
    fIsStandard(kTRUE),
    fFillAOD(kTRUE),
    fFillAODRun(kTRUE),
    fFillExtension(kTRUE),
    fNeedsHeaderReplication(kFALSE),
    fNeedsTracksBranchReplication(kFALSE),
    fNeedsVerticesBranchReplication(kFALSE),
    fNeedsV0sBranchReplication(kFALSE),
    fNeedsCascadesBranchReplication(kFALSE),
    fNeedsTrackletsBranchReplication(kFALSE),
    fNeedsPMDClustersBranchReplication(kFALSE),
    fNeedsJetsBranchReplication(kFALSE),
    fNeedsFMDClustersBranchReplication(kFALSE),
    fNeedsCaloClustersBranchReplication(kFALSE),
    fNeedsCaloTriggerBranchReplication(kFALSE),
    fNeedsMCParticlesBranchReplication(kFALSE),
    fNeedsDimuonsBranchReplication(kFALSE),
    fNeedsHMPIDBranchReplication(kFALSE),
    fAODIsReplicated(kFALSE),
    fAODEvent(NULL),
    fMCEventH(NULL),
    fTreeA(NULL),
    fFileA(NULL),
    fFileName(""),
    fExtensions(NULL),
    fFilters(NULL)
{
// Normal constructor.
}

//______________________________________________________________________________
IlcAODHandler::~IlcAODHandler() 
{
 // Destructor.
  
  delete fAODEvent;

  if (fFileA) fFileA->Close();

  delete fFileA;
  delete fTreeA;
  delete fExtensions;
  delete fFilters;
}

//______________________________________________________________________________
Bool_t IlcAODHandler::Init(Option_t* opt)
{
  // Initialize IO
  //
  // Create the AODevent object
    
  Bool_t createStdAOD = fIsStandard || fFillAOD;
  if(!fAODEvent && createStdAOD){
    fAODEvent = new IlcAODEvent();
    if (fIsStandard) 
      fAODEvent->CreateStdContent();
  }
  //
  // File opening according to execution mode
  TString option(opt);
  option.ToLower();
  if (createStdAOD) {
    TDirectory *owd = gDirectory;
    if (option.Contains("proof")) {
      // proof
      // Merging via files. Need to access analysis manager via interpreter.
      gROOT->ProcessLine(Form("IlcAnalysisDataContainer *c_common_out = IlcAnalysisManager::GetAnalysisManager()->GetCommonOutputContainer();"));
      gROOT->ProcessLine(Form("IlcAnalysisManager::GetAnalysisManager()->OpenProofFile(c_common_out, \"RECREATE\");"));
      fFileA = gFile;
    } else {
      // local and grid
      fFileA = new TFile(fFileName.Data(), "RECREATE");
    }
    CreateTree(1);
    owd->cd();
  }  
  if (fExtensions) {
     TIter next(fExtensions);
     IlcAODExtension *ext;
     while ((ext=(IlcAODExtension*)next())) ext->Init(option);
  }   
  if (fFilters) {
     TIter nextf(fFilters);
     IlcAODExtension *filteredAOD;
     while ((filteredAOD=(IlcAODExtension*)nextf())) {
        filteredAOD->SetEvent(fAODEvent);
        filteredAOD->Init(option);
     }
  }   
  
  return kTRUE;
}

//______________________________________________________________________________
void IlcAODHandler::Print(Option_t* opt) const
{
  // Print info about this object

  cout << opt << Form("IsStandard %d filename=%s",fIsStandard,fFileName.Data()) << endl;
  
  if ( fExtensions ) 
  {
    cout << opt << fExtensions->GetEntries() << " extensions :" << endl;
    PrintExtensions(*fExtensions);    
  }
  if ( fFilters ) 
  {
    cout << opt << fFilters->GetEntries() << " filters :" << endl;
    PrintExtensions(*fFilters);      
  }
}

//______________________________________________________________________________
void IlcAODHandler::PrintExtensions(const TObjArray& array) const
{
  // Show the list of aod extensions
  TIter next(&array);
  IlcAODExtension* ext(0x0);
  while ( ( ext = static_cast<IlcAODExtension*>(next()) ) )
  {
    ext->Print("   ");
  }
}

//______________________________________________________________________________
void IlcAODHandler::StoreMCParticles(){

  // 
  // Remap the labels from ESD stack and store
  // the AODMCParticles, makes only sense if we have
  // the mcparticles branch
  // has to be done here since we cannot know in advance 
  // which particles are needed (e.g. by the tracks etc.)
  //
  // Particles have been selected by IlcMCEventhanlder->SelectParticle()
  // To use the MCEventhandler here we need to set it from the outside
  // can vanish when Handler go to the ANALYSISilc library
  //
  // The Branch booking for mcParticles and mcHeader has to happen 
  // in an external task for now since the AODHandler does not have access
  // the AnalysisManager. For the same reason the pointer t o the MCEventH
  // has to passed to the AOD Handler by this task 
  // (doing this in the steering macro would not work on PROOF)

  if (!fAODEvent) return;
  TClonesArray *mcarray = (TClonesArray*)fAODEvent->FindListObject(IlcAODMCParticle::StdBranchName()); 
  if(!mcarray)return;

  IlcAODMCHeader *mcHeader = (IlcAODMCHeader*)fAODEvent->FindListObject(IlcAODMCHeader::StdBranchName()); 
  if(!mcHeader)return;

  // Get the MC Infos.. Handler needs to be set before 
  // while adding the branch
  // This needs to be done, not to depend on the AnalysisManager

  if(!fMCEventH)return;
  if(!fMCEventH->MCEvent())return;
  IlcStack *pStack = fMCEventH->MCEvent()->Stack();
  if(!pStack)return;

  fMCEventH->CreateLabelMap();

  //
  // Get the Event Header 
  // 

  IlcHeader* header = fMCEventH->MCEvent()->Header();
   // get the MC vertex
  IlcGenEventHeader* genHeader = 0;
  if (header) genHeader = header->GenEventHeader();
  if (genHeader) {
      TArrayF vtxMC(3);
      genHeader->PrimaryVertex(vtxMC);
      mcHeader->SetVertex(vtxMC[0],vtxMC[1],vtxMC[2]);
      // we search the MCEventHeaders first 
      // Two cases, cocktail or not...
      IlcGenCocktailEventHeader* genCocktailHeader = dynamic_cast<IlcGenCocktailEventHeader*>(genHeader);
      if(genCocktailHeader){
	  // we have a coktail header add the name once
	  mcHeader->AddGeneratorName(genHeader->GetName());
	  TList* headerList = genCocktailHeader->GetHeaders();
	  // the first entry defines some extra general settings
	  IlcGenEventHeader *headerEntry = dynamic_cast<IlcGenEventHeader*>(headerList->At(0));
	  if (!headerEntry) {
	    IlcFatal("IlcGenEventHeader entry not found in the header list");
	  } else {   
	    SetMCHeaderInfo(mcHeader,headerEntry);
	  }  
      }
      else{
	// No Cocktail just take the first one
	SetMCHeaderInfo(mcHeader,genHeader);
      }
      // Add all the headers and names, if no cocktail header 
      // there will be only one entry
      mcHeader->AddCocktailHeaders(genHeader);
  }
  




  // Store the IlcAODParticlesMC
  IlcMCEvent* mcEvent = fMCEventH->MCEvent();
  
  Int_t np    = mcEvent->GetNumberOfTracks();
  Int_t nprim = mcEvent->GetNumberOfPrimaries();


  Int_t j = 0;
  TClonesArray& l = *mcarray;

  for(int i = 0; i < np; ++i){
      if(fMCEventH->IsParticleSelected(i)){
	  Int_t flag = 0;
	  IlcMCParticle* mcpart =  (IlcMCParticle*) mcEvent->GetTrack(i);
	  if(i<nprim)flag |= IlcAODMCParticle::kPrimary;
	  
	  if(mcEvent->IsPhysicalPrimary(i))flag |= IlcAODMCParticle::kPhysicalPrim;
	  
	  if(fMCEventH->GetNewLabel(i)!=j){
	      IlcError(Form("MISMATCH New label %d j: %d",fMCEventH->GetNewLabel(i),j));
	  }

	  IlcAODMCParticle mcpartTmp(mcpart,i,flag);
	  
	  mcpartTmp.SetStatus(mcpart->Particle()->GetStatusCode());
	  // 
	  Int_t d0 =  mcpartTmp.GetDaughter(0);
	  Int_t d1 =  mcpartTmp.GetDaughter(1);
	  Int_t m =   mcpartTmp.GetMother();
	  
	  // other than for the track labels, negative values mean
	  // no daughter/mother so preserve it
	  
	  if(d0<0 && d1<0){
	      // no first daughter -> no second daughter
	      // nothing to be done
	      // second condition not needed just for sanity check at the end
	      mcpartTmp.SetDaughter(0,d0);
	      mcpartTmp.SetDaughter(1,d1);
	  } else if(d1 < 0 && d0 >= 0) {
	      // Only one daughter
	      // second condition not needed just for sanity check at the end
	      if(fMCEventH->IsParticleSelected(d0)){
		  mcpartTmp.SetDaughter(0,fMCEventH->GetNewLabel(d0));
	      } else {
		  mcpartTmp.SetDaughter(0,-1);
	      }
	      mcpartTmp.SetDaughter(1,d1);
	  }
	  else if (d0 > 0 && d1 > 0 ){
	      // we have two or more daughters loop on the stack to see if they are
	      // selected
	      Int_t d0Tmp = -1;
	      Int_t d1Tmp = -1;
	      for(int id = d0; id<=d1;++id){
		  if(fMCEventH->IsParticleSelected(id)){
		      if(d0Tmp==-1){
			  // first time
			  d0Tmp = fMCEventH->GetNewLabel(id);
			  d1Tmp = d0Tmp; // this is to have the same schema as on the stack i.e. with one daugther d0 and d1 are the same 
		      }
		      else d1Tmp = fMCEventH->GetNewLabel(id);
		  }
	      }
	      mcpartTmp.SetDaughter(0,d0Tmp);
	      mcpartTmp.SetDaughter(1,d1Tmp);
	  } else {
	      IlcError(Form("Unxpected indices %d %d",d0,d1));
	  }
	  
	  if(m<0){
	      mcpartTmp.SetMother(m);
	  } else {
	      if(fMCEventH->IsParticleSelected(m))mcpartTmp.SetMother(fMCEventH->GetNewLabel(m));
	      else IlcError(Form("PROBLEM Mother not selected %d \n", m));
	  }

	  new (l[j++]) IlcAODMCParticle(mcpartTmp);
	  
      }
  }
  IlcInfo(Form("IlcAODHandler::StoreMCParticles: Selected %d (Primaries %d / total %d) after validation \n",
	       j,nprim,np));
  
  // Set the labels in the AOD output...
  // Remapping

  // AODTracks
  TClonesArray* tracks = fAODEvent->GetTracks();
  if(tracks){
    for(int it = 0; it < fAODEvent->GetNTracks();++it){
      IlcAODTrack *track = fAODEvent->GetTrack(it);
      
      Int_t sign = 1;
      Int_t label = track->GetLabel();
      if(label<0){ // preserve the sign for later usage
	label *= -1;
	sign  = -1;
      }

      if (label >= IlcMCEvent::BgLabelOffset()) label =  mcEvent->BgLabelToIndex(label);
      if(label > np || track->GetLabel() == 0){
	IlcWarning(Form("Wrong ESD track label %5d (%5d)",track->GetLabel(), label));
      }
      if(fMCEventH->GetNewLabel(label) == 0){
	IlcWarning(Form("New label not found for %5d (%5d)",track->GetLabel(), label));
      }
      track->SetLabel(sign*fMCEventH->GetNewLabel(label));
    }
  }
  
  // AOD calo cluster
  TClonesArray *clusters = fAODEvent->GetCaloClusters();
  if(clusters){
    for (Int_t iClust = 0;iClust < fAODEvent->GetNumberOfCaloClusters(); ++iClust) {
      IlcAODCaloCluster * cluster = fAODEvent->GetCaloCluster(iClust);
      UInt_t nLabel    = cluster->GetNLabels();
      // Ugly but do not want to fragment memory by creating 
      // new Int_t (nLabel)
      Int_t* labels    = const_cast<Int_t*>(cluster->GetLabels());
      if (labels){
	for(UInt_t i = 0;i < nLabel;++i){
	  labels[i] = fMCEventH->GetNewLabel(cluster->GetLabelAt(i));
	}
      }
      //      cluster->SetLabels(labels,nLabel);
    }// iClust
  }// clusters

  // AOD tracklets
  IlcAODTracklets *tracklets = fAODEvent->GetTracklets();
  if(tracklets){
    for(int it = 0;it < tracklets->GetNumberOfTracklets();++it){
      int label0 = tracklets->GetLabel(it,0);
      int label1 = tracklets->GetLabel(it,1);
      if(label0>=0)label0 = fMCEventH->GetNewLabel(label0);      
      if(label1>=0)label1 = fMCEventH->GetNewLabel(label1);
      tracklets->SetLabel(it,0,label0);
      tracklets->SetLabel(it,1,label1);
    }
  }

}

//______________________________________________________________________________
Bool_t IlcAODHandler::FinishEvent()
{
  // Fill data structures
  if(fFillAOD && fFillAODRun && fAODEvent){
      fAODEvent->MakeEntriesReferencable();
      fTreeA->BranchRef();
      FillTree();
  }

  if ((fFillAOD && fFillAODRun) || fFillExtension) {
    if (fExtensions && fFillExtension) {
      // fFillExtension can be set by the ESD filter or by a delta filter in case of AOD inputs
      TIter next(fExtensions);
      IlcAODExtension *ext;
      while ((ext=(IlcAODExtension*)next())) ext->FinishEvent();
    }
    if (fFilters && fFillAOD && fFillAODRun) {
      TIter nextf(fFilters);
      IlcAODExtension *ext;
      while ((ext=(IlcAODExtension*)nextf())) {
	      ext->FinishEvent();
      }  
    }       
  }  
  
  if (fIsStandard && fAODEvent) 
  {
    fAODEvent->ResetStd();    
  }
  
  if (fAODEvent) 
  {
    TClonesArray *mcarray = static_cast<TClonesArray*>(fAODEvent->FindListObject(IlcAODMCParticle::StdBranchName()));
    if(mcarray) mcarray->Delete();
    
    IlcAODMCHeader *mcHeader = static_cast<IlcAODMCHeader*>(fAODEvent->FindListObject(IlcAODMCHeader::StdBranchName()));
    if(mcHeader) mcHeader->Reset();
  }
  
  // Reset AOD replication flag
  fAODIsReplicated = kFALSE;
  return kTRUE;
}

//______________________________________________________________________________
Bool_t IlcAODHandler::Terminate()
{
  // Terminate 
  AddAODtoTreeUserInfo();
  
  TIter nextF(fFilters);
  IlcAODExtension *ext;
  while ((ext=static_cast<IlcAODExtension*>(nextF())))
  {
    ext->AddAODtoTreeUserInfo();
  }

  TIter nextE(fExtensions);
  while ((ext=static_cast<IlcAODExtension*>(nextE())))
  {
    ext->AddAODtoTreeUserInfo();
  }
  
  return kTRUE;
}

//______________________________________________________________________________
Bool_t IlcAODHandler::TerminateIO()
{
  // Terminate IO
  if (fFileA) {
    fFileA->Write();
    fFileA->Close();
    delete fFileA;
    fFileA = 0;
    // When closing the file, the tree is also deleted.
    fTreeA = 0;
  }
  
  TIter nextF(fFilters);
  IlcAODExtension *ext;
  while ((ext=static_cast<IlcAODExtension*>(nextF())))
  {
    ext->TerminateIO();
  }  

  TIter nextE(fExtensions);
  while ((ext=static_cast<IlcAODExtension*>(nextE())))
  {
    ext->TerminateIO();
  }  
  
  return kTRUE;
}

//______________________________________________________________________________
void IlcAODHandler::CreateTree(Int_t flag)
{
    // Creates the AOD Tree
    fTreeA = new TTree("aodTree", "IlcAOD tree");
    fTreeA->Branch(fAODEvent->GetList());
    if (flag == 0) fTreeA->SetDirectory(0);
}

//______________________________________________________________________________
void IlcAODHandler::FillTree()
{
 
    // Fill the AOD Tree
    fTreeA->Fill();
}

//______________________________________________________________________________
void IlcAODHandler::AddAODtoTreeUserInfo()
{
  // Add aod event to tree user info
  if (fTreeA) fTreeA->GetUserInfo()->Add(fAODEvent);
  // Now the tree owns our fAODEvent...
  fAODEvent = 0;
}

//______________________________________________________________________________
void IlcAODHandler::AddBranch(const char* cname, void* addobj, const char* filename)
{
  // Add a new branch to the aod. Added optional filename parameter if the
  // branch should be written to a separate file.
  
  if (strlen(filename)) 
  {
    IlcAODExtension *ext = AddExtension(filename);
    ext->AddBranch(cname, addobj);
    return;
  } 
  
  // Add branch to all filters
  // Add branch to all filters
  if (fFilters) {
    TIter next(fFilters);
    IlcAODExtension *ext;
    while ((ext=(IlcAODExtension*)next())) ext->AddBranch(cname, addobj);
  }
  
  TDirectory *owd = gDirectory;
  if (fFileA) 
  {
    fFileA->cd();
  }

  char** apointer = (char**) addobj;
  TObject* obj = (TObject*) *apointer;
  
  fAODEvent->AddObject(obj);
  
  const Int_t kSplitlevel = 99; // default value in TTree::Branch()
  const Int_t kBufsize = 32000; // default value in TTree::Branch()
  
  if (!fTreeA->FindBranch(obj->GetName())) 
  {
    // Do the same as if we book via 
    // TTree::Branch(TCollection*)
    
    fTreeA->Bronch(obj->GetName(), cname, fAODEvent->GetList()->GetObjectRef(obj),
                   kBufsize, kSplitlevel - 1);
  }
  owd->cd();
}

//______________________________________________________________________________
IlcAODExtension *IlcAODHandler::AddExtension(const char *filename, const char *title)
{
  // Add an AOD extension with some branches in a different file.
  
  TString fname(filename);
  if (!fname.EndsWith(".root")) fname += ".root";
  if (!fExtensions) {
    fExtensions = new TObjArray();
    fExtensions->SetOwner();
  }   
  IlcAODExtension *ext = (IlcAODExtension*)fExtensions->FindObject(fname);
  if (!ext) {
    ext = new IlcAODExtension(fname, title);
    fExtensions->Add(ext);
  }   
  return ext;
}

//______________________________________________________________________________
IlcAODExtension *IlcAODHandler::GetExtension(const char *filename) const
{
  // Getter for AOD extensions via file name.
  if (!fExtensions) return NULL;
  return (IlcAODExtension*)fExtensions->FindObject(filename);
}   

//______________________________________________________________________________
IlcAODExtension *IlcAODHandler::AddFilteredAOD(const char *filename, const char *filtername)
{
  // Add an AOD extension that can write only AOD events that pass a user filter.
  if (!fFilters) {
    fFilters = new TObjArray();
    fFilters->SetOwner();
  } 
  IlcAODExtension *filter = (IlcAODExtension*)fFilters->FindObject(filename);
  if (!filter) {
    filter = new IlcAODExtension(filename, filtername, kTRUE);
    fFilters->Add(filter);
  }
  return filter;
}      

//______________________________________________________________________________
IlcAODExtension *IlcAODHandler::GetFilteredAOD(const char *filename) const
{
  // Getter for AOD filters via file name.
  if (!fFilters) return NULL;
  return (IlcAODExtension*)fFilters->FindObject(filename);
}   

//______________________________________________________________________________
void IlcAODHandler::SetOutputFileName(const char* fname)
{
// Set file name.
   fFileName = fname;
}

//______________________________________________________________________________
const char *IlcAODHandler::GetOutputFileName()
{
// Get file name.
   return fFileName.Data();
}

//______________________________________________________________________________
const char *IlcAODHandler::GetExtraOutputs() const
{
  // Get extra outputs as a string separated by commas.
  static TString eoutputs;
  eoutputs = "";
  TObject *obj;
  if (fExtensions) {
    TIter next1(fExtensions);
    while ((obj=next1())) {
      if (!eoutputs.IsNull()) eoutputs += ",";
      eoutputs += obj->GetName();
    }
  }
  if (fFilters) {
    TIter next2(fFilters);
    while ((obj=next2())) {
      if (!eoutputs.IsNull()) eoutputs += ",";
      eoutputs += obj->GetName();
    }
  }
  return eoutputs.Data();
}

//______________________________________________________________________________
Bool_t IlcAODHandler::HasExtensions() const
{
  // Whether or not we manage extensions
  
  if ( fExtensions && fExtensions->GetEntries()>0 ) return kTRUE;
  
  return kFALSE;
}

//______________________________________________________________________________
void  IlcAODHandler::SetMCHeaderInfo(IlcAODMCHeader *mcHeader,IlcGenEventHeader *genHeader){


  // Utility function to cover different cases for the IlcGenEventHeader
  // Needed since different ProcessType and ImpactParamter are not 
  // in the base class...

  if(!genHeader)return;
  IlcGenPythiaEventHeader *pythiaGenHeader = dynamic_cast<IlcGenPythiaEventHeader*>(genHeader);
  if (pythiaGenHeader) {
    mcHeader->SetEventType(pythiaGenHeader->ProcessType());
    mcHeader->SetPtHard(pythiaGenHeader->GetPtHard());
    return;
  }
  
  IlcGenDPMjetEventHeader* dpmJetGenHeader = dynamic_cast<IlcGenDPMjetEventHeader*>(genHeader);
  
  if (dpmJetGenHeader){
    mcHeader->SetEventType(dpmJetGenHeader->ProcessType());
    return;
  } 

  IlcGenHijingEventHeader* hijingGenHeader = dynamic_cast<IlcGenHijingEventHeader*>(genHeader);
  if(hijingGenHeader){
    mcHeader->SetImpactParameter(hijingGenHeader->ImpactParameter());
    return;
  }
  
  //  IlcWarning(Form("MC Eventheader not known: %s",genHeader->GetName()));
  
}

