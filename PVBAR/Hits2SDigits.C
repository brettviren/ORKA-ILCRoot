void Hits2SDigits(){
 gSystem->Setenv("CONFIG_SPLIT_FILE","1") ;
 if (gSystem->Getenv("CONFIG_SPLIT_FILE")) 
  cout << "SPLIT" << endl; 
 else
  cout << "NO SPLIT" << endl ; 
 TFile * f = TFile::Open("gilc.root","update");
 gIlc = (IlcRun*) f->Get("gIlc") ;  
 IlcPVBARv1*    fPVBAR  = (IlcPVBARv1 *)gIlc->GetDetector("PVBAR") ;  
 gIlc->SetDebug(10) ; 
 gIlc->Hits2SDigits("PVBAR") ;



}
