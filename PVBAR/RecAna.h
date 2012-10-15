//////////////////////////////////////////////////////////
//  A Demo Macro that shows how to analyze the 
//  reconstructed Tree
//
//  Y. Schutz (SUBATECH)
//////////////////////////////////////////////////////////


#ifndef RecAna_h
#define RecAna_h

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TTree.h>
#include <TFile.h>
#endif

const Int_t kMaxPVBARTS = 9;
const Int_t kMaxPVBARRP = 9;

class RecAna {
  public:
  IlcPVBARv0 * fPVBAR ; 
  TTree          *fTree;    //pointer to the analyzed TTree or TChain
  TTree          *fCurrent; //pointer to the current TTree
  //Declaration of leaves types
  TObjArray       *PVBAREmcRP;
  TObjArray       *PVBARPpsdRP;
  Int_t           PVBARTS_;
  Int_t           PVBARTS_fEmcRecPoint[kMaxPVBARTS];
  Int_t           PVBARTS_fPpsdLowRecPoint[kMaxPVBARTS];
  Int_t           PVBARTS_fPpsdUpRecPoint[kMaxPVBARTS];
  UInt_t          PVBARTS_fUniqueID[kMaxPVBARTS];
  UInt_t          PVBARTS_fBits[kMaxPVBARTS];
  Int_t           PVBARRP_;
  Int_t           PVBARRP_fPVBARTrackSegment[kMaxPVBARRP];
  Int_t           PVBARRP_fIndexInList[kMaxPVBARRP];
  Int_t           PVBARRP_fPrimary[kMaxPVBARRP];
  Int_t           PVBARRP_fType[kMaxPVBARRP];
  Int_t           PVBARRP_fPdgCode[kMaxPVBARRP];
  Int_t           PVBARRP_fStatusCode[kMaxPVBARRP];
  Int_t           PVBARRP_fMother[2][kMaxPVBARRP];
  Int_t           PVBARRP_fDaughter[2][kMaxPVBARRP];
  Float_t         PVBARRP_fWeight[kMaxPVBARRP];
  Double_t        PVBARRP_fCalcMass[kMaxPVBARRP];
  Double_t        PVBARRP_fPx[kMaxPVBARRP];
  Double_t        PVBARRP_fPy[kMaxPVBARRP];
  Double_t        PVBARRP_fPz[kMaxPVBARRP];
  Double_t        PVBARRP_fE[kMaxPVBARRP];
  Double_t        PVBARRP_fVx[kMaxPVBARRP];
  Double_t        PVBARRP_fVy[kMaxPVBARRP];
  Double_t        PVBARRP_fVz[kMaxPVBARRP];
  Double_t        PVBARRP_fVt[kMaxPVBARRP];
  Double_t        PVBARRP_fPolarTheta[kMaxPVBARRP];
  Double_t        PVBARRP_fPolarPhi[kMaxPVBARRP];
  UInt_t          PVBARRP_fUniqueID[kMaxPVBARRP];
  UInt_t          PVBARRP_fBits[kMaxPVBARRP];
  Short_t         PVBARRP_fLineColor[kMaxPVBARRP];
  Short_t         PVBARRP_fLineStyle[kMaxPVBARRP];
  Short_t         PVBARRP_fLineWidth[kMaxPVBARRP];
  
  //List of branches
  TBranch        *b_PVBAREmcRP;
  TBranch        *b_PVBARPpsdRP;
  TBranch        *b_PVBARTS_;
  TBranch        *b_PVBARTS_fEmcRecPoint;
  TBranch        *b_PVBARTS_fPpsdLowRecPoint;
  TBranch        *b_PVBARTS_fPpsdUpRecPoint;
  TBranch        *b_PVBARTS_fUniqueID;
  TBranch        *b_PVBARTS_fBits;
  TBranch        *b_PVBARRP_;
  TBranch        *b_PVBARRP_fPVBARTrackSegment;
  TBranch        *b_PVBARRP_fIndexInList;
  TBranch        *b_PVBARRP_fPrimary;
  TBranch        *b_PVBARRP_fType;
  TBranch        *b_PVBARRP_fPdgCode;
  TBranch        *b_PVBARRP_fStatusCode;
  TBranch        *b_PVBARRP_fMother;
  TBranch        *b_PVBARRP_fDaughter;
  TBranch        *b_PVBARRP_fWeight;
  TBranch        *b_PVBARRP_fCalcMass;
  TBranch        *b_PVBARRP_fPx;
  TBranch        *b_PVBARRP_fPy;
  TBranch        *b_PVBARRP_fPz;
  TBranch        *b_PVBARRP_fE;
  TBranch        *b_PVBARRP_fVx;
  TBranch        *b_PVBARRP_fVy;
  TBranch        *b_PVBARRP_fVz;
  TBranch        *b_PVBARRP_fVt;
  TBranch        *b_PVBARRP_fPolarTheta;
  TBranch        *b_PVBARRP_fPolarPhi;
  TBranch        *b_PVBARRP_fUniqueID;
  TBranch        *b_PVBARRP_fBits;
  TBranch        *b_PVBARRP_fLineColor;
  TBranch        *b_PVBARRP_fLineStyle;
  TBranch        *b_PVBARRP_fLineWidth;
  
