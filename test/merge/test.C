// Usage in compiled mode
// gSystem->SetIncludePath("-I$ROOTSYS/include -I$ILC_ROOT/include");  
// gROOT->LoadMacro("test.C+");
// test()

#if !defined(__CINT__) || defined(__MAKECINT__)

// Root include files
#include <Riostream.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TStopwatch.h>
#include <TObject.h>
#include <TParticle.h>

// IlcRoot include files
#include "IlcESDEvent.h"
#include "IlcRunLoader.h"
#include "IlcRun.h"
#include "IlcStack.h"

#endif

void test(const char * sdir ="signal",
	  const char * bdir ="backgr") {

  TStopwatch timer;
  timer.Start();

  TString name;

  // Signal file, tree, and branch
  name = sdir;
  name += "/IlcESDs.root";
  TFile * fSig = TFile::Open(name.Data());
  TTree * tSig = (TTree*)fSig->Get("esdTree");

  IlcESDEvent * esdSig = new IlcESDEvent();// The signal ESD object is put here
  esdSig->ReadFromTree(tSig);

  // Run loader (signal events)
  name = sdir;
  name += "/gilc.root";
  IlcRunLoader* rlSig = IlcRunLoader::Open(name.Data());

  // Run loader (underlying events)
  name = bdir;
  name += "/gilc.root";
  IlcRunLoader* rlUnd = IlcRunLoader::Open(name.Data(),"Underlying");

  // gIlc
  rlSig->LoadgIlc();
  rlUnd->LoadgIlc();
  gIlc = rlSig->GetIlcRun();

  // Now load kinematics and event header
  rlSig->LoadKinematics();
  rlSig->LoadHeader();
  rlUnd->LoadKinematics();
  rlUnd->LoadHeader();

  // Loop on events: check that MC and data contain the same number of events
  Long64_t nevSig = rlSig->GetNumberOfEvents();
  Long64_t nevUnd = rlUnd->GetNumberOfEvents();
  Long64_t nSigPerUnd = nevSig/nevUnd;

  cout << nevSig << " signal events" << endl;
  cout << nevUnd << " underlying events" << endl;
  cout << nSigPerUnd << " signal events per one underlying" << endl;

  for (Int_t iev=0; iev<nevSig; iev++) {
    cout << "Signal event " << iev << endl;
    Int_t ievUnd = iev/nSigPerUnd;
    cout << "Underlying event " << ievUnd << endl;

    // Get signal ESD
    tSig->GetEntry(iev);
    // Get signal kinematics
    rlSig->GetEvent(iev);
    // Get underlying kinematics
    rlUnd->GetEvent(ievUnd);

    // Particle stack
    IlcStack * stackSig = rlSig->Stack();
    Int_t nPartSig = stackSig->GetNtrack();
    IlcStack * stackUnd = rlUnd->Stack();
    Int_t nPartUnd = stackUnd->GetNtrack();

    Int_t nrec = esdSig->GetNumberOfTracks();
    cout << nrec << " reconstructed tracks" << endl;
    for(Int_t irec=0; irec<nrec; irec++) {
      IlcESDtrack * track = esdSig->GetTrack(irec);
      UInt_t label = TMath::Abs(track->GetTPCLabel());
      if (label>=10000000) {
	// Underlying event. 10000000 is the
	// value of fkMASKSTEP in IlcRunDigitizer
// 	cout << " Track from the underlying event" << endl;
	label %=10000000;
	if (label>=nPartUnd) continue;
	TParticle * part = stackUnd->Particle(label);
 	if(part) part->Print();
      }
      else {
	cout << " Track " << label << " from the signal event" << endl;
	if (label>=nPartSig) {
	  cout <<"Strange, label outside the range "<< endl;
	  continue;
	}
	TParticle * part = stackSig->Particle(label);
	if(part) part->Print();
      }

    }

  }

  fSig->Close();

  timer.Stop();
  timer.Print();
}
