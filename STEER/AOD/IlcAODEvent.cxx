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

/* $Id: IlcAODEvent.cxx 58340 2012-08-28 15:56:24Z hristov $ */

//-------------------------------------------------------------------------
//     AOD base class
//     Author: Markus Oldenburg, CERN
//-------------------------------------------------------------------------

#include <TROOT.h>
#include <TTree.h>
#include <TFolder.h>
#include <TFriendElement.h>
#include <TProcessID.h>
#include <TCollection.h>
#include "Riostream.h"
#include "IlcAODEvent.h"
#include "IlcAODHeader.h"
#include "IlcAODTrack.h"
#include "IlcAODDimuon.h"

ClassImp(IlcAODEvent)

// definition of std AOD member names
  const char* IlcAODEvent::fAODListName[kAODListN] = {"header",
						      "tracks",
						      "vertices",
						      "v0s",
						      "cascades",
						      "tracklets",
						      "jets",
						      "emcalCells",
						      "phosCells",
									"caloClusters",
									"emcalTrigger",
									"phosTrigger",
						      "fmdClusters",
						      "pmdClusters",
                                                      "hmpidRings",
						      "dimuons",
						      "IlcAODTZERO",
						      "IlcAODVZERO",
						      "IlcAODZDC",
						      "IlcTOFHeader"
#ifdef MFT_UPGRADE	  
						      ,"IlcAODMFT"
#endif						      
						      
};
//______________________________________________________________________________
IlcAODEvent::IlcAODEvent() :
  IlcVEvent(),
  fAODObjects(0),
  fAODFolder(0),
  fConnected(kFALSE),
  fHeader(0),
  fTracks(0),
  fVertices(0),
  fV0s(0),
  fCascades(0),
  fTracklets(0),
  fJets(0),
  fEmcalCells(0),
  fPhosCells(0),
  fCaloClusters(0),
  fEMCALTrigger(0),
  fPHOSTrigger(0),
  fFmdClusters(0),
  fPmdClusters(0),
  fHMPIDrings(0),
  fDimuons(0),
  fAODTZERO(0),
  fAODVZERO(0),
  fAODZDC(0),
  fTOFHeader(0)
#ifdef MFT_UPGRADE
  ,fAODMFT(0)
#endif
{
  // default constructor
  if (TClass::IsCallingNew() != TClass::kDummyNew) fAODObjects = new TList();
}

//______________________________________________________________________________
IlcAODEvent::IlcAODEvent(const IlcAODEvent& aod):
  IlcVEvent(aod),
  fAODObjects(new TList()),
  fAODFolder(0),
  fConnected(kFALSE),
  fHeader(new IlcAODHeader(*aod.fHeader)),
  fTracks(new TClonesArray(*aod.fTracks)),
  fVertices(new TClonesArray(*aod.fVertices)),
  fV0s(new TClonesArray(*aod.fV0s)),
  fCascades(new TClonesArray(*aod.fCascades)),
  fTracklets(new IlcAODTracklets(*aod.fTracklets)),
  fJets(new TClonesArray(*aod.fJets)),
  fEmcalCells(new IlcAODCaloCells(*aod.fEmcalCells)),
  fPhosCells(new IlcAODCaloCells(*aod.fPhosCells)),
  fCaloClusters(new TClonesArray(*aod.fCaloClusters)),
  fEMCALTrigger(new IlcAODCaloTrigger(*aod.fEMCALTrigger)),
  fPHOSTrigger(new IlcAODCaloTrigger(*aod.fPHOSTrigger)),
  fFmdClusters(new TClonesArray(*aod.fFmdClusters)),
  fPmdClusters(new TClonesArray(*aod.fPmdClusters)),
  fHMPIDrings(new TClonesArray(*aod.fHMPIDrings)),
  fDimuons(new TClonesArray(*aod.fDimuons)),
  fAODTZERO(new IlcAODTZERO(*aod.fAODTZERO)),
  fAODVZERO(new IlcAODVZERO(*aod.fAODVZERO)),
  fAODZDC(new IlcAODZDC(*aod.fAODZDC)),
  fTOFHeader(new IlcTOFHeader(*aod.fTOFHeader))
#ifdef MFT_UPGRADE
  ,fAODMFT(new IlcAODMFT(*aod.fAODMFT))
