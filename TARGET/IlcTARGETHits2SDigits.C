#if !defined(__CINT__) || defined(__MAKECINT__)

#include "Riostream.h"
#include "TDatime.h"
#include "TClassTable.h"
#include "IlcRun.h"
#include "IlcRunDigitizer.h"
#include "IlcTARGETDigitizer.h"
#include "IlcTARGET.h"
#include "IlcTARGETDetType.h"
#include "IlcTARGETLoader.h"
#include "IlcTARGETresponseSDD.h"
#include "TStopwatch.h"

#endif

Int_t IlcTARGETHits2SDigits(TString  filename = "gilc.root")
 {
    // Standard TARGET Hits to SDigits.

    // Dynamically link some shared libs
    if (gClassTable->GetID("IlcRun") < 0) {
      gROOT->ProcessLine(".x $(ILC_ROOT)/macros/loadlibs.C");
    }else if (gIlc){
      delete gIlc->GetRunLoader();
      delete gIlc;
      gIlc=0;
     } 

    // Connect the Root Gilc file containing Geometry, Kine and Hits

    IlcRunLoader* rl = IlcRunLoader::Open(filename);
    if (rl == 0x0)
     {
      cerr<<"IlcTARGETHits2SDigits.C : Can not open session RL=NULL"
           << endl;
       return 3;
     }
     
    Int_t retval = rl->LoadgIlc();
    if (retval)
     {
      cerr<<"IlcTARGETHits2SDigits.C : LoadgIlc returned error"
           << endl;
       return 3;
     }
    gIlc=rl->GetIlcRun();
    IlcTARGETLoader* gime = (IlcTARGETLoader*) rl->GetLoader("TARGETLoader");
    if (gime == 0x0)
     {
      cerr<<"IlcTARGETHits2SDigits.C : can not get TARGET loader"
           << endl;
     }
    IlcTARGET *TARGET = (IlcTARGET*)gIlc->GetDetector("TARGET");      
    if (!TARGET) {
	cerr<<"IlcTARGETHits2SDigits.C : IlcTARGET object not found on file"
	    << endl;
	return 3;
    }  // end if !TARGET
    if(!(TARGET->GetTARGETgeom())){
       cerr << " IlcTARGETgeom not found. Can't digitize without it." << endl;
       return 4;
    } // end if

    TStopwatch timer;
    Int_t evNumber1 = 0;
    Int_t evNumber2 = gIlc->GetEventsPerRun();
    timer.Start();
    retval = gime->LoadHits();
    if (retval)
     {
      cerr<<"IlcTARGETHits2SDigits.C : TARGETLoader::LoadHits returned error"
           << endl;
       return 3;
     }
    retval = gime->LoadSDigits("recreate");
    if (retval)
     {
      cerr<<"IlcTARGETHits2SDigits.C : TARGETLoader::LoadSDigits returned error"
           << endl;
       return 3;
     }
    for(Int_t event = evNumber1; event < evNumber2; event++){
       rl->GetEvent(event);
       if(!gime->TreeS()){ 
           cout << "Having to create the SDigits Tree." << endl;
           gime->MakeTree("S");
       } // end 

       TARGET->MakeBranch("S");
       TARGET->SetTreeAddress();
       cout<<"Making TARGET SDigits for event "<<event<<endl;
       TARGET->Hits2SDigits();
    } // end for event
    timer.Stop();
    timer.Print();

    delete rl; // sdigfile is closed by deleting gIlc if != hitfile.
    return 0;
}
