void IlcTARGETHits2FastRecPoints (Int_t evNumber1=0,Int_t evNumber2=2e9, Int_t nsignal=10025, Int_t size=-1) 
{
  /////////////////////////////////////////////////////////////////////////
  //   
  //   This macro creates fast recpoints, optionally on a separate file
  //   
  /////////////////////////////////////////////////////////////////////////


  // Dynamically link some shared libs

  if (gClassTable->GetID("IlcRun") < 0) {
    gROOT->LoadMacro("loadlibs.C");
    loadlibs();
  } else if (gIlc){
     delete gIlc->GetRunLoader();
     delete gIlc; 
     gIlc=0;
  }

  // Connect the Root Gilc file containing Geometry, Kine and Hits
    IlcRunLoader* rl = IlcRunLoader::Open("gilc.root");
    if (rl == 0x0)
     {
       ::Error("IlcTARGETHits2FastRecPoints.C","Can not open session RL=NULL");
       return;
     }

    Int_t retval = rl->LoadgIlc();

    if (retval)
     {
       ::Error("IlcTARGETHits2FastRecPoints.C","LoadgIlc returned error");
       delete rl;
       return;
     }

    gIlc=rl->GetIlcRun();
    rl->LoadHeader();
    retval = rl->LoadKinematics();
    if (retval)
     {
       ::Error("IlcTARGETHits2FastRecPoints.C","LoadKinematics returned error");
       delete rl;
       return;
     }
    
    IlcTARGETLoader* gime = (IlcTARGETLoader*)rl->GetLoader("TARGETLoader");
    if (gime == 0x0)
     {
       ::Error("IlcTARGETHits2FastRecPoints.C","can not get TARGET loader");
       delete rl;
       return;
     }
    retval = gime->LoadHits("read");
    if (retval)
     {
       ::Error("IlcTARGETHits2FastRecPoints.C","LoadHits returned error");
       delete rl;
       return;
     }
    //    gime->SetRecPointsFileName("TARGET.FastRecPoints.root"); 
    retval = gime->LoadRecPoints("recreate");
    if (retval)
     {
       ::Error("IlcTARGETHits2FastRecPoints.C","LoadRecPoints returned error");
       delete rl;
       return;
     }
    
   
    TDirectory * olddir = gDirectory;
    rl->CdGAFile();
    IlcTARGETgeom* geom = (IlcTARGETgeom*)gDirectory->Get("IlcTARGETgeom");
    olddir->cd();
    if(!geom){
      Error("GetTARGETgeom","no TARGET geometry available");
      return NULL;
    }

 
   IlcTARGET *TARGET  = (IlcTARGET*) gIlc->GetModule("TARGET");
   if (!TARGET) return;

  // Set the simulation model

   

  //
  // Event Loop
  //

  Int_t nbgr_ev=0;
  TStopwatch timer;
  //IlcLog::SetGlobalDebugLevel(1);
  cout << "Creating fast reconstructed points from hits for the TARGET..." << endl;
  IlcTARGETDetTypeSim* dettyp = new IlcTARGETDetTypeSim();
  dettyp->SetDetVersion(TARGET->GetMinorVersion());
  dettyp->Init();
  dettyp->SetTARGETgeom2(geom);
  dettyp->SetLoader(gime);
  TARGET->SetDetTypeSim(dettyp);
  IlcTARGETsimulationFastPoints *simfast=new IlcTARGETsimulationFastPoints();
  IlcTARGETsimulationFastPoints *simfast2=new IlcTARGETsimulationFastPoints();
  //  simfast->SetExactPoints(true);
  //  simfast2->SetExactPoints(true);
if (TARGET->GetMinorVersion()==3){
    TARGET->SetSimulationModel(0,simfast);
    TARGET->SetSimulationModel(1,simfast2);
   }
   if (TARGET->GetMinorVersion()==2){
     TARGET->SetSimulationModel(0,simfast);
     TARGET->SetSimulationModel(1,simfast2);
   }
   if (TARGET->GetMinorVersion()==1){
     TARGET->SetSimulationModel(0,simfast);
//     TARGET->SetSimulationModel(1,simfast2);
   }
   int bgr_ev=0;
   int nbunches=1;

  //TARGET->SetSimulationModel(1,simfast2);
  int  n = rl->GetNumberOfEvents();
  
  for (int ev=0; ev< n; ev++) {
    cout << "...working on event "<< ev << " ..." << endl;
    Int_t nparticles = gIlc->GetEvent(ev);
    cout << "event         " <<ev<<endl;
    cout << "nparticles  " <<nparticles<<endl;

    if (ev < evNumber1) continue;
    if (ev > evNumber2)  return;
    if (nparticles <= 0) return;

    rl->GetEvent(ev);

    Int_t bgr_ev=ev;
    timer.Start();
    Int_t nmodules;
    TARGET->InitModules(size,nmodules);

    if (TARGET->GetBackgroundFile()->IsNull())
      TARGET->FillModules(ev,bgr_ev,nbunches,nmodules," "," ");
    else
      TARGET->FillModules(ev,bgr_ev,nbunches,nmodules,"ADD",TARGET->GetBackgroundFile()->Data());


    if (TARGET->GetMinorVersion()==3)   TARGET->HitsToFastRecPoints(ev,bgr_ev,nbunches,size," ","SPD SSD"," ");
    if (TARGET->GetMinorVersion()==2)   TARGET->HitsToFastRecPoints(ev,bgr_ev,nbunches,size," ","SPD SSD"," ");
    if (TARGET->GetMinorVersion()==1)   TARGET->HitsToFastRecPoints(ev,bgr_ev,nbunches,size," ","SPD"," ");
    //void IlcTARGET::HitsToFastRecPoints(Int_t evNumber,Int_t bgrev,Int_t size,Option_t *opt0,Option_t *opt1,const char *flnm)
    // keep gilc.root for signal and name differently the file for 
    // background when add! otherwise the track info for signal will be lost !
    // the condition below will disappear when the geom class will be
    // initialized for all versions - for the moment it is only for v5 !
    // Inputs:
    //      Int_t evnt       Event to be processed.
    //      Int_t bgrev      Background Hit tree number.
    //      Int_t size       Size used by InitModules. See InitModules.
    //      Option_t *opt0   Option passed to FillModules. See FillModules.
    //      Option_t *opt1   
    //      Test_t *flnm     File name containing the background hits..
    
    
    timer.Stop(); timer.Print();
  } // event loop 
//  delete dettyp;
  delete rl;
}

