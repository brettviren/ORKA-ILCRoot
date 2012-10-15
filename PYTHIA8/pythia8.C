IlcGenerator*  CreateGenerator();

void pythia8(Int_t nev = 1, char* filename = "gilc.root")
{
//  Runloader
    gSystem->Load("liblhapdf.so");    
    gSystem->Load("libEGPythia6.so"); 
    gSystem->Load("libpythia6.so");   
    gSystem->Load("libIlcPythia6.so");
    gSystem->Load("libpythia8.so");   
    gSystem->Load("libIlcPythia8.so");
    
    IlcRunLoader* rl = IlcRunLoader::Open("gilc.root","FASTRUN","recreate");
    
    rl->SetCompressionLevel(2);
    rl->SetNumberOfEventsPerFile(10000);
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
//	    printf("Particle %d\n", mpart);
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
    IlcGenPythiaPlus* gener = new IlcGenPythiaPlus(IlcPythia8::Instance());
//
//
    gener->SetProcess(kPyMbDefault);
//   Centre of mass energy 
    gener->SetEnergyCMS(7000.);
//   Initialize generator    
    gener->SetEventListRange(-1, 10);
    return gener;
}








