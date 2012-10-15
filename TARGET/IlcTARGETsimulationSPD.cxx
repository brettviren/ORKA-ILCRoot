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

/*
$Id: IlcTARGETsimulationSPD.cxx,v 1.4 2008/11/06 23:57:35 cgatto Exp $
*/
#include <TRandom.h>
#include <Riostream.h>
#include <TH1.h>
#include <TString.h>
#include "IlcTARGETsimulationFastPoints.h"
#include "IlcTARGET.h"
#include "IlcTARGETdigitSPD.h"
#include "IlcTARGEThit.h"
#include "IlcTARGETmodule.h"
#include "IlcTARGETpList.h"
#include "IlcTARGETCalibrationSPD.h"
#include "IlcTARGETsegmentationSPD.h"
#include "IlcTARGETsegmentationSPD2.h"
#include "IlcTARGETsimulationSPD.h"
#include "IlcLog.h"
#include "IlcRun.h"
#include "IlcCDBEntry.h"
#include "IlcCDBLocal.h"
#include "IlcTARGETParam.h"

//#define DEBUG

ClassImp(IlcTARGETsimulationSPD)
////////////////////////////////////////////////////////////////////////
//  Version: 1
//  Modified by D. Elia, G.E. Bruno, H. Tydesjo 
//  Fast diffusion code by Bjorn S. Nilsen
//  March-April 2006
//
//  Version: 0
//  Written by Boris Batyunya
//  December 20 1999
//
//
// IlcTARGETsimulationSPD is to do the simulation of SPDs.
//
////////////////////////////////////////////////////////////////////////

