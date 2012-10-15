void IlcTARGETSD2D(TString inFile, TString outFile);

void IlcTARGETSDigits2Digits(TString inFile= "gilc.root", TString outFile = "")
 {
    // This macro takes SDigits and produces Digits. No merging is done
    // and only one gilc.root file is used. 
    // Dynamically link some shared libs 
    TStopwatch timer;
    if(gIlc)
     {
       delete gIlc->GetRunLoader();
       delete gIlc;
       gIlc = 0x0;
    } // end if gIlc
    cout << "Creating digits from summable digits for the TARGET..." << endl;
    IlcTARGETSD2D(inFile,outFile);
    timer.Stop(); 
    timer.Print();
}

void IlcTARGETSD2D(TString inFile, TString outFile){
  IlcRunDigitizer * manager = new IlcRunDigitizer(1,1);
  manager->SetInputStream(0,inFile);
  if(outFile != "")manager->SetOutputFile(outFile);
  IlcTARGETDigitizer *dTARGET  = new IlcTARGETDigitizer(manager);
  manager->Exec("");
  delete manager;
}






