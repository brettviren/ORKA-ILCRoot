IlcGenerator*  CreateGenerator();

void fastGenDPMjet(Int_t nev = 1, char* filename = "gilc.root")
{
//  Runloader
    gSystem->Load("liblhapdf"); 
    gSystem->Load("libpythia6");     
    gSystem->Load("libdpmjet.so");
    gSystem->Load("libTDPMjet.so");
    
    IlcRunLoader* rl = IlcRunLoader::Open("gilc.root","FASTRUN","recreate");
    IlcPythiaRndm::SetPythiaRandom(new TRandom());

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
	    Int_t pdg  = MPart->GetPdgCode();
	    Int_t pis  = MPart->GetStatusCode();
//	    printf("Particle %5d %5d\n", part, pdg, pis);
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
    //  kDpmMb
    //  kDpmMbNonDiffr
    //  kDpmDiffr
    //  kDpmSingleDiffr
    //  kDpmDoubleDiffr
    gener = new IlcGenDPMjet(1);
    gener->SetProcess(kDpmMb);
    gener->SetProjectile("P", 1, 1);
    gener->SetTarget("P", 1, 1);
    gener->SetEnergyCMS(14000.);
    return gener;
}












