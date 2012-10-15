void IlcTARGETPrintGeom(TString hfn="gilc.root",Int_t mod=-1){
    // Macro to print out the information kept in the IlcTARGETgeom class, for 
    // all or a specific module.
    // Inputs:
    //   TString hfn    input file name
    //   Int_t   mod    The specific module to print out transformations for.
    //                  if mod<0, does all modules.
    // Output:
    //    none.
    // Return:
    //    none.

    // Dynamically link some shared libs
    if (gClassTable->GetID("IlcRun") < 0) {
        gROOT->LoadMacro("loadlibs.C");
        loadlibs();
    }else {
	if(gIlc){
	    delete gIlc->GetRunLoader();
	    delete gIlc;
	    gIlc=0;
	} // end if gIlc
    } // end if ilcroot or not.

    IlcRunLoader* rl = IlcRunLoader::Open(hfn.Data());
    if (rl == 0x0){
	cerr<<"IlcTARGETPrintGeom.C : Can not open session RL=NULL"<< endl;
	return;
    } // end if no loader

    Int_t retval = rl->LoadgIlc();
    if (retval){
	cerr<<"IlcTARGETHits2SDigits.C : LoadgIlc returned error"<< endl;
	return 3;
    } // end if loader error
    if(!gIlc){
	cerr<<"IlcTARGETPrintGeom.C. IlcRun object not found\n";
	return;
    } // end if no gIlc error

    IlcTARGET *TARGET = (IlcTARGET*)gIlc->GetDetector("TARGET");
    if(!TARGET){
        cout << "Error: no TARGET found. Aborting"<<endl;
        return;
    } // end if !TARGET

    IlcTARGETgeom *gm = TARGET->GetTARGETgeom();
    Int_t mod1 = 0;
    Int_t mod2 = gm->GetIndexMax();
    if(mod>=0){
        mod1 = mod;
        mod2 = mod+1;
    } // end if mod>=0
    IlcTARGETgeomMatrix *gmm = gm->GetGeomMatrix(0);
    Int_t m,lay,lad,det;
    Double_t xyz[3],rcyl;
    cout<<endl<<endl<<"====================================\n";
    if(mod<-1){ gmm->PrintComment(&cout); cout << endl;}
    cout<<endl<<endl<<"====================================\n";
    for(m=mod1;m<mod2;m++){
	gm->GetModuleId(m,lay,lad,det);
	gmm = gm->GetGeomMatrix(m);
	gmm->GetTranslation(xyz);
	rcyl = TMath::Sqrt(xyz[0]*xyz[0]+xyz[1]*xyz[1]);
	if(mod<-1){
	    gmm->Print(&cout);
	    cout << endl;
	}else{
	    cout << "module="<<m<<" "<<lay<<" "<<lad<<" "<<det;
	    cout << " Rcyl="<<rcyl<<" cm"<<endl;
	} // end if
    } // end for m
}
