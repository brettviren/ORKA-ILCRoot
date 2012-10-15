void SDigits2Digits(){
 gSystem->Setenv("CONFIG_SPLIT_FILE","1") ;
 
 TFile * f = TFile::Open("gilc.root","update") ;
 gIlc = (IlcRun*) f->Get("gIlc") ;  
 IlcPVBARv1*    fPVBAR  = (IlcPVBARv1 *)gIlc->GetDetector("PVBAR") ;  
 gIlc->SetDebug(10) ; 
 gIlc->SDigits2Digits("PVBAR") ;



}