  RecAna() {};
  RecAna(char * filename);
  RecAna(TTree *tree) {};
  ~RecAna() {;}
  Int_t GetEntry(Int_t entry = 0);
  Int_t GetEvent(Int_t evt);
  Int_t LoadTree(Int_t entry = 0);
  void  Init(TTree *tree);
  void  Loop();
  void  Notify();
  void  Show(Int_t entry = -1);
};

#endif

#ifdef RecAna_cxx
RecAna::RecAna(char * filename)
{
  // connect the file used to generate this class and read the Tree.
  
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(filename);
  if (!f) 
    f = new TFile(filename);
  
  // setup the gIlc evironment
  
  gIlc = (IlcRun*) f->Get("gIlc") ;   
  
  // get the PVBAR detectector, the geometry instance and the index to object converter instance
  
  fPVBAR  = (IlcPVBARv0 *)gIlc->GetDetector("PVBAR") ;     
  IlcPVBARGeometry::GetInstance( fPVBAR->GetGeometry()->GetName(), fPVBAR->GetGeometry()->GetTitle() );
  IlcPVBARIndexToObject::GetInstance(fPVBAR) ;
}

Int_t RecAna::GetEntry(Int_t entry)
{
  // Read contents of entry, always = 0.
  
  if (!fTree) 
    return 0;
  
  return fTree->GetEntry(entry);
}

Int_t RecAna::GetEvent(Int_t evt)
{
  // get the selected event
  
  gIlc->GetEvent(evt);
  
  // connect to the Reconstruction tree
  
  fTree = gIlc->TreeR();     
  
  // set the branches 
  
  Init(fTree);
  
  // gets the data
  
  GetEntry(0); 
  cout << "macro EmcRecpoints = " << fPVBAR->EmcRecPoints() << endl ; 
}

Int_t RecAna::LoadTree(Int_t entry)
{
  // Set the environment to read one entry, always = 0.
  
  if (!fTree) 
    return -5;
  Int_t centry = fTree->LoadTree(entry);
  if (centry < 0) 
    return centry;
  if (fTree->GetTree() != fCurrent) {
    fCurrent = fTree->GetTree();
    Notify();
  }
  return centry;
}

void RecAna::Init(TTree *tree)
{
  //   Set branch addresses
  if (tree == 0) return;
  fTree    = tree;
  fCurrent = 0;
  
  fTree->SetBranchAddress("PVBAREmcRP",&PVBAREmcRP);
  fTree->SetBranchAddress("PVBARPpsdRP",&PVBARPpsdRP);
  fTree->SetBranchAddress("PVBARTS_",&PVBARTS_);
  fTree->SetBranchAddress("PVBARTS.fEmcRecPoint",PVBARTS_fEmcRecPoint);
  fTree->SetBranchAddress("PVBARTS.fPpsdLowRecPoint",PVBARTS_fPpsdLowRecPoint);
  fTree->SetBranchAddress("PVBARTS.fPpsdUpRecPoint",PVBARTS_fPpsdUpRecPoint);
  fTree->SetBranchAddress("PVBARTS.fUniqueID",PVBARTS_fUniqueID);
  fTree->SetBranchAddress("PVBARTS.fBits",PVBARTS_fBits);
  fTree->SetBranchAddress("PVBARRP_",&PVBARRP_);
  fTree->SetBranchAddress("PVBARRP.fPVBARTrackSegment",PVBARRP_fPVBARTrackSegment);
  fTree->SetBranchAddress("PVBARRP.fIndexInList",PVBARRP_fIndexInList);
  fTree->SetBranchAddress("PVBARRP.fPrimary",PVBARRP_fPrimary);
  fTree->SetBranchAddress("PVBARRP.fType",PVBARRP_fType);
  fTree->SetBranchAddress("PVBARRP.fPdgCode",PVBARRP_fPdgCode);
  fTree->SetBranchAddress("PVBARRP.fStatusCode",PVBARRP_fStatusCode);
  fTree->SetBranchAddress("PVBARRP.fMother[2]",PVBARRP_fMother);
  fTree->SetBranchAddress("PVBARRP.fDaughter[2]",PVBARRP_fDaughter);
  fTree->SetBranchAddress("PVBARRP.fWeight",PVBARRP_fWeight);
  fTree->SetBranchAddress("PVBARRP.fCalcMass",PVBARRP_fCalcMass);
  fTree->SetBranchAddress("PVBARRP.fPx",PVBARRP_fPx);
  fTree->SetBranchAddress("PVBARRP.fPy",PVBARRP_fPy);
  fTree->SetBranchAddress("PVBARRP.fPz",PVBARRP_fPz);
  fTree->SetBranchAddress("PVBARRP.fE",PVBARRP_fE);
  fTree->SetBranchAddress("PVBARRP.fVx",PVBARRP_fVx);
  fTree->SetBranchAddress("PVBARRP.fVy",PVBARRP_fVy);
  fTree->SetBranchAddress("PVBARRP.fVz",PVBARRP_fVz);
  fTree->SetBranchAddress("PVBARRP.fVt",PVBARRP_fVt);
  fTree->SetBranchAddress("PVBARRP.fPolarTheta",PVBARRP_fPolarTheta);
  fTree->SetBranchAddress("PVBARRP.fPolarPhi",PVBARRP_fPolarPhi);
  fTree->SetBranchAddress("PVBARRP.fUniqueID",PVBARRP_fUniqueID);
  fTree->SetBranchAddress("PVBARRP.fBits",PVBARRP_fBits);
  fTree->SetBranchAddress("PVBARRP.fLineColor",PVBARRP_fLineColor);
  fTree->SetBranchAddress("PVBARRP.fLineStyle",PVBARRP_fLineStyle);
  fTree->SetBranchAddress("PVBARRP.fLineWidth",PVBARRP_fLineWidth);
}

