void fastGen(Int_t nev = 1, char* filename = "gilc.root")
{
  // Run loader
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
  
  //  Generator
  IlcGenPythia *gener = new IlcGenPythia(-1);
  gener->SetMomentumRange(0,999999);
  gener->SetProcess(kPyMb);
  gener->SetEnergyCMS(14000.);
  gener->SetThetaRange(45, 135);
  gener->SetPtRange(0., 1000.);
  gener->SetStack(stack);
  gener->Init();
  rl->CdGAFile();
  //
  //                        Event Loop
  //
  for (Int_t iev = 0; iev < nev; iev++) {
    
    //  Initialize event
    header->Reset(0,iev);
    rl->SetEventNumber(iev);
    stack->Reset();
    rl->MakeTree("K");
    
    //  Generate event
    Int_t nprim = 0;
    Int_t ntrial = 0;
    Int_t minmult = 1000;

    while(nprim<minmult) {
      // Selection of events with multiplicity
      // bigger than "minmult"
      stack->Reset();
      gener->Generate();
      ntrial++;
      nprim = stack->GetNprimary();
      
    }
    cout << "Number of particles " << nprim << endl;
    cout << "Number of trials " << ntrial << endl;
    
    //  Finish event
    header->SetNprimary(stack->GetNprimary());
    header->SetNtrack(stack->GetNtrack());  
    
    //      I/O
    stack->FinishEvent();
    header->SetStack(stack);
    rl->TreeE()->Fill();
    rl->WriteKinematics("OVERWRITE");
    
  } // event loop
  
  //                         Termination
  //  Generator
  gener->FinishRun();
  //  Stack
  stack->FinishRun();
  //  Write file
  rl->WriteHeader("OVERWRITE");
  gener->Write();
  rl->Write();
}




