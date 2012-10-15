/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project.
 // Part of the code has been developed by Alice Off-line Project.
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* $Id: IlcTARGETsimulationSSD.cxx,v 1.3 2008/11/07 01:49:49 cgatto Exp $ */

#include <stdio.h>
#include <stdlib.h>
#include <Riostream.h>
#include <TObjArray.h>
#include <TRandom.h>

#include "IlcTARGETmodule.h"
#include "IlcTARGETMapA2.h"
#include "IlcTARGETpList.h"
#include "IlcTARGETCalibrationSSD.h"
#include "IlcTARGETsegmentationSSD.h"
//#include "IlcTARGETdcsSSD.h"
#include "IlcTARGET.h"
#include "IlcTARGEThit.h"
#include "IlcTARGETdigitSSD.h"
#include "IlcRun.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETsimulationSSD.h"
#include "IlcTARGETTableSSD.h"
//#include "IlcTARGETresponseSSD.h"

ClassImp(IlcTARGETsimulationSSD)
////////////////////////////////////////////////////////////////////////
//                                                                    //
// Author: Enrico Fragiacomo                                          //
//         enrico.fragiacomo@ts.infn.it                               //
// Last revised: march 2006                                           //
//                                                                    //
// IlcTARGETsimulationSSD is the simulation of SSD.                     //
////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------
IlcTARGETsimulationSSD::IlcTARGETsimulationSSD():IlcTARGETsimulation(),
        //fDCS(0),
        fMapA2(0),
        fIonE(0.0),
        fDifConst(),
        fDriftVel() {
    //default Constructor
    //Inputs:
    // none.
    // Outputs:
    // none.
    // Return:
    //  A default construction IlcTARGETsimulationSSD class
}
//----------------------------------------------------------------------
IlcTARGETsimulationSSD::IlcTARGETsimulationSSD(IlcTARGETDetTypeSim* dettyp):
        IlcTARGETsimulation(dettyp),
