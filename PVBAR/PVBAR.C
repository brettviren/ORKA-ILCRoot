PVBAR (Int_t event_number=0,Float_t SignalStep=0.001,Int_t SignalMin=15)
{
// From Int_t Sread ()

   // Dynamically link some shared libs
   if (gClassTable->GetID("IlcRun") < 0) {
      gSystem->Load("libGeant3Dummy.so");   // a dummy version of Geant3
      gSystem->Load("PVBAR/libPVBARreconstruction.so");        // the standard Ilc classes 
      gSystem->Load("libgilc.so");        // the standard Ilc classes 
   }
    
   // Connect the Root Gilc file containing Geometry, Kine and Hits
   TFile *file = (TFile*)gROOT->GetListOfFiles()->FindObject("gilc.root");
   if (!file) file = new TFile("gilc.root");

   // Get IlcRun object from file or create it if not on file
   if (!gIlc) {
      gIlc = (IlcRun*)file->Get("gIlc");
      if (gIlc) printf("IlcRun object found on file\n");
      if (!gIlc) gIlc = new IlcRun("gIlc","Ilc test program");
   }
////////////////////////////////

  IlcPVBAR *PVBAR= gIlc->GetDetector("PVBAR");
  if( NULL==PVBAR )
  {
    cout << "Can't find PVBAR detector!\n";
    exit(1);
  }

  PVBAR->SetTreeAddress();

  if( 0==PVBAR->fTreePVBAR->GetEvent(event_number) )
  {
    printf("Cannot read event number %d\n",event_number);
    return;
  }

  printf("This is event number %d from %d\n",
         event_number,PVBAR->fTreePVBAR->GetEntries());

  for( int i=0; i<PVBAR->fCradles->GetEntries(); i++ )
  {
    IlcPVBARCradle &cradle = PVBAR->GetCradle(i);
    printf("===============================================================\n");
    printf("Cradle %d\n",i+1);
    cradle.Print();
    //cout.flush();
    

    cradle.Reconstruction(SignalStep,SignalMin);

    printf("It were %d particles in that cradle\n",cradle.GetParticles().GetEntries());
    for( int j=0; j<cradle.GetParticles().GetEntries(); j++ )
    {
      TObjArray &pp = cradle.GetParticles();
      IlcPVBARgamma *g = (IlcPVBARgamma *) pp.At(j);
      printf("%3d  ",j+1);
      g->Print();
    }

    printf("\nReconstruction: %d gammas\n",cradle.GetGammasReconstructed().GetEntries());
    for( int j=0; j<cradle.GetGammasReconstructed().GetEntries(); j++ )
    {
      TObjArray &pp = cradle.GetGammasReconstructed();
      IlcPVBARgamma *g = (IlcPVBARgamma *) pp.At(j);
      printf("%3d  ",j+1);
      g->Print();
    }

    //cout.flush();
    
  }

////////////////////////////////////////////////////////////////////////////////

  printf("Done\n");
}
