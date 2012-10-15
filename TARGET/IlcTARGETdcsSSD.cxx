#include <TRandom.h>
#include <TArrayS.h>

#include "IlcTARGETdcsSSD.h"
#include "IlcTARGETCalibrationSSD.h"
#include "IlcTARGETresponseSSD.h"
#include "IlcTARGETsegmentationSSD.h"
///////////////////////////////////////////////////////////////////////////
//                                                                       //
//  Class IlcTARGETdcsSSD                                                   //
//  describes Detector Control System parameters for one SSD module.     //
//                                                                       //
//  This class stores parametrers such as gain, threshold                //
//  capacitive coupling.                                                 //
//                                                                       //
//  Class takes care of invilcd strip menagement during                  //
//  simulation and runtime                                               //
//                                                                       //
//                                                                       //
//  created at: Warsaw University of Technology                          //
//  ver. 1.0    WARSAW, 23.12.1999                                       //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

ClassImp(IlcTARGETdcsSSD)

// Constructor and Destructor
//______________________________________________________________________
IlcTARGETdcsSSD::IlcTARGETdcsSSD(){
    // Default Constructor

    fInvilcdP = 0;
    fInvilcdN = 0;
}
//______________________________________________________________________
IlcTARGETdcsSSD::IlcTARGETdcsSSD(IlcTARGETsegmentation *seg, IlcTARGETCalibration *resp){
    // Standard constructor

    fNstrips =(Float_t) (((IlcTARGETsegmentationSSD*)seg)->Npx(1));
    
    fInvilcdP = new TArrayS();
    fInvilcdN = new TArrayS();

    Int_t npar=((IlcTARGETCalibrationSSD*)resp)->NDetParam();
    if (npar < 6) {
	Warning("IlcTARGETdcsSSD","I need 6 parameters ");
	npar=6;
    } // end if

    Double_t *detpar= new Double_t[npar];
    resp->GetDetParam(detpar);

    fNInvilcd = detpar[0];
    fISigma   = detpar[1];

    fCouplingPR = detpar[2];
    fCouplingPL = detpar[3];
    fCouplingNR = detpar[4];
    fCouplingNL = detpar[5];

    char opt[30],dummy[20];
    ((IlcTARGETCalibrationSSD*)resp)->GetParamOptions(opt,dummy);
    if (strstr(opt,"SetInvilcd")) SetInvilcdMC(fNInvilcd,fISigma);

    delete [] detpar;
}
//______________________________________________________________________
IlcTARGETdcsSSD::~IlcTARGETdcsSSD() {
    // destructor

    delete fInvilcdP;
    delete fInvilcdN;
}
//______________________________________________________________________
IlcTARGETdcsSSD::IlcTARGETdcsSSD(const IlcTARGETdcsSSD &source) : TObject(source){
    //     Copy Constructor 

    if(&source == this) return;

    this->fCouplingPR = source.fCouplingPR;
    this->fCouplingPL = source.fCouplingPL;
    this->fCouplingNR = source.fCouplingNR;
    this->fCouplingNL = source.fCouplingNL;
    this->fNstrips = source.fNstrips;
    this->fNInvilcd = source.fNInvilcd;
    this->fISigma = source.fISigma;
    this->fInvilcdP = source.fInvilcdP;
    this->fInvilcdN = source.fInvilcdN;

    return;
}
//_________________________________________________________________________
IlcTARGETdcsSSD& IlcTARGETdcsSSD::operator=(const IlcTARGETdcsSSD &source) {
    //    Assignment operator

    if(&source == this) return *this;

    this->fCouplingPR = source.fCouplingPR;
    this->fCouplingPL = source.fCouplingPL;
    this->fCouplingNR = source.fCouplingNR;
    this->fCouplingNL = source.fCouplingNL;
    this->fNstrips = source.fNstrips;
    this->fNInvilcd = source.fNInvilcd;
    this->fISigma = source.fISigma;
    this->fInvilcdP = source.fInvilcdP;
    this->fInvilcdN = source.fInvilcdN;

    return *this;
}
//_____________________________________________________________________
//
//  Methods for creating invilcd strips
//_____________________________________________________________________
void IlcTARGETdcsSSD::SetInvilcdMC(Float_t mean, Float_t sigma) {
    // set invilcd MC

    SetInvilcdParam(mean, sigma);
    SetInvilcdMC();
}
//______________________________________________________________________
void IlcTARGETdcsSSD::SetInvilcdMC() {
    // set invilcd MC
    Int_t pside;
    Int_t nside;
    Int_t i;
    Int_t strip;

    pside = (Int_t)gRandom->Gaus(fNInvilcd, fISigma);
    nside = (Int_t)gRandom->Gaus(fNInvilcd, fISigma);
    
    fInvilcdP->Set(pside);
    fInvilcdN->Set(nside);
     
    for(i=0 ;i<pside; i++) {
       strip = (Int_t)(gRandom->Rndm() * fNstrips);
       fInvilcdP->AddAt(strip, i); 
    } // end for i
    
    for(i=0 ;i<nside; i++) {
       strip = (Int_t)(gRandom->Rndm() * fNstrips);
       fInvilcdN->AddAt(strip, i); 
    } // end for i
}
//______________________________________________________________________
void IlcTARGETdcsSSD::SetInvilcdParam(Float_t mean, Float_t sigma) {
    // set invilcd param

    fNInvilcd = mean;
    fISigma = sigma;

    fNInvilcd = (fNInvilcd<0)? 0 : fNInvilcd;
    fNInvilcd = (fNInvilcd>fNstrips)? fNstrips: fNInvilcd;
    
    fISigma = (fISigma < 0)? 0 : fISigma;
    fISigma = (fISigma > fNstrips/10) ? fNstrips/10 : fISigma;
}
//______________________________________________________________________
void IlcTARGETdcsSSD::GetInvilcdParam(Float_t &mean, Float_t &sigma) const {
    // get invilcd param

    mean = fNInvilcd;
    sigma = fISigma;
}
//_____________________________________________________________________
//
//  Methods for accessing to invilcd strips
//_____________________________________________________________________
Bool_t IlcTARGETdcsSSD::IsValidP(Int_t strip) {
    // isvilcdP
    Int_t nElem = fInvilcdP->GetSize();

    for(Int_t i = 0; i<nElem; i++) if(fInvilcdP->At(i) == strip) return kFALSE;
    return kTRUE;
}
//______________________________________________________________________
Bool_t IlcTARGETdcsSSD::IsValidN(Int_t strip) {
    // is vilcd N
    Int_t nElem = fInvilcdN->GetSize();

    for(Int_t i = 0; i<nElem; i++) if(fInvilcdN->At(i) == strip) return kFALSE;
    return kTRUE;
}
//______________________________________________________________________
TArrayS* IlcTARGETdcsSSD::GetInvilcdP() {
    // get invilcd P

    return fInvilcdP;
}
//______________________________________________________________________
TArrayS* IlcTARGETdcsSSD::GetInvilcdN() {
    // get invilcd N

    return fInvilcdN;
}
//______________________________________________________________________
Int_t IlcTARGETdcsSSD::GetNInvilcdP(){
    // get numeber of invilcd P

    return fInvilcdP->GetSize();
}
//______________________________________________________________________
Int_t IlcTARGETdcsSSD::GetNInvilcdN() {
    // get number of invilcd N

    return fInvilcdN->GetSize();
}
//_____________________________________________________________________





