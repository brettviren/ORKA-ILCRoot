#include "IlcAODExtension.h"

//-------------------------------------------------------------------------
//     Support class for AOD extensions. This is created by the user analysis
//     that requires a separate file for some AOD branches. The name of the 
//     IlcAODExtension object is the file name where the AOD branches will be
//     stored.
//-------------------------------------------------------------------------

#include "IlcAODBranchReplicator.h"
#include "IlcAODEvent.h"
#include "IlcCodeTimer.h"
#include "IlcLog.h"
#include "Riostream.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TList.h"
#include "TMap.h"
#include "TMap.h"
#include "TObjString.h"
#include "TROOT.h"
#include "TString.h"
#include "TTree.h"

ClassImp(IlcAODExtension)

//______________________________________________________________________________
IlcAODExtension::IlcAODExtension() : TNamed(), 
fAODEvent(0), fTreeE(0), fFileE(0), fNtotal(0), fNpassed(0), 
fSelected(kFALSE), fRepFiMap(0x0), fRepFiList(0x0), fEnableReferences(kTRUE), fObjectList(0x0)
{
  // default ctor
}

//______________________________________________________________________________
IlcAODExtension::IlcAODExtension(const char* name, const char* title, Bool_t isfilter)
:TNamed(name,title), 
fAODEvent(0), 
fTreeE(0), 
fFileE(0), 
fNtotal(0), 
fNpassed(0),
fSelected(kFALSE),
fRepFiMap(0x0),
fRepFiList(0x0),
fEnableReferences(kTRUE),
fObjectList(0x0)
{
  // Constructor.
  if (isfilter) {
    TObject::SetBit(kFilteredAOD);
    printf("####### Added AOD filter %s\n", name);
  } else printf("####### Added AOD extension %s\n", name);
  KeepUnspecifiedBranches();
}   

//______________________________________________________________________________
IlcAODExtension::~IlcAODExtension()
{
  // Destructor.
  if(fFileE){
    // is already handled in TerminateIO
    fFileE->Close();
    delete fFileE;
    fTreeE = 0;
    fAODEvent = 0;
  }
  if (fTreeE) delete fTreeE;
  if (fRepFiMap) fRepFiMap->DeleteAll();
  delete fRepFiMap; // the map is owner
  delete fRepFiList; // the list is not
  delete fObjectList; // not owner
}

//______________________________________________________________________________
void IlcAODExtension::AddBranch(const char* cname, void* addobj)
{
  // Add a new branch to the aod 
  
  if (!fAODEvent) {
    char type[20];
    gROOT->ProcessLine(Form("TString s_tmp; IlcAnalysisManager::GetAnalysisManager()->GetAnalysisTypeString(s_tmp); sprintf((char*)%p, \"%%s\", s_tmp.Data());", type));
    Init(type);
  }
  TDirectory *owd = gDirectory;
  if (fFileE) {
    fFileE->cd();
  }
  char** apointer = (char**) addobj;
  TObject* obj = (TObject*) *apointer;
  
  fAODEvent->AddObject(obj);
  
  TString bname(obj->GetName());
  
  if (!fTreeE->FindBranch(bname.Data())) 
  {
    Bool_t acceptAdd(kTRUE);
    
    if ( TestBit(kDropUnspecifiedBranches) )
    {
      // check that this branch is in our list of specified ones...
      // otherwise do not add it !
      TIter next(fRepFiMap);
      TObjString* p;
      
      acceptAdd=kFALSE;
      
      while ( ( p = static_cast<TObjString*>(next()) ) && !acceptAdd )
      {
        if ( p->String() == bname ) acceptAdd=kTRUE;
      }
    }
    
    if ( acceptAdd ) 
    {
      // Do the same as if we book via 
      // TTree::Branch(TCollection*)
      
      fObjectList->Add(obj);

      const Int_t kSplitlevel = 99; // default value in TTree::Branch()
      const Int_t kBufsize = 32000; // default value in TTree::Branch()
      
      fTreeE->Bronch(bname.Data(), cname, 
                     fAODEvent->GetList()->GetObjectRef(obj),
                     kBufsize, kSplitlevel - 1);
    }
  }
  owd->cd();
}