//______________________________________________________________________
IlcTARGETsimulationSPD::IlcTARGETsimulationSPD():
IlcTARGETsimulation(),
fHis(0),
fSPDname(),
fCoupling(){
    // Default constructor.
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    A default constructed IlcTARGETsimulationSPD class.

    IlcDebug(1,Form("Calling default constructor"));
//    Init();
}
//______________________________________________________________________
IlcTARGETsimulationSPD::IlcTARGETsimulationSPD(IlcTARGETDetTypeSim *dettyp):
IlcTARGETsimulation(dettyp),
fHis(0),
fSPDname(),
fCoupling(){
    // standard constructor
    // Inputs:
    //    IlcTARGETsegmentation *seg  A pointer to the segmentation class
    //                             to be used for this simulation
    //    IlcTARGETCalibration     *resp A pointer to the responce class to
    //                             be used for this simulation
    // Outputs:
    //    none.
    // Return:
    //    A default constructed IlcTARGETsimulationSPD class.

    IlcDebug(1,Form("Calling standard constructor "));
//      IlcTARGETCalibrationSPD* res = (IlcTARGETCalibrationSPD*)GetCalibrationModel(fDetType->GetTARGETgeom()->GetStartSPD());
//      res->SetTemperature(0.0);
//      res->SetDistanceOverVoltage(0.0);

    Init();
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::Init(){
    // Initilization
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    none.
    //const Double_t kmictocm = 1.0e-4; // convert microns to cm.

    SetModuleNumber(0);
    SetEventNumber(0);
// non c'erano: +fParam->GetNlayersStrip()+fParam->GetNendcapsStrip()
   for (Int_t lay=0;lay<fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();lay++)
    {
    SetMap2(lay,new IlcTARGETpList(GetNPixelsZ(lay),GetNPixelsX(lay)));
    }
    //IlcTARGETCalibrationSPD* res = (IlcTARGETCalibrationSPD*)GetCalibrationModel(fDetType->GetTARGETgeom()->GetStartSPD());
    //IlcTARGETsegmentationSPD2* seg = (IlcTARGETsegmentationSPD2*)GetSegmentationModel(0);
   // Double_t bias = res->GetBiasVoltage();
//    cout << "Bias Voltage --> " << bias << endl; // dom    
    //res->SetDistanceOverVoltage(kmictocm*seg->Dy(),bias);
// set kind of coupling ("old" or "new")
  //  char opt[20];
    //res->GetCouplingOption(opt);
    //char *old = strstr(opt,"old");
    //if (old) {
      //  fCoupling=2;
   // } else {
        fCoupling=1;
   // } // end if

    // Get the calibration objects for each module(ladder)
    //GetCalibrationObjects(0); //RunNr 0 hard coded for now

}
//______________________________________________________________________
IlcTARGETsimulationSPD::~IlcTARGETsimulationSPD(){
    // destructor
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //     none.

    if (fHis) {
        fHis->Delete(); 
        delete fHis;     
    } // end if fHis
}
//______________________________________________________________________
IlcTARGETsimulationSPD::IlcTARGETsimulationSPD(const 
						   IlcTARGETsimulationSPD 
						   &s) : IlcTARGETsimulation(s){
    //     Copy Constructor
    // Inputs:
    //    IlcTARGETsimulationSPD &s The original class for which
    //                                this class is a copy of
    // Outputs:
    //    none.
    // Return:

    *this = s;
    return;
}
//______________________________________________________________________
IlcTARGETsimulationSPD&  IlcTARGETsimulationSPD::operator=(const 
                                           IlcTARGETsimulationSPD &s){
    //    Assignment operator
    // Inputs:
    //    IlcTARGETsimulationSPD &s The original class for which
    //                                this class is a copy of
    // Outputs:
    //    none.
    // Return:

    if(&s == this) return *this;
    this->fHis = s.fHis;
    fCoupling  = s.fCoupling;
    fSPDname   = s.fSPDname;
    return *this;
}
//______________________________________________________________________
IlcTARGETsimulation&  IlcTARGETsimulationSPD::operator=(const 
                                           IlcTARGETsimulation &s){
    //    Assignment operator
    // Inputs:
    //    IlcTARGETsimulationSPD &s The original class for which
    //                                this class is a copy of
    // Outputs:
    //    none.
    // Return:

    if(&s == this) return *this;
    Error("IlcTARGETsimulationSPD","Not allowed to make a = with "
          "IlcTARGETsimulationSPD","Using default creater instead");

    return *this;
}

//______________________________________________________________________
void IlcTARGETsimulationSPD::GetCalibrationObjects(Int_t RunNr) {
    //    Gets the calibration objects for each module (ladder) 
    // Inputs:
    //    RunNr: hard coded to RunNr=0 for now
    // Outputs:
    //    none.
    // Return:
    //    none.

  IlcCDBManager* man = IlcCDBManager::Instance();
  if(!man->IsDefaultStorageSet()) {
    man->SetDefaultStorage("local://$ILC_ROOT");
  }
  IlcCDBEntry *entrySPD = man->Get("TARGET/Calib/CalibSPD", RunNr);
  if(!entrySPD){
    IlcWarning("Cannot find SPD calibration entry!");
    return;
  }
  TObjArray *respSPD = (TObjArray *)entrySPD->GetObject();
  if ((! respSPD)) {
    IlcWarning("Cannot get data from SPD database entry!");
    return;
  }
  for (Int_t mod=0; mod<240; mod++) {
    fCalObj[mod] = (IlcTARGETCalibrationSPD*) respSPD->UncheckedAt(mod);
  }
}

//______________________________________________________________________
void IlcTARGETsimulationSPD::InitSimulationModule(Int_t module, Int_t event){
    //  This function creates maps to build the list of tracks for each
    //  summable digit. Inputs defined by base class.
    //  Inputs:
    //    Int_t module   // Module number to be simulated
    //    Int_t event    // Event number to be simulated
    //  Outputs:
    //    none
    //  Returns:
    //    none

    IlcDebug(1,Form("(module=%d,event=%d)",module,event));
    SetModuleNumber(module);
    SetEventNumber(event);
    int lay,lad,det;
    fDetType->GetTARGETgeom2()->GetModuleId(module,lay,lad,det);
    ClearMap(lay-1);
}
//_____________________________________________________________________
void IlcTARGETsimulationSPD::SDigitiseModule(IlcTARGETmodule *mod,Int_t,
                                               Int_t event){
    //  This function begins the work of creating S-Digits.  Inputs defined
    //  by base class.
    //  Inputs:
    //    IlcTARGETmodule *mod  //  module
    //    Int_t              //  not used
    //    Int_t event        //  Event number
    //  Outputs:
    //    none
    //  Return:
    //    test              //  test returns kTRUE if the module contained hits
    //                      //  test returns kFALSE if it did not contain hits

//  IlcTARGET *TARGET  = (IlcTARGET*) gIlc->GetModule("TARGET");
    IlcDebug(1,Form("(mod=%p, ,event=%d)",mod,event));
// // // //     IlcInfo(Form("In event %d module %d there are %d hits returning.", event, mod->GetIndex(),mod->GetNhits()));
    if(!(mod->GetNhits())){
        IlcDebug(1,Form("In event %d module %d there are %d hits returning.",
			event, mod->GetIndex(),mod->GetNhits()));
        return;// if module has no hits don't create Sdigits
    } // end if
    SetModuleNumber(mod->GetIndex());
    SetEventNumber(event);
//if (mod->GetIndex() >= 96) {
//      IlcTARGETsimulationFastPoints *simfast=new IlcTARGETsimulationFastPoints();
//      TARGET->SetSimulationModel(0,simfast);
//    TARGET->HitsToFastRecPoints(event,event/10020,-1," ","SPD"," ");
 
//}
    HitToSDigit(mod);
    
    //HitToSDigitFast(mod);
    //RemoveDeadPixels(mod);
//    cout << "After Remove in SDigitiseModule !!!!!" << endl; // dom
//    cout << "Module " << mod->GetIndex() << " Event " << event << endl; // dom
    WriteSDigits(mod->GetLayer()-1);
    ClearMap(mod->GetLayer()-1);
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::WriteSDigits(Int_t lay){
    //  This function adds each S-Digit to pList
    //  Inputs:
    //    none.
    //  Outputs:
    //    none.
    //  Return:
    //    none
    Int_t ix, iz;
    static IlcTARGET *ilcTARGET = (IlcTARGET*)gIlc->GetModule("TARGET");

    IlcDebug(1,Form("Writing SDigits for module %d",GetModuleNumber()));
// // // //     IlcInfo(Form("Writing SDigits for module %d layer: %d *** %d",GetModuleNumber(), lay, GetMap2(lay)->GetNRealSDigit()));
//    cout << "WriteSDigits for module " << GetModuleNumber() << endl; // dom
    //GetMap2(lay)->GetMaxMapIndex(niz, nix);
    //Int_t sdigit=0;
    //for(iz=0; iz<niz; iz++)for(six=0; ix<nix; ix++){
      for (Int_t pix=0;pix<=GetMap2(lay)->GetNRealSDigit();pix++){
        GetMap2(lay)->GetMapIndex(GetMap2(lay)->GetRealSDigitAt(pix),iz,ix);
        if(GetMap2(lay)->GetSignalOnly(iz,ix)>0.0){
        //IlcDebug(1,Form("WriteSDigit: lay:%d, ix:%d, iz:%d, nix: %d, niz:%d, signal: %2f\n",lay,ix,iz,nix,niz,GetMap2(lay)->GetSignalOnly(iz,ix)));


            ilcTARGET->AddSumDigit(*(GetMap2(lay)->GetpListItem(iz,ix)));
	    if(IlcDebugLevel()>0) {
	      cout << "-IIlcTARGETsimulationSPD::WriteSDigits: " << *(GetMap2(lay)->GetpListItem(iz,ix)) << endl;
            } // end if GetDebug
        } // end if GetMap()->GetSignalOnly(iz,ix)>0.0
    } // end for iz,ix
    return; 
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::FinishSDigitiseModule(){
    //  This function calls SDigitsToDigits which creates Digits from SDigits
    //  Inputs:
    //    none
    //  Outputs:
    //    none
    //  Return
    //    none

    IlcDebug(1,"()");
//    cout << "FinishSDigitiseModule for module " << GetModuleNumber() << endl; // dom
    FrompListToDigits(); // Charge To Signal both adds noise and
    int lay,lad,det;
    fDetType->GetTARGETgeom2()->GetModuleId(fModule,lay,lad,det);
    ClearMap(lay-1);
    return;
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::DigitiseModule(IlcTARGETmodule *mod,Int_t,
                                              Int_t){
    //  This function creates Digits straight from the hits and then adds
    //  electronic noise to the digits before adding them to pList
    //  Each of the input variables is passed along to HitToSDigit
    //  Inputs:
    //    IlcTARGETmodule *mod     module
    //    Int_t                 Dummy.
    //    Int_t                 Dummy
    //  Outputs:
    //     none.
    //  Return:
    //    none.

    IlcDebug(1,Form("(mod=%p,,)",mod));
    // HitToSDigit(mod);
    HitToSDigitFast(mod);
    RemoveDeadPixels(mod);
//    cout << "After Remove in DigitiseModule in module " << mod->GetIndex() << endl; // dom
    FrompListToDigits();
    ClearMap(mod->GetLayer()-1);
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::HitToSDigit(IlcTARGETmodule *mod){
    // Does the charge distributions using Gaussian diffusion charge charing.
    // Inputs:
    //    IlcTARGETmodule *mod  Pointer to this module
    // Output:
    //    none.
    // Return:
    //    none.
    const Float_t kmictocm = 0.0001; // convert microns to cm.
    const Double_t kcon2 = 1.723467527E-04; // == 2k/e [J/col or volts]
    TObjArray *hits = mod->GetHits();
    Int_t nhits = hits->GetEntriesFast();
    Int_t h,ix,iz,i;
    Int_t idtrack;
    Double_t x0=0.0,x1=0.0,y0=0.0,y1=0.0,z0=0.0,z1=0.0,de=0.0;
    Double_t x,y,z,t,tp,st,dt=0.2,el,sig,sigx,sigz;

    IlcTARGETsegmentationSPD2* seg = (IlcTARGETsegmentationSPD2*)GetSegmentationModel2(0);

    Double_t fdv=(seg->Dy(mod->GetLayer()-1)*kmictocm)/(fParam->GetBiasVoltage());
    Double_t fda=fParam->GetFda();
    //fda=0.283333333;    
    //IlcTARGETCalibrationSPD* res = (IlcTARGETCalibrationSPD*)GetCalibrationModel(fDetType->GetTARGETgeom()->GetStartSPD());
    Double_t thick = 0.5*kmictocm*seg->Dy(mod->GetLayer()-1);  // Half Thickness
    //res->GetSigmaDiffusionAsymmetry(fda);

    IlcDebug(1,Form("(mod=%p) fCoupling=%d",mod,fCoupling));
    if(nhits<=0) { return;}
    for(h=0;h<nhits;h++){
      if ((gRandom->Rndm())>fParam->GetSingleHitPixelEfficiency()) { Info("IlcTARGETsimulationSPD","module:%d hit:%d rejected!!!",mod->GetIndex(),h); continue;}
      Double_t Sigmade=fParam->GetSigmaDe(0);
      Double_t ionization=mod->GetHit(h)->GetIonization();
      if ((ionization<=0.) || (ionization+(gRandom->Gaus(0.,Sigmade)))<=0.) continue;
      if(IlcDebugLevel()>0) {
	IlcDebug(1,Form("Hits, %d", h));
	cout << *(mod->GetHit(h)) << endl;
      } // end if GetDebug
        if(!mod->LineSegmentL(h,x0,x1,y0,y1,z0,z1,de,idtrack)) continue;
        st = TMath::Sqrt(x1*x1+y1*y1+z1*z1);
//        cout <<"layer: "<<mod->GetLayer()-1<<" hits x0: "<<x0<<"hits y0: "<<y0<<" hits z0: "<<z0<<endl;
//        cout <<"layer: "<<mod->GetLayer()-1<<" hits x1: "<<x1+x0<<"hits y1: "<<y1+y0<<" hits z1: "<<z1+z0<<endl;
        if(st>0.0){
            st = (Double_t)((Int_t)(st/kmictocm)); // number of microns
            if(st<=1.0) st = 1.0;
            dt = 1.0/st;
//                cout<< "layer: "<<mod->GetLayer()-1<<"dt: "<<0.5*dt<<" sdigit x: "<<x0+x1*0.5*dt<<" sdigit y: "<<y0+y1*0.5*dt<<" sdigit z: "<<z0+z1*0.5*dt<<endl;
        //        el  = res->GeVToCharge((Double_t)(dt*de));
	    el=((Double_t)(dt*de))/3.6e-9;
                
	    //sig = res->SigmaDiffusion1D(TMath::Abs(thick + y));
//                cout << "Dy= " << seg->Dy(mod->GetLayer()-1) << endl;
	    if(IlcDebugLevel()>0) IlcDebug(1,Form("Distance Over Voltage: %f",fdv));

            for(t=0.0;t+0.5*dt<1.0;t+=dt){ // Integrate over t
               tp  = t+0.5*dt;
	       x   = x0+x1*tp;
	       y   = y0+y1*tp;
	       z   = z0+z1*tp;
// // // // 	       IlcInfo(Form("lay: %d x: %f %f %f z: %f %f %f %d", mod->GetLayer()-1, x, x0, x1, z, z0, z1,seg->LocalToDet(mod->GetLayer()-1,x,z,ix,iz)));

	       if(!(seg->LocalToDet(mod->GetLayer()-1,x,z,ix,iz))) continue; // outside
		if(IlcDebugLevel()>0) IlcDebug(1,Form("Daniele Debug_______Row: %d, Column: %d",ix,iz));

		sig=TMath::Sqrt(kcon2*300.0*fdv*(TMath::Abs(thick + y)));  // [cm]
		sig = 0.; // FIXME

                //  SpreadCharge(x,z,ix,iz,el,sig,idtrack,h);
                sigx=sig;
                sigz=sig*fda;

// // // // 		IlcInfo("(mod=%d, x0=%e, z0=%e, ix0=%d, iz0=%d, el=%e, sig=%e, t=%d, i=%d)", mod->GetIndex(),x,z,ix,iz,el,sigx,sigz,idtrack,h);
                SpreadChargeAsym(mod->GetLayer()-1,x,z,ix,iz,el,sigx,sigz,idtrack,h);
            } // end for t
  //              cout<< "layer: "<<mod->GetLayer()-1<<" step: "<<tp<<" sdigit x: "<<x<<" sdigit y: "<<y<<" sdigit z: "<<z<<endl;

	} else { // st == 0.0 deposit it at this point
	    Float_t tp=0.5;
	    x   = x0+x1*tp;
	    y   = y0+y1*tp;
	    z   = z0+z1*tp;
            if(!(seg->LocalToDet(mod->GetLayer()-1,x,z,ix,iz))) continue; // outside
            //el  = res->GeVToCharge((Double_t)de);
            el=((Double_t)de)/3.6e-9;

	    sig=TMath::Sqrt(kcon2*300.0*0.000375*(TMath::Abs(thick + y)));  // 
	    sig = 0.; // FIXME
            //sig = res->SigmaDiffusion1D(TMath::Abs(thick + y));
            // SpreadCharge(x,z,ix,iz,el,sig,idtrack,h);
            sigx=sig;
            sigz=sig*fda;

            SpreadChargeAsym(mod->GetLayer()-1,x,z,ix,iz,el,sigx,sigz,idtrack,h);

        } // end if st>0.0

	// Coupling
	IlcTARGETpList *plist=GetMap2(mod->GetLayer()-1);
	int nreal=plist->GetNRealSDigit();
        switch (fCoupling) {
        default:
            break;
        case 1: //case 3:
	    for(i=0;i<=nreal;i++)
                if(plist->GetpListItem(plist->GetRealSDigitAt(i))==0) continue;
                else{
                    plist->GetMapIndex(plist->GetRealSDigitAt(i),iz,ix);
                    SetCoupling(mod->GetLayer()-1,iz,ix,idtrack,h);
                } // end for i
            break;
        case 2: // case 4:
            for(i=0;i<=nreal;i++) 
                if(plist->GetpListItem(plist->GetRealSDigitAt(i))==0) continue;
                else{
                    plist->GetMapIndex(plist->GetRealSDigitAt(i),iz,ix);
                    SetCouplingOld(mod->GetLayer()-1,iz,ix,idtrack,h);
                } // end for i
            break;
        } // end switch

// // // //         IlcInfo(Form("(mod: idx: %d layer: %d ladder: %d) idtrack=%d", mod->GetIndex(), mod->GetLayer(), mod->GetLadder(), idtrack));

    } // Loop over all hits h
                
    if(GetDebug(1))Info("HitToSDigit","Finished fCoupling=%d",fCoupling);
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::HitToSDigitFast(IlcTARGETmodule *mod){
    // Does the charge distributions using Gaussian diffusion charge charing.    // Inputs:
    //    IlcTARGETmodule *mod  Pointer to this module
    // Output:
    //    none.
    // Return:
    //    none.
    const Double_t kmictocm = 1.0e-4; // convert microns to cm.
    const Int_t kn10=10;
    const Double_t kti[kn10]={7.443716945e-3,2.166976971e-1,3.397047841e-1,
                            4.325316833e-1,4.869532643e-1,5.130467358e-1,
                            5.674683167e-1,6.602952159e-1,7.833023029e-1,
                            9.255628306e-1};
    const Double_t kwi[kn10]={1.477621124e-1,1.346333597e-1,1.095431813e-1,
                            7.472567455e-2,3.333567215e-2,3.333567215e-2,
                            7.472567455e-2,1.095431813e-1,1.346333597e-1,
                            1.477621124e-1};
    TObjArray *hits = mod->GetHits();
    Int_t nhits = hits->GetEntriesFast();
    Int_t h,ix,iz,i;
    Int_t idtrack;
    Double_t x0=0.0,x1=0.0,y0=0.0,y1=0.0,z0=0.0,z1=0.0,de=0.0;
    Double_t x,y,z,t,st,el,sig,sigx,sigz,fda;
    IlcTARGETsegmentationSPD2* seg = (IlcTARGETsegmentationSPD2*)GetSegmentationModel(0);
    //IlcTARGETCalibrationSPD* res = (IlcTARGETCalibrationSPD*)GetCalibrationModel(fDetType->GetTARGETgeom()->GetStartSPD());
    Double_t thick = 0.5*kmictocm*seg->Dy(mod->GetLayer()-1);  // Half thickness
    fda=0.85;
    //res->GetSigmaDiffusionAsymmetry(fda);
//    cout << "Half Thickness " << thick << endl;  // dom
//    cout << "Diffusion asymm " << fda << endl;  // dom

    IlcDebug(1,Form("(mod=%p) fCoupling=%d",mod,fCoupling));
    if(nhits<=0) return;
    for(h=0;h<nhits;h++){
      if(IlcDebugLevel()>0) {
        IlcDebug(1,Form("Hits, %d", h));
        cout << *(mod->GetHit(h)) << endl;
      } // end if GetDebug
        if(!mod->LineSegmentL(h,x0,x1,y0,y1,z0,z1,de,idtrack)) continue;
        st = TMath::Sqrt(x1*x1+y1*y1+z1*z1);
        if(st>0.0) for(i=0;i<kn10;i++){ // Integrate over t
            t   = kti[i];
            x   = x0+x1*t;
            y   = y0+y1*t;
            z   = z0+z1*t;
                if(!(seg->LocalToDet(mod->GetLayer()-1,x,z,ix,iz))) continue; // outside
                // el  = res->GeVToCharge((Double_t)(dt*de));
                // el  = 1./kn10*res->GeVToCharge((Double_t)de);
                el  = kwi[i]*((Double_t)de)/3.6e-9; 
                if(GetDebug(1)){
                    if(el<=0.0) cout<<"el="<<el<<" kwi["<<i<<"]="<<kwi[i]
                                    <<" de="<<de<<endl;
                } // end if GetDebug
                //sig = res->SigmaDiffusion1D(TMath::Abs(thick + y));
                const Double_t kcon2 = 1.723467527E-04; // == 2k/e [J/col or volts]

                sig=TMath::Sqrt(kcon2*300.0*0.000375*(TMath::Abs(thick + y)));  // 
                sigx=sig;
                sigz=sig*fda;
                //SpreadCharge(x,z,ix,iz,el,sig,idtrack,h);
                SpreadChargeAsym(mod->GetLayer()-1,x,z,ix,iz,el,sigx,sigz,idtrack,h);
                // cout << "sigx sigz " << sigx << " " << sigz << endl; // dom
            } // end for i // End Integrate over t
        else { // st == 0.0 deposit it at this point
            x   = x0;
            y   = y0;
            z   = z0;
            if(!(seg->LocalToDet(mod->GetLayer()-1,x,z,ix,iz))) continue; // outside
            el  = ((Double_t)de)/3.6e-9;
            //sig = res->SigmaDiffusion1D(TMath::Abs(thick + y));
            const Double_t kcon2 = 1.723467527E-04; // == 2k/e [J/col or volts]

                sig=TMath::Sqrt(kcon2*300.0*0.000375*(TMath::Abs(thick + y)));  // 
            //SpreadCharge(x,z,ix,iz,el,sig,idtrack,h);
            sigx=sig;
            sigz=sig*fda;
            SpreadChargeAsym(mod->GetLayer()-1,x,z,ix,iz,el,sigx,sigz,idtrack,h);
        } // end if st>0.0
        // Coupling
        switch (fCoupling) {
        default:
            break;
        case 1: // case 3:
            for(i=0;i<GetMap2(mod->GetLayer()-1)->GetEntries();i++)
                if(GetMap2(mod->GetLayer()-1)->GetpListItem(i)==0) continue;
                else{
                    GetMap2(mod->GetLayer()-1)->GetMapIndex(GetMap2(mod->GetLayer()-1)->GetpListItem(i)->GetIndex(),iz,ix);
                    SetCoupling(mod->GetLayer()-1,iz,ix,idtrack,h);
                } // end for i
            break;
        case 2: // case 4:
            for(i=0;i<GetMap2(mod->GetLayer()-1)->GetEntries();i++)
                if(GetMap2(mod->GetLayer()-1)->GetpListItem(i)==0) continue;
                else{
                    GetMap2(mod->GetLayer()-1)->GetMapIndex(GetMap2(mod->GetLayer()-1)->GetpListItem(i)->GetIndex(),iz,ix);  
                    SetCouplingOld(mod->GetLayer()-1,iz,ix,idtrack,h);
                } // end for i
            break;
        } // end switch
    } // Loop over all hits h
    if(GetDebug(2))Info("HitToSDigit","Finished fCoupling=%d",fCoupling);
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::SpreadCharge(Int_t lay,Double_t x0,Double_t z0,
                                            Int_t ix0,Int_t iz0,
					    Double_t el,Double_t sig,Int_t t,
					    Int_t hi){
    // Spreads the charge over neighboring cells. Assume charge is distributed
    // as charge(x,z) = (el/2*pi*sig*sig)*exp(-arg)
    // arg=((x-x0)*(x-x0)/2*sig*sig)+((z-z0*z-z0)/2*sig*sig)
    // Defined this way, the integral over all x and z is el.
    // Inputs:
    //    Double_t x0   x position of point where charge is liberated
    //    Double_t y0   y position of point where charge is liberated
    //    Double_t z0   z position of point where charge is liberated
    //    Int_t    ix0  row of cell corresponding to point x0
    //    Int_t    iz0  columb of cell corresponding to point z0
    //    Double_t el   number of electrons liberated in this step
    //    Double_t sig  Sigma difusion for this step (y0 dependent)
    //    Int_t    t    track number
    //    Int_t    ti   hit track index number
    //    Int_t    hi   hit "hit" index number
    // Outputs:
    //     none.
    // Return:
    //     none.
    const Int_t knx = 3,knz = 2;
    const Double_t kRoot2 = 1.414213562; // Sqrt(2).
    const Double_t kmictocm = 1.0e-4; // convert microns to cm.
    Int_t ix,iz,ixs,ixe,izs,ize;
    Float_t x,z;
    Double_t x1,x2,z1,z2,s,sp;
    IlcTARGETsegmentationSPD2* seg = (IlcTARGETsegmentationSPD2*)GetSegmentationModel2(0);


    if(GetDebug(4)) Info("SpreadCharge","(x0=%e,z0=%e,ix0=%d,iz0=%d,el=%e,"
                         "sig=%e,t=%d,i=%d)",x0,z0,ix0,iz0,el,sig,t,hi);
    if(sig<=0.0) { // if sig<=0 No diffusion to simulate.
        GetMap2(lay)->AddSignal(iz0,ix0,t,hi,GetModuleNumber(),el);
        if(GetDebug(2)){
            cout << "sig<=0.0=" << sig << endl;
        } // end if GetDebug
        return;
    } // end if
    sp = 1.0/(sig*kRoot2);
    if(GetDebug(2)){
        cout << "sig=" << sig << " sp=" << sp << endl;
    } // end if GetDebug
    ixs = TMath::Max(-knx+ix0,0);
    ixe = TMath::Min(knx+ix0,seg->Npx(lay)-1);
    izs = TMath::Max(-knz+iz0,0);
    ize = TMath::Min(knz+iz0,seg->Npz(lay)-1);
    for(ix=ixs;ix<=ixe;ix++) for(iz=izs;iz<=ize;iz++){
        seg->DetToLocal(lay,ix,iz,x,z); // pixel center
        x1  = x;
        z1  = z;
        x2  = x1 + 0.5*kmictocm*seg->Dpx(lay,ix); // Upper
        x1 -= 0.5*kmictocm*seg->Dpx(lay,ix);  // Lower
        z2  = z1 + 0.5*kmictocm*seg->Dpz(lay,iz); // Upper
        z1 -= 0.5*kmictocm*seg->Dpz(lay,iz);  // Lower
        x1 -= x0; // Distance from where track traveled
        x2 -= x0; // Distance from where track traveled
        z1 -= z0; // Distance from where track traveled
        z2 -= z0; // Distance from where track traveled
        s   = 0.25; // Correction based on definision of Erfc
        s  *= TMath::Erfc(sp*x1) - TMath::Erfc(sp*x2);
        if(GetDebug(3)){
            cout <<"el="<<el<<" ix0="<<ix0<<" ix="<<ix<<" x0="<<x<<
                " iz0="<<iz0<<" iz="<<iz<<" z0="<<z<< 
                " sp*x1="<<sp*x1<<" sp*x2="<<sp*x2<<" s="<<s;
        } // end if GetDebug
        s  *= TMath::Erfc(sp*z1) - TMath::Erfc(sp*z2);
        if(GetDebug(3)){
            cout<<" sp*z1="<<sp*z1<<" sp*z2="<<sp*z2<<" s="<<s<< endl;
        } // end if GetDebug
        GetMap2(lay)->AddSignal(iz,ix,t,hi,GetModuleNumber(),s*el);
    } // end for ix, iz
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::SpreadChargeAsym(Int_t lay,Double_t x0,Double_t z0,
                                            Int_t ix0,Int_t iz0,
                                            Double_t el,Double_t sigx,Double_t sigz,
                                            Int_t t,Int_t hi){
    // Spreads the charge over neighboring cells. Assume charge is distributed
    // as charge(x,z) = (el/2*pi*sigx*sigz)*exp(-arg)
    // arg=((x-x0)*(x-x0)/2*sigx*sigx)+((z-z0*z-z0)/2*sigz*sigz)
    // Defined this way, the integral over all x and z is el.
    // Inputs:
    //    Double_t x0   x position of point where charge is liberated
    //    Double_t y0   y position of point where charge is liberated
    //    Double_t z0   z position of point where charge is liberated
    //    Int_t    ix0  row of cell corresponding to point x0
    //    Int_t    iz0  columb of cell corresponding to point z0
    //    Double_t el   number of electrons liberated in this step
    //    Double_t sigx Sigma difusion along x for this step (y0 dependent)
    //    Double_t sigz Sigma difusion along z for this step (y0 dependent)
    //    Int_t    t    track number
    //    Int_t    ti   hit track index number
    //    Int_t    hi   hit "hit" index number
    // Outputs:
    //     none.
    // Return:
    //     none.
    const Int_t knx = 3,knz = 2;
    const Double_t kRoot2 = 1.414213562; // Sqrt(2).
    const Double_t kmictocm = 1.0e-4; // convert microns to cm.
    Int_t ix,iz,ixs,ixe,izs,ize;
    Float_t x,z,arg=0.0;
    Double_t x1,x2,z1,z2,s,spx,spz;
    IlcTARGETsegmentationSPD2* seg = (IlcTARGETsegmentationSPD2*)GetSegmentationModel2(0);


    if(GetDebug(4)) Info("SpreadChargeAsym","(x0=%e,z0=%e,ix0=%d,iz0=%d,el=%e,"
                         "sig=%e,t=%d,i=%d)",x0,z0,ix0,iz0,el,sigx,sigz,t,hi);
    //sigx=0.0;
    //sigz=0.0;
    if(sigx<=0.0 || sigz<=0.0) { // if sig<=0 No diffusion to simulate.
        GetMap2(lay)->AddSignal(iz0,ix0,t,hi,GetModuleNumber(),el);
        if(GetDebug(2)){
            cout << "sigx<=0.0=" << sigx << endl;
            cout << "sigz<=0.0=" << sigz << endl;
        } // end if GetDebug
        return;
    } // end if
    spx = 1.0/(sigx*kRoot2);     spz = 1.0/(sigz*kRoot2);
    if(GetDebug(2)){
        cout << "sigx=" << sigx << " spx=" << spx << endl;
        cout << "sigz=" << sigz << " spz=" << spz << endl;
    } // end if GetDebug
    ixs = TMath::Max(-knx+ix0,0);
    ixe = TMath::Min(knx+ix0,seg->Npx(lay)-1);
    izs = TMath::Max(-knz+iz0,0);
    ize = TMath::Min(knz+iz0,seg->Npz(lay)-1);
    double dpx=0.5*kmictocm*seg->Dpx(lay,0);
    double dpz=0.5*kmictocm*seg->Dpz(lay,0);
    

    for(ix=ixs;ix<=ixe;ix++) for(iz=izs;iz<=ize;iz++){

        seg->DetToLocal(lay,ix,iz,x,z); // pixel center
        x1  = x;
        z1  = z;
        //cout << "Dpx(" << lay <<"," << ix << ")=" << seg->Dpx(lay,ix) << endl;
        x2  = x1 + dpx; // Upper
        x1 -= dpx;  // Lower
        //cout << "Dpz(" << lay <<"," << iz << ")=" << seg->Dpz(lay,iz) << endl;
        z2  = z1 + dpz; // Upper
        z1 -= dpz;  // Lower
        x1 -= x0; // Distance from where track traveled
        x2 -= x0; // Distance from where track traveled
        z1 -= z0; // Distance from where track traveled
        z2 -= z0; // Distance from where track traveled
	//s   = 0.25; // Correction based on definision of Erfc
        //s  *= TMath::Erfc(spx*x1) - TMath::Erfc(spx*x2);
            //cout <<"el="<<el<<" ix0="<<ix0<<" ix="<<ix<<" x0="<<x<<
              //  " iz0="<<iz0<<" iz="<<iz<<" z0="<<z<< 
              //  " spx*x1="<<spx*x1<<" spx*x2="<<spx*x2<<" s="<<s;
         // end if GetDebug
        //s  *= TMath::Erfc(spz*z1) - TMath::Erfc(spz*z2);
        // if (ix==290)   cout<<" spx*x1="<<spx*x1<<" spx*x2="<<spx*x2<<" s="<<s<< endl;
       //if (ix==290)     cout<<" spz*z1="<<spz*z1<<" spz*z2="<<spz*z2<<" s="<<s<< endl;
	arg=((x-x0)*(x-x0)/2*sigx*sigx)+((z-z0*z-z0)/2*sigz*sigz);
	s=  (el/2*TMath::Pi()*sigx*sigz)*exp(-arg);
//        cout<<s<<" x "<<x<<" x0 "<<x0<<" dx "<<dpx<<" sx "<<sigx
// 	   <<" z "<<z<<" z0 "<<z0<<" dz "<<dpz<<" sz "<<sigz<<endl;
    // 
//        cout << "In SpreadChargeAsymm: ix= "<< ix << " iz= " << iz << " segnal= " << s*el << endl;
        //cout <<"Dallo Spread"<<endl;
        GetMap2(lay)->AddSignal(iz,ix,t,hi,GetModuleNumber(),s*el);


    } // end for ix, iz
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::RemoveDeadPixels(IlcTARGETmodule *mod){
    //    Removes dead pixels on each module (ladder)
    // Inputs:
    //    Module Index (0,239)
    // Outputs:
    //    none.
    // Return:
    //    none.

  Int_t moduleNr = mod->GetIndex();
  Int_t nrDead = fCalObj[moduleNr]->GetNrDead();
  for (Int_t i=0; i<nrDead; i++) {
    GetMap2(mod->GetLayer()-1)->DeleteHit(fCalObj[moduleNr]->GetDeadColAt(i),fCalObj[moduleNr]->GetDeadRowAt(i));
  }
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::FrompListToDigits(){
    // add noise and electronics, perform the zero suppression and add the
    // digit to the list
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //    none.
    static IlcTARGET *ilcTARGET = (IlcTARGET*)gIlc->GetModule("TARGET");
    Int_t j,ix,iz,lay,lad,det;
    Double_t  electronics;
    Double_t sig;
    const Int_t    knmaxtrk=IlcTARGETdigitSPD::GetNTracks();
    static IlcTARGETdigitSPD dig;
    //IlcTARGETCalibrationSPD* res = (IlcTARGETCalibrationSPD*)GetCalibrationModel(fDetType->GetTARGETgeom()->GetStartSPD());
    if(GetDebug(1)) Info("FrompListToDigits","()");
    (fDetType->GetTARGETgeom2())->GetModuleId(fModule,lay,lad,det);

   lay--;
    for (Int_t pix=0;pix<=GetMap2(lay)->GetNRealSDigit();pix++){
     GetMap2(lay)->GetMapIndex(GetMap2(lay)->GetRealSDigitAt(pix),iz,ix);
    //for(iz=0; iz<GetNPixelsZ(lay); iz++) for(ix=0; ix<GetNPixelsX(lay); ix++){
 

// NEW (for the moment plugged by hand, in the future possibly read from Data Base)
// here parametrize the efficiency of the pixel along the row for the test columns (1,9,17,25)
//        if(iz==1 || iz == 9 || iz == 17 || iz == 25) {
//        Double_t eff,p1=0.,p2=0.; 
//        Double_t x=ix;
//        switch (iz) {
//          case 1:   p1=0.63460;p2=0.42438E-01;break;  
//          case 9:   p1=0.41090;p2=0.75914E-01;break;
//	  case 17:  p1=0.31883;p2=0.91502E-01;break;
//	  case 25:  p1=0.48828;p2=0.57975E-01;break;
//         } // end switch
//          eff=1.-p1*exp(-p2*x);
//          if (gRandom->Rndm() >= eff) continue;
//        } // end  if 
 // END parametrize the efficiency
 // 
        electronics = 0.0;

        //UpdateMapNoise(lay,ix,iz,electronics);


        //
        // Apply Threshold and write Digits.
        sig = GetMap2(lay)->GetSignalOnly(iz,ix);
       //if (sig!=0) IlcDebug(1,Form("Ho preso il segnale: %f\n",sig));

       //FillHistograms(ix,iz,sig+electronics);

        
        if (sig+electronics <= 0.) continue; //Threshold da mettere in param
        IlcDebug(1,Form("signal for pixel x: %d, pixel z: %d con energia: %2f\n",ix,iz,sig));
        dig.SetCoord1(iz);
        dig.SetCoord2(ix);
        dig.SetSignal(1);

//        dig.SetSignalSPD((Int_t) GetMap()->GetSignal(iz,ix));
        Double_t aSignal =  GetMap2(lay)->GetSignal(iz,ix);
        if (TMath::Abs(aSignal)>2147483647.0) {
          //PH 2147483647 is the max. integer
          //PH This apparently is a problem which needs investigation
          IlcWarning(Form("Too big or too small signal value %f",aSignal));
          aSignal = TMath::Sign((Double_t)2147483647,aSignal);
        }
        dig.SetSignalSPD((Int_t)aSignal);

        for(j=0;j<knmaxtrk;j++){
            if (j<GetMap2(lay)->GetNEntries()) {
                dig.SetTrack(j,GetMap2(lay)->GetTrack(iz,ix,j));
                dig.SetHit(j,GetMap2(lay)->GetHit(iz,ix,j));
            }else { // Default values
                dig.SetTrack(j,-3);
                dig.SetHit(j,-1);
            } // end if GetMap()
        } // end for j
        if(GetDebug(3)){
            cout<<iz<<","<<ix<<","<<*(GetMap2(lay)->GetpListItem(iz,ix))<<endl;
        } // end if GetDebug
        //dan-commento cout <<"Sto per aggiungere i digits: lay: "<<lay<<" module: "<<module<<endl;
        ilcTARGET->AddSimDigit(0,&dig);
    } //  for Map Entries
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::CreateHistograms(Int_t lay){
    // create 1D histograms for tests
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //     none.

    if(GetDebug(1)) Info("CreateHistograms","create histograms");

    fHis = new TObjArray(GetNPixelsZ(lay));
    fSPDname = "spd_";
    for(Int_t i=0;i<GetNPixelsZ(lay);i++) {
        Char_t pixelz[4];
        sprintf(pixelz,"%d",i);
        fSPDname.Append(pixelz);
        fHis->AddAt(new TH1F(fSPDname.Data(),"SPD maps",
                             GetNPixelsX(lay),0.,(Double_t)GetNPixelsX(lay)),i);
    } // end for i
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::FillHistograms(Int_t ix,Int_t iz,Double_t v){
    // Fill the histogram
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //     none.

    if(!GetHistArray()) return; // Only fill if setup.
    if(GetDebug(2)) Info("FillHistograms","fill histograms");
    GetHistogram(iz)->Fill(ix,v);
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::ResetHistograms(Int_t lay){
    // Reset histograms for this detector
    // Inputs:
    //    none.
    // Outputs:
    //    none.
    // Return:
    //     none.

    if(!GetHistArray()) return; // Only fill if setup.
    if(GetDebug(2)) Info("FillHistograms","fill histograms");
    for ( int i=0;i<GetNPixelsZ(lay);i++ ) {
      if (fHis->UncheckedAt(i))    ((TH1F*)fHis->UncheckedAt(i))->Reset();
    } // end for i
}

//______________________________________________________________________
void IlcTARGETsimulationSPD::SetCoupling(Int_t lay,Int_t col, Int_t row, Int_t ntrack,
				      Int_t idhit) {
    //  Take into account the coupling between adiacent pixels.
    //  The parameters probcol and probrow are the probability of the
    //  signal in one pixel shared in the two adjacent pixels along
    //  the column and row direction, respectively.
    //  Note pList is goten via GetMap() and module is not need any more.
    //  Otherwise it is identical to that coded by Tiziano Virgili (BSN).
    //Begin_Html
    /*
      <img src="picts/TARGET/barimodel_3.gif">
      </pre>
      <br clear=left>
      <font size=+2 color=red>
      <a href="mailto:tiziano.virgili@cern.ch"></a>.
      </font>
      <pre>
    */
    //End_Html
    // Inputs:
    //    Int_t col            z cell index
    //    Int_t row            x cell index
    //    Int_t ntrack         track incex number
    //    Int_t idhit          hit index number
    // Outputs:
    //    none.
    // Return:
    //     none.
return;
    Int_t j1,j2,flag=0;
    Double_t pulse1,pulse2;
    Double_t couplR=0.0,couplC=0.0;
    Double_t xr=0.;

    GetCouplings(couplC,couplR);
    /*if(GetDebug(1))*/ Info("SetCoupling","(col=%d,row=%d,ntrack=%d,idhit=%d) "
                         "Calling SetCoupling couplC=%e couplR=%e",
                         col,row,ntrack,idhit,couplC,couplR);
    j1 = col;
    j2 = row;
    pulse1 = GetMap2(lay)->GetSignalOnly(col,row);
    pulse2 = pulse1;
    for (Int_t isign=-1;isign<=1;isign+=2){// loop in col direction
        do{
            j1 += isign;
            xr = gRandom->Rndm();
            if ((j1<0) || (j1>GetNPixelsZ(lay)-1) || (xr>couplC)){
                j1 = col;
                flag = 1;
            }else{
                //cout <<"Dal coupling"<<endl;
                UpdateMapSignal(lay,row,j1,ntrack,idhit,pulse1);
                //  flag = 0;
                flag = 1; // only first next!!
            } // end if
        } while(flag == 0);
        // loop in row direction
        do{
            j2 += isign;
            xr = gRandom->Rndm();
            if ((j2<0) || (j2>GetNPixelsX(lay)-1) || (xr>couplR)){
                j2 = row;
                flag = 1;
            }else{
                //cout <<"Dal coupling"<<endl;
                UpdateMapSignal(lay,j2,col,ntrack,idhit,pulse2);
                //  flag = 0;
                flag = 1; // only first next!!
            } // end if
        } while(flag == 0);
    } // for isign
}
//______________________________________________________________________
void IlcTARGETsimulationSPD::SetCouplingOld(Int_t lay,Int_t col, Int_t row,
                Int_t ntrack,Int_t idhit) {
    //  Take into account the coupling between adiacent pixels.
    //  The parameters probcol and probrow are the fractions of the
    //  signal in one pixel shared in the two adjacent pixels along
    //  the column and row direction, respectively.
    //Begin_Html
    /*
      <img src="picts/TARGET/barimodel_3.gif">
      </pre>
      <br clear=left>
      <font size=+2 color=red>
      <a href="mailto:Rocco.Caliandro@ba.infn.it"></a>.
      </font>
      <pre>
    */
    //End_Html
    // Inputs:
    //    Int_t col            z cell index
    //    Int_t row            x cell index
    //    Int_t ntrack         track incex number
    //    Int_t idhit          hit index number
    //    Int_t module         module number
    // Outputs:
    //    none.
    // Return:
    //     none.
    return;
    Int_t j1,j2,flag=0;
    Double_t pulse1,pulse2;
    Double_t couplR=0.0,couplC=0.0;

    GetCouplings(couplC,couplR);

    //  Debugging ...
//    cout << "Threshold --> " << GetThreshold() << endl;  // dom
//    cout << "Couplings --> " << couplC << " " << couplR << endl;  //dom


    /*if(GetDebug(1))*/ Info("SetCouplingOld","(col=%d,row=%d,ntrack=%d,idhit=%d) "
                         "Calling SetCoupling couplC=%e couplR=%e",
                         col,row,ntrack,idhit,couplC,couplR);
    for (Int_t isign=-1;isign<=1;isign+=2){// loop in col direction
    pulse1 = GetMap2(lay)->GetSignalOnly(col,row);
    pulse2 = pulse1;
    j1 = col;
    j2 = row;
        do{
            j1 += isign;
            pulse1 *= couplC;
            if ((j1<0)||(j1>GetNPixelsZ(lay)-1)||(pulse1<GetThreshold())){
                pulse1 = GetMap2(lay)->GetSignalOnly(col,row);
                j1 = col;
                flag = 1;
            }else{
                //cout <<"Dal couplingOld"<<endl;
                UpdateMapSignal(lay,row,j1,ntrack,idhit,pulse1);
                // flag = 0;
                flag = 1;  // only first next !!
            } // end if
        } while(flag == 0);
        // loop in row direction
        do{
            j2 += isign;
            pulse2 *= couplR;
            if((j2<0)||(j2>(GetNPixelsX(lay)-1))||(pulse2<GetThreshold())){
                pulse2 = GetMap2(lay)->GetSignalOnly(col,row);
                j2 = row;
                flag = 1;
            }else{
                //cout <<"Dal couplingOld"<<endl;
                UpdateMapSignal(lay,j2,col,ntrack,idhit,pulse2);
                // flag = 0;
                flag = 1; // only first next!!
            } // end if
        } while(flag == 0);
    } // for isign
}