#endif
{
  // Copy constructor
  AddObject(fHeader);
  AddObject(fTracks);
  AddObject(fVertices);
  AddObject(fV0s);
  AddObject(fCascades);
  AddObject(fTracklets);
  AddObject(fJets);
  AddObject(fEmcalCells);
  AddObject(fPhosCells);
  AddObject(fCaloClusters);
  AddObject(fEMCALTrigger);
  AddObject(fPHOSTrigger);
  AddObject(fFmdClusters);
  AddObject(fPmdClusters);
  AddObject(fHMPIDrings);
  AddObject(fDimuons);
  AddObject(fAODTZERO);
  AddObject(fAODVZERO);
  AddObject(fAODZDC);
  AddObject(fTOFHeader);
#ifdef MFT_UPGRADE	
  AddObject(fAODVZERO);
#endif
  fConnected = aod.fConnected;
  GetStdContent();
  CreateStdFolders();
}

//______________________________________________________________________________
IlcAODEvent & IlcAODEvent::operator=(const IlcAODEvent& aod) {

    // Assignment operator

  if(&aod == this) return *this;
  IlcVEvent::operator=(aod);

  // This assumes that the list is already created
  // and that the virtual void Copy(Tobject&) function
  // is correctly implemented in the derived class
  // otherwise only TObject::Copy() will be used

  if((fAODObjects->GetSize()==0)&&(aod.fAODObjects->GetSize()>=kAODListN)){
    // We cover the case that we do not yet have the 
    // standard content but the source has it
    CreateStdContent();
  }
  
  // Here we have the standard content without user additions, but the content is 
  // not matching the aod source.
  
  // Iterate the list of source objects
  TIter next(aod.GetList());
  TObject *its = 0;
  TString name;
  while ((its = next())) {
    name = its->GetName();
    // Check if we have this object type in out list
    TObject *mine = fAODObjects->FindObject(name);    
    if(!mine) {
      // We have to create the same type of object.
      TClass* pClass=TClass::GetClass(its->ClassName());     
      if (!pClass) {
        IlcWarning(Form("Can not find class description for entry %s (%s)\n",
                   its->ClassName(), name.Data()));
        continue;
      }
      mine=(TObject*)pClass->New();
      if(!mine){
        // not in this: can be added to list
        IlcWarning(Form("%s:%d Could not find %s for copying \n",
                   (char*)__FILE__,__LINE__,name.Data()));
        continue;
      }  
      if(mine->InheritsFrom("TNamed"))	{
        ((TNamed*)mine)->SetName(name);
      } else if(mine->InheritsFrom("TCollection")){
        if(mine->InheritsFrom("TClonesArray")) {
          TClonesArray *itscl = dynamic_cast<TClonesArray*>(its);
          if (!itscl) {
            IlcWarning(Form("Class description for entry %s (%s) not TClonesArray\n",
                   its->ClassName(), name.Data()));
            continue;
          
          }
	       dynamic_cast<TClonesArray*>(mine)->SetClass(itscl->GetClass(), itscl->GetSize());
        }
        dynamic_cast<TCollection*>(mine)->SetName(name);
      }
      IlcDebug(1, Form("adding object %s of type %s", mine->GetName(), mine->ClassName()));
      AddObject(mine);
    }
    // Now we have an object of the same type and name, but different content.        
    if(!its->InheritsFrom("TCollection")){
      // simple objects (do they have a Copy method that calls operator= ?)
      its->Copy(*mine);
    } else if (its->InheritsFrom("TClonesArray")) {
      // Create or expand the tclonesarray pointers
      // so we can directly copy to the object
      TClonesArray *its_tca = (TClonesArray*)its;
      TClonesArray *mine_tca = (TClonesArray*)mine;
      // this leaves the capacity of the TClonesArray the same
      // except for a factor of 2 increase when size > capacity
      // does not release any memory occupied by the tca
      Int_t its_entries = its_tca->GetEntriesFast();
      mine_tca->ExpandCreate(its_entries);
      for(int i=0; i<its_entries; i++){
        // copy 
        TObject *mine_tca_obj = mine_tca->At(i);
        TObject *its_tca_obj = its_tca->At(i);
        // no need to delete first
        // pointers within the class should be handled by Copy()...
        // Can there be Empty slots?
        its_tca_obj->Copy(*mine_tca_obj);
      }
    } else {
      IlcWarning(Form("%s:%d cannot copy TCollection \n",
		      (char*)__FILE__,__LINE__));
    }
  }  
  fConnected = aod.fConnected;
  return *this;
}


//______________________________________________________________________________
IlcAODEvent::~IlcAODEvent() 
{
// destructor
    delete fAODFolder;
    fAODFolder = 0;
    if(!fConnected) {
//       fAODObjects->Delete("slow");
       delete fAODObjects;
    }   
}

