// Functions used to access input files - shared by several macros
 
//______________________________________________________________________
Bool_t GilcTARGETok(){
    // Checks gIlc to see that TARGET and the TARGET geometry are properly
    // defined. If not return kFALSE and deletes gIlc and set it to zero.

    if(!gIlc){
	return kFALSE;
    } // end if !gIlc
    // gIlc defined check to see if TARGET is properly defined.
    IlcTARGET *TARGET = (IlcTARGET*)gIlc->GetDetector("TARGET"); 
    if(!TARGET){ // TARGET not defined, delete and reload gIlc
	delete gIlc;
	gIlc = 0;
	return kFALSE;
    } // end if !TARGET
    // TARGET defined
    if(!(TARGET->GetTARGETgeom())){
	delete gIlc;
	gIlc = 0;
	return kFALSE;
    } // end if !(TARGET->GetTARGETgeom())
    // TARGET and TARGET geometry properly defined defined.
    return kTRUE;
}
//______________________________________________________________________
IlcRunLoader* AccessFile(TString FileName){
  // Function used to open the input file and fetch the IlcRun object

  IlcRunLoader* rl = IlcRunLoader::Open(FileName.Data());
  if (rl == 0x0){
    cerr<<"AccessFile : Can not open session RL=NULL"<< endl;
    return rl;
  }
 
  Int_t retval = rl->LoadgIlc();
  if (retval){
    cerr<<"AccessFile : LoadgIlc returned error"<<endl;
    return rl;
  }
  return rl;
}

