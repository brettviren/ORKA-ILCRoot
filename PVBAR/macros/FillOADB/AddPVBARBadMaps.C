void AddPVBARBadMaps(){
  //Add OADB entry with PVBAR bad map 
  //You probably will need alien connection
  
  //Init Bad channels map
  IlcOADBContainer badmapContainer("PVBARBadMap");
  badmapContainer.InitFromFile("$ILC_ROOT/OADB/PVBAR/PVBARBadMaps.root","PVBARBadMap");

  printf("So far we stored the following list of bad maps\n") ;
  Int_t n=badmapContainer.GetNumberOfEntries() ;
  for(Int_t i=0;i<n;i++){
    TObjArray* a= (TObjArray*)badmapContainer.GetObjectByIndex(i);
    printf("Entry(%d): %s,  runs %d-%d \n",i,a->GetName(),badmapContainer.LowerLimit(i),badmapContainer.UpperLimit(i)) ;
  }
  
  
  TGrid::Connect("alien://") ;
  
  //For the period LHC10e we use map same as LHC10b
  TObjArray * lhc10e = new TObjArray(5) ;
  lhc10e->SetName("LHC10e") ;
  TFile * fLHC10e = TFile::Open("alien:///ilc/cern.ch/user/p/prsnko/BadMaps/BadMap_LHC10b.root") ;
  char key[55] ;
  if(fLHC10e->IsOpen()){
    for(Int_t mod=1;mod<4; mod++){
      sprintf(key,"PVBAR_BadMap_mod%d",mod) ;
      TH2I * h = (TH2I*)fLHC10e->Get(key) ;
      if(!h)
        lhc10e->AddAt(0x0,mod);
      else	
        lhc10e->AddAt(new TH2I(*h),mod) ;
    }
    badmapContainer.AppendObject(lhc10e,127712,130850) ;
  }

  badmapContainer.WriteToFile("PVBARBadMaps.root");
}