//______________________________________________________________________________
void IlcAODEvent::AddObject(TObject* obj) 
{
  // Add an object to the list of objects.
  // Please be aware that in order to increase performance you should
  // refrain from using TObjArrays (if possible). Use TClonesArrays, instead.
  
//  if ( !fAODObjects ) {
//     fAODObjects = new TList();
//     fAODObjects->SetOwner();
//  }
  if ( !fAODObjects->FindObject(obj) ) 
  {
    fAODObjects->AddLast(obj);
  }
}

//______________________________________________________________________________
void IlcAODEvent::RemoveObject(TObject* obj) 
{
  // Removes an object from the list of objects.
  
  fAODObjects->Remove(obj);
}

//______________________________________________________________________________
TObject *IlcAODEvent::FindListObject(const char *objName) const
{
  // Return the pointer to the object with the given name.

  return fAODObjects->FindObject(objName);
}

//______________________________________________________________________________
void IlcAODEvent::CreateStdContent() 
{
  // create the standard AOD content and set pointers

  // create standard objects and add them to the TList of objects
  AddObject(new IlcAODHeader());
  AddObject(new TClonesArray("IlcAODTrack", 0));
  AddObject(new TClonesArray("IlcAODVertex", 0));
  AddObject(new TClonesArray("IlcAODv0", 0));
  AddObject(new TClonesArray("IlcAODcascade", 0));
  AddObject(new IlcAODTracklets());
  AddObject(new TClonesArray("IlcAODJet", 0));
  AddObject(new IlcAODCaloCells());
  AddObject(new IlcAODCaloCells());
  AddObject(new TClonesArray("IlcAODCaloCluster", 0));
  AddObject(new IlcAODCaloTrigger()); // EMCAL 
  AddObject(new IlcAODCaloTrigger()); // PHOS
  AddObject(new TClonesArray("IlcAODFmdCluster", 0));
  AddObject(new TClonesArray("IlcAODPmdCluster", 0));
  AddObject(new TClonesArray("IlcAODHMPIDrings", 0));
  AddObject(new TClonesArray("IlcAODDimuon", 0));
  AddObject(new IlcAODTZERO());
  AddObject(new IlcAODVZERO());
  AddObject(new IlcAODZDC());
  AddObject(new IlcTOFHeader());
#ifdef MFT_UPGRADE
  AddObject(new IlcAODMFT());
#endif
  // set names
  SetStdNames();

  // read back pointers
  GetStdContent();
  CreateStdFolders();
  return;
}

void  IlcAODEvent::MakeEntriesReferencable()
{
    // Make all entries referencable in a subsequent process
    //
    TIter next(fAODObjects);
    TObject* obj;
    while ((obj = next()))
    {
	if(obj->InheritsFrom("TCollection"))
	    {
		AssignIDtoCollection((TCollection*)obj);
	    }
    }
}

//______________________________________________________________________________
void IlcAODEvent::SetStdNames()
{
  // introduce the standard naming

  if(fAODObjects->GetEntries()==kAODListN){
    for(int i = 0;i < fAODObjects->GetEntries();i++){
      TObject *fObj = fAODObjects->At(i);
      if(fObj->InheritsFrom("TNamed")){
	((TNamed*)fObj)->SetName(fAODListName[i]);
      }
      else if(fObj->InheritsFrom("TClonesArray")){
	((TClonesArray*)fObj)->SetName(fAODListName[i]);
      }
    }
  }
  else{
    printf("%s:%d SetStdNames() Wrong number of Std Entries \n",(char*)__FILE__,__LINE__);
  }
} 

void IlcAODEvent::CreateStdFolders()
{
    // Create the standard folder structure
  if(fAODFolder)delete fAODFolder;
    fAODFolder = gROOT->GetRootFolder()->AddFolder("AOD", "AOD");
    if(fAODObjects->GetEntries()==kAODListN){
	for(int i = 0;i < fAODObjects->GetEntries();i++){
	    TObject *fObj = fAODObjects->At(i);
	    if(fObj->InheritsFrom("TClonesArray")){
		fAODFolder->AddFolder(fAODListName[i], fAODListName[i], (TCollection*) fObj);
	    } else {
		fAODFolder->AddFolder(fAODListName[i], fAODListName[i], 0);
	    }
	}
    }
    else{
	printf("%s:%d CreateStdFolders() Wrong number of Std Entries \n",(char*)__FILE__,__LINE__);
    }
} 