//fDCS(0),
        fMapA2(0),
        fIonE(0.0),
        fDifConst(),
        fDriftVel() {
    // Constructor
    // Input:
    //   IlcTARGETDetTypeSim    Pointer to the SSD dettype to be used
    // Outputs:
    //   none.
    // Return
    //   A standard constructed IlcTARGETsimulationSSD class

    Init();
}
//----------------------------------------------------------------------
void IlcTARGETsimulationSSD::Init() {
    // Inilizer, Inilizes all of the variable as needed in a standard place.
    // Input:
    //   IlcTARGETsegmentationSSD *seg  Pointer to the SSD segmentation to be used
    //   IlcTARGETCalibrationSSD   *resp Pointer to the SSD responce class to be used
    // Outputs:
    //   none.
    // Return
    //   none.

    IlcTARGETsegmentationSSD* seg = (IlcTARGETsegmentationSSD*)GetSegmentationModel2(1);
    //Int_t nlayersStrip=4; //modaniele

    SetDriftVelocity(); // use default values in .h file
    SetIonizeE();       // use default values in .h file
    SetDiffConst(150.,300.);     // use default values in .h file

    Int_t nlayersStrip=fParam->GetNlayersStrip();
    Int_t nendcapsStrip=fParam->GetNendcapsStrip();
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();
    for (Int_t lay=0;lay<nendcapsStrip+nlayersStrip;lay++) {
        // cout <<"Sto settando per lay: "<<lay<<endl;
//dan-commento     cout <<"layer: "<<lay<<" Numero delle strip: "<<GetNStrips(lay)<<endl;
        SetMap2(lay+nlayers+nendcaps, new IlcTARGETpList(2,GetNStrips(lay)));
        SetMapA2(lay,new IlcTARGETMapA2(lay,seg));
    }

    //}

}
//______________________________________________________________________
IlcTARGETsimulationSSD& IlcTARGETsimulationSSD::operator=(
    const IlcTARGETsimulationSSD &s) {
    // Operator =

    if (this==&s) return *this;

    //  this->fDCS         = new IlcTARGETdcsSSD(*(s.fDCS));
    this->fMapA2       = s.fMapA2;
    this->fIonE        = s.fIonE;
    this->fDifConst[0] = s.fDifConst[0];
    this->fDifConst[1] = s.fDifConst[1];
    this->fDriftVel[0] = s.fDriftVel[0];
    this->fDriftVel[1] = s.fDriftVel[1];
    return *this;
}
//______________________________________________________________________
IlcTARGETsimulation& IlcTARGETsimulationSSD::operator=(
    const IlcTARGETsimulation &s) {
    // Operator =

    if (this==&s) return *this;
    Error("IlcTARGETsimulationSSD","Not allowed to make a = with "
          "IlcTARGETsimulationSSD Using default creater instead");

    return *this;
}
//______________________________________________________________________
IlcTARGETsimulationSSD::IlcTARGETsimulationSSD(const IlcTARGETsimulationSSD &source):
        IlcTARGETsimulation(source) {
    // copy constructor

    *this = source;
}
//______________________________________________________________________
IlcTARGETsimulationSSD::~IlcTARGETsimulationSSD() {
    // destructor
    delete fMapA2;
    //delete fDCS;
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::InitSimulationModule(Int_t module,Int_t event) {
    // Creates maps to build the list of tracks for each sumable digit
    // Inputs:
    //   Int_t module    // Module number to be simulated
    //   Int_t event     // Event number to be simulated
    // Outputs:
    //   none.
    // Return
    //    none.
    SetModuleNumber(module);
    SetEventNumber(event);
    int lay,lad,det;
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();
    fDetType->GetTARGETgeom2()->GetModuleId(module,lay,lad,det);
    lay--;
    ClearMapA2(lay-(nlayers+nendcaps));
    ClearMap(lay);
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::FinishSDigitiseModule() {
    // Does the Sdigits to Digits work
    // Inputs:
    //   none.
    // Outputs:
    //   none.
    // Return:
    //   none.
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();
    Int_t lay,lad,det;
    (fDetType->GetTARGETgeom2())->GetModuleId(fModule,lay,lad,det);
    lay--;
    FillMapFrompList(lay,GetMap2(lay));  // need to check if needed here or not????
    SDigitToDigit(lay,fModule,GetMap2(lay));
    ClearMap(lay);
    ClearMapA2(lay-(nlayers+nendcaps));
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::DigitiseModule(IlcTARGETmodule *mod,Int_t,Int_t) {
    // Digitizes hits for one SSD module
    SetModuleNumber(mod->GetIndex());

    HitsToAnalogDigits(mod,fpList);
    SDigitToDigit(mod->GetLayer()-1,GetModuleNumber(),fpList);

    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();
    ClearMap(mod->GetLayer()-1);
    ClearMapA2(mod->GetLayer()-1-(nlayers+nendcaps));
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::SDigitiseModule(IlcTARGETmodule *mod,Int_t,Int_t event) {
    // Produces Summable/Analog digits and writes them to the SDigit tree.
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();
  
    //IlcDebug(1,Form("(mod=%p, ,event=%d)",mod,event));
    if(!(mod->GetNhits())){
      IlcDebug(1,Form("In event %d module %d there are %d hits returning.",
		      event, mod->GetIndex(),mod->GetNhits()));
      return;// if module has no hits don't create Sdigits
    } // end if

    HitsToAnalogDigits(mod,GetMap2(mod->GetLayer()-1));

    WriteSDigits(GetMap2(mod->GetLayer()-1));
    ClearMap(mod->GetLayer()-1);
    ClearMapA2(mod->GetLayer()-1-(nlayers+nendcaps));
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::SDigitToDigit(Int_t lay,Int_t module,IlcTARGETpList *pList) {
    // Takes the pList and finishes the digitization.

    ApplyNoise(lay,pList,module);
    //ApplyCoupling(lay,pList,module);
    //ApplyDeadChannels(module);
    //cout <<"lay: "<<lay<<" module: "<<module<<endl;
    ChargeToSignal(lay,module,pList);
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::HitsToAnalogDigits(IlcTARGETmodule *mod,
        IlcTARGETpList *pList) {
    // Loops over all hits to produce Analog/floating point digits. This
    // is also the first task in producing standard digits.
    Int_t lasttrack     = -2;
    Int_t idtrack       = -2;
    Double_t x0=0.0, y0=0.0, z0=0.0;
    Double_t x1=0.0, y1=0.0, z1=0.0;
    Double_t de=0.0;
    Int_t module = mod->GetIndex();
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();
    //cout <<"numero del modulo in HitsToAnalogDigits: "<<module<<endl;

    TObjArray *hits = mod->GetHits();
    Int_t nhits     = hits->GetEntriesFast();
    //printf("module %d there are %d hits returning.\n",module,nhits);
    //IlcDebug(1,Form("module %d there are %d hits returning.",module,nhits));
    //cout <<"modulo: "<<module<<" hits: "<<nhits<<endl;
    if (nhits<=0) return;
    //cout <<"arriva al table, layer: "<<mod->GetLayer()-1<<endl;
    //cout <<"numero strips: "<<GetNStrips((mod->GetLayer()-1)-nlayers-nendcaps)<<endl;

    IlcTARGETTableSSD * tav = new IlcTARGETTableSSD(GetNStrips((mod->GetLayer()-1)-(nlayers+nendcaps)));
    //cout <<"supera il table"<<endl;
    module = mod->GetIndex();
    //if ( mod->GetLayer() == 6 ) seg->SetLayer(6);
    //if ( mod->GetLayer() == 5 ) seg->SetLayer(5);
    //seg->SetLayer((mod->GetLayer()-1)-(nlayers+nendcaps));
    for (Int_t i=0; i<nhits; i++) {
        // LineSegmentL returns 0 if the hit is entering
        // If hits is exiting returns positions of entering and exiting hits
        // Returns also energy loss
        if ((gRandom->Rndm())>fParam->GetSingleHitStripEfficiency()) {Info("IlcTARGETsimulationSSD","module:%d hit:%d rejected!!!",module,i); continue;}
        if (GetDebug(4)) {
            cout << i << " ";
            cout << mod->GetHit(i)->GetXL() << " "<<mod->GetHit(i)->GetYL();
            cout << " " << mod->GetHit(i)->GetZL();
            cout << endl;
        } // end if
        if (mod->LineSegmentL(i, x0, x1, y0, y1, z0, z1, de, idtrack)) {
            //cout <<"Sto per chiamare HitToDigit"<<endl;
            HitToDigit(mod->GetLayer()-1,module, x0, y0, z0, x1, y1, z1, de,tav);
            //cout <<"Ho finito con HitToDigit"<<endl;
            //cout <<"lasttrack: "<<lasttrack<<" idtrack: "<<idtrack<<endl;
            if (lasttrack != idtrack || i==(nhits-1)) {
                //cout <<"numero del layer per GetList: "<<mod->GetLayer()-1<<endl;
                GetList(mod->GetLayer()-1,idtrack,i,module,pList,tav);
            } // end if
            lasttrack=idtrack;
        } // end if
    }  // end loop over hits
    delete tav;
    tav=0;

    return;
}
//----------------------------------------------------------------------
void IlcTARGETsimulationSSD::HitToDigit(Int_t lay,Int_t module, Double_t x0, Double_t y0,
                                     Double_t z0, Double_t x1, Double_t y1,
                                     Double_t z1, Double_t de,
                                     IlcTARGETTableSSD *tav) {

    // hit to digit conversion


    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();


    IlcTARGETsegmentationSSD* seg = (IlcTARGETsegmentationSSD*)GetSegmentationModel2(1);
    // Turns hits in SSD module into one or more digits.
    Float_t tang[2] = {0.0,0.0};
    //cout <<"Sto calcolando il layer: "<<lay<<endl;
    seg->Angles(lay-(nlayers+nendcaps),tang[0], tang[1]);//stereo<<->tan(stereo)~=stereo
    //cout <<"tang[0]: "<<tang[0]<<" tang[1]: "<<tang[1]<<endl;
    Double_t x, y, z;
    Double_t dex=0.0, dey=0.0, dez=0.0;
    Double_t pairs; // pair generation energy per step.
    Double_t sigma[2] = {0.,0.};// standard deviation of the diffusion gaussian
    Double_t tdrift[2] = {0.,0.}; // time of drift
    Double_t w;
    Double_t inf[2], sup[2], par0[2];
    Int_t p=0;
    // Steps in the module are determined "manually" (i.e. No Geant)
    // NumOfSteps divide path between entering and exiting hits in steps
    Int_t numOfSteps = NumOfSteps(x1, y1, z1, dex, dey, dez);
    //cout <<"numero di steps"<<numOfSteps<<endl;
    // Enery loss is equally distributed among steps
    if (IlcLog::GetGlobalDebugLevel()>0) cout <<"Total Energy: "<<de<<" Total pairs: "<<de/GetIonizeE()<<endl;
    de    = de/numOfSteps;
    pairs = de/GetIonizeE(); // e-h pairs generated
    if (IlcLog::GetGlobalDebugLevel()>0) cout <<"numOfSteps: "<<numOfSteps<<endl;
    if (IlcLog::GetGlobalDebugLevel()>0) cout <<"pairs: "<<pairs<<endl;
    for (Int_t j=0; j<numOfSteps; j++) {    // stepping
        x = x0 + (j+0.5)*dex;
        y = y0 + (j+0.5)*dey;
        //cout <<"seg->Dy(lay): "<<seg->Dy(lay)<<endl;
        z = z0 + (j+0.5)*dez;
        //dan-commento cout <<"nella digitizazione: seg->Dy(lay): "<<seg->Dy(lay)<<endl;
        if ( y > (seg->Dy(lay)/2+10)*1.0E-4 ) {
            // check if particle is within the detector
            Warning("HitToDigit",
                    "hit out of detector y0=%e,y=%e,dey=%e,j =%d module=%d",
                    y0,y,dey,j,module);
            return;
        } // end if
        /*} else
        {
        cout <<"nella digitizazione: seg->Dy(lay): "<<seg->Dy(lay)<<endl;
        if ( z > (seg->Dy(lay)/2+10)*1.0E-4 ) {
          // check if particle is within the detector
          Warning("HitToDigit",
           "hit out of detector z0=%e,z=%e,dez=%e,j =%d module=%d",
           z0,z,dez,j,module);
          return;
        } // end if
        }*/
        if (GetDebug(4)) cout <<"HitToDigit "<<x<<" "<<y<<" "<<z<< " "
            <<dex<<" "<<dey<<" "<<dez<<endl;
        // calculate drift time
        // y is the minimum path
        //cout <<"position y:"<<y<<endl;
        //if ((module<192) || (module>7503)) {
        tdrift[0] = (y+(seg->Dy(lay)*1.0E-4)/2)/GetDriftVelocity(0);
        tdrift[1] = ((seg->Dy(lay)*1.0E-4)/2-y)/GetDriftVelocity(1);
        /*}
        else {
        tdrift[0] = (z+(seg->Dy(lay)*1.0E-4)/2)/GetDriftVelocity(0);
        tdrift[1] = ((seg->Dy(lay)*1.0E-4)/2-z)/GetDriftVelocity(1);
        }*/

        for (Int_t k=0; k<fParam->GetSensor(lay-(nlayers+nendcaps)); k++) {  // both sides    remember: 0=Pside 1=Nside
            //cout <<"Sta eseguendo per k="<<k<<endl;
            tang[k]=TMath::Tan(tang[k]);

            // w is the coord. perpendicular to the strips
            Float_t xp=x*1.e+4,zp=z*1.e+4; // microns
            //if ((module>191) || (module<7504)) zp=y*1.e+4;
            //cout <<"sto per chiamare GetPadTxz, layers: "<<lay-nlayers-nendcaps<<endl;
            seg->GetPadTxz(lay,xp,zp);
            //cout <<"ho finito con GetPadTxz:"<<endl;
            if (k==0) {
                w = xp;
            }// P side strip number
            else {
                w = zp;
            }// N side strip number
            //dan-commento cout <<"Strip: "<<w<<endl;
            //cout <<"strip number: "<<w<<endl;
            //cout <<"GetNStrips(lay): "<<GetNStrips(lay-nlayers-nendcaps)<<endl;
            if ((w<(-0.5)) || (w>(GetNStrips(lay-(nlayers+nendcaps))-0.5))) {
                // this check rejects hits in regions not covered by strips
                // 0.5 takes into account boundaries

                //cout << "x,z="<<x<<","<<z<<" w="<<w
                //	     <<" Nstrips="<<GetNStrips(lay-(nlayers+nendcaps))<<endl;
            //    cout <<"region not covered by strip"<<endl;
                return; // There are dead region on the SSD sensitive volume.
            } // end if

            // sigma is the standard deviation of the diffusion gaussian
            //cout <<"tdrift[k]: "<<tdrift[k]<<endl;
            //cout <<"GetDiffConst(k): "<<GetDiffConst(k)<<endl;


            if (tdrift[k]<0) {//dan-commento cout <<"ritorna sul drift"<<endl;
                return;
            }
            sigma[k] = TMath::Sqrt(2.*GetDiffConst(k)*tdrift[k]);
            //sigma[k]= 250.*1.E-4;
            sigma[k] /= (GetStripPitch(lay-(nlayers+nendcaps))*1.0E-4);  //units of Pitch

            if (sigma[k]==0.0) {
                Error("HitToDigit"," sigma[%d]=0",k);
                exit(0);
            } // end if
            //cout <<"esegue per "<<p++<<endl;
            par0[k] = pairs;
            //cout <<"par0[k]: "<<par0[k]<<endl;
            // we integrate the diffusion gaussian from -3sigma to 3sigma
            inf[k] = w - 3*sigma[k]; // 3 sigma from the gaussian average
            sup[k] = w + 3*sigma[k]; // 3 sigma from the gaussian average
            // IntegrateGaussian does the actual
            // integration of diffusion gaussian
            //cout <<"sta per integrare"<<endl;
            if (IlcLog::GetGlobalDebugLevel()>0) cout <<"step: "<<j<<endl;
            IntegrateGaussian(lay,k, par0[k], w, sigma[k], inf[k], sup[k],tav);
        }  // end for loop over side (0=Pside, 1=Nside)
    } // end stepping
}

//______________________________________________________________________
void IlcTARGETsimulationSSD::ApplyNoise(Int_t lay,IlcTARGETpList *pList,Int_t module) {
    // Apply Noise.
    Int_t ix;
    Double_t signal,noise;
    //IlcTARGETCalibrationSSD* res =(IlcTARGETCalibrationSSD*)GetCalibrationModel(module);
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();
    Int_t k2;
    Int_t ix2;
    // Pside
    //for(k=0;k<fParam->GetSensor(lay-(nlayers+nendcaps));k++){         // both sides (0=Pside, 1=Nside)

        for (Int_t pix=0;pix<=pList->GetNRealSDigit();pix++) {
            pList->GetMapIndex(pList->GetRealSDigitAt(pix),k2,ix2);
            //if (k2!=0)/* || (ix!=ix2))*/ continue;
            // noise is gaussian
            //if (GetMapA2(lay-(nlayers+nendcaps))->GetSignal(0,ix)==0.) continue;
            noise  = (Double_t) gRandom->Gaus(0,3.3333);
            //    noise = 4.;
            // need to calibrate noise
            // NOTE. noise from the calibration database comes uncalibrated,
            // it needs to be calibrated in order to be added
            // to the signal. It will be decalibrated later on together with the noise
            //noise *= (Double_t) res->GetGainP(ix);

            // noise comes in ADC channels from the calibration database
            // It needs to be converted back to electronVolts
            noise /= (200./30000.); //res->GetDEvToADC(1.);

            // Finally, noise is added to the signal
            if (IlcLog::GetGlobalDebugLevel()>0) cout <<"noise in Electron volts: "<<noise<<endl;
            if (IlcLog::GetGlobalDebugLevel()>0) cout <<"signal in Electron Volts: "<<GetMapA2(lay-(nlayers+nendcaps))->GetSignal(k2,ix2)<<endl;
            signal = noise + GetMapA2(lay-(nlayers+nendcaps))->GetSignal(k2,ix2);//get signal from map
            GetMapA2(lay-(nlayers+nendcaps))->SetHit(k2,ix2,signal); // give back signal to map
            if(signal>0.0) pList->AddNoise(k2,ix2,module,noise);
        } // loop over strip
    //}
    // Nside
/*    //for (ix=0;ix<GetNStrips(lay-(nlayers+nendcaps));ix++) {   // loop over strips

    for (ix=0;ix<nstrips;ix++) {   // loop over strips
>>>>>>> 1.23
        for (Int_t pix=0;pix<=pList->GetNRealSDigit();pix++) {
            pList->GetMapIndex(pList->GetRealSDigitAt(pix),k2,ix2);
            if (k2!=1) || (ix!=ix2)) continue;
            noise  = (Double_t) gRandom->Gaus(0,3.3333);// give noise to signal
            //noise *= (Double_t) res->GetGainN(ix);
            //noise=4.;
            noise /= (200./30000.);// res->GetDEvToADC(1.);
            if (IlcLog::GetGlobalDebugLevel()>0) cout <<"noise in Electron volts: "<<noise<<endl;
            if (IlcLog::GetGlobalDebugLevel()>0) cout <<"signal in Electron Volts: "<<GetMapA2(lay-(nlayers+nendcaps))->GetSignal(1,ix2)<<endl;

            signal = noise + GetMapA2(lay-(nlayers+nendcaps))->GetSignal(1,ix2);//get signal from map
            GetMapA2(lay-(nlayers+nendcaps))->SetHit(1,ix2,signal); // give back signal to map
            if(signal>0.0) pList->AddNoise(1,ix2,module,noise);
        } // loop over strip
    //}*/
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::ApplyCoupling(Int_t lay,IlcTARGETpList *pList,Int_t module) {
    // Apply the effect of electronic coupling between channels
    Int_t ix;
    Double_t signal=0;
    //IlcTARGETCalibrationSSD* res =(IlcTARGETCalibrationSSD*)GetCalibrationModel(module);
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();

    Double_t *contrLeft  = new Double_t[GetNStrips(lay-(nlayers+nendcaps))];
    Double_t *contrRight = new Double_t[GetNStrips(lay-(nlayers+nendcaps))];

    // P side coupling
    for (ix=0;ix<GetNStrips(lay-(nlayers+nendcaps));ix++) {

        if (ix>0)contrLeft[ix] = GetMapA2(lay-(nlayers+nendcaps))->GetSignal(0,ix-1)*0.01;
        else contrLeft[ix] = 0.0;
        if (ix<(GetNStrips(lay-(nlayers+nendcaps))-1)) contrRight[ix] = GetMapA2(lay-(nlayers+nendcaps))->GetSignal(0,ix+1)*0.01;
        else contrRight[ix] = 0.0;
    } // loop over strips

    for (ix=0;ix<GetNStrips(lay-(nlayers+nendcaps));ix++) {
        signal = contrLeft[ix] + contrRight[ix] - 0.01 * GetMapA2(lay-(nlayers+nendcaps))->GetSignal(0,ix)
                 - 0.01 * GetMapA2(lay-(nlayers+nendcaps))->GetSignal(0,ix);
        GetMapA2(lay-(nlayers+nendcaps))->AddSignal(0,ix,signal);
        if (signal>0.0) pList->AddNoise(0,ix,module,signal);
    } // loop over strips

    // N side coupling
    for (ix=0;ix<GetNStrips(lay-(nlayers+nendcaps));ix++) {
        if (ix>0) contrLeft[ix] = GetMapA2(lay-(nlayers+nendcaps))->GetSignal(1,ix-1)*0.01;
        else contrLeft[ix] = 0.0;
        if (ix<(GetNStrips(lay-(nlayers+nendcaps))-1)) contrRight[ix] = GetMapA2(lay-(nlayers+nendcaps))->GetSignal(1,ix+1)*0.01;
        else contrRight[ix] = 0.0;
    } // loop over strips

    for (ix=0;ix<GetNStrips(lay-(nlayers+nendcaps));ix++) {
        signal = contrLeft[ix] + contrRight[ix] - 0.01 * GetMapA2(lay-(nlayers+nendcaps))->GetSignal(0,ix)
                 - 0.01 * GetMapA2(lay-(nlayers+nendcaps))->GetSignal(0,ix);
        GetMapA2(lay-(nlayers+nendcaps))->AddSignal(1,ix,signal);
        if (signal>0.0) pList->AddNoise(1,ix,module,signal);
    } // loop over strips


    delete [] contrLeft;
    delete [] contrRight;
}

//______________________________________________________________________
void IlcTARGETsimulationSSD::ApplyDeadChannels(Int_t module) {
    // Kill dead channels setting gain to zero

    Int_t deadentries;

    IlcTARGETCalibrationSSD* res = (IlcTARGETCalibrationSSD*)GetCalibrationModel(module);

    deadentries = res->GetDeadPChannelsList().GetSize();
    cout<<module<<" "<<deadentries<<endl;
    for (Int_t i=0; i<deadentries; i++) {
        res->AddGainP(res->GetDeadPChannelsList().At(i),0.0);
    }

    deadentries = res->GetDeadNChannelsList().GetSize();
    for (Int_t i=0; i<deadentries; i++) {
        res->AddGainN(res->GetDeadNChannelsList().At(i),0.0);
    }

}

//______________________________________________________________________
Float_t IlcTARGETsimulationSSD::F(Float_t av, Float_t x, Float_t s) {
    // Computes the integral of a gaussian using Error Function
    Float_t sqrt2 = TMath::Sqrt(2.0);
    Float_t sigm2 = sqrt2*s;
    Float_t integral;

    integral = 0.5 * TMath::Erf( (x - av) / sigm2);
    return integral;
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::IntegrateGaussian(Int_t lay,Int_t k,Double_t par, Double_t w,
        Double_t sigma,
        Double_t inf, Double_t sup,
        IlcTARGETTableSSD *tav) {
    // integrate the diffusion gaussian
    // remind: inf and sup are w-3sigma and w+3sigma
    //         we could define them here instead of passing them
    //         this way we are free to introduce asimmetry
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();

    Double_t a=0.0, b=0.0;
    Double_t dXCharge1 = 0.0, dXCharge2 = 0.0;
    // dXCharge1 and 2 are the charge to two neighbouring strips
    // Watch that we only involve at least two strips
    // Numbers greater than 2 of strips in a cluster depend on
    //  geometry of the track and delta rays, not charge diffusion!
    //cout <<"w: "<<w<<endl;
    Double_t strip = TMath::Floor(w);         // closest strip on the left

    if ( TMath::Abs((strip - w)) < 0.5) {
        // gaussian mean is closer to strip on the left
        a = inf;                         // integration starting point
        if ((strip+0.5)<=sup) {
            // this means that the tail of the gaussian goes beyond
            // the middle point between strips ---> part of the signal
            // is given to the strip on the right
            b = strip + 0.5;               // integration stopping point
            dXCharge1 = F( w, b, sigma) - F(w, a, sigma);
            dXCharge2 = F( w, sup, sigma) - F(w ,b, sigma);
        } else {
            // this means that all the charge is given to the strip on the left
            b = sup;
            dXCharge1 = 0.9973;   // gaussian integral at 3 sigmas
            dXCharge2 = 0.0;
        } // end if
        dXCharge1 = par * dXCharge1;// normilcze by mean of number of carriers
        dXCharge2 = par * dXCharge2;

        // for the time being, signal is the charge
        // in ChargeToSignal signal is converted in ADC channel

        GetMapA2(lay-(nlayers+nendcaps))->AddSignal(k,(Int_t)strip,dXCharge1);
        if (IlcLog::GetGlobalDebugLevel()>0) cout <<"dXCharge1: "<<dXCharge1<<endl;

        tav->Add(k,(Int_t)strip);
        if (((Int_t) strip) < (GetNStrips(lay-(nlayers+nendcaps))-1)) {
            // strip doesn't have to be the last (remind: last=GetNStrips()-1)
            // otherwise part of the charge is lost

            GetMapA2(lay-(nlayers+nendcaps))->AddSignal(k,((Int_t)strip+1),dXCharge2);
            if (IlcLog::GetGlobalDebugLevel()>0) cout <<"dXCharge2: "<<dXCharge2<<endl;
            tav->Add(k,((Int_t)(strip+1)));
        } // end if
    } else {
        // gaussian mean is closer to strip on the right
        strip++;     // move to strip on the rigth
        b = sup;     // now you know where to stop integrating
        //cout <<"inf for left: "<<inf<<endl;
        if ((strip-0.5)>=inf) {
            // tail of diffusion gaussian on the left goes left of
            // middle point between strips
            a = strip - 0.5;        // integration starting point
            dXCharge1 = F(w, b, sigma) - F(w, a, sigma);
            dXCharge2 = F(w, a, sigma) - F(w, inf, sigma);
        } else {
            a = inf;
            dXCharge1 = 0.9973;   // gaussian integral at 3 sigmas
            dXCharge2 = 0.0;
        } // end if
        dXCharge1 = par * dXCharge1;    // normilcze by means of carriers
        dXCharge2 = par * dXCharge2;
        // for the time being, signal is the charge
        // in ChargeToSignal signal is converted in ADC channel
        GetMapA2(lay-(nlayers+nendcaps))->AddSignal(k,(Int_t)strip,dXCharge1);
        if (IlcLog::GetGlobalDebugLevel()>0) cout <<"dXCharge1: "<<dXCharge1<<endl;
        tav->Add(k,(Int_t)strip);
        if (((Int_t) strip) > 0) {
            // strip doesn't have to be the first
            // otherwise part of the charge is lost
            GetMapA2(lay-(nlayers+nendcaps))->AddSignal(k,((Int_t)strip-1),dXCharge2);
            if (IlcLog::GetGlobalDebugLevel()>0) cout <<"dXCharge2: "<<dXCharge2<<endl;
            tav->Add(k,((Int_t)(strip-1)));
        } // end if
    } // end if
}
//______________________________________________________________________
Int_t IlcTARGETsimulationSSD::NumOfSteps(Double_t x, Double_t y, Double_t z,
                                      Double_t &dex,Double_t &dey,
                                      Double_t &dez) {
    // number of steps
    // it also returns steps for each coord
    //IlcTARGETsegmentationSSD *seg = new IlcTARGETsegmentationSSD();

    Double_t step = 25.e-4;
    //step = (Double_t) seg->GetStepSize();  // step size (cm)
    Int_t numOfSteps = (Int_t) (TMath::Sqrt(x*x+y*y+z*z)/step);
    //dan-commento cout <<"lunghezza segmento: "<<TMath::Sqrt(x*x+y*y+z*z)<<endl;
    if (numOfSteps < 1) numOfSteps = 1;       // one step, at least
    //numOfSteps=1;

    // we could condition the stepping depending on the incident angle
    // of the track
    dex = x/numOfSteps;
    dey = y/numOfSteps;
    dez = z/numOfSteps;
    //dan-commento cout <<"num of steps: "<<numOfSteps<<endl;
    return numOfSteps;
}
//----------------------------------------------------------------------
void IlcTARGETsimulationSSD::GetList(Int_t lay,Int_t label,Int_t hit,Int_t mod,
                                  IlcTARGETpList *pList,IlcTARGETTableSSD *tav) {
    // loop over nonzero digits
    Int_t ix,i;
    Double_t signal=0.;
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();

    for (Int_t k=0; k<fParam->GetSensor(lay-(nlayers+nendcaps)); k++) {
        ix=tav->Use(k);
        //cout <<"valori di ix: "<<ix<<" k:"<<k<<endl;
        //cout <<"Sto in GetList!!!"<<endl;
        while (ix>-1) {
            signal = GetMapA2(lay-(nlayers+nendcaps))->GetSignal(k,ix);
            //cout <<"assegna il segnale"<<endl;
            //cout <<"sto prendendo il segnale signal: "<<signal<<endl;
            //cout<<"1) Sballa qui il TClonesArray!!!"<<endl;
            if (signal==0.0) {
                //cout <<"il segnale Ã¨ zero!"<<endl;
                ix=tav->Use(k);
                continue;
            } // end if signal==0.0
            // check the signal magnitude
            //cout <<"qui e l errore!!!"<<endl;
            //cout <<"layer: "<<lay<<endl;
            //cout <<"pList->GetNSignals(k,ix): "<<GetMap2(lay)->GetNSignals(k,ix)<<endl;
            for (i=0;i<GetMap2(lay)->GetNSignals(k,ix);i++) {
                //cout<<"2) Sballa qui il TClonesArray!!!"<<endl;
                signal -= pList->GetTSignal(k,ix,i);
                //cout <<"nel loop signal: "<<signal<<endl;
            } // end for i
            //  compare the new signal with already existing list


            if (signal>0) {
                //cout <<"aggiunge il segnale alla lista"<< " k: "<<k<< " ix: " <<ix<< " layer: "<<lay<<" module: "<<mod<<endl;
                pList->AddSignal(k,ix,label,hit,mod,signal);
            }

            ix=tav->Use(k);
            //cout <<"2) valore di ix: "<<ix<<endl;
            //cout<<"3) Sballa qui il TClonesArray!!!"<<endl;
        } // end of loop on strips
    } // end of loop on P/N side
    tav->Clear();
    //cout <<"Ho finito con GetList!!!"<<endl;
}
//----------------------------------------------------------------------
void IlcTARGETsimulationSSD::ChargeToSignal(Int_t lay,Int_t module,IlcTARGETpList *pList) {
    // charge to signal
    static IlcTARGET *ilcTARGET = (IlcTARGET*)gIlc->GetModule("TARGET");
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();


    Float_t threshold = 0.;
    Int_t size = IlcTARGETdigitSSD::GetNTracks();
    Int_t * digits = new Int_t[size];
    Int_t * tracks = new Int_t[size];
    Int_t * hits = new Int_t[size];
    Int_t j1;
    Int_t ix,k,ix2,k2;
    Float_t charges[3] = {
                             0.0,0.0,0.0
                         };
    Float_t signal;
    //IlcTARGETCalibrationSSD* res =(IlcTARGETCalibrationSSD*)GetCalibrationModel(module);
    Double_t gainP=1.;
    Double_t gainN=1.;
    Int_t nsensor=fParam->GetSensor(lay-(nlayers+nendcaps));
    Int_t nstrips=GetNStrips(lay-(nlayers+nendcaps));
    Int_t nrealsdigit=pList->GetNRealSDigit();
    for (k=0;k<nsensor;k++) {       // both sides (0=Pside, 1=Nside)
        for (ix=0;ix<nstrips;ix++) {   // loop over strips
            for (Int_t pix=0;pix<=nrealsdigit;pix++) {
                pList->GetMapIndex(pList->GetRealSDigitAt(pix),k2,ix2);
                if ((k!=k2) || (ix!=ix2)) continue;
                //  if ((ix%2)!=0) continue;
                //cout <<"ci sono sdigit nella mappa"<<endl;
                // if strip is dead -> gain=0
                //if( ((k==0)&&(res->GetGainP(ix)==0)) || ((k==1)&&(res->GetGainN(ix)==0))) continue;

                // signal has to be uncalibrated
                // In real life, gains are supposed to be calculated from calibration runs,
                // stored in the calibration DB and used in the reconstruction
                // (see IlcTARGETClusterFinderSSD.cxx)
                if (k==0) signal /= gainP;
                else signal /= gainN;

                Double_t DEvToADC=200./30000.;
                // signal is converted in unit of ADC
                //cout <<"1) qui esce fuori range"<<endl;
                //cout <<"signal prima della conversione in ADC: "<<GetMapA2(lay-(nlayers+nendcaps))->GetSignal(k,ix)<<endl;
                //cout <<"in chargesignal sono sul layer: "<<lay<<endl;
                if (IlcLog::GetGlobalDebugLevel()>0) cout <<"GetMapA2(lay-(nlayers+nendcaps))->GetSignal(k,ix): "<<GetMapA2(lay-(nlayers+nendcaps))->GetSignal(k,ix)<<endl;
                signal = DEvToADC*GetMapA2(lay-(nlayers+nendcaps))->GetSignal(k,ix);

                //cout <<"2) qui esce fuori range"<<endl;
                if (signal>4096.) {//dan-commento cout <<"esce fuori range"<<endl;
                    signal = 4096.;
                }//if exceeding, accumulate last one
                if (IlcLog::GetGlobalDebugLevel()>0) cout <<"segnale convertito in ADC: "<<signal<<endl;

                // threshold for zero suppression is set on the basis of the noise
                // A good value is 3*sigma_noise
                //if(k==0) threshold = res->GetNoiseP().At(ix);
                //else threshold = res->GetNoiseN().At(ix);
                threshold=3.*3.3333;
                //threshold *= res->GetZSThreshold(); // threshold at 3 sigma noise
                //cout <<"segnale: "<<signal<<endl;
                if (signal < threshold) continue;

                digits[0] = k;
                digits[1] = ix;
                digits[2] = TMath::Nint(signal);
                for (j1=0;j1<size;j1++)if (j1<pList->GetNEntries()) {
                        // only three in digit.
                        tracks[j1]  = pList->GetTrack(k,ix,j1);
                        hits[j1]    = pList->GetHit(k,ix,j1);
                    } else {
                        tracks[j1]  = -3;
                        hits[j1]    = -1;
                    } // end for j1
                // finally add digit
                //dan-commento cout <<"Sto per aggiungere i digits: lay: "<<lay<<" module: "<<module<<endl;
                ilcTARGET->AddSimDigit(1,0,digits,tracks,hits,charges);
            } // end for ix
        }
    } // end for k
    delete [] digits;
    delete [] tracks;
    delete [] hits;
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::WriteSDigits(IlcTARGETpList *pList) {
    // Fills the Summable digits Tree
    Int_t ix,iz;
    static IlcTARGET *ilcTARGET = (IlcTARGET*)gIlc->GetModule("TARGET");

    for (Int_t pix=0;pix<=pList->GetNRealSDigit();pix++) {
        pList->GetMapIndex(pList->GetRealSDigitAt(pix),iz,ix);
        if ((ix%2)==0) continue;

        if (pList->GetSignalOnly(iz,ix)>0.0) {
            //dan-commento  <<"Sto aggiungendo sdigit"<<endl;
            ilcTARGET->AddSumDigit(*(pList->GetpListItem(iz,ix)));
            if (GetDebug(4)) cout << "pListSSD: "<<*(pList->GetpListItem(iz,ix))
                << endl;
        }else cout<<"segnale negativo"<<endl; // end if
    } // end for i,j
    return;
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::FillMapFrompList(Int_t lay,IlcTARGETpList *pList) {
    // Fills fMap2A from the pList of Summable digits
    Int_t k,ix;
    //cout <<"Sta riempiendo la mappa!"<<endl;
    //for(k=0;k<2;k++)
    Int_t nlayers=fParam->GetNlayers();
    Int_t nendcaps=fParam->GetNendcaps();


    for (Int_t pix=0;pix<=pList->GetNRealSDigit();pix++) {
        pList->GetMapIndex(pList->GetRealSDigitAt(pix),k,ix);
        GetMapA2(lay-(nlayers+nendcaps))->AddSignal(k,ix,pList->GetSignal(k,ix));
    }
    //cout <<"Ha riempito la mappa!"<<endl;
    return;
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::Print(ostream *os) {
    //Standard output format for this class

    //IlcTARGETsimulation::Print(os);
    *os << fIonE <<",";
    *os << fDifConst[0] <<","<< fDifConst[1] <<",";
    *os << fDriftVel[0] <<","<< fDriftVel[1];
    //*os <<","; fDCS->Print(os);
    //*os <<","; GetMapA2(lay)->Print(os);
}
//______________________________________________________________________
void IlcTARGETsimulationSSD::Read(istream *is) {
    // Standard output streaming function.

    //IlcTARGETsimulation::Read(is);
    *is >> fIonE;
    *is >> fDifConst[0] >> fDifConst[1];
    *is >> fDriftVel[0] >> fDriftVel[1];
    //fDCS->Read(is);
    //GetMapA2(lay)->Read(is);
}
//______________________________________________________________________
ostream &operator<<(ostream &os,IlcTARGETsimulationSSD &source) {
    // Standard output streaming function.

    source.Print(&os);
    return os;
}
//______________________________________________________________________
istream &operator>>(istream &os,IlcTARGETsimulationSSD &source) {
    // Standard output streaming function.

    source.Read(&os);
    return os;
}
//______________________________________________________________________





