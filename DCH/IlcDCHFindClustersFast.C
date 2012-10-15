Int_t IlcDCHFindClustersFast(Int_t n=0) {
   
   IlcRunLoader* rl = IlcRunLoader::Open("gilc.root");
   if (rl == 0x0) {
      cerr<<"Can not open session"<<endl;
      return 1;
   }
   
   IlcLoader *dchl = (IlcLoader*)rl->GetLoader("DCHLoader");
   if (dchl == 0x0) {
      cerr<<"Can not get DCH Loader"<<endl;
      return 1;
   }
   if (dchl->LoadHits()) {
      cerr<<"Error occured while loading hits"<<endl;
      return 1;
   }


   if (dchl->LoadRecPoints("recreate")) {
      cerr<<"Error occured while loading digits"<<endl;
      return 1;
   }
   
   if (rl->LoadgIlc()) {
      cerr<<"Error occured while l"<<endl;
      return 1;
   }
   rl->LoadKinematics();
   rl->LoadTrackRefs();
   rl->LoadHeader();
   gIlc=rl->GetIlcRun();
   if (!gIlc) {
      cerr<<"Can't get gIlc !\n";
      return 1;
   }

   TDirectory *cwd = gDirectory;

   IlcDCH *dch = (IlcDCH*)gIlc->GetDetector("DCH"); 
//   Int_t ver = dch->IsVersion(); 
//   cerr<<"DCH version "<<ver<<" has been found !\n";
   
   /*if (!gGeoManager) {
     TString geom = "geometry.root";
     TGeoManager::Import(geom.Data());
     }*/
   IlcDCHParam *param=dch->GetParam();
   //param->ReadGeoMatrices();
//   param->SetMWPCReadout(false);

   rl->CdGAFile();

   TStopwatch timer;
   
//    IlcDCHwireposition *wireMatr = new IlcDCHwireposition();
//    IlcDCHclusterizer *clus = new IlcDCHclusterizer("clusterer", "DCH clusterizer");
//    IlcDCHFast dchfast(wireMatr,clus);
   IlcDCHFast dchfast;
   //n = 30;
  
  if (n==0) n = rl->GetNumberOfEvents();

   for(Int_t i=0;i<n;i++){
     printf("Processing event %d\n",i);
     rl->GetEvent(i);
     //     tpcfast.Hits2ExactClusters(rl);
    
     dchfast.Hits2Clusters(rl,i);

   } 
   
   timer.Stop(); 
   timer.Print();
   //cleans everything
   delete rl;
   
//    delete wireMatr;
//    delete clus;


   return 0;
}
