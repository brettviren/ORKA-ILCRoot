ReadImpacts(Int_t nEvents=1,char* file="gilc.root")
{

  // Script reads PVBAR impacts and prints them
  // Impacts are exacts values of the track coming to EMC, CPV or PPSD
  // and was stored to separate branches of TreeH by IlcPVBARvImpacts
  //
  // Yuri Kharlov 4 June 2001

  f = new TFile(file,"readonly");
  (IlcRun*)gIlc=(IlcRun*)f->Get("gIlc");
  IlcPVBARvImpacts *  fPVBAR  = (IlcPVBARvImpacts *)gIlc->GetDetector("PVBAR") ;
  IlcPVBARGeometry *  fGeom  = IlcPVBARGeometry::GetInstance(fPVBAR->GetGeometry()->
                                                           GetName(),
                                                           fPVBAR->GetGeometry()->GetTitle() ) ;
  Int_t nPVBARModules = fGeom->GetNModules();
  Int_t nCPVModules  = fGeom->GetNCPVModules();
  Int_t nPPSDModules = fGeom->GetNPPSDModules();

  TBranch * branchEMCimpacts;
  TBranch * branchCPVimpacts;
  TBranch * branchPPSDimpacts;
  TList * fEmcImpacts ;
  TList * fCpvImpacts ;
  TList * fPpsdImpacts ;

  // Loop over events
  for (Int_t iEvent=0; iEvent<nEvents; iEvent++) {
    printf("===========> Event %5d <====================\n",iEvent);
    gIlc->GetEvent(iEvent) ;
    
    // Get branches EMC, CPV and PPSD impacts
    if (! (branchEMCimpacts =gIlc->TreeH()->GetBranch("PVBAREmcImpacts")) )  return 1;
    if (! (branchCPVimpacts =gIlc->TreeH()->GetBranch("PVBARCpvImpacts")) )  return 1;
    if (! (branchPPSDimpacts=gIlc->TreeH()->GetBranch("PVBARPpsdImpacts")) ) return 1;
    
    // Loop over primary tracks
    for (itrack=0; itrack < gIlc->GetNtrack(); itrack++){
      // Set addresses of impacts
      branchEMCimpacts ->SetAddress(&fEmcImpacts) ;
      branchCPVimpacts ->SetAddress(&fCpvImpacts) ;
      branchPPSDimpacts->SetAddress(&fPpsdImpacts) ;
      branchEMCimpacts ->GetEntry(itrack,0);
      branchCPVimpacts ->GetEntry(itrack,0);
      branchPPSDimpacts->GetEntry(itrack,0);
      
      TClonesArray  *impacts;
      IlcPVBARImpact *impact;
      Int_t iModule;

      // Do loop over EMC modules
      for (iModule=0; iModule<nPVBARModules; iModule++) {
	impacts = (TClonesArray *)fEmcImpacts->At(iModule);
	// Do loop over impacts in the module
	for (Int_t iImpact=0; iImpact<impacts->GetEntries(); iImpact++) {
	  impact=(IlcPVBARImpact*)impacts->At(iImpact);
	  printf("EMC  module %d: ",iModule);
	  impact->Print();
	}
      }

      // Do loop over CPV modules
      for (iModule=0; iModule<nCPVModules; iModule++) {
	impacts = (TClonesArray *)fCpvImpacts->At(iModule);
	// Do loop over impacts in the module
	for (Int_t iImpact=0; iImpact<impacts->GetEntries(); iImpact++) {
	  impact=(IlcPVBARImpact*)impacts->At(iImpact);
	  printf("CPV  module %d: ",iModule);
	  impact->Print();
	}
      }

      // Do loop over PPSD modules
      for (iModule=0; iModule<nPPSDModules; iModule++) {
	impacts = (TClonesArray *)fPpsdImpacts->At(iModule);
	// Do loop over impacts in the module
	for (Int_t iImpact=0; iImpact<impacts->GetEntries(); iImpact++) {
	  impact=(IlcPVBARImpact*)impacts->At(iImpact);
	  printf("PPSD Module %d: ",iModule);
	  impact->Print();
	}
      }

    }
  }
}