//______________________________________________________________________________
Bool_t IlcAODExtension::FinishEvent()
{
  // Fill current event.
  fNtotal++;
  if (!IsFilteredAOD()) {
    fAODEvent->MakeEntriesReferencable();
    fTreeE->Fill();
    return kTRUE;
  }  
  // Filtered AOD. Fill only if event is selected.
  if (!fSelected) return kTRUE;
  
  TIter next(fRepFiList);
  
  IlcAODBranchReplicator* repfi;
  
  while ( ( repfi = static_cast<IlcAODBranchReplicator*>(next()) ) )
  {
    repfi->ReplicateAndFilter(*fAODEvent);
  }
  fNpassed++;
  fTreeE->Fill();
  fSelected = kFALSE; // so that next event will not be selected unless demanded
  return kTRUE;
}  

//______________________________________________________________________________
Bool_t IlcAODExtension::Init(Option_t *option)
{
  // Initialize IO.
  
  IlcCodeTimerAuto(GetName(),0);
  
  if(!fAODEvent) 
  {
    fAODEvent = new IlcAODEvent();    
  }
  
  TDirectory *owd = gDirectory;
  TString opt(option);
  opt.ToLower();
  
  if (opt.Contains("proof")) 
  {
    // proof
    // Merging via files. Need to access analysis manager via interpreter.
    gROOT->ProcessLine(Form("IlcAnalysisDataContainer *c_common_out = IlcAnalysisManager::GetAnalysisManager()->GetCommonOutputContainer();"));
    gROOT->ProcessLine(Form("IlcAnalysisManager::GetAnalysisManager()->OpenProofFile(c_common_out, \"RECREATE\", \"%s\");", fName.Data()));
    fFileE = gFile;
  } 
  else 
  {
    fFileE = new TFile(GetName(), "RECREATE");
  }  
  fTreeE = new TTree("aodTree", "IlcAOD tree");
  
  delete fObjectList;
  fObjectList = new TList;
  fObjectList->SetOwner(kFALSE); // be explicit we're not the owner...
  TList* inputList = fAODEvent->GetList();
  TIter next(inputList);
  TObject* o;
  
  while ( ( o = next() ) )
  {
    // Loop on the objects that are within the main AOD, and see what to do with them :
    // - transmit them to our AOD as they are
    // - filter them (by means of an IlcAODBranchReplicator)
    // - drop them completely
    
    Bool_t mustKeep(kFALSE);
    
    TString test(o->ClassName());
    test.ToUpper();
    if (test.BeginsWith("ILCAODHEADER"))
    {
      // do not allow to drop header branch
      mustKeep=kTRUE;
    }
    
    if ( fRepFiMap && !mustKeep )
    {
      // we have some replicators, so let's see what the relevant one decides about this object
      TObject* specified = fRepFiMap->FindObject(o->GetName()); // FindObject finds key=o->GetName() in the map
      if (specified)
      {
        IlcAODBranchReplicator* repfi = dynamic_cast<IlcAODBranchReplicator*>(fRepFiMap->GetValue(o->GetName())); // GetValue gets the replicator corresponding to key=o->GetName()
        if ( repfi ) 
        {        
          TList* replicatedList = repfi->GetList();
          if (replicatedList)
          {
            IlcAODEvent::AssignIDtoCollection(replicatedList);
            TIter nextRep(replicatedList);
            TObject* objRep;
            while ( ( objRep = nextRep() ) )
            {
              if ( !fObjectList->FindObject(objRep) ) // insure we're not adding several times the same object
              {                
                fObjectList->Add(objRep);                  
              }
            }
          }
          else
          {
            IlcError(Form("replicatedList from %s is null !",repfi->GetName()));
          }
        }
      }
      else
      {
        if ( !TestBit(kDropUnspecifiedBranches) ) 
        {
          // object o will be transmitted to the output AOD, unchanged
          fObjectList->Add(o);
        }
      }
    } 
    else
    {
      // no replicator, so decide based on the policy about dropping unspecified branches
      if ( mustKeep || !TestBit(kDropUnspecifiedBranches) )
      {
        // object o will be transmitted to the output AOD, unchanged
        fObjectList->Add(o);
      }
    }
  }
    
  if (fEnableReferences) 
  {
    fTreeE->BranchRef();    
  }
    
  fTreeE->Branch(fObjectList);
  
  owd->cd();
  
  return kTRUE;
}