//______________________________________________________________________________
void IlcAODEvent::GetStdContent()
{
  // set pointers for standard content

  fHeader        = (IlcAODHeader*)fAODObjects->FindObject("header");
  fTracks        = (TClonesArray*)fAODObjects->FindObject("tracks");
  fVertices      = (TClonesArray*)fAODObjects->FindObject("vertices");
  fV0s           = (TClonesArray*)fAODObjects->FindObject("v0s");
  fCascades      = (TClonesArray*)fAODObjects->FindObject("cascades");
  fTracklets     = (IlcAODTracklets*)fAODObjects->FindObject("tracklets");
  fJets          = (TClonesArray*)fAODObjects->FindObject("jets");
  fEmcalCells    = (IlcAODCaloCells*)fAODObjects->FindObject("emcalCells");
  fPhosCells     = (IlcAODCaloCells*)fAODObjects->FindObject("phosCells");
  fCaloClusters  = (TClonesArray*)fAODObjects->FindObject("caloClusters");
  fEMCALTrigger  = (IlcAODCaloTrigger*)fAODObjects->FindObject("emcalTrigger");
  fPHOSTrigger   = (IlcAODCaloTrigger*)fAODObjects->FindObject("phosTrigger");
  fFmdClusters   = (TClonesArray*)fAODObjects->FindObject("fmdClusters");
  fPmdClusters   = (TClonesArray*)fAODObjects->FindObject("pmdClusters");
  fHMPIDrings    = (TClonesArray*)fAODObjects->FindObject("hmpidRings");  
  fDimuons       = (TClonesArray*)fAODObjects->FindObject("dimuons");
  fAODTZERO      = (IlcAODTZERO*)fAODObjects->FindObject("IlcAODTZERO");
  fAODVZERO      = (IlcAODVZERO*)fAODObjects->FindObject("IlcAODVZERO");
  fAODZDC        = (IlcAODZDC*)fAODObjects->FindObject("IlcAODZDC");
  fTOFHeader     = (IlcTOFHeader*)fAODObjects->FindObject("IlcTOFHeader");
#ifdef MFT_UPGRADE
  fAODMFT        = (IlcAODMFT*)fAODObjects->FindObject("IlcAODMFT");
#endif
}

//______________________________________________________________________________
void IlcAODEvent::ResetStd(Int_t trkArrSize, 
			   Int_t vtxArrSize, 
			   Int_t v0ArrSize,
			   Int_t cascadeArrSize,
			   Int_t jetSize, 
			   Int_t caloClusSize, 
			   Int_t fmdClusSize, 
			   Int_t pmdClusSize,
                           Int_t hmpidRingsSize,
			   Int_t dimuonArrSize
			   )
{
  // deletes content of standard arrays and resets size 
  if (fTracks) {
    fTracks->Delete();
    if (trkArrSize > fTracks->GetSize()) 
      fTracks->Expand(trkArrSize);
  }
  if (fVertices) {
    fVertices->Delete();
    if (vtxArrSize > fVertices->GetSize()) 
      fVertices->Expand(vtxArrSize);
  }
  if (fV0s) {
    fV0s->Delete();
    if (v0ArrSize > fV0s->GetSize()) 
      fV0s->Expand(v0ArrSize);
  }
  if (fCascades) {
    fCascades->Delete();
    if (cascadeArrSize > fCascades->GetSize()) 
      fCascades->Expand(cascadeArrSize);
  }
  if (fJets) {
    fJets->Delete();
    if (jetSize > fJets->GetSize())
      fJets->Expand(jetSize);
  }
  if (fCaloClusters) {
    fCaloClusters->Delete();
    if (caloClusSize > fCaloClusters->GetSize()) 
      fCaloClusters->Expand(caloClusSize);
  }
  if (fFmdClusters) {
    fFmdClusters->Delete();
    if (fmdClusSize > fFmdClusters->GetSize()) 
      fFmdClusters->Expand(fmdClusSize);
  }
  if (fPmdClusters) {
    fPmdClusters->Delete();
    if (pmdClusSize > fPmdClusters->GetSize()) 
      fPmdClusters->Expand(pmdClusSize);
  }
  if (fHMPIDrings) {
     fHMPIDrings->Delete();
    if (hmpidRingsSize > fHMPIDrings->GetSize()) 
      fHMPIDrings->Expand(hmpidRingsSize);
  }
  if (fDimuons) {
    fDimuons->Delete();
    if (dimuonArrSize > fDimuons->GetSize()) 
      fDimuons->Expand(dimuonArrSize);
  }
  if (fTracklets)
    fTracklets->DeleteContainer();
  if (fPhosCells)
    fPhosCells->DeleteContainer();  
  if (fEmcalCells)
    fEmcalCells->DeleteContainer();
  
  if (fEMCALTrigger)
	fEMCALTrigger->DeAllocate();
  if (fPHOSTrigger)
	fPHOSTrigger->DeAllocate();

}