void RecAna::Notify()
{
  //   called by LoadTree when loading a new file
  //   get branch pointers
  b_PVBAREmcRP = fTree->GetBranch("PVBAREmcRP");
  b_PVBARPpsdRP = fTree->GetBranch("PVBARPpsdRP");
  b_PVBARTS_ = fTree->GetBranch("PVBARTS_");
  b_PVBARTS_fEmcRecPoint = fTree->GetBranch("PVBARTS.fEmcRecPoint");
  b_PVBARTS_fPpsdLowRecPoint = fTree->GetBranch("PVBARTS.fPpsdLowRecPoint");
  b_PVBARTS_fPpsdUpRecPoint = fTree->GetBranch("PVBARTS.fPpsdUpRecPoint");
  b_PVBARTS_fUniqueID = fTree->GetBranch("PVBARTS.fUniqueID");
  b_PVBARTS_fBits = fTree->GetBranch("PVBARTS.fBits");
  b_PVBARRP_ = fTree->GetBranch("PVBARRP_");
  b_PVBARRP_fPVBARTrackSegment = fTree->GetBranch("PVBARRP.fPVBARTrackSegment");
  b_PVBARRP_fIndexInList = fTree->GetBranch("PVBARRP.fIndexInList");
  b_PVBARRP_fPrimary = fTree->GetBranch("PVBARRP.fPrimary");
  b_PVBARRP_fType = fTree->GetBranch("PVBARRP.fType");
  b_PVBARRP_fPdgCode = fTree->GetBranch("PVBARRP.fPdgCode");
  b_PVBARRP_fStatusCode = fTree->GetBranch("PVBARRP.fStatusCode");
  b_PVBARRP_fMother = fTree->GetBranch("PVBARRP.fMother[2]");
  b_PVBARRP_fDaughter = fTree->GetBranch("PVBARRP.fDaughter[2]");
  b_PVBARRP_fWeight = fTree->GetBranch("PVBARRP.fWeight");
  b_PVBARRP_fCalcMass = fTree->GetBranch("PVBARRP.fCalcMass");
  b_PVBARRP_fPx = fTree->GetBranch("PVBARRP.fPx");
  b_PVBARRP_fPy = fTree->GetBranch("PVBARRP.fPy");
  b_PVBARRP_fPz = fTree->GetBranch("PVBARRP.fPz");
  b_PVBARRP_fE = fTree->GetBranch("PVBARRP.fE");
  b_PVBARRP_fVx = fTree->GetBranch("PVBARRP.fVx");
  b_PVBARRP_fVy = fTree->GetBranch("PVBARRP.fVy");
  b_PVBARRP_fVz = fTree->GetBranch("PVBARRP.fVz");
  b_PVBARRP_fVt = fTree->GetBranch("PVBARRP.fVt");
  b_PVBARRP_fPolarTheta = fTree->GetBranch("PVBARRP.fPolarTheta");
  b_PVBARRP_fPolarPhi = fTree->GetBranch("PVBARRP.fPolarPhi");
  b_PVBARRP_fUniqueID = fTree->GetBranch("PVBARRP.fUniqueID");
  b_PVBARRP_fBits = fTree->GetBranch("PVBARRP.fBits");
  b_PVBARRP_fLineColor = fTree->GetBranch("PVBARRP.fLineColor");
  b_PVBARRP_fLineStyle = fTree->GetBranch("PVBARRP.fLineStyle");
  b_PVBARRP_fLineWidth = fTree->GetBranch("PVBARRP.fLineWidth");
}

void RecAna::Show(Int_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fTree) return;
  fTree->Show(entry);
}
#endif // #ifdef RecAna_cxx

