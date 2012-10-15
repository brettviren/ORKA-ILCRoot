void MakePVBARGeometry(){
   //Fill OADB object with PVBAR rotation-alignment matrixes
   //To fill this file you need any IlcESD.root from official reconstruction
   //download it to the current directory
   
  IlcOADBContainer * geomContainer = new IlcOADBContainer("PVBARRotationMatrixes");
  TObjArray *matrixes = new TObjArray(5) ;
  matrixes->SetName("PVBARRotationMatrixes");

  //Read them from ESDs
  TFile* esdFile = TFile::Open("IlcESDs.root");
  if (!esdFile || !esdFile->IsOpen()) {
    Error("MakePVBARGeometry", "Can not open ESD file, please download any from official reconstruction.");
    return ;
  }
  IlcESDEvent * event = new IlcESDEvent;
  TTree* tree = (TTree*) esdFile->Get("esdTree");
  if (!tree) {
    Error("MakePVBARGeometry", "no ESD tree found");
    return ;
  }
  event->ReadFromTree(tree);
  tree->GetEvent(0) ;

  for(Int_t mod=0; mod<5; mod++) {
    TGeoHMatrix * m = event->GetPVBARMatrix(mod) ;
    if(!m)
      matrixes->AddAt(0x0,mod);
    else
      matrixes->AddAt(new TGeoHMatrix(*m), mod) ;
  }
  esdFile->Close() ;

  //Controll
  for(Int_t mod=0; mod<5; mod++) {
     printf("Module %d: \n", mod) ;
     if(matrixes->At(mod))
       ((TGeoHMatrix*)matrixes->At(mod))->Print() ;
     else
       printf("Maxrix=null \n") ;
  }
  //Fill 
  geomContainer->AppendObject(matrixes,90000,IlcCDBRunRange::Infinity()) ;
  geomContainer->AddDefaultObject(matrixes) ;
  geomContainer->WriteToFile("PVBARGeometry.root");
  
}