void IlcAODEvent::ClearStd()
{
  // clears the standard arrays
  if (fHeader)
    fHeader        ->Clear();
  if (fTracks)
    fTracks        ->Delete();
  if (fVertices)
    fVertices      ->Delete();
  if (fV0s)
    fV0s           ->Delete();
  if (fCascades)
    fCascades      ->Delete();
  if (fTracklets)
    fTracklets     ->DeleteContainer();
  if (fJets)
    fJets          ->Delete();
  if (fEmcalCells)
    fEmcalCells    ->DeleteContainer();
  if (fPhosCells)
    fPhosCells     ->DeleteContainer();
  if (fCaloClusters)
    fCaloClusters  ->Delete();
  if (fFmdClusters)
    fFmdClusters   ->Clear();
  if (fPmdClusters)
    fPmdClusters   ->Clear();  
  if (fHMPIDrings) 
     fHMPIDrings   ->Clear();    
  if (fDimuons)
    fDimuons       ->Clear();
	
  if (fEMCALTrigger)
	fEMCALTrigger->DeAllocate();
  if (fPHOSTrigger)
	fPHOSTrigger->DeAllocate();
}

//_________________________________________________________________
Int_t IlcAODEvent::GetPHOSClusters(TRefArray *clusters) const
{
  // fills the provided TRefArray with all found phos clusters
  
  clusters->Clear();
  
  IlcAODCaloCluster *cl = 0;
  Bool_t first = kTRUE;
  for (Int_t i = 0; i < GetNumberOfCaloClusters() ; i++) {
    if ( (cl = GetCaloCluster(i)) ) {
      if (cl->IsPHOS()){
	if(first) {
	  new (clusters) TRefArray(TProcessID::GetProcessWithUID(cl)); 
	  first=kFALSE;
	}
	clusters->Add(cl);
	//printf("IsPHOS cluster %d, E %2.3f Size: %d \n",i,cl->E(),clusters->GetEntriesFast());
      }
    }
  }
  return clusters->GetEntriesFast();
}

//_________________________________________________________________
Int_t IlcAODEvent::GetEMCALClusters(TRefArray *clusters) const
{
  // fills the provided TRefArray with all found emcal clusters

  clusters->Clear();
  IlcAODCaloCluster *cl = 0;
  Bool_t first = kTRUE;
  for (Int_t i = 0; i < GetNumberOfCaloClusters(); i++) {
    if ( (cl = GetCaloCluster(i)) ) {
      if (cl->IsEMCAL()){
	if(first) {
	  new (clusters) TRefArray(TProcessID::GetProcessWithUID(cl)); 
	  first=kFALSE;
	}
	clusters->Add(cl);
	//printf("IsEMCal cluster %d, E %2.3f Size: %d \n",i,cl->E(),clusters->GetEntriesFast());
      }
    }
  }
  return clusters->GetEntriesFast();
}


//______________________________________________________________________________
Int_t IlcAODEvent::GetMuonTracks(TRefArray *muonTracks) const
{
  // fills the provided TRefArray with all found muon tracks

  muonTracks->Clear();

  IlcAODTrack *track = 0;
  for (Int_t iTrack = 0; iTrack < GetNTracks(); iTrack++) {
    track = GetTrack(iTrack);
    if (track->IsMuonTrack()) {
      muonTracks->Add(track);
    }
  }
  
  return muonTracks->GetEntriesFast();
}


//______________________________________________________________________________
Int_t IlcAODEvent::GetNumberOfMuonTracks() const
{
  // get number of muon tracks
  Int_t nMuonTracks=0;
  for (Int_t iTrack = 0; iTrack < GetNTracks(); iTrack++) {
    if ((GetTrack(iTrack))->IsMuonTrack()) {
       nMuonTracks++;
    }
  }
  
  return nMuonTracks;
}

