void IlcTARGETPrintHits(TString hfn="gilc.root",Int_t mod=-1,
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

  IlcRunLoader *rl = AccessFile(hfn); // Set up to read in Data
  Int_t retval = rl->LoadHeader();
  if (retval){
    cerr<<"IlcTARGETPrintHits.C : LoadHeader returned error"<<endl;
    return;
  }

  IlcTARGETLoader* TARGETloader =  (IlcTARGETLoader*) rl->GetLoader("TARGETLoader");

  if(!TARGETloader){
    cerr<<"IlcTARGETPrintHits.C :  TARGET loader not found"<<endl;
    return;
  }

  TARGETloader->LoadHits("read");
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
  IlcTARGEThit *hp = 0;

  Int_t nmodules,size=-1;
  Int_t event,m,i,i2,hit,trk;
  for(event = evNumber1; event < evNumber2; event++){
    cout<<"Processing event "<<event<<endl;
    rl->GetEvent(event);
    TARGET->InitModules(size,nmodules);
    TARGET->FillModules(event,0,-1," "," ");
    for(m=mod1;m<mod2;m++){
      i2 = (TARGET->GetModule(m))->GetNhits();
      cout <<  "Event=" << event << " module=" << m <<
	" Number of Hits=" << i2 <<endl;
      for(i=0;i<i2;i++){
	trk = (TARGET->GetModule(m))->GetHitTrackIndex(i);
	hit = (TARGET->GetModule(m))->GetHitHitIndex(i);
	hp  = (TARGET->GetModule(m))->GetHit(i);
	cout << i << " trk#="<<trk<<" hit#="<< hit << " ";
	hp->Print((ostream*)cout);
	cout << endl;
      } // end for i
    } // end for m
    TARGET->ClearModules();
  } // end for event
}
