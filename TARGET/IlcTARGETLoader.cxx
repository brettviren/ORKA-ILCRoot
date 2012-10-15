#include <TClonesArray.h>
#include <TBranch.h>
#include <TTree.h>

#include "IlcTARGETdigit.h"
#include "IlcTARGETLoader.h"
#include "IlcRunLoader.h"

///////////////////////////////////////////////////////////////////////////
// Loader for TARGET
// it manages the I/O for:
// raw clusters, primary vertices
// V0 and cascade
// and tracks propagated to the origin
//////////////////////////////////////////////////////////////////////////
const TString IlcTARGETLoader::fgkDefaultRawClustersContainerName = "TreeC";
const TString IlcTARGETLoader::fgkDefaultBackTracksContainerName = "TreeB";
const TString IlcTARGETLoader::fgkDefaultVerticesContainerName = "Vertex";
const TString IlcTARGETLoader::fgkDefaultV0ContainerName = "V0";
const TString IlcTARGETLoader::fgkDefaultCascadeContainerName = "Cascade";
ClassImp(IlcTARGETLoader)

/**********************************************************************/
  IlcTARGETLoader::IlcTARGETLoader():IlcLoader(){
  // Default constructor
  fTARGETpid = 0;
  fGeom = 0;
}
/*********************************************************************/
IlcTARGETLoader::IlcTARGETLoader(const Char_t *name,const Char_t *topfoldername):
IlcLoader(name,topfoldername){
  //ctor   
    IlcDataLoader* rawClustersDataLoader = new IlcDataLoader(
        fDetectorName + ".RawCl.root",fgkDefaultRawClustersContainerName,
        "Raw Clusters");
    fDataLoaders->Add(rawClustersDataLoader);
    rawClustersDataLoader->SetEventFolder(fEventFolder);
    rawClustersDataLoader->SetFolder(GetDetectorDataFolder());

    IlcDataLoader* backTracksDataLoader =  new IlcDataLoader(
        fDetectorName + ".BackTracks.root",fgkDefaultBackTracksContainerName,
        "Back Propagated Tracks");
    fDataLoaders->Add(backTracksDataLoader);
    backTracksDataLoader->SetEventFolder(fEventFolder);
    backTracksDataLoader->SetFolder(GetDetectorDataFolder());

    IlcDataLoader* vertexDataLoader = new IlcDataLoader(
        fDetectorName + ".Vertex.root",fgkDefaultVerticesContainerName,
        "Primary Vertices","O");
    fDataLoaders->Add(vertexDataLoader);
    vertexDataLoader->SetEventFolder(fEventFolder);
    vertexDataLoader->SetFolder(GetDetectorDataFolder());

    IlcDataLoader* v0DataLoader = new IlcDataLoader(
        fDetectorName + ".V0s.root",fgkDefaultV0ContainerName,"V0 Vertices");
    fDataLoaders->Add(v0DataLoader);
    v0DataLoader->SetEventFolder(fEventFolder);
    v0DataLoader->SetFolder(GetDetectorDataFolder());

    IlcDataLoader* cascadeDataLoader = new IlcDataLoader(
        fDetectorName + ".Cascades.root",fgkDefaultCascadeContainerName,
        "Cascades");
    fDataLoaders->Add(cascadeDataLoader);
    cascadeDataLoader->SetEventFolder(fEventFolder);
    cascadeDataLoader->SetFolder(GetDetectorDataFolder());
    fTARGETpid=0;
    fGeom = 0;
}
/**********************************************************************/
IlcTARGETLoader::IlcTARGETLoader(const Char_t *name,TFolder *topfolder): 
IlcLoader(name,topfolder) {
  //ctor  
    IlcDataLoader*  rawClustersDataLoader = new IlcDataLoader(
        fDetectorName + ".RawCl.root",fgkDefaultRawClustersContainerName,
        "Raw Clusters"); 
    fDataLoaders->Add(rawClustersDataLoader);
    rawClustersDataLoader->SetEventFolder(fEventFolder);
    rawClustersDataLoader->SetFolder(GetDetectorDataFolder());

    IlcDataLoader*  backTracksDataLoader =  new IlcDataLoader(
        fDetectorName + ".BackTracks.root",fgkDefaultBackTracksContainerName,
        "Back Propagated Tracks");
    fDataLoaders->Add(backTracksDataLoader);
    backTracksDataLoader->SetEventFolder(fEventFolder);
    backTracksDataLoader->SetFolder(GetDetectorDataFolder());

    IlcDataLoader* vertexDataLoader = new IlcDataLoader(
        fDetectorName + ".Vertex.root",fgkDefaultVerticesContainerName,
        "Primary Vertices","O");
    fDataLoaders->Add(vertexDataLoader);
    vertexDataLoader->SetEventFolder(fEventFolder);
    vertexDataLoader->SetFolder(GetDetectorDataFolder());

    IlcDataLoader* v0DataLoader = new IlcDataLoader(
        fDetectorName + ".V0.root",fgkDefaultV0ContainerName,"V0 Vertices");
    fDataLoaders->Add(v0DataLoader);
    v0DataLoader->SetEventFolder(fEventFolder);
    v0DataLoader->SetFolder(GetDetectorDataFolder());

    IlcDataLoader* cascadeDataLoader = new IlcDataLoader(
        fDetectorName + ".Cascade.root",fgkDefaultCascadeContainerName,
        "Cascades");
    fDataLoaders->Add(cascadeDataLoader);
    cascadeDataLoader->SetEventFolder(fEventFolder);
    cascadeDataLoader->SetFolder(GetDetectorDataFolder());
    fTARGETpid = 0;
    fGeom = 0;
}

