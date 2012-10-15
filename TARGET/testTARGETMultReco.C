#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

#include "IlcRunLoader.h"
#include "IlcESD.h"
#include "IlcRun.h"

#include "IlcTARGET.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETLoader.h"
#include "IlcTARGETMultReconstructor.h"

#endif

void testTARGETMultReco(Char_t* dir = ".") {

  Char_t str[256];

  // ########################################################
  // defining pointers
  IlcRunLoader* runLoader;
  TFile* esdFile = 0;
  TTree* esdTree = 0;
  IlcESD* esd = 0;

  // #########################################################
  // setup gilc and runloader

  if (gIlc) {
    delete gIlc->GetRunLoader();
    delete gIlc;
    gIlc=0;
  }

  sprintf(str,"%s/gilc.root",dir);
  runLoader = IlcRunLoader::Open(str);
  if (runLoader == 0x0) {
    cout << "Can not open session"<<endl;
    return;
  }
  runLoader->LoadgIlc();

  gIlc = runLoader->GetIlcRun();
  runLoader->LoadKinematics();
  runLoader->LoadHeader();

  // #########################################################
  // open esd file and get the tree

  // close it first to avoid memory leak
  if (esdFile)
    if (esdFile->IsOpen())
      esdFile->Close();

  sprintf(str,"%s/IlcESDs.root",dir);
  esdFile = TFile::Open(str);
  esdTree = (TTree*)esdFile->Get("esdTree");
  TBranch * esdBranch = esdTree->GetBranch("ESD");
  esdBranch->SetAddress(&esd);


  // #########################################################
  // setup its stuff

  IlcTARGET* its=(IlcTARGET*)runLoader->GetIlcRun()->GetDetector("TARGET");
  if (!its) {
    cout << " Can't get the TARGET!" << endl;
    return ;
  }
  IlcTARGETgeom* itsGeo=its->GetTARGETgeom();
  if (!itsGeo) {
    cout << " Can't get the TARGET geometry!" << endl;
    return ;
  }
  IlcTARGETLoader* itsLoader = (IlcTARGETLoader*)runLoader->GetLoader("TARGETLoader");
  if (!itsLoader) {
    cout << " Can't get the TARGET loader!" << endl;
    return ;
  }
  itsLoader->LoadRecPoints("read");

  // #########################################################
  IlcTARGETMultReconstructor* multReco = new IlcTARGETMultReconstructor();
  multReco->SetGeometry(itsGeo);

  // #########################################################
  // getting number of events

  Int_t nEvents = (Int_t)runLoader->GetNumberOfEvents();
  Int_t nESDEvents = esdBranch->GetEntries();

  if (nEvents!=nESDEvents) {
    cout << " Different number of events from runloader and esdtree!!!" 
	 << nEvents << " / " << nESDEvents << endl;
    return;
  }

  // ########################################################
  // loop over number of events
  cout << nEvents << " event(s) found in the file set" << endl;
  for(Int_t i=0; i<nEvents; i++) {
    
    cout << "-------------------------" << endl << " event# " << i << endl;
    
    runLoader->GetEvent(i);
    esdBranch->GetEntry(i);

    // ########################################################
    // get the EDS vertex
    const IlcESDVertex* vtxESD = esd->GetVertex();
    Double_t vtx[3];
    vtxESD->GetXYZ(vtx);   
    Float_t esdVtx[3];
    esdVtx[0] = vtx[0];
    esdVtx[1] = vtx[1];
    esdVtx[2] = vtx[2];
    
    ///#########################################################
    // get TARGET clusters 
    TTree* itsClusterTree = itsLoader->TreeR();
    if (!itsClusterTree) {
      cerr<< " Can't get the TARGET cluster tree !\n";
      return;
    }
    multReco->SetHistOn(kTRUE);
    multReco->Reconstruct(itsClusterTree, esdVtx, esdVtx);

    
    for (Int_t t=0; t<multReco->GetNTracklets(); t++) {
      
      cout << "  tracklet " << t 
	   << " , theta = " << multReco->GetTracklet(t)[0]
	   << " , phi = " << multReco->GetTracklet(t)[1] << endl; 
    }

  }
 
  TFile* fout = new TFile("out.root","RECREATE");  

  multReco->SaveHists();
  fout->Write();
  fout->Close();


}
