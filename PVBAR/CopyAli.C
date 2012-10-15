////////////////////////////////////////////////////////////////////////
//
// name: CopyIlc.C
// date: 17.3.2002
// last update: 21.3.2002
// author: Jiri Chudoba
// version: 1.0
//
// description: 
//    copy some ilc objects from 1 file to another
//
// ToDo: 
//    add support for more events in 1 file
//
//
// Note:
//    copied objects are not deleted, I assume that the root
//    session is exited after this copy
//
// Example:
//     ilcroot -b -q CopyIlc.C\("gilc.root","TreeK.root",0,1,1,1\)
//
// History:
//
// 21.3.02 - first version
//
////////////////////////////////////////////////////////////////////////

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "iostream.h"
#include "TTree.h"
#include "TBranch.h"
#include "TDirectory.h"
#include "TFile.h"
#include "IlcRun.h"
#include "TParticle.h"
#include "IlcHeader.h"
#include "TObjArray.h"
#include "TString.h"
#endif

void CopyIlc(TString fnOrig="rfio:gilc.root", TString fnNew="gilc_new.root",Int_t iEvent = 0, Bool_t copygIlc=kTRUE,Bool_t copyTreeK = kFALSE) 
{

  TFile *fileOrig = TFile::Open(fnOrig);
  if (!fileOrig->IsOpen()) {
    cerr<<"Cannot open input file "<<fnOrig.Data()<<endl;
    return;
  }
//  IlcRun *gIlc;
  if (gIlc) {delete gIlc; gIlc = 0;}  
  gIlc = (IlcRun*)(fileOrig->Get("gIlc"));
  if (!gIlc) {
    cerr<<"Cannot read gIlc from the input file"<<endl;
    return;
  }

  Int_t nAllTracks = gIlc->GetEvent(iEvent);
  cout<<"nAllTracks = "<<nAllTracks<<endl;

// Open the new file

  TFile *fileNew = TFile::Open(fnNew,"update");
  if (!fileNew->IsOpen()) {
    cerr<<"Cannot open output file "<<fnNew.Data()<<endl;
    return;
  }
  if (copygIlc) {
    cout<<"Copy gIlc: ";
    gIlc->Write();
    cout<<"done"<<endl;

    TTree *treeE  = gIlc->TreeE();
    if (!treeE) {
      cerr<<"No TreeE found for event "<<iEvent<<endl;
      return;
    }      
    cout<<"Copy TreeE: ";
    IlcHeader *header = new IlcHeader();
    treeE->SetBranchAddress("Header", &header);
    treeE->SetBranchStatus("*",1);
    TTree *treeENew =  treeE->CloneTree();
    treeENew->Write();
    cout<<"done"<<endl;

  if (copyTreeK) {
    cout<<"Copy TreeK: ";
    TTree *treeK  = gIlc->TreeK();
    if (!treeK) {
      cerr<<"No TreeK found for event "<<iEvent<<endl;
      return;
    }
    TParticle *particle = new TParticle();
    treeK->SetBranchAddress("Particles",&particle);
    treeK->SetBranchStatus("*",1);
    TTree *treeKNew =  treeK->CloneTree();
    treeKNew->Write();
    cout<<"done"<<endl;
  }

 
  delete gIlc;
  gIlc = 0;
  fileNew->Close();
  fileOrig->Close();
  delete fileNew;
  delete fileOrig;
}