// // // // //______________________________________________________________________
// // // // IlcTARGETLoader::IlcTARGETLoader(const IlcTARGETLoader &ob) : IlcLoader(ob) {
// // // //   // Copy constructor
// // // //   // Copies are not allowed. The method is protected to avoid misuse.
// // // //   Error("IlcTARGETLoader","Copy constructor not allowed\n");
// // // // }

//______________________________________________________________________
IlcTARGETLoader& IlcTARGETLoader::operator=(const IlcTARGETLoader& /* ob */){
  // Assignment operator
  // Assignment is not allowed. The method is protected to avoid misuse.
  Error("= operator","Assignment operator not allowed\n");
  return *this;
}

/**********************************************************************/
IlcTARGETLoader::~IlcTARGETLoader(){
    //destructor
    IlcDataLoader* dl = 0;
    UnloadRawClusters();
    dl = GetRawClLoader();
    fDataLoaders->Remove(dl);

    UnloadBackTracks();
    dl = GetBackTracksDataLoader();
    fDataLoaders->Remove(dl);

    UnloadVertices();
    dl = GetVertexDataLoader();
    fDataLoaders->Remove(dl);

    UnloadV0s();
    dl = GetV0DataLoader();
    fDataLoaders->Remove(dl);

    UnloadCascades();
    dl = GetCascadeDataLoader();
    fDataLoaders->Remove(dl);
    if(fTARGETpid)delete fTARGETpid;
    if(fGeom)delete fGeom;
}
/*
//----------------------------------------------------------------------
IlcTARGET* IlcTARGETLoader::GetTARGET(){
    // Returns the pointer to the TARGET, kept on the file. A short cut metthod
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Returns:
    //    Returns a pointer to the TARGET, if not found returns 0.
    IlcTARGET *its;

    if(gIlc){
        its = dynamic_cast<IlcTARGET*> (gIlc->GetDetector(
            GetDetectorName().Data()));
        if(its) return its;
    } // end if gIlc
    IlcRunLoader *rl=0;
    rl = GetRunLoader();
    if(!rl) return 0;
    IlcRun *ar=0;
    ar = rl->GetIlcRun();
    if(!ar) return 0;
    its = dynamic_cast<IlcTARGET*> (ar->GetDetector(GetDetectorName().Data()));
    return its;
}
//----------------------------------------------------------------------
void IlcTARGETLoader::SetupDigits(IlcTARGET *its){
    // Sets up to store TARGET Digits in side IlcTARGET::fDtype TObjArray
    // Inputs:
    //    IlcTARGET *its  Pointer to the TARGET
    // Outputs:
    //    none.
    // Return:
    //    none.

    its->SetTreeAddressD(TreeD());
}
*/
//----------------------------------------------------------------------
void IlcTARGETLoader::SetupDigits(TObjArray *digPerDet,Int_t n,
                               const Char_t **digclass){
    // Sets up digPerDet to store TARGET Digits.
    // Inputs:
    //    TObjArray *digPerDet   A pointer to a TObject Array size>=3.
    //    Int_t      n           The size of the TObjArray and digclass array
    //    Char_t     **digclass  Array of digit class names
    // Outputs:
    //    TObjArray *digPerDet   Setup and linked to the tree of digits
    // Return:
    //    none.
    Int_t i,m;
    TClonesArray *cl = 0;
    TTree *td = 0;
    TBranch *br = 0;
    Char_t branch[13];
    const Char_t *det[3] = {"SPD","SDD","SSD"};

    if(!digPerDet){
        Error("SetUpDigits","TObject Array digPerDet does not exist");
        return;
    } // end if
    m = digPerDet->GetSize();
    if(m<n){
        Error("SetUpDigits","TObject Array digPerDet=%p must have a size"
              " at least that of n=%d",digPerDet,n);
    } // end if
    if(m<3){
        Error("SetUpDigits","TObject Array digPerDet=%p must have a size >2",
              digPerDet);
        return;
    } // end if
    td = TreeD();
    for(i=0;i<n;i++){
      if(digPerDet->UncheckedAt(i)==0){ // set up TClones Array
            digPerDet->AddAt(new TClonesArray(digclass[i],1000),i);
            if(n==3) sprintf(branch,"TARGETDigits%s",det[i]);
            else     sprintf(branch,"TARGETDigits%d",i+1);
            br = td->GetBranch(branch);
            br->SetAddress(&((*digPerDet)[i]));
            continue; // do next one.
        } // end if
        cl =  dynamic_cast<TClonesArray*> (digPerDet->UncheckedAt(i));
        if(!cl && digPerDet->UncheckedAt(i)!=0){  // not a TClonesArray
            Error("SetUpDigits","TObject Array digPerDet-At(%d)=%p must be "
                  "zeroed or filled with TClonesArrays",i,digPerDet);
            return;
        } // end if
        if(!(cl->GetClass()->GetBaseClass(IlcTARGETdigit::Class()))){
            Error("SetUPDigits","TClones array at digPerDet[%d}=%p must be"
                "derived from IlcTARGETdigit",i,digPerDet->UncheckedAt(i));
        } // end if
        cl->Clear();
        if(n==3) sprintf(branch,"TARGETDigits%s",det[i]);
        else     sprintf(branch,"TARGETDigits%d",i+1);
        br = td->GetBranch(branch);
        br->SetAddress(&((*digPerDet)[i]));
        continue;
    } // end for i
}
//---------------------------------------------------------------------
IlcTARGETdigit * IlcTARGETLoader::GetDigit(TObjArray *digPerDet,Int_t module,
                                     Int_t digit){
    // Gets the digit for for a specific detector type and module.
    // To be used in conjustion with Setupdigits(IlcTARGET *its).
    // Inputs:
    //   TObjArray *digPereDet    Pointer to the Array of digits
    //   Int_t      module        Module number
    //   Int_t      digit         Digit number
    // Outputs:
    //   none.
    // Return:
    //   returns the pointer to the digit. if zero then last digit for that
    //   module.

    if(digPerDet==0){
        Error("GetDigit","digPerDet=%p, module=%d, digit=%d",
              digPerDet,module,digit);
        return 0;
    } // end if
    return 0;
}
/*
//---------------------------------------------------------------------
IlcTARGETdigit * IlcTARGETLoader::GetDigit(IlcTARGET *its,Int_t module,Int_t digit){
    // Gets the digit for for a specific detector type and module.
    // To be used in conjustion with Setupdigits(IlcTARGET *its).
    // Inputs:
    //   IlcTARGET *its    Pointer to the TARGET
    //   Int_t  module  Module number
    //   Int_t digit    Digit number
    // Outputs:
    //   none.
    // Return:
    //   returns the pointer to the digit. if zero then last digit for that
    //   module.
    //IlcTARGETDetType *idtype;
    IlcTARGETgeom *geom = its->GetTARGETgeom();
    Int_t idet = geom->GetModuleType(module);
    TClonesArray *digits;
 
    its->ResetDigits();
    TreeD()->GetEvent(module);
    digits = its->DigitsAddress(idet);
    if(digit>-1 && digit<digits->GetEntriesFast()){ // if in range.
        return (IlcTARGETdigit*) digits->At(digit);
    } // end if
    return 0;
}
*/
//----------------------------------------------------------------------
void IlcTARGETLoader::MakeTree(Option_t *opt){
    // invokes IlcLoader::MakeTree + specific TARGET tree(s)
    // Valid options: H,S,D,R,T and C (C=raw clusters)
    IlcLoader::MakeTree(opt);
    const char *oC = strstr(opt,"C");
    if (oC) MakeRawClustersContainer();

    const char *oB = strstr(opt,"B");
    if (oB) MakeBackTracksContainer();

    const char *oV0 = strstr(opt,"V0");
    if (oV0) MakeV0Container();

    const char *oX = strstr(opt,"X");
    if (oX) MakeCascadeContainer();
}

//----------------------------------------------------------------------
IlcTARGETgeom* IlcTARGETLoader::GetTARGETgeom(Bool_t force) {
  // retrieves the TARGET geometry from file
  if(fGeom && !force)return fGeom;
  if(fGeom && force){
    delete fGeom;
    fGeom = 0;
  }
  IlcRunLoader *runLoader = GetRunLoader();
  if (!runLoader->GetIlcRun()) runLoader->LoadgIlc();
  if (!runLoader->GetIlcRun()) {
    Error("GetTARGETgeom", "couldn't get IlcRun object");
    return NULL;
  }
  
  TDirectory *curdir = gDirectory;
  runLoader->CdGAFile();
  fGeom = (IlcTARGETgeom*)gDirectory->Get("IlcTARGETgeom");
  curdir->cd();
  if(!fGeom){
    Error("GetTARGETgeom","no TARGET geometry available");
    return NULL;
  }
  
  return fGeom;
}

