void Digits2RecParticles(){
  gSystem->Setenv("CONFIG_SPLIT_FILE","1") ;   

  TFile * fRootFile = TFile::Open("gilc.root","update");

  //========== Get IlcRun object from file 
  gIlc = (IlcRun*) fRootFile->Get("gIlc") ;

  //=========== Get the PVBAR object and associated geometry from the file      
  IlcPVBARv1 * fPVBAR  = (IlcPVBARv1 *)gIlc->GetDetector("PVBAR") ;
  IlcPVBARGeometry * fGeom  = IlcPVBARGeometry::GetInstance( fPVBAR->GetGeometry()->GetName(), fPVBAR->GetGeometry()->GetTitle() );
 
  IlcPVBARIndexToObject * fObjGetter = IlcPVBARIndexToObject::GetInstance(fPVBAR) ; 


  
  //========== Create the Clusterizer
  IlcPVBARClusterizerv1 * fClu = new IlcPVBARClusterizerv1() ; 
  
  //========== Creates the track segment maker
  IlcPVBARTrackSegmentMakerv1 * fTrs = new IlcPVBARTrackSegmentMakerv1()  ;
  //	  fTrs->UnsetUnfoldFlag() ; 
  
  //========== Creates the particle identifier
  IlcPVBARPIDv1* fPID = new IlcPVBARPIDv1() ;
  fPID->SetShowerProfileCuts(0.3, 1.8, 0.3, 1.8 ) ;       
      
  //========== Creates the Reconstructioner
  IlcPVBARReconstructor * fRec = new IlcPVBARReconstructor(fClu, fTrs, fPID) ; 
  // fRec -> SetDebugReconstruction(kTRUE);     
     
  gIlc->GetEvent(0) ;

  gIlc->TreeD()->GetEvent(0) ;

  if (gSystem->Getenv("CONFIG_SPLIT_FILE"))    
   if(gIlc->TreeR() == 0) gIlc->MakeTree("R","Reco.PVBAR.root");
  else 
   if(gIlc->TreeR() == 0) gIlc->MakeTree("R");    

  //=========== Do the reconstruction
  fPVBAR->Reconstruction(fRec);    
  

}
