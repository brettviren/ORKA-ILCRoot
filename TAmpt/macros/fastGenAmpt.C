// Example: generation of kinematics tree with selected properties.
// Below we select events containing the decays D* -> D0 pi, D0 -> K- pi+
// inside the barrel part of the ILC detector (45 < theta < 135)

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TH1F.h>
#include <TStopwatch.h>
#include <TDatime.h>
#include <TRandom.h>
#include <TDatabasePDG.h>
#include <TParticle.h>
#include <TArrayI.h>
#include <TTree.h>
#include <TPDGCode.h>
#include "IlcGenerator.h"
#include "IlcPDG.h"
#include "IlcRunLoader.h"
#include "IlcDecayer.h"
#include "IlcDecayerPythia.h"
#include "IlcRun.h"
#include "IlcStack.h"
#include "IlcHeader.h"
#include "IlcGenAmpt.h"
#endif

void fastGenAmpt(Int_t nev = 100, const char* filename = "gilc.root")
{
  IlcPDG::AddParticlesToPdgDataBase();
  TDatabasePDG::Instance();

  // Run loader
  IlcRunLoader* rl = IlcRunLoader::Open(filename,"FASTRUN","recreate");
  rl->SetCompressionLevel(2);
  rl->SetNumberOfEventsPerFile(nev);
  rl->LoadKinematics("RECREATE");
  rl->MakeTree("E");
  rl->SetNumberOfEventsPerRun(nev);
  gIlc->SetRunLoader(rl);
  
  // Create stack
  rl->MakeStack();
  IlcStack* stack = rl->Stack();
  
  // Header
  IlcHeader* header = rl->GetHeader();
  
  IlcGenAmpt *genHi = new IlcGenAmpt(-1);
//=============================================================================
// THE DECAYER
  IlcDecayer *decayer = new IlcDecayerPythia();
  cout << "*****************************************" << endl;
  genHi->SetForceDecay( kHadronicD );
  genHi->SetDecayer( decayer );
//=============================================================================
  genHi->SetEnergyCMS(2760);
  genHi->SetReferenceFrame("CMS");
  genHi->SetProjectile("A", 208, 82);
  genHi->SetTarget    ("A", 208, 82);
  genHi->SetIsoft(4); //1: defaul - 4: string melting
  genHi->SetPtHardMin (3);
  //genHi->SetImpactParameterRange(9.,9.5);
  genHi->SetImpactParameterRange(0.,20.0);
  genHi->SetNtMax(150); //NTMAX: number of timesteps (D=150)
  genHi->SetXmu(3.2264); //parton screening mass in fm^(-1) (D=3.2264d0)
  genHi->SetJetQuenching(0); // enable jet quenching
  genHi->SetShadowing(1);    // enable shadowing
  genHi->SetDecaysOff(1);    // neutral pion and heavy particle decays switched off
  genHi->SetSpectators(0);   // track spectators 
  genHi->Init();

  rl->CdGAFile();

  TStopwatch timer;
  timer.Start();
  for (Int_t iev = 0; iev < nev; iev++) {
    cout <<"Event number "<< iev << endl;
    //  Initialize event
    header->Reset(0,iev);
    rl->SetEventNumber(iev);
    stack->Reset();
    rl->MakeTree("K");
    Int_t nprim = 0;
    Int_t ntrial = 0;
    Int_t ndstar = 0;
    stack->Reset();
    stack->ConnectTree(rl->TreeK());
    genHi->Generate();
    ntrial++;
    nprim = stack->GetNprimary();
    cout << "Number of particles " << nprim << endl;
    cout << "Number of trials " << ntrial << endl;
    header->SetNprimary(stack->GetNprimary());
    header->SetNtrack(stack->GetNtrack());  
    stack->FinishEvent();
    header->SetStack(stack);
    rl->TreeE()->Fill();
    rl->WriteKinematics("OVERWRITE");
  } // event loop
  timer.Stop();
  timer.Print();
  genHi->FinishRun();
  rl->WriteHeader("OVERWRITE");
  genHi->Write();
  rl->Write();
}