//______________________________________________________________________________
void IlcAODExtension::Print(Option_t* opt) const
{
  // Print info about this extension
  
  cout << opt << Form("%s - %s - %s - aod %p",IsFilteredAOD() ? "FilteredAOD" : "Extension",
                      GetName(),GetTitle(),GetAOD()) << endl;
  if ( !fEnableReferences ) 
  {
    cout << opt << opt << "References are disabled ! Hope you know what you are doing !" << endl;
  }
  if ( TestBit(kDropUnspecifiedBranches) )
  {
    cout << opt << opt << "All branches not explicitely specified will be dropped" << endl;
  }
  
  TIter next(fRepFiMap);
  TObjString* s;
  
  while ( ( s = static_cast<TObjString*>(next()) ) )
  {
    IlcAODBranchReplicator* br = static_cast<IlcAODBranchReplicator*>(fRepFiMap->GetValue(s->String().Data()));
    
    cout << opt << opt << "Branch " << s->String();
    if (br)
    {
      cout << " will be filtered by class " << br->ClassName();
    }
    else
    {
      cout << " will be transmitted as is";
    }
    cout << endl;
  }
}

//______________________________________________________________________________
void IlcAODExtension::SetEvent(IlcAODEvent* event)
{
  // Connects to an external event
  if (!IsFilteredAOD()) {
    Error("SetEvent", "Not allowed to set external event for non filtered AOD's");   
    return;
  }
  fAODEvent = event;
}

//______________________________________________________________________________
void IlcAODExtension::AddAODtoTreeUserInfo()
{
  // Add aod event to tree user info
  
  if (!fTreeE) return;
  
  IlcAODEvent* aodEvent(fAODEvent);
  
  if ( IsFilteredAOD() )
  {
    // cannot attach fAODEvent (which is shared with our IlcAODHandler mother)
    // so we create a custom (empty) IlcAODEvent 
    // Has also the advantage we can specify only the list of objects
    // that are actually there in this filtered aod
    //
    aodEvent = new IlcAODEvent;
    TIter nextObj(fObjectList);
    TObject* o;
    while ( ( o = nextObj() ) ) 
    {
      aodEvent->AddObject(o);
    }    
  }
  
  fTreeE->GetUserInfo()->Add(aodEvent);
}

//______________________________________________________________________________
Bool_t IlcAODExtension::TerminateIO()
{
  // Terminate IO
  if (TObject::TestBit(kFilteredAOD))
    printf("AOD Filter %s: events processed: %d   passed: %d\n", GetName(), fNtotal, fNpassed);
  else
    printf("AOD extension %s: events processed: %d\n", GetName(), fNtotal);
  if (fFileE) 
  {
    fFileE->Write();
    fFileE->Close();
    delete fFileE;
    fFileE = 0;
    fTreeE = 0;
    fAODEvent = 0;
  }
  return kTRUE;
}

//______________________________________________________________________________
void IlcAODExtension::FilterBranch(const char* branchName, IlcAODBranchReplicator* repfi)
{
  // Specify a filter/replicator for a given branch
  //
  // If repfi=0x0, this will disable the branch (in the output) completely.
  //
  // repfi is adopted by this class, i.e. user should not delete it.
  //
  // WARNING : branch name must be exact.
  //
  // See also the documentation for IlcAODBranchReplicator class.
  //
  
  if (!fRepFiMap)
  {
    fRepFiMap = new TMap;
    fRepFiMap->SetOwnerKeyValue(kTRUE,kTRUE);
    fRepFiList = new TList;
    fRepFiList->SetOwner(kFALSE);
  }
  
  fRepFiMap->Add(new TObjString(branchName),repfi);
  
  if (repfi && !fRepFiList->FindObject(repfi))
  {
    // insure we get unique and non-null replicators in this list
    fRepFiList->Add(repfi);
  }
}