//______________________________________________________________________________
void IlcAODEvent::ReadFromTree(TTree *tree, Option_t* opt /*= ""*/)
{
    // Connects aod event to tree
  
  if(!tree){
    IlcWarning("Zero Pointer to Tree \n");
    return;
  }
    // load the TTree
  if(!tree->GetTree())tree->LoadTree(0);
  
    // Try to find IlcAODEvent
  IlcAODEvent *aodEvent = 0;
  aodEvent = (IlcAODEvent*)tree->GetTree()->GetUserInfo()->FindObject("IlcAODEvent");
  if(aodEvent){
    // This event is connected to the tree by definition, just say so
    aodEvent->SetConnected();
      // Check if already connected to tree
    TList* connectedList = (TList*) (tree->GetUserInfo()->FindObject("AODObjectsConnectedToTree"));
    if (connectedList && (!strcmp(opt, "reconnect"))) {
        // If connected use the connected list of objects
        if (fAODObjects != connectedList) {
           delete fAODObjects;
           fAODObjects = connectedList;
        }   
        GetStdContent(); 
        fConnected = kTRUE;
        return;
    } 
      // Connect to tree
      // prevent a memory leak when reading back the TList
//      if (!(strcmp(opt, "reconnect"))) fAODObjects->Delete();
    
      // create a new TList from the UserInfo TList... 
      // copy constructor does not work...
    //    fAODObjects = (TList*)(aodEvent->GetList()->Clone());
    fAODObjects = (TList*)aodEvent->GetList();
    fAODObjects->SetOwner(kTRUE);
    if(fAODObjects->GetEntries()<kAODListN)
    {
      IlcWarning(Form("IlcAODEvent::ReadFromTree() TList contains less than the standard contents %d < %d"
                      " That might be fine though (at least for filtered AODs)",fAODObjects->GetEntries(),kAODListN));
    }
      //
      // Let's find out whether we have friends
    TList* friendL = tree->GetTree()->GetListOfFriends();
    if (friendL) 
    {
      TIter next(friendL);
      TFriendElement* fe;
      while ((fe = (TFriendElement*)next())){
        aodEvent = (IlcAODEvent*)(fe->GetTree()->GetUserInfo()->FindObject("IlcAODEvent"));
        if (!aodEvent) {
          printf("No UserInfo on tree \n");
        } else {
          
	  //          TList* objL = (TList*)(aodEvent->GetList()->Clone());
          TList* objL = (TList*)aodEvent->GetList();
          printf("Get list of object from tree %d !!\n", objL->GetEntries());
          TIter nextobject(objL);
          TObject* obj =  0;
          while((obj = nextobject()))
          {
            printf("Adding object from friend %s !\n", obj->GetName());
            fAODObjects->Add(obj);
          } // object "branch" loop
        } // has userinfo  
      } // friend loop
    } // has friends	
      // set the branch addresses
    TIter next(fAODObjects);
    TNamed *el;
    while((el=(TNamed*)next())){
      TString bname(el->GetName());
        // check if branch exists under this Name
      TBranch *br = tree->GetTree()->GetBranch(bname.Data());
      if(br){
        tree->SetBranchAddress(bname.Data(),fAODObjects->GetObjectRef(el));
      } else {
        br = tree->GetBranch(Form("%s.",bname.Data()));
        if(br){
          tree->SetBranchAddress(Form("%s.",bname.Data()),fAODObjects->GetObjectRef(el));
        }
        else{
          printf("%s %d IlcAODEvent::ReadFromTree() No Branch found with Name %s. \n",
                 (char*)__FILE__,__LINE__,bname.Data());
        }	
      }
    }
    GetStdContent();
      // when reading back we are not owner of the list 
      // must not delete it
    fAODObjects->SetOwner(kTRUE);
    fAODObjects->SetName("AODObjectsConnectedToTree");
      // we are not owner of the list objects 
      // must not delete it
    tree->GetUserInfo()->Add(fAODObjects);
    fConnected = kTRUE;
  }// no aodEvent
  else {
      // we can't get the list from the user data, create standard content
      // and set it by hand
    CreateStdContent();
    TIter next(fAODObjects);
    TNamed *el;
    while((el=(TNamed*)next())){
      TString bname(el->GetName());    
      tree->SetBranchAddress(bname.Data(),fAODObjects->GetObjectRef(el));
    }
    GetStdContent();
      // when reading back we are not owner of the list 
      // must not delete it
    fAODObjects->SetOwner(kTRUE);
  }
}
  //______________________________________________________________________________
