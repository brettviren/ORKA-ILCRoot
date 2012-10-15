void IlcTARGETPrintRecPoints(TString rfn="gilc.root",Int_t mod=-1,
			  Int_t evnt=-1){
  // Macro to print out the recpoints for all or a specific module

  // Dynamically link some shared libs
  if (gClassTable->GetID("IlcRun") < 0) {
    gROOT->LoadMacro("loadlibs.C");
    loadlibs();
  } 
  else {
    if(gIlc){
      delete gIlc->GetRunLoader();
      delete gIlc;
      gIlc=0;
    }
  }

  gROOT->LoadMacro("$(ILC_ROOT)/TARGET/IlcTARGETstandard.C");

  IlcRunLoader *rl = AccessFile(rfn); // Set up to read in Data
  Int_t retval = rl->LoadHeader();
  if (retval){
    cerr<<"IlcTARGETPrintRecPoints.C : LoadHeader returned error"<<endl;
    return;
  }

  IlcTARGETLoader* TARGETloader =  (IlcTARGETLoader*) rl->GetLoader("TARGETLoader");

  if(!TARGETloader){
    cerr<<"IlcTARGETPrintRecPoints.C :  TARGET loader not found"<<endl;
    return;
  }

  TARGETloader->LoadHits("read");
  TARGETloader->LoadDigits("read");
  TARGETloader->LoadRecPoints("read");
  IlcTARGET *TARGET = (IlcTARGET*)gIlc->GetDetector("TARGET");
  if(!TARGET){
    cout << "Error: no TARGET found. Aborting"<<endl;
    return;
  } // end if !TARGET

  Int_t evNumber1 = 0;
  Int_t evNumber2 = gIlc->GetEventsPerRun();
  if(evnt>=0){
    evNumber1 = evnt;
    evNumber2 = evnt+1;
  } // end if evnt>=0
  Int_t mod1 = 0;
  Int_t mod2 = TARGET->GetTARGETgeom()->GetIndexMax();
  if(mod>=0){
    mod1 = mod;
    mod2 = mod+1;
  } // end if mod>=0
  TClonesArray *rpa;
  IlcTARGETRecPoint *rp = 0;
  IlcTARGETDetTypeRec* rec = new IlcTARGETDetTypeRec();
  rec->SetLoader(TARGETloader);
  rec->SetTARGETgeom(TARGET->GetTARGETgeom());
  rec->SetDefaults();

  Int_t event,m,i,i2;
  for(event = evNumber1; event < evNumber2; event++){
    rl->GetEvent(event);
    rec->SetTreeAddress();
    for(m=mod1;m<mod2;m++){
      rec->ResetRecPoints();
      TTree *TR = TARGETloader->TreeR();
      TR->GetEvent(m);
      rpa = rec->RecPoints();
      i2 = rpa->GetEntriesFast();
      cout <<  "Event=" << event << " module=" << m <<
	" Number of Recpoints=" << i2 <<endl;
      for(i=0;i<i2;i++){
	rp = (IlcTARGETRecPoint*)(rpa->At(i));
	cout << i << " ";
	rp->Print((ostream*)cout);
	cout << endl;
      } // end for i
    } // end for m
  } // end for event

}
