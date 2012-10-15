IlcGenerator*  CreateGenerator();

void fastGen(Int_t nev = 1, char* filename = "gilc.root")
{
//  Load libraries
  gSystem->Load("liblhapdf.so");
  gSystem->Load("libpythia6.so");
  gSystem->Load("libEGPythia6.so");
  gSystem->Load("libIlcPythia6.so");

//  Runloader
    
    IlcRunLoader* rl = IlcRunLoader::Open("gilc.root","FASTRUN","recreate");
    
    rl->SetCompressionLevel(2);
    rl->SetNumberOfEventsPerFile(nev);
    rl->LoadKinematics("RECREATE");
    rl->MakeTree("E");
    gIlc->SetRunLoader(rl);

//  Create stack
    rl->MakeStack();
    IlcStack* stack      = rl->Stack();
 
//  Header
    IlcHeader* header = rl->GetHeader();
//
//  Create and Initialize Generator
    IlcGenerator *gener = CreateGenerator();
    gener->Init();
    gener->SetStack(stack);
    
//
//                        Event Loop
//
    Int_t iev;
     
    for (iev = 0; iev < nev; iev++) {

	printf("\n \n Event number %d \n \n", iev);
	
//  Initialize event
	header->Reset(0,iev);
	rl->SetEventNumber(iev);
	stack->Reset();
	rl->MakeTree("K");
//	stack->ConnectTree();
    
//  Generate event
	gener->Generate();
//  Analysis
	Int_t npart = stack->GetNprimary();
	printf("Analyse %d Particles\n", npart);
	for (Int_t part=0; part<npart; part++) {
	    TParticle *MPart = stack->Particle(part);
	    Int_t mpart  = MPart->GetPdgCode();
	    printf("Particle %d\n", mpart);
	}
	
//  Finish event
	header->SetNprimary(stack->GetNprimary());
	header->SetNtrack(stack->GetNtrack());  
//      I/O
//	
	stack->FinishEvent();
	header->SetStack(stack);
	rl->TreeE()->Fill();
	rl->WriteKinematics("OVERWRITE");

    } // event loop
//
//                         Termination
//  Generator
    gener->FinishRun();
//  Write file
    rl->WriteHeader("OVERWRITE");
    gener->Write();
    rl->Write();
    
}


IlcGenerator*  CreateGenerator()
{
    gener = new IlcGenPythia(1);
//
//
//   vertex position and smearing 
    gener->SetVertexSmear(kPerEvent);
//   structure function
    gener->SetStrucFunc(kCTEQ6);
//   charm, beauty, charm_unforced, beauty_unforced, jpsi, jpsi_chi, mb
    gener->SetProcess(kPyJets);
//   Centre of mass energy 
    gener->SetEnergyCMS(5500.);
//   Pt transfer of the hard scattering
    gener->SetPtHard(50.,50.2);
//   Initialize generator    
    return gener;
}












