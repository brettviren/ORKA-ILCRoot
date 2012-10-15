// $Id: gen.C 39165 2010-02-25 16:12:24Z ivana $
//
// Generation of kinematics tree which can be then used as an extrenal
// event generator.
// According to: $ILC_ROOT/test/genkine/gen/fastGen.C
//
// By I. Hrivnacova, IPN Orsay

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TH1F.h>
#include <TStopwatch.h>
#include <TDatime.h>
#include <TRandom.h>
#include <TDatabasePDG.h>
#include <TParticle.h>
#include <TArrayI.h>

#include "IlcGenerator.h"
#include "IlcPDG.h"
#include "IlcRunLoader.h"
#include "IlcRun.h"
#include "IlcStack.h"
#include "IlcHeader.h"
#include "PYTHIA6/IlcGenPythia.h"
#include "PYTHIA6/IlcPythia.h"
#endif

void gen(Int_t nev = 1, char* filename = "gilc.root")
{
  // Load libraries
  // gSystem->SetIncludePath("-I$ROOTSYS/include -I$ILC_ROOT/include -I$ILC_ROOT");
  gSystem->Load("liblhapdf.so");      // Parton density functions
  gSystem->Load("libEGPythia6.so");   // TGenerator interface
  gSystem->Load("libpythia6.so");     // Pythia
  gSystem->Load("libIlcPythia6.so");  // ILC specific implementations

  IlcPDG::AddParticlesToPdgDataBase();
  TDatabasePDG::Instance();

  // Run loader
  IlcRunLoader* rl = IlcRunLoader::Open("gilc.root","FASTRUN","recreate");
  
  rl->SetCompressionLevel(2);
  rl->SetNumberOfEventsPerFile(nev);
  rl->LoadKinematics("RECREATE");
  rl->MakeTree("E");
  gIlc->SetRunLoader(rl);
  
  //  Create stack
  rl->MakeStack();
  IlcStack* stack = rl->Stack();
  
  //  Header
  IlcHeader* header = rl->GetHeader();
  
  //  Create and Initialize Generator
  gROOT->LoadMacro("$ILC_ROOT/test/vmctest/ppbench/genPPbenchConfig.C");
  IlcGenerator* gener = genPPbenchConfig();

  // Go to gilc.root
  rl->CdGAFile();

  // Forbid some decays. Do it after gener->Init(0, because
  // the initialization of the generator includes reading of the decay table.
  // ...

  //
  // Event Loop
  //
  
  TStopwatch timer;
  timer.Start();
  for (Int_t iev = 0; iev < nev; iev++) {
    
    cout <<"Event number "<< iev << endl;
    
    // Initialize event
    header->Reset(0,iev);
    rl->SetEventNumber(iev);
    stack->Reset();
    rl->MakeTree("K");
    
    // Generate event
    stack->Reset();
    stack->ConnectTree(rl->TreeK());
    gener->Generate();
    cout << "Number of particles " << stack->GetNprimary() << endl;
    
    // Finish event
    header->SetNprimary(stack->GetNprimary());
    header->SetNtrack(stack->GetNtrack());  
    
    // I/O
    stack->FinishEvent();
    header->SetStack(stack);
    rl->TreeE()->Fill();
    rl->WriteKinematics("OVERWRITE");
    
  } // event loop
  timer.Stop();
  timer.Print();
  
  //                         Termination
  //  Generator
  gener->FinishRun();
  //  Write file
  rl->WriteHeader("OVERWRITE");
  gener->Write();
  rl->Write();
}
