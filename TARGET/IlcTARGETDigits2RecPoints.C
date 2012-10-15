#if !defined(__CINT__) || defined(__MAKECINT__)

#include "Riostream.h"
#include "TClassTable.h"
#include "TDatime.h"
#include "TStopwatch.h"
#include "IlcRun.h"
#include "IlcRunDigitizer.h"
#include "IlcTARGETDigitizer.h"
#include "IlcTARGET.h"
#include "IlcTARGETDetType.h"
#include "IlcTARGETresponseSDD.h"
#include "IlcTARGETreconstruction.h"

#endif
#define DEBUG

Int_t IlcTARGETDigits2RecPoints(TString filename="gilc.root",TString fileRP=""){
    // Standard TARGET Digits to RecPoints.

    // Dynamically link some shared libs
    if (gClassTable->GetID("IlcRun") < 0) {
      gROOT->ProcessLine(".x $(ILC_ROOT)/macros/loadlibs.C");
    }else if (gIlc){
      delete gIlc->GetRunLoader();
      delete gIlc;
      gIlc=0;
     } // end if

    TStopwatch timer;
#ifdef DEBUG
    cout << "Creating reconstructed points from digits for the TARGET..." << endl;
#endif
    IlcTARGETreconstruction *itsr = new IlcTARGETreconstruction(filename);

    timer.Start();
    if(!(fileRP.IsNull()))itsr->SetOutputFile(fileRP);
    itsr->Init();
    itsr->Exec(); 
    timer.Stop(); 
    timer.Print();
    delete itsr;
    return 0;
}