Int_t  IlcAODEvent::GetNumberOfPileupVerticesSPD() const{
  // count number of SPD pileup vertices
  Int_t nVertices=GetNumberOfVertices();
  Int_t nPileupVertices=0;
  for(Int_t iVert=0; iVert<nVertices; iVert++){
    IlcAODVertex *v=GetVertex(iVert);
    if(v->GetType()==IlcAODVertex::kPileupSPD) nPileupVertices++;
  }
  return nPileupVertices;
}
//______________________________________________________________________________
Int_t  IlcAODEvent::GetNumberOfPileupVerticesTracks() const{
  // count number of track pileup vertices
  Int_t nVertices=GetNumberOfVertices();
  Int_t nPileupVertices=0;
  for(Int_t iVert=0; iVert<nVertices; iVert++){
    IlcAODVertex *v=GetVertex(iVert);
    if(v->GetType()==IlcAODVertex::kPileupTracks) nPileupVertices++;
  }
  return nPileupVertices;
}
//______________________________________________________________________________
IlcAODVertex* IlcAODEvent::GetPrimaryVertexSPD() const{
  // Get SPD primary vertex
  Int_t nVertices=GetNumberOfVertices();
  for(Int_t iVert=0; iVert<nVertices; iVert++){
    IlcAODVertex *v=GetVertex(iVert);
    if(v->GetType()==IlcAODVertex::kMainSPD) return v;
  }
  return 0;
}
//______________________________________________________________________________
IlcAODVertex* IlcAODEvent::GetPileupVertexSPD(Int_t iV) const{
  // Get pile-up vertex iV
  Int_t nVertices=GetNumberOfVertices();
  Int_t counter=0;
  for(Int_t iVert=0; iVert<nVertices; iVert++){
    IlcAODVertex *v=GetVertex(iVert);
    if(v->GetType()==IlcAODVertex::kPileupSPD){
      if(counter==iV) return v;
      ++counter;
    }
  }
  return 0;
}
//______________________________________________________________________________
IlcAODVertex* IlcAODEvent::GetPileupVertexTracks(Int_t iV) const{
  // Get pile-up vertex iV
  Int_t nVertices=GetNumberOfVertices();
  Int_t counter=0;
  for(Int_t iVert=0; iVert<nVertices; iVert++){
    IlcAODVertex *v=GetVertex(iVert);
    if(v->GetType()==IlcAODVertex::kPileupTracks){
      if(counter==iV) return v;
      ++counter;
    }
  }
  return 0;
}
//______________________________________________________________________________
Bool_t  IlcAODEvent::IsPileupFromSPD(Int_t minContributors, 
				     Double_t minZdist, 
				     Double_t nSigmaZdist, 
				     Double_t nSigmaDiamXY, 
				     Double_t nSigmaDiamZ) const{
  //
  // This function checks if there was a pile up
  // reconstructed with SPD
  //
  IlcAODVertex *mainV=GetPrimaryVertexSPD();
  if(!mainV) return kFALSE;
  Int_t nc1=mainV->GetNContributors();
  if(nc1<1) return kFALSE;
  Int_t nPileVert=GetNumberOfPileupVerticesSPD();
  if(nPileVert==0) return kFALSE;
  Int_t nVertices=GetNumberOfVertices();
  
  for(Int_t iVert=0; iVert<nVertices; iVert++){
    IlcAODVertex *pv=GetVertex(iVert);
    if(pv->GetType()!=IlcAODVertex::kPileupSPD) continue;
    Int_t nc2=pv->GetNContributors();
    if(nc2>=minContributors){
      Double_t z1=mainV->GetZ();
      Double_t z2=pv->GetZ();
      Double_t distZ=TMath::Abs(z2-z1);
      Double_t distZdiam=TMath::Abs(z2-GetDiamondZ());
      Double_t cutZdiam=nSigmaDiamZ*TMath::Sqrt(GetSigma2DiamondZ());
      if(GetSigma2DiamondZ()<0.0001)cutZdiam=99999.; //protection for missing z diamond information
      if(distZ>minZdist && distZdiam<cutZdiam){
	Double_t x2=pv->GetX();
	Double_t y2=pv->GetY();
	Double_t distXdiam=TMath::Abs(x2-GetDiamondX());
	Double_t distYdiam=TMath::Abs(y2-GetDiamondY());
	Double_t cov1[6],cov2[6];	
	mainV->GetCovarianceMatrix(cov1);
	pv->GetCovarianceMatrix(cov2);
	Double_t errxDist=TMath::Sqrt(cov2[0]+GetSigma2DiamondX());
	Double_t erryDist=TMath::Sqrt(cov2[2]+GetSigma2DiamondY());
	Double_t errzDist=TMath::Sqrt(cov1[5]+cov2[5]);
	Double_t cutXdiam=nSigmaDiamXY*errxDist;
	if(GetSigma2DiamondX()<0.0001)cutXdiam=99999.; //protection for missing diamond information
	Double_t cutYdiam=nSigmaDiamXY*erryDist;
	if(GetSigma2DiamondY()<0.0001)cutYdiam=99999.; //protection for missing diamond information
	if( (distXdiam<cutXdiam) && (distYdiam<cutYdiam) && (distZ>nSigmaZdist*errzDist) ){
	  return kTRUE;
	}
      }
    }
  }
  return kFALSE;
}

//______________________________________________________________________________
void IlcAODEvent::Print(Option_t *) const
{
  // Print the names of the all branches
  TIter next(fAODObjects);
  TNamed *el;
  Printf(">>>>>  AOD  Content <<<<<");    
  while((el=(TNamed*)next())){
    Printf(">> %s ",el->GetName());      
  }
  Printf(">>>>>                <<<<<");    
  
  return;
}

