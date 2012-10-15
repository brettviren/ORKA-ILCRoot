//

#if !defined(__CINT__) || defined(__MAKECINT__)
  #include <Riostream.h>
  #include <TClassTable.h>
     //#include <fstream.h>

  #include "TH2.h"
  #include "TFile.h"
  #include "TKey.h"
  #include "TObjArray.h"
  #include "TStyle.h"
  #include "TCanvas.h"
  #include "TLine.h"
  #include "TText.h"
  #include "TParticle.h"
  #include "TStopwatch.h"
  #include "TPDGCode.h"

  #include "IlcRun.h"
  #include "IlcESD.h"
  #include "IlcCascadeVertex.h"

  #include "IlcMC.h"
  #include "IlcHeader.h"
  #include "IlcConfig.h"
  #include "IlcRunLoader.h"
  #include "IlcTARGETLoader.h"
  #include "IlcTARGET.h"
  #include "IlcTARGETdigit.h"
  #include "IlcTARGETgeom.h"
#endif

    const Int_t ndets=5;
    TH2I *BeamSpot[ndets];
//----------------------------------------------------------------------
void SetUPHistograms(){
    // Define the histograms to be filled
    Int_t i;
    Char_t name[10],title[50];

    for(i=0;i<ndets;i++){
        sprintf(name,"BeamSpot%d",i);
        sprintf(title,"Beam Spot for SPD %d detectors row-column values",i);
        BeamSpot[i] = new TH2I(name,title,256,0.,256.,160,0.,160.);
    } // end for i
}
//----------------------------------------------------------------------
void FillHistograms(Int_t module,Int_t row,Int_t colm,Int_t signal){
    // Fill the histograms

    if(module<0 || module>=ndets) return;
    BeamSpot[module]->Fill((Double_t) row,(Double_t) colm,(Double_t) signal);
}
//----------------------------------------------------------------------
void DisplayHistograms(){
    // Display the histograms

    TCanvas *c0 = new TCanvas("c0","SPD Digits",400,10,600,700);
    BeamSpot[2]->Draw();
}
//----------------------------------------------------------------------
void IlcTARGETDigitPlot(Int_t istart=0,Int_t iend=-1,
                     const char *filename="gilc.root"){
    // Macro to plot digits from many events
    // Inputs:
    //   Int_t istart   Starting event number
    //   Int_t iend     Last event number, =-1 all
    // Outputs:
    //   none.
    // Return:
    //   none.
    if (gClassTable->GetID("IlcRun") < 0) {
        gROOT->ProcessLine(".x $(ILC_ROOT)/macros/loadlibs.C");
    }
    if(gIlc){
        delete gIlc->GetRunLoader();
        delete gIlc;
        gIlc=0;
    } // end if gIlc

    Int_t nevents=0,nmodules=0,retval=0;
    Int_t i,j,module,dig,ndig,row,column,signal,det;
    IlcTARGET       *its    = 0;
    IlcTARGETgeom   *gm     = 0;
    IlcRunLoader *rl     = 0;
    IlcTARGETLoader *ld     = 0;
    TTree        *treeD  = 0;
    TBranch      *br     = 0;
    TClonesArray *digits = 0;
    IlcTARGETdigit  *d      = 0;
    TObjArray    *digDet = 0;
    Char_t *branchname[3] = {"TARGETDigitsSPD","TARGETDigitsSDD","TARGETDigitsSSD"};
    //
    rl = IlcRunLoader::Open(filename);
    if(!rl){
        cerr<<"Error, can not open file "<<filename<<endl;
        return;
    } // end if !rl
    retval = rl->LoadgIlc();
    if (retval){
        cerr<<"Error, LoadgIlc returned error"<<endl;
        return;
    }
    gIlc = rl->GetIlcRun();
    retval = rl->LoadHeader();
    if (retval){
        cerr<<"Error, LoadHeader returned error"<<endl;
        return;
    } // end if
    ld = (IlcTARGETLoader*) rl->GetLoader("TARGETLoader");
    if(!ld){
        cerr<<"Error, TARGET loader not found"<<endl;
        return;
    } // end if
    its = (IlcTARGET*) gIlc->GetModule("TARGET");
    if(!its){
        cerr <<"Error, No IlcDetector TARGET found on file"<<endl;
        return;
    } // end if
    gm  = its->GetTARGETgeom();
    if(!gm){
        cerr <<"Error, IlcTARGETgeom not initilized in module TARGET"<<endl;
        return;
    } // end if
    nevents = rl->GetNumberOfEvents();
    if(iend>nevents) iend = nevents;
    if(iend<0)       iend = nevents;
    if(iend<=istart){delete rl; return;}
    nmodules = gm->GetIndexMax();
    ld->GetDigitsDataLoader()->Load("read");
    treeD = ld->TreeD();
    if(!treeD){
        cerr <<"Error, could not get TreeD="<<treeD << endl;
        return;
    } // end if !treeD
    digDet = new TObjArray(3);
    its->SetDefaults();
    for(det=0;det<3;det++){
        digDet->AddAt(new TClonesArray(its->GetDetTypeSim()->
                                        GetDigitClassName(det),1000),det);
        br = treeD->GetBranch(branchname[det]);
        br->SetAddress(&((*digDet)[det]));
    } // end for det

    //
    SetUPHistograms();
    //
    for(i=istart;i<iend;i++){
        rl->GetEvent(i);
        treeD = ld->TreeD();
        for(det=0;det<3;det++){
            ((TClonesArray*)(digDet->At(det)))->Clear();
            br = treeD->GetBranch(branchname[det]);
            br->SetAddress(&((*digDet)[det]));
        } // end for det
        for(module=0;module<nmodules;module++){
            for(j=0;j<3;j++) ((TClonesArray*)(digDet->At(j)))->Clear();
            treeD->GetEvent(module);
            digits = (TClonesArray*) (digDet->At(0)); // SPD only.
            ndig = digits->GetEntriesFast();
            for(dig=0;dig<ndig;dig++){
                d = (IlcTARGETdigit*) digits->At(dig);
                row    = d->GetCoord1();
                column = d->GetCoord1();
                signal = d->GetSignal();
                     //cout <<"event="<<i<< " ndig="<< ndig<< " mod="
                     //<<module<<" row="<<row<<" col="<<column<< " sig="
                     //<<signal<<endl;
                FillHistograms(module,row,column,signal);
            } // end for mod
        } // end for module
    } // end for i
    DisplayHistograms();
    delete digits;
    return;
}
