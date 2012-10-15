void IlcTARGETDumpVertices(Int_t firstEv=0, Int_t noev=1,
                        TString fileimp="gilc.root"){
  // This is a simple example on how to access the vertex objects
  Int_t evmax = firstEv+noev;
  IlcRunLoader *rl = IlcRunLoader::Open(fileimp.Data());
  IlcTARGETLoader* TARGETloader =  (IlcTARGETLoader*) rl->GetLoader("TARGETLoader");
  TARGETloader->LoadVertices();
  for(Int_t i=firstEv; i<evmax; i++){
    rl->GetEvent(i);
    IlcESDVertex *vert = TARGETloader->GetVertex();
    if(vert){
      cout <<"===============================================\n";
      cout <<" Event n. "<<i<<endl;
      vert->PrintStatus();
    }
  }
}