void IlcAODEvent::AssignIDtoCollection(const TCollection* col)
{
    // Static method which assigns a ID to each object in a collection
    // In this way the objects are marked as referenced and written with 
    // an ID. This has the advantage that TRefs to this objects can be 
    // written by a subsequent process.
    TIter next(col);
    TObject* obj;
    while ((obj = next()))
	TProcessID::AssignID(obj);
}

Bool_t IlcAODEvent::IsPileupFromSPDInMultBins() const {
    Int_t nTracklets=GetTracklets()->GetNumberOfTracklets();
    if(nTracklets<20) return IsPileupFromSPD(3,0.8);
    else if(nTracklets<50) return IsPileupFromSPD(4,0.8);
    else return IsPileupFromSPD(5,0.8);
}

void IlcAODEvent::Reset()
{
  // Handle the cases
  // Std content + Non std content

  ClearStd();

  if(fAODObjects->GetSize()>kAODListN){
    // we have non std content
    // this also covers aodfriends
    for(int i = kAODListN;i < fAODObjects->GetSize();++i){
      TObject *pObject = fAODObjects->At(i);
      // TClonesArrays
      if(pObject->InheritsFrom(TClonesArray::Class())){
       ((TClonesArray*)pObject)->Delete();
      }
      else if(!pObject->InheritsFrom(TCollection::Class())){
       TClass *pClass = TClass::GetClass(pObject->ClassName());
       if (pClass && pClass->GetListOfMethods()->FindObject("Clear")) {
         IlcDebug(1, Form("Clear for object %s class %s", pObject->GetName(), pObject->ClassName()));
         pObject->Clear();
       }
       else {
         IlcDebug(1, Form("ResetWithPlacementNew for object %s class %s", pObject->GetName(), pObject->ClassName()));
          Long_t dtoronly = TObject::GetDtorOnly();
          TObject::SetDtorOnly(pObject);
          delete pObject;
          pClass->New(pObject);
          TObject::SetDtorOnly((void*)dtoronly);
       }
      }
      else{
       IlcWarning(Form("No reset for %s \n",
                       pObject->ClassName()));
      }
    }
  }
}

Float_t IlcAODEvent::GetVZEROEqMultiplicity(Int_t i) const
{
  // Get VZERO Multiplicity for channel i
  // Themethod uses the equalization factors
  // stored in the ESD-run object in order to
  // get equal multiplicities within a VZERO rins (1/8 of VZERO)
  if (!fAODVZERO || !fHeader) return -1;

  Int_t ring = i/8;
  Float_t factorSum = 0;
  for(Int_t j = 8*ring; j < (8*ring+8); ++j) {
    factorSum += fHeader->GetVZEROEqFactors(j);
  }
  Float_t factor = fHeader->GetVZEROEqFactors(i)*8./factorSum;

  return (fAODVZERO->GetMultiplicity(i)/factor);
}

//------------------------------------------------------------
void  IlcAODEvent::SetTOFHeader(const IlcTOFHeader *header)
{
  //
  // Set the TOF event_time
  //

  if (fTOFHeader) {
    *fTOFHeader=*header;
    //fTOFHeader->SetName(fgkESDListName[kTOFHeader]);
  }
  else {
    // for analysis of reconstructed events
    // when this information is not avaliable
    fTOFHeader = new IlcTOFHeader(*header);
    //AddObject(fTOFHeader);
  }

}
//------------------------------------------------------------
IlcAODHMPIDrings *IlcAODEvent::GetHMPIDringForTrackID(Int_t trackID)
{
  //
  // Returns the HMPID object if any for a given track ID
  //
  if(GetHMPIDrings())
  {
    for(Int_t ien = 0 ; ien < GetNHMPIDrings(); ien++)
    {
      if( ien == trackID ) return GetHMPIDring(ien);      
    }//rings loop  
  }
  return 0;
}
//------------------------------------------------------------
Int_t IlcAODEvent::GetNHMPIDrings()   
{ 
  //
  // If there is a list of HMPID rings in the given AOD event, return their number
  //
  if ( fHMPIDrings) return fHMPIDrings->GetEntriesFast(); 
  else return -1;
} 
//------------------------------------------------------------
IlcAODHMPIDrings *IlcAODEvent::GetHMPIDring(Int_t nRings)   
{ 
  //
  // If there is a list of HMPID rings in the given AOD event, return corresponding ring
  //
  if(fHMPIDrings) {
    if(   (IlcAODHMPIDrings*)fHMPIDrings->UncheckedAt(nRings) ) {
      return (IlcAODHMPIDrings*)fHMPIDrings->UncheckedAt(nRings);
    }
    else return 0x0;
  }
  else return 0x0;  
}
//------------------------------------------------------------
