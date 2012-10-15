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

/* $Id: IlcDCHv1.cxx,v 1.12 2009/12/11 10:40:28 vitomeg Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Transition Radiation Detector version 1 -- slow simulator                //
//                                                                           //
//Begin_Html
/*
<img src="picts/IlcDCHfullClass.gif">
*/
//End_Html
//                                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <stdlib.h> 

#include <TF1.h>
#include <TLorentzVector.h>
#include <TMath.h>
#include <TRandom.h>
#include <TVector.h>
#include <TVirtualMC.h>

#include "IlcConst.h"
#include "IlcLog.h"
#include "IlcMC.h"
#include "IlcRun.h"
#include "IlcDCHgeometry.h"
#include "IlcDCHhit.h"
#include "IlcDCHsim.h"
#include "IlcDCHv1.h"
#include "IlcDCHwireposition.h"

ClassImp(IlcDCHv1)
 
//_____________________________________________________________________________
IlcDCHv1::IlcDCHv1():IlcDCH()
{
  //
  // Default constructor
  //

  fSensSelect        =  0;
  fSensPlane         = -1;
  fSensChamber       = -1;
  fSensSector        = -1;
  fSensSectorRange   =  0;
//  fDeltaE            = NULL;
//  fDeltaG            = NULL;
//  fTR                = NULL;
  fTRon              = kFALSE;
  oldtrk             = -1;
  lastaddedhit       = 0x0;
  exiting            = kFALSE;
  fExitingPrev_det   = 0;
  fPrev_det          = 0;
  fStepSize          = 1.;
  fTypeOfStepManager = 2;
  fDCHwire=0;

  oldtrkWS=0;
  oldevWS=0;
  WSDataBlk = 0x0;
  fHitWiresData =  0x0;
  fHitWires = 0x0;
  tmpIdSL = 0x0;
  tmpIdRing = 0x0;
  tmpIdAbsRing = 0x0;
  tmpIsSense = 0x0;
}

//_____________________________________________________________________________
IlcDCHv1::IlcDCHv1(const char *name, const char *title) 
         :IlcDCH(name, title) 
{
  //
  // Standard constructor for Transition Radiation Detector version 1
  //

  fSensSelect        =  0;
  fSensPlane         = -1;
  fSensChamber       = -1;
  fSensSector        = -1;
  fSensSectorRange   =  0;
//  fDeltaE            = NULL;
//  fDeltaG            = NULL;
//  fTR                = NULL;
  fTRon              = kTRUE;
  oldtrk             = -1;
  lastaddedhit       = 0x0;
  exiting            = kFALSE;
  fExitingPrev_det   = 0;
  fPrev_det          = 0;
  fStepSize          = 1.;
  fTypeOfStepManager = 2;
  //SetBufferSize(128000);
  fDCHwire=0;

  oldtrkWS=0;
  oldevWS=0;
  WSDataBlk = 0x0;
  fHitWiresData =  0x0;
  fHitWires = 0x0;
  tmpIdSL = 0x0;
  tmpIdRing = 0x0;
  tmpIdAbsRing = 0x0;
  tmpIsSense = 0x0;

}

// // // // //_____________________________________________________________________________
// // // // IlcDCHv1::IlcDCHv1(const IlcDCHv1 &dch):IlcDCH(dch)
// // // // {
// // // //   //
// // // //   // Copy constructor
// // // //   //
// // // // 
// // // //   ((IlcDCHv1 &) dch).Copy(*this);
// // // // 
// // // // }

//_____________________________________________________________________________
IlcDCHv1::~IlcDCHv1()
{
  //
  // IlcDCHv1 destructor
  //
//  if (fDeltaE) delete fDeltaE;
//  if (fDeltaG) delete fDeltaG;
//  if (fTR)     delete fTR;

  if (IlcDebugLevelClass()>=1 && fHitWiresData){
    if (oldtrkWS!=-1 && WSDataBlk) {
      Int_t tmpDim=WSDataBlk->GetEntries();
      WSDataBlk->New(tmpDim);
      IlcDCHHitWireStat *tmpBuffWS = (IlcDCHHitWireStat *) WSDataBlk->At(tmpDim);
      tmpBuffWS->SetTrckId(oldtrkWS);
      tmpBuffWS->SetNev(oldevWS);
      tmpBuffWS->SetNHits(tmpNHits);
      tmpBuffWS->SetIdSL(tmpIdSL);
      tmpBuffWS->SetIdRing(tmpIdRing);
      tmpBuffWS->SetIdAbsRing(tmpIdAbsRing);
      tmpBuffWS->SetIsSense(tmpIsSense);
    }
    fHitWiresData->Fill();
    fHitWires->cd();
    fHitWiresData->Write();
    fHitWires->Close();

//     WSDataBlk->Delete();
//     fHitWiresData->Delete();
//     fHitWires->Delete();
    delete fHitWiresData;
    delete WSDataBlk;
    delete fHitWires;
    if (tmpIdSL) delete tmpIdSL;
    if (tmpIdRing) delete tmpIdRing;
    if (tmpIdAbsRing) delete tmpIdAbsRing;
    if (tmpIsSense)  delete tmpIsSense;
  }

}
 
//_____________________________________________________________________________
IlcDCHv1 &IlcDCHv1::operator=(const IlcDCHv1 &dch)
{
  //
  // Assignment operator
  //

  if (this != &dch) ((IlcDCHv1 &) dch).Copy(*this);
  return *this;

}
 
//_____________________________________________________________________________
void IlcDCHv1::Copy(TObject &dch) const
{
	printf("void IlcDCHv1::Copy(TObject &dch) const\n");
  //
  // Copy function
  //

  ((IlcDCHv1 &) dch).fSensSelect        = fSensSelect;
  ((IlcDCHv1 &) dch).fSensPlane         = fSensPlane;
  ((IlcDCHv1 &) dch).fSensChamber       = fSensChamber;
  ((IlcDCHv1 &) dch).fSensSector        = fSensSector;
  ((IlcDCHv1 &) dch).fSensSectorRange   = fSensSectorRange;

  ((IlcDCHv1 &) dch).fTypeOfStepManager = fTypeOfStepManager;
  ((IlcDCHv1 &) dch).fStepSize          = fStepSize;

  ((IlcDCHv1 &) dch).fTRon              = fTRon;
  ((IlcDCHv1 &) dch).fPrev_det          = fPrev_det;

//  fDeltaE->Copy(*((IlcDCHv1 &) dch).fDeltaE);
//  fDeltaG->Copy(*((IlcDCHv1 &) dch).fDeltaG);
//  fTR->Copy(*((IlcDCHv1 &) dch).fTR);

}

//_____________________________________________________________________________
void IlcDCHv1::CreateGeometry()
{
  //
  // Create the GEANT geometry for the Transition Radiation Detector - Version 1
  // This version covers the full azimuth. 
  //

  // Check that FRAME is there otherwise we have no place where to put the DCH
//  IlcModule* frame = gIlc->GetModule("FRAME");
//  if (!frame) return;

  // Define the chambers
  IlcDCH::CreateGeometry();



}

//_____________________________________________________________________________
void IlcDCHv1::CreateMaterials()
{
  //
  // Create materials for the Transition Radiation Detector version 1
  //
  IlcDCH::CreateMaterials();
}

//_____________________________________________________________________________
/*
void IlcDCHv1::CreateTRhit(Int_t det)
{
  //
  // Creates an electron cluster from a TR photon.
  // The photon is assumed to be created a the end of the radiator. The 
  // distance after which it deposits its energy takes into account the 
  // absorbtion of the entrance window and of the gas mixture in drift
  // volume.
  //

  // PDG code electron
  const Int_t   kPdgElectron = 11;

  // Ionization energy
  const Float_t kWion        = 23.53;

  // Maximum number of TR photons per track
  const Int_t   kNTR         = 50;

  TLorentzVector mom, pos;

  // Create TR at the entrance of the chamber
  if (gMC->IsTrackEntering()) {

    // Create TR only for electrons 
    Int_t iPdg = gMC->TrackPid();
    if (TMath::Abs(iPdg) != kPdgElectron) return;

    Float_t eTR[kNTR];
    Int_t   nTR;

    // Create TR photons
    gMC->TrackMomentum(mom);
    Float_t pTot = mom.Rho();
    fTR->CreatePhotons(iPdg,pTot,nTR,eTR);
    if (nTR > kNTR) {
      IlcFatal(Form("Boundary error: nTR = %d, kNTR = %d",nTR,kNTR));
    }

    // Loop through the TR photons
    for (Int_t iTR = 0; iTR < nTR; iTR++) {

      Float_t energyMeV = eTR[iTR] * 0.001;
      Float_t energyeV  = eTR[iTR] * 1000.0;
      Float_t absLength = 0;
      Float_t sigma     = 0;

      // Take the absorbtion in the entrance window into account
      Double_t muMy = fTR->GetMuMy(energyMeV);
      sigma = muMy * fFoilDensity;
      if (sigma > 0.0) {
        absLength = gRandom->Exp(1.0/sigma);
        if (absLength < IlcDCHgeometry::MyThick()) continue;
      }
      else {
        continue;
      }

      // The absorbtion cross sections in the drift gas
      // Gas-mixture (Xe/CO2)
      Double_t muXe = fTR->GetMuXe(energyMeV);
      Double_t muCO = fTR->GetMuCO(energyMeV);
      sigma = (0.85 * muXe + 0.15 * muCO) * fGasDensity * fTR->GetTemp();

      // The distance after which the energy of the TR photon
      // is deposited.
      if (sigma > 0.0) {
        absLength = gRandom->Exp(1.0/sigma);
        if (absLength > (IlcDCHgeometry::DrThick()
                       + IlcDCHgeometry::AmThick())) {
          continue;
	}
      }
      else {
        continue;
      }

      // The position of the absorbtion
      Float_t posHit[3];
      gMC->TrackPosition(pos);
      posHit[0] = pos[0] + mom[0] / pTot * absLength;
      posHit[1] = pos[1] + mom[1] / pTot * absLength;
      posHit[2] = pos[2] + mom[2] / pTot * absLength;

      // Create the charge 
      Int_t q = ((Int_t) (energyeV / kWion));

      // Add the hit to the array. TR photon hits are marked 
      // by negative charge
     //AddHit(gIlc->GetMCApp()->GetCurrentTrackNumber(),det,posHit,-q,kTRUE); 

    }

  }

}
*/

//_____________________________________________________________________________
void IlcDCHv1::Init() 
{
  //
  // Initialise Transition Radiation Detector after geometry has been built.
  //

  IlcDCH::Init();

  gMC->SetMaxNStep(300000); // max. number of steps increased
  IlcDebug(1,"Slow simulator\n");
/* 
  if (fSensSelect) {
    if (fSensPlane   >= 0)
      IlcInfo(Form("Only plane %d is sensitive"));
    if (fSensChamber >= 0)   
      IlcInfo(Form("Only chamber %d is sensitive",fSensChamber));
    if (fSensSector  >= 0) {
      Int_t sens1  = fSensSector;
      Int_t sens2  = fSensSector + fSensSectorRange;
            sens2 -= ((Int_t) (sens2 / IlcDCHgeometry::Nsect())) 
                   * IlcDCHgeometry::Nsect();
	    IlcInfo(Form("Only sectors %d - %d are sensitive\n",sens1,sens2-1));
    }
  }

  // Switch on TR simulation as default
  if (!fTRon) {
    IlcInfo("TR simulation off");
  }
  else {
    fTR = new IlcDCHsim();
  }
 */
  // First ionization potential (eV) for the gas mixture (90% Xe + 10% CO2)
  const Float_t kPoti = 12.1;
  // Maximum energy (50 keV);
  const Float_t kEend = 50000.0;
  // Ermilova distribution for the delta-ray spectrum
  Float_t poti = TMath::Log(kPoti);
  Float_t eEnd = TMath::Log(kEend);

  // Ermilova distribution for the delta-ray spectrum
//  fDeltaE = new TF1("deltae" ,Ermilova ,poti,eEnd,0);

  // Geant3 distribution for the delta-ray spectrum
//  fDeltaG = new TF1("deltag",IntSpecGeant,2.421257,28.536469,0);

  IlcDebug(1,"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

  if (IlcDebugLevelClass()>=1){
    oldtrkWS=-1;
    oldevWS=-1;
    WSDataBlk = new TClonesArray("IlcDCHHitWireStat");
    fHitWiresData =  new TTree("HitWireData","HitWireData");
    fHitWiresData->Branch("HitWireData","TClonesArray",&WSDataBlk,60000,2);
    fHitWires = new TFile("HitWireStat.root","recreate");
    tmpIdSL = new TArrayI();
    tmpIdRing = new TArrayI();
    tmpIdAbsRing = new TArrayI();
    tmpIsSense = new TArrayS();
  }
}

//_____________________________________________________________________________
/* void IlcDCHv1::SetSensPlane(Int_t iplane)
{
  //
  // Defines the hit-sensitive plane
  //

  if ((iplane < 0) || (iplane > IlcDCHgeometry::Nplan()-1)) {
    IlcWarning(Form("Wrong input value:%d",iplane));
    IlcWarning("Use standard setting");
    fSensPlane  = -1;
    fSensSelect =  0;
    return;
  }

  fSensSelect = 1;
  fSensPlane  = iplane;

}

//_____________________________________________________________________________
void IlcDCHv1::SetSensChamber(Int_t ichamber)
{
  //
  // Defines the hit-sensitive chamber
  //

  if ((ichamber < 0) || (ichamber > IlcDCHgeometry::Ncham())) {
    IlcWarning(Form("Wrong input value: %d",ichamber));
    IlcWarning("Use standard setting");
    fSensChamber = -1;
    fSensSelect  =  0;
    return;
  }

  fSensSelect  = 1;
  fSensChamber = ichamber;

}
//_____________________________________________________________________________
void IlcDCHv1::SetSensSector(Int_t isector)
{
  //
  // Defines the hit-sensitive sector 
  //

  SetSensSector(isector,1);

}

//_____________________________________________________________________________
void IlcDCHv1::SetSensSector(Int_t isector, Int_t nsector)
{
  //
  // Defines a range of hit-sensitive sectors. The range is defined by
  // <isector> (0-17) as the starting point and <nsector> as the number 
  // of sectors to be included.
  //

  if ((isector < 0) || (isector > IlcDCHgeometry::Nsect()-1)) {
    IlcWarning(Form("Wrong input value <isector>: %d",isector));
    IlcWarning("Use standard setting");
    fSensSector      = -1;
    fSensSectorRange =  0;
    fSensSelect      =  0;
    return;
  }

  if ((nsector < 1) || (nsector > IlcDCHgeometry::Nsect())) {
    IlcWarning(Form("Wrong input value <nsector>: %d",nsector));
    IlcWarning("Use standard setting");
    fSensSector      = -1;
    fSensSectorRange =  0;
    fSensSelect      =  0;
    return;
  }

  fSensSelect      = 1;
  fSensSector      = isector;
  fSensSectorRange = nsector;

}
 */

//_____________________________________________________________________________
void IlcDCHv1::StepManager()
{
  //
  // Slow simulator. Every charged track produces electron cluster as hits
  // along its path across the drift volume. 
  //

  switch (fTypeOfStepManager) {
    case 0  : StepManagerErmilova();  break;  // 0 is Ermilova
    case 1  : StepManagerGeant();     break;  // 1 is Geant
    case 2  : StepManagerFixedStep(); break;  // 2 is fixed step
    default : IlcWarning("Not a valid Step Manager.");
  }

}

//_____________________________________________________________________________
void IlcDCHv1::SelectStepManager(Int_t t)
{
  //
  // Selects a step manager type:
  //   0 - Ermilova
  //   1 - Geant3
  //   2 - Fixed step size
  //

/*  if (t == 1) {
    IlcWarning("Sorry, Geant parametrization step manager is not implemented yet. Please ask K.Oyama for detail.");
  }
*/
  fTypeOfStepManager = t;
  IlcInfo(Form("Step Manager type %d was selected",fTypeOfStepManager));

}

//_____________________________________________________________________________
void IlcDCHv1::StepManagerGeant()
{
  //
  // Slow simulator. Every charged track produces electron cluster as hits
  // along its path across the drift volume. The step size is set acording
  // to Bethe-Bloch. The energy distribution of the delta electrons follows
  // a spectrum taken from Geant3.
  //
  Int_t    pla = 0;
  Int_t    cha = 0;
  Int_t    sec = 0;
  ULong_t  det = 0;
  Int_t    iPdg;
  UShort_t qTot;

  Int_t    superlayer = 0;
  Int_t    ring2=0,ring = 0;
  Int_t    wire2=0,wire = 0;
  UShort_t trkStat=0;
  Int_t    num_wire;
  Int_t    up_down=0;

  Float_t  hits[3];
  Float_t  charge;
  Float_t  aMass;

  Float_t  pos_local[3];

  Double_t pTot = 0;
  Double_t eDelta;
  Double_t betaGamma, pp;
  Double_t stepSize=0;

  Bool_t   drRegion = kFALSE;
  TString  cIdCurrent,cIdMoth;
  Char_t   cIdChamber[1];
  TString  NodePath=gMC->CurrentVolPath();

  TString  cIdvoltube="Cl";
  TString  nsuperlayer;
  TString  nring;
  TString  nwire;
  TString  nup_down;


  TLorentzVector pos, mom;

//  const Int_t    kNsec       = IlcDCHgeometry::Nsect();

  const Double_t kBig         = 1.0E+12; // Infinitely big
  const Float_t  kWion        = 23.53;   // Ionization energy
  const Float_t  kPTotMaxEl   = 0.002;   // Maximum momentum for e+ e- g
  const Float_t  kEkinMinStep = 1.0e-5;  // Minimum energy for the step size adjustment
  const Float_t  kECut = 1.0e4;    // energy threshold for production of delta electrons
  const float ra=5.37E-4, rb=0.9815, rc=3.123E-3; // Parameters entering the parametrized range for delta electrons
  const float rho=0.004945 ; //// Gas density -> To be made user adjustable ! [0.85*0.00549+0.15*0.00186 (Xe-CO2 85-15)]

  // Plateau value of the energy-loss for electron in xenon
  // taken from: Allison + Comb, Ann. Rev. Nucl. Sci. (1980), 30, 253
  //const Double_t kPlateau = 1.70;
  // the averaged value (26/3/99)
  const Float_t  kPlateau     = 1.55;

  const Float_t  kPrim        = 19.34;  // dN1/dx|min for the gas mixture (90% Xe + 10% CO2)
  // First ionization potential (eV) for the gas mixture (90% Xe + 10% CO2)
  const Float_t  kPoti        = 12.1;

  const Int_t    kPdgElectron = 11;  // PDG code electron

  // Set the maximum step size to a lastaddedhitvery large number for all
  // neutral particles and those outside the driftvolume
  gMC->SetMaxStep(kBig);

  // Use only charged tracks
  if (( gMC->TrackCharge()       ) &&
      (!gMC->IsTrackStop()       ) &&
      (!gMC->IsTrackDisappeared())) {


    // Inside a sensitive volume?
    cIdCurrent = gMC->CurrentVolName();
    if(cIdCurrent.Contains(cIdvoltube.Data())) drRegion=kTRUE;

    if (drRegion) {
      Int_t inode=0;
      //gMC->CurrentVolOffID(3,inode);
      //cIdMoth=gMC->CurrentVolOffName(2);


      // The hit coordinates and charge
      gMC->TrackPosition(pos);
      hits[0] = pos[0];
      hits[1] = pos[1];
      hits[2] = pos[2];

      //Cl0S01R03W343
      nup_down=cIdCurrent(2,1);
      nsuperlayer=cIdCurrent(4,2);
      nring=cIdCurrent(7,2);
      ///ILCC_1/SuperLayer_01_03_1/Cl1S01R03_4
      nwire=NodePath(37,3);
   
//      cout<<NodePath<<endl;
  
      up_down=atoi(nup_down.Data());
      superlayer=atoi(nsuperlayer.Data());
      ring=atoi(nring.Data());
      wire=atoi(nwire.Data())-1;
  

      if ((ring==0) && (up_down==1)) return;
      if ((ring==10) && (up_down==0)) return;
      //cout <<"VolumeDCH: "<<NodePath.Data()<<endl;
      //num_wire=(80+(superlayer-1)*20)*2;

      if (up_down==1){  //to couple down and up trapezium in a drift cell 
        ring--;
        if (ring%2 == 1)  {
          wire++;
          if (wire == (80 + 20*superlayer)) wire = 0; 
        }
      }

      det=((superlayer+1)*100000)+(ring*1000)+(wire);
     
      if(exiting && det!=fExitingPrev_det ){
        AddTrackReference(oldtrk);
        lastaddedhit->SetTrkStat(2);
        exiting = kFALSE;
      }
      // The detector number
      // Special hits only in the drift region
      // Create a track reference at the entrance and
      // exit of each chamber that contain the
            // momentum components of the particle

      if (gMC->IsTrackEntering() && det!=fPrev_det) {
        gMC->TrackMomentum(mom);
        AddTrackReference(gIlc->GetMCApp()->GetCurrentTrackNumber());
        trkStat = 1;  
        if (!gMC->IsNewTrack()) {
          // determine if hit belong to primary track 
          fPrimaryTrackPid=gIlc->GetMCApp()->GetCurrentTrackNumber();
          //determine track length when entering the detector
          fTrackLength0=gMC->TrackLength();
        }

      }

      if (gMC->IsTrackExiting()) {
        fExitingPrev_det = det;
        gMC->TrackMomentum(mom);
        oldtrk=gIlc->GetMCApp()->GetCurrentTrackNumber();
        exiting = kTRUE;
      }

/*
         if (gMC->IsTrackEntering() || gMC->IsTrackExiting()) {
          gMC->TrackMomentum(mom);
          AddTrackReference(gIlc->GetMCApp()->GetCurrentTrackNumber());
        }
        if (gMC->IsTrackEntering()) trkStat = 1;
        if (gMC->IsTrackExiting()) trkStat = 2;
        		      
        if (gMC->IsTrackEntering() && !gMC->IsNewTrack()) {
          // determine if hit belong to primary track 
          fPrimaryTrackPid=gIlc->GetMCApp()->GetCurrentTrackNumber();
          //determine track length when entering the detector
          fTrackLength0=gMC->TrackLength();
        }
 */

 // Calculate the energy of the delta-electrons
 // modified by Alex Bercuci (A.Bercuci@gsi.de) on 26.01.06
 // take into account correlation with the underlying GEANT tracking
 // mechanism. see
 // http://www-linux.gsi.de/~abercuci/Contributions/DCH/index.html
 // determine the most significant process (last on the processes list)
 // which caused this hit
      TArrayI processes;
      gMC->StepProcesses(processes);
      int nofprocesses=processes.GetSize(), pid;
      if(!nofprocesses) pid=0;
      else pid=       processes[nofprocesses-1];	      
 // generate Edep according to GEANT parametrisation
//      eDelta =TMath::Exp(fDeltaG->GetRandom()) - kPoti;
eDelta=0.0;
      eDelta=TMath::Max(eDelta,0.0);
      float pr_range=0.;
      float range=gMC->TrackLength()-fTrackLength0;
      // merge GEANT tracker information with localy cooked one
      if(gIlc->GetMCApp()->GetCurrentTrackNumber()==fPrimaryTrackPid) {
//    printf("primary pid=%d eDelta=%f\n",pid,eDelta);
      if(pid==27){ 
      if(eDelta>=kECut){		
      pr_range=ra*eDelta*.001*(1.-rb/(1.+rc*eDelta*0.001))/rho;
      if(pr_range>=(3.7-range)) eDelta*=.1;
              }
      } else if(pid==1){
      if(eDelta<kECut) eDelta*=.5;
      else {		    
      pr_range=ra*eDelta*.001*(1.-rb/(1.+rc*eDelta*0.001))/rho;
//       if(pr_range>=((IlcDCHgeometry::DrThick()
//          + IlcDCHgeometry::AmThick())-range)) eDelta*=.05;
//       else eDelta*=.5;
      eDelta*=.5;  //modiica temporanea poiche' non esistonopiu' i meotdi IlcDCHgeometry::DrThick() IlcDCHgeometry::AmThick()
      }
      } else eDelta=0.;       
      } else eDelta=0.;

      // Generate the electron cluster size
      if(eDelta==0.) qTot=0;
      else qTot = ((UShort_t) (eDelta / kWion) + 1);

      // Create a new dEdx hit
      lastaddedhit = AddHit(gIlc->GetMCApp()->GetCurrentTrackNumber(),(UShort_t) superlayer,(UShort_t) ring,(UShort_t) wire, det, trkStat, hits, qTot);
      fPrev_det = det;
      
      //AddHit(gIlc->GetMCApp()->GetCurrentTrackNumber(),det,hits,qTot, drRegion);
        		      
      // Calculate the maximum step size for the next tracking step
      // Produce only one hit if Ekin is below cutoff
      aMass = gMC->TrackMass();
      if ((gMC->Etot() - aMass) > kEkinMinStep) {

        // The energy loss according to Bethe Bloch
        iPdg  = TMath::Abs(gMC->TrackPid());
        if ( (iPdg != kPdgElectron) ||
        		      ((iPdg == kPdgElectron) && (pTot < kPTotMaxEl))) {
          gMC->TrackMomentum(mom);
          pTot      = mom.Rho();
          betaGamma = pTot / aMass;
          pp	    = BetheBlochGeant(betaGamma);
        	      // Take charge > 1 into account
          charge = gMC->TrackCharge();
          if (TMath::Abs(charge) > 1) pp = pp * charge*charge;
        } else { // Electrons above 20 Mev/c are at the plateau
        				      pp = kPrim * kPlateau;
        }

        Int_t nsteps = 0;
        do {nsteps = gRandom->Poisson(pp);} while(!nsteps);
        stepSize = 1./nsteps;
        gMC->SetMaxStep(stepSize);
      }

    }
    
    if(exiting){
      AddTrackReference(oldtrk);
      lastaddedhit->SetTrkStat(2);
      exiting = kFALSE;
    }
  }




}

//_____________________________________________________________________________
void IlcDCHv1::StepManagerErmilova()
{
/*   //
  // Slow simulator. Every charged track produces electron cluster as hits 
  // along its path across the drift volume. The step size is set acording
  // to Bethe-Bloch. The energy distribution of the delta electrons follows
  // a spectrum taken from Ermilova et al.
  //

  Int_t    pla = 0;
  Int_t    cha = 0;
  Int_t    sec = 0;
  Int_t    det = 0;
  Int_t    iPdg;
  Int_t    qTot;

  Float_t  hits[3];
  Double_t random[1];
  Float_t  charge;
  Float_t  aMass;

  Double_t pTot = 0;
  Double_t eDelta;
  Double_t betaGamma, pp;
  Double_t stepSize;

  Bool_t   drRegion = kFALSE;

  TString  cIdCurrent,cIdMoth;
  TString  cIdvoltube="DTR1";
  Char_t   cIdChamber[1];
 

  TLorentzVector pos, mom;

  const Int_t    kNsec      = IlcDCHgeometry::Nsect();

  const Double_t kBig         = 1.0E+12; // Infinitely big
  const Float_t  kWion        = 23.53;   // Ionization energy
  const Float_t  kPTotMaxEl   = 0.002;   // Maximum momentum for e+ e- g 

  // energy threshold for production of delta electrons
  //const Float_t  kECut = 1.0e4;
  // Parameters entering the parametrized range for delta electrons
  //const float ra=5.37E-4, rb=0.9815, rc=3.123E-3;
  // Gas density -> To be made user adjustable !
  //const float rho=0.004945 ; //[0.85*0.00549+0.15*0.00186 (Xe-CO2 85-15)]

  // Minimum energy for the step sizlastaddedhite adjustment
  const Float_t  kEkinMinStep = 1.0e-5;

  // Plateau value of the energy-loss for electron in xenon
  // taken from: Allison + Comb, Ann. Rev. Nucl. Sci. (1980), 30, 253
  //const Double_t kPlateau = 1.70;
  // the averaged value (26/3/99)
  const Float_t  kPlateau     = 1.55;

  const Float_t  kPrim        = 48.0;  // dN1/dx|min for the gas mixture (90% Xe + 10% CO2)
  // First ionization potential (eV) for the gas mixture (90% Xe + 10% CO2)
  const Float_t  kPoti        = 12.1;

  const Int_t    kPdgElectron = 11;  // PDG code electron

  // Set the maximum step size to a very large number for all 
  // neutral particles and those outside the driftvolume
  gMC->SetMaxStep(kBig); 

  // Use only charged tracks 
  if (( gMC->TrackCharge()       ) &&
      (!gMC->IsTrackStop()       ) && 
      (!gMC->IsTrackDisappeared())) {

    // Inside a sensitive volume?
    drRegion = kFALSE;
   
    cIdCurrent = gMC->CurrentVolName();
    if (cIdvoltube == cIdCurrent) drRegion = kTRUE;
    if (drRegion ) {
       Int_t inode=0;
      gMC->CurrentVolOffID(3,inode);
      cIdMoth=gMC->CurrentVolOffName(2);

      // The hit coordinates and charge
      gMC->TrackPosition(pos);
      hits[0] = pos[0];
      hits[1] = pos[1];
      hits[2] = pos[2];

      // The sector number 
      // The numbering goes clockwise and starts at y = 0
      Float_t phi = kRaddeg*TMath::ATan2(pos[0],pos[1]);
      if (phi < 90.) 
        phi = phi + 270.;
      else
        phi = phi -  90.;
         sec = ((Int_t) (phi / 2/fDCHGeometry->GetAlpha()));
	if((phi-sec*fDCHGeometry->GetAlpha())>fDCHGeometry->GetAlpha1())sec=2*sec+1;
      else sec=2*sec;
      // The plane and chamber number
      cIdChamber[0] = cIdMoth[3];
      pla= (atoi(cIdChamber));
      cha =int((inode-1)/kNsec);

    
      // Check on selected volumes
      Int_t addthishit = 1;
      if (fSensSelect) {
        if ((fSensPlane   >= 0) && (pla != fSensPlane  )) addthishit = 0;
        if ((fSensChamber >= 0) && (cha != fSensChamber)) addthishit = 0;
        if (fSensSector  >= 0) {
          Int_t sens1  = fSensSector;
          Int_t sens2  = fSensSector + fSensSectorRange;
                sens2 -= ((Int_t) (sens2 / IlcDCHgeometry::Nsect())) 
                       * IlcDCHgeometry::Nsect();
          if (sens1 < sens2) {
            if ((sec < sens1) || (sec >= sens2)) addthishit = 0;
	  			}
          else {
            if ((sec < sens1) && (sec >= sens2)) addthishit = 0;
	  			}
				}
      }

      // Add this hit
      if (addthishit) {

	// The detector number
        det = fDCHGeometry->GetDetector(pla,cha,sec);

	// Special hits only in the drift region
        if (drRegion) {

          // Create a track reference at the entrance and
          // exit of each chamber that contain the 
	  // momentum components of the particle
          if (gMC->IsTrackEntering() || gMC->IsTrackExiting()) {
            gMC->TrackMomentum(mom);
            AddTrackReference(gIlc->GetMCApp()->GetCurrentTrackNumber());
          }
 				}


        // Calculate the energy of the delta-electrons
        eDelta = TMath::Exp(fDeltaE->GetRandom()) - kPoti;
        eDelta = TMath::Max(eDelta,0.0);
        // Generate the electron cluster size
        if(eDelta==0.) qTot=0;
				else qTot = ((Int_t) (eDelta / kWion) + 1);

				// Create a new dEdx hit
        if (drRegion) {
          //AddHit(gIlc->GetMCApp()->GetCurrentTrackNumber()
            //    ,det,hits,qTot, kTRUE);
				}
        else {
          //AddHit(gIlc->GetMCApp()->GetCurrentTrackNumber()
            //    ,det,hits,qTot,kFALSE);
				}

        // Calculate the maximum step size for the next tracking step
	// Produce only one hit if Ekin is below cutoff 
        aMass = gMC->TrackMass();
        if ((gMC->Etot() - aMass) > kEkinMinStep) {

          // The energy loss according to Bethe Bloch
          iPdg  = TMath::Abs(gMC->TrackPid());
          if ( (iPdg != kPdgElectron) ||
	      			((iPdg == kPdgElectron) && (pTot < kPTotMaxEl))) {
            gMC->TrackMomentum(mom);
            pTot      = mom.Rho();
            betaGamma = pTot / aMass;
            pp        = kPrim * BetheBloch(betaGamma);
	    // Take charge > 1 into account
            charge = gMC->TrackCharge();
            if (TMath::Abs(charge) > 1) pp = pp * charge*charge;
          } else { // Electrons above 20 Mev/c are at the plateau
						pp = kPrim * kPlateau;
          }
      
          if (pp > 0) {
            do 
            gMC->GetRandom()->RndmArray(1, random);
            while ((random[0] == 1.) || (random[0] == 0.));
            stepSize = - TMath::Log(random[0]) / pp; 
            gMC->SetMaxStep(stepSize);
	  			}
				}
      }
    }
  }
 */
}

//_____________________________________________________________________________
void IlcDCHv1::StepManagerFixedStep()
{

//STEMANAGER FOR GEOMETRY GEANT4
  // Slow simulator. Every charged track produces electron cluster as hits 
  // along its path across the drift volume. The step size is fixed in
  // this version of the step manager.
  //
  //cout <<"entra in questo step manager"<<endl;
  const Double_t kBig         = 1.0E+12;
  const Float_t  kWion        = 22.8;   // Ionization energy
  const Float_t  kEkinMinStep = 1.0e-5;  // Minimum energy for the step size adjustment
  const Float_t  kPlateau     = 1.55;
  const Float_t  kprim = 13.;
  const Float_t  kpoti =12.; 

  ULong_t  det = 0;

  // Set the maximum step size to a very large number for all 
  // neutral particles and those outside the driftvolume
  gMC->SetMaxStep(kBig); 

  // If not charged track or already stopped or disappeared, just return.
  if ((!gMC->TrackCharge()) || 
        gMC->IsTrackStop()  || 
        gMC->IsTrackDisappeared()) {
	  oldtrk=-1;
	  return;
	}

  // Inside a sensitive volume?
  TString  cIdCurrent = gMC->CurrentVolName();
  //  cout<< gMC->CurrentVolName()<<endl;

  currenttrk=gIlc->GetMCApp()->GetCurrentTrackNumber();

  currentev=gIlc->GetEventNrInRun();//gIlc->GetEvNumber();
  //gIlc->GetEvNumber() gIlc->GetEventNrInRun()
  
  Int_t tmpDim;
  
  if (IlcDebugLevelClass()>=1 && currentev<1000){
    if ( (cIdCurrent.Contains("tubeS") || cIdCurrent.Contains("tubeF")) && gMC->IsTrackExiting() ) {
      if (currentev!=oldevWS) {
   	
   	  if (oldtrkWS!=-1) {
	    tmpDim=WSDataBlk->GetEntries();
   	    WSDataBlk->New(tmpDim);
   	    IlcDCHHitWireStat *tmpBuffWS = (IlcDCHHitWireStat *) WSDataBlk->At(tmpDim);
   	    tmpBuffWS->SetTrckId(oldtrkWS);
   	    tmpBuffWS->SetNev(oldevWS);
   	    tmpBuffWS->SetNHits(tmpNHits);
   	    tmpBuffWS->SetIdSL(tmpIdSL);
   	    tmpBuffWS->SetIdRing(tmpIdRing);
   	    tmpBuffWS->SetIdAbsRing(tmpIdAbsRing);
   	    tmpBuffWS->SetIsSense(tmpIsSense);
   	  }

	oldtrkWS=-1;
	oldevWS=currentev;
      }
//      else {
   	if (currenttrk==oldtrkWS) {
   	  tmpLast=tmpNHits;
   	  tmpNHits++;
   	  tmpIdSL->Set(tmpNHits);
   	  tmpIdRing->Set(tmpNHits);
    	  tmpIdAbsRing->Set(tmpNHits);
  	  tmpIsSense->Set(tmpNHits);
   	  tmpFist_=cIdCurrent.First('_');
   	  tmpFist_++;
   	  tmpLast_=cIdCurrent.Last('_');
   	  tmpIdSL->AddAt( atoi(cIdCurrent(tmpFist_,tmpLast_-tmpFist_).Data()), tmpLast );
   	  tmpLast_++;
   	  tmpIdRing->AddAt( atoi(cIdCurrent(tmpLast_,10).Data()), tmpLast );
   	  tmpIdAbsRing->AddAt( tmpIdSL->At(tmpLast)*fDCHParam->GetRingNum() + tmpIdRing->At(tmpLast), tmpLast );
   	  tmpIsSense->AddAt( cIdCurrent.Contains("S"), tmpLast );
   	}
   	else {
   	
   	  if (oldtrkWS!=-1) {
	    tmpDim=WSDataBlk->GetEntries();
   	    WSDataBlk->New(tmpDim);
   	    IlcDCHHitWireStat *tmpBuffWS = (IlcDCHHitWireStat *) WSDataBlk->At(tmpDim);

   	    tmpBuffWS->SetTrckId(oldtrkWS);
   	    tmpBuffWS->SetNev(oldevWS);
   	    tmpBuffWS->SetNHits(tmpNHits);
   	    tmpBuffWS->SetIdSL(tmpIdSL);
   	    tmpBuffWS->SetIdRing(tmpIdRing);
   	    tmpBuffWS->SetIdAbsRing(tmpIdAbsRing);
   	    tmpBuffWS->SetIsSense(tmpIsSense);
   	  }

   	  tmpNHits=1;
   	  tmpLast=0;
	  if (tmpIdSL) delete tmpIdSL;
	  if (tmpIdRing) delete tmpIdRing;
	  if (tmpIdAbsRing) delete tmpIdAbsRing;
	  if (tmpIsSense) delete tmpIsSense;
	  tmpIdSL = new TArrayI();
	  tmpIdRing = new TArrayI();
	  tmpIdAbsRing = new TArrayI();
	  tmpIsSense = new TArrayS();
   	  tmpIdSL->Set(tmpNHits);
   	  tmpIdRing->Set(tmpNHits);
   	  tmpIdAbsRing->Set(tmpNHits);
   	  tmpIsSense->Set(tmpNHits);
   	  tmpFist_=cIdCurrent.First('_');
   	  tmpFist_++;
   	  tmpLast_=cIdCurrent.Last('_');
   	  tmpIdSL->AddAt( atoi(cIdCurrent(tmpFist_,tmpLast_-tmpFist_).Data()), tmpLast );
   	  tmpLast_++;
   	  tmpIdRing->AddAt( atoi(cIdCurrent(tmpLast_,10).Data()), tmpLast );
   	  tmpIdAbsRing->AddAt(  tmpIdSL->At(tmpLast)*fDCHParam->GetRingNum() + tmpIdRing->At(tmpLast), tmpLast );
   	  tmpIsSense->AddAt( cIdCurrent.Contains("S"), tmpLast );
   	  oldtrkWS=currenttrk;
   	}
//      }
    }
  }

  if (!cIdCurrent.Contains("gvolS")&&!cIdCurrent.Contains("wvolS")){
    if(exiting){
//       AddTrackReference(oldtrk);
      if(cIdCurrent.Contains("wvolS"))  lastaddedhit->SetTrkStat(4);
      else   lastaddedhit->SetTrkStat(2);
      exiting = kFALSE;
    }
    if (!cIdCurrent.Contains("tube")) oldtrk=-1;
    return;
  }

  TLorentzVector pos, mom;
  // The hit coordinates and charge
  gMC->TrackPosition(pos);
  Float_t hits[4]={pos[0],pos[1],pos[2],gMC->TrackTime()};
  Double_t hit[4]={pos[0],pos[1],pos[2],gMC->TrackTime()};

  //cout<< gMC->CurrentVolName()<<endl;

  int superlayer=atoi(cIdCurrent(5,2).Data());
  int ring=atoi(cIdCurrent(8,2).Data());
  //cout<<" SUP "<<superlayer<<endl;

  if(ring==0){
//     if(exiting){
//     //  AddTrackReference(oldtrk);
//       if(cIdCurrent.Contains("wvolS"))  lastaddedhit->SetTrkStat(4);
//       else    lastaddedhit->SetTrkStat(2);
//       exiting = kFALSE;
//     }

    return;
  }

  Double_t phihit=TMath::ATan2(pos[1],pos[0]);
 

  Int_t nwires=fDCHParam->GetSWireNum()+superlayer*fDCHParam->GetSDeltaWireNum();

  if(superlayer>=10 && superlayer<18) nwires=fDCHParam->GetSWireNum()+(superlayer-4)*fDCHParam->GetSDeltaWireNum();
  if(superlayer>=18 && superlayer<24) nwires=fDCHParam->GetSWireNum()+(superlayer-6)*fDCHParam->GetSDeltaWireNum();

 //cout<<" Superlayer "<<superlayer<<" "<<nwires<<endl;

  Double_t Dphi=2*TMath::Pi()/nwires;

  int ring1=ring-2;
  int ring2=ring-1;

  Float_t xywire[3];
  double phiwire1=0,phiwire0=0;
  double phirel1=0,phirel2=0,radius1=0,radius2=0;
  int wire1=-1,wire2=-1;
  double invcosstereo;

  if(ring1>=0){
    fDCHwire->SelectWire(superlayer,ring1,1);
    invcosstereo=1./TMath::Cos(fDCHwire->GetWireEpsilon());
    fDCHwire->WirePosAtZ(pos[2]*invcosstereo,xywire);
    phiwire1=TMath::ATan2(xywire[1],xywire[0]);
    fDCHwire->SelectWire(superlayer,ring1,0);
    fDCHwire->WirePosAtZ(pos[2]*invcosstereo,xywire);
    phiwire0=TMath::ATan2(xywire[1],xywire[0]);
    phirel1=phihit-phiwire0;
    if(phiwire1<phiwire0) phirel1*=-1.;

    if(phirel1<-0.5*Dphi) phirel1+=TMath::TwoPi();
    if(phirel1>=TMath::TwoPi()-0.5*Dphi) phirel1-=TMath::TwoPi();
    
    wire1=int((phirel1+0.5*Dphi)/Dphi);
    if(wire1<0||wire1>=nwires)
      IlcError(Form("Wrong wire number %i",wire1));

    phirel1=phirel1-wire1*Dphi;
    if(phiwire1<phiwire0) phirel1*=-1.;
 
    radius1=TMath::Hypot(xywire[0],xywire[1]);
  }

  if(ring2<fDCHParam->GetRingNum()-1){
    fDCHwire->SelectWire(superlayer,ring2,1);
    invcosstereo=1./TMath::Cos(fDCHwire->GetWireEpsilon());
    fDCHwire->WirePosAtZ(pos[2]*invcosstereo,xywire);
    phiwire1=TMath::ATan2(xywire[1],xywire[0]);
    fDCHwire->SelectWire(superlayer,ring2,0);
    fDCHwire->WirePosAtZ(pos[2]*invcosstereo,xywire);
    phiwire0=TMath::ATan2(xywire[1],xywire[0]);
    
    phirel2=phihit-phiwire0;
    if(phiwire1<phiwire0) phirel2*=-1.;
    
    if(phirel2<-0.5*Dphi) phirel2+=TMath::TwoPi();
    if(phirel2>=TMath::TwoPi()-0.5*Dphi) phirel2-=TMath::TwoPi();
    
    wire2=int((phirel2+0.5*Dphi)/Dphi);
    if(wire2<0||wire2>=nwires)
      IlcError(Form("Wrong wire number %i",wire2));
   
    phirel2=phirel2-wire2*Dphi;
    if(phiwire1<phiwire0) phirel2*=-1.;

    radius2=TMath::Hypot(xywire[0],xywire[1]);
  }

  double dist1,dist2;
  if(wire1>=0){
    fDCHwire->SelectWire(superlayer,ring1,wire1);
    dist1=fDCHwire->DistFromWireCenter(hit);
  }else{
    radius1=radius2-radius2*TMath::Sin(Dphi/3./2.)*2.*TMath::Sqrt(3.)/2.;
    double phiw=phiwire0+wire2*(phiwire1-phiwire0)+TMath::Sign(0.5*Dphi,phirel2);
    xywire[0]=TMath::Cos(phiw)*radius1;
    xywire[1]=TMath::Sin(phiw)*radius1;
    dist1=TMath::Hypot(hit[0]-xywire[0],hit[1]-xywire[1]);
  }

  if(wire2>=0){
    fDCHwire->SelectWire(superlayer,ring2,wire2);
    dist2=fDCHwire->DistFromWireCenter(hit);
  }else{
    radius2=radius1+radius1*TMath::Sin(Dphi/3./2.)*2.*TMath::Sqrt(3.)/2.;
    double phiw=phiwire0+wire1*(phiwire1-phiwire0)+TMath::Sign(0.5*Dphi,phirel1);
    xywire[0]=TMath::Cos(phiw)*radius2;
    xywire[1]=TMath::Sin(phiw)*radius2;
    dist2=TMath::Hypot(hit[0]-xywire[0],hit[1]-xywire[1]);
  }

  int wire;
  if(dist1<=dist2){
    ring=ring1;
    wire=wire1;
  }else{
    ring=ring2;
    wire=wire2;
  }

      
  //gMC->SetMaxStep(0.004);

  //Set Maximum Step Size
 //  // Produce only one hit if Ekin is below cutoff
 //  if ((gMC->Etot() - gMC->TrackMass()) < kEkinMinStep) return;


//-----------------------------------------------------------------
    //  charged particle is in the sensitive volume
    //-----------------------------------------------------------------

  // Stemax calculation for the next step
  
  Float_t pp;
  Float_t charge = gMC->TrackCharge();
  gMC->TrackMomentum(mom);
  Float_t ptot=mom.Rho();
  Float_t betaGamma = ptot/gMC->TrackMass();  
  Int_t pid=gMC->TrackPid();
  if((pid==11|| pid==-11) && ptot > 0.002)
    { 
      pp = kprim*kPlateau; // electrons above 20 MeV/c are on the plateau!
    }
  else
    {
      //cout<<" betaGamma "<<betaGamma <<endl;
      betaGamma =TMath::Max(betaGamma,(Float_t)7.e-3); // protection against too small bg
      //cout<<" betaGamma "<<betaGamma <<endl;
      pp=kprim*BetheBloch(betaGamma); 
      //cout<<" BetheBloch(betaGamma) "<<BetheBloch(betaGamma)<<endl;
      if(TMath::Abs(charge) > 1.) pp *= (charge*charge);
    }

  Double_t rnd = gMC->GetRandom()->Rndm();
  
   gMC->SetMaxStep(-TMath::Log(rnd)/pp);


  if(wire<0){
//     if(exiting){
//       AddTrackReference(oldtrk);
//       if(cIdCurrent.Contains("wvolS")) lastaddedhit->SetTrkStat(4);
//       else  lastaddedhit->SetTrkStat(2);
//       exiting = kFALSE;
//     }
     return;
  }


  det=((superlayer+1)*100000)+((ring)*1000)+(wire);
  UShort_t trkStat=0;    
    
  int tracknumber=gIlc->GetMCApp()->GetCurrentTrackNumber();

  if (det!=fPrev_det) {
//     if (oldtrk!=-1) AddTrackReference(oldtrk);
    gMC->TrackMomentum(mom);
//     AddTrackReference(gIlc->GetMCApp()->GetCurrentTrackNumber());
    if(cIdCurrent.Contains("wvolS")) trkStat = 3;
    else  trkStat = 1;
  } 
/*  
  if(exiting && det!=fExitingPrev_det ){
    AddTrackReference(oldtrk);
    if(cIdCurrent.Contains("wvolS")) trkStat = 4;
    else  lastaddedhit->SetTrkStat(2);
    exiting = kFALSE;
    
  }
  
  if (gMC->IsTrackEntering() && (det!=fPrev_det)) {
    gMC->TrackMomentum(mom);
    AddTrackReference(gIlc->GetMCApp()->GetCurrentTrackNumber());
    if(cIdCurrent.Contains("wvolS")) trkStat = 3;
    else  trkStat = 1;
  } 
*/  
  if (gMC->IsTrackExiting()) {
    fExitingPrev_det = det;
    exiting = kTRUE;
  }

  // Calculate the charge according to GEANT Edep
  // Create a new dEdx hit

  Double_t eDep = TMath::Max(gMC->Edep(),0.0) * 1.0e+09;
  Int_t qTot = (Int_t) (eDep / kWion); // Nro elettroni primari

  lastaddedhit=AddHit(gIlc->GetMCApp()->GetCurrentTrackNumber()
			    ,(UShort_t) superlayer,(UShort_t) ring,(UShort_t) wire,
			    det,trkStat,hits,qTot);
  
  gMC->TrackMomentum(mom);
  oldtrk=gIlc->GetMCApp()->GetCurrentTrackNumber();
  fPrev_det = det;




}


//_____________________________________________________________________________
Double_t IlcDCHv1::BetheBloch(Double_t bg) 
{
  //
  // Parametrization of the Bethe-Bloch-curve
  // The parametrization is the same as for the TPC and is taken from Lehrhaus.
  //

  // This parameters have been adjusted to averaged values from GEANT
  // const Double_t kP1 = 17.17960e-02;
//   const Double_t kP2 = 8.4196;
//   const Double_t kP3 = 1.38065e-06;
//   const Double_t kP4 = 5.30972;
//   const Double_t kP5 = 2.83798;





  const Double_t kP1 = 7.17960e-02;
   const Double_t kP2 = 8.54196;
   const Double_t kP3 = 1.38065e-06;
   const Double_t kP4 = 5.30972;
   const Double_t kP5 = 2.83798;

  // This parameters have been adjusted to Xe-data found in:
  // Allison & Cobb, Ann. Rev. Nucl. Sci. (1980), 30, 253
 //  const Double_t kP1 = 0.76176E-1;
//   const Double_t kP2 = 10.632;
//   const Double_t kP3 = 3.17983E-6;
//   const Double_t kP4 = 1.8631;
//   const Double_t kP5 = 1.9479;




  // Lower cutoff of the Bethe-Bloch-curve to limit step sizes
  const Double_t kBgMin = 0.8;
  const Double_t kBBMax = 6.83298;
  //const Double_t kBgMin = 0.6;
  //const Double_t kBBMax = 17.2809;
  //const Double_t kBgMin = 0.4;
  //const Double_t kBBMax = 82.0;
 
  if (bg > kBgMin) {
    Double_t yy = bg / TMath::Sqrt(1. + bg*bg);
    Double_t aa = TMath::Power(yy,kP4);
    Double_t bb = TMath::Power((1./bg),kP5);
             bb = TMath::Log(kP3 + bb);
    return ((kP2 - aa - bb)*kP1 / aa);
   }
   else {
    
     return kBBMax;
   }

}

//_____________________________________________________________________________
Double_t IlcDCHv1::BetheBlochGeant(Double_t bg)
{
  //
  // Return dN/dx (number of primary collisions per centimeter)
  // for given beta*gamma factor.
  //
  // Implemented by K.Oyama according to GEANT 3 parametrization shown in
  // A.Andronic's webpage: http://www-alice.gsi.de/trd/papers/dedx/dedx.html
  // This must be used as a set with IntSpecGeant.
  //

  Double_t arr_g[20] = {
    1.100000,   1.200000,    1.300000,    1.500000,
    1.800000,   2.000000,    2.500000,    3.000000,
    4.000000,   7.000000,    10.000000,   20.000000,
    40.000000,  70.000000,   100.000000,  300.000000,
    600.000000, 1000.000000, 3000.000000, 10000.000000 };

  Double_t arr_nc[20] = {
    75.009056,   45.508083,   35.299252,   27.116327,
    22.734999,   21.411915,   19.934095,   19.449375,
    19.344431,   20.185553,   21.027925,   22.912676,
    24.933352,   26.504053,   27.387468,   29.566597,
    30.353779,   30.787134,   31.129285,   31.157350 };

  // betagamma to gamma
  Double_t g = TMath::Sqrt( 1. + bg*bg );

  // Find the index just before the point we need.
  int i;
  for( i = 0 ; i < 18 ; i++ )
    if( arr_g[i] < g && arr_g[i+1] > g )
      break;

  // Simple interpolation.
  Double_t pp = ((arr_nc[i+1] - arr_nc[i]) / 
		 (arr_g[i+1]-arr_g[i])) * (g-arr_g[i]) + arr_nc[i];

  return pp; //arr_nc[8];

}

//_____________________________________________________________________________
void IlcDCHv1::Stepping()
{    
// Stepping info
// ---

   cout << "X(cm)    "
       << "Y(cm)    "
       << "Z(cm)  "
       << "KinE(MeV)   "
       << "dE(MeV) "
       << "Step(cm) "
       << "TrackL(cm) "
       << "Volume  "
       << "Process "  
       << endl;

   // Position
    //
    Double_t x, y, z;
    gMC->TrackPosition(x, y, z);
    cout << setw(8) << setprecision(3) << x << " "
         << setw(8) << setprecision(3) << y << " "
         << setw(8) << setprecision(3) << z << "  ";

    // Kinetic energy
    //
    Double_t px, py, pz, etot;
    gMC->TrackMomentum(px, py, pz, etot);
    Double_t ekin = etot - gMC->TrackMass();
    cout << setw(9) << setprecision(4) << ekin*1e03 << " ";

    // Energy deposit
    //
    cout << setw(9) << setprecision(4) << gMC->Edep()*1e03 << " ";

    // Step length
    //
    cout << setw(8) << setprecision(3) << gMC->TrackStep() << " ";

    // Track length
    //
    cout << setw(8) << setprecision(3) << gMC->TrackLength() << "     ";

    // Volume
    //
    if (gMC->CurrentVolName() != 0)
      cout << setw(4) << gMC->CurrentVolName() << "  ";
    else
      cout << setw(4) << "None"  << "  ";

    // Process
    //
    TArrayI processes;
    Int_t nofProcesses = gMC->StepProcesses(processes);
    for(int ip=0;ip<nofProcesses; ip++)
      cout << TMCProcessName[processes[ip]]<<" / ";

    cout << endl;
}


//_____________________________________________________________________________
/* Double_t Ermilova(Double_t *x, Double_t *)
{
  //
  // Calculates the delta-ray energy distribution according to Ermilova.
  // Logarithmic scale !
  //

  Double_t energy;
  Double_t dpos;
  Double_t dnde;

  Int_t    pos1, pos2;

  const Int_t kNv = 31;

  Float_t vxe[kNv] = { 2.3026, 2.9957, 3.4012, 3.6889, 3.9120  
                     , 4.0943, 4.2485, 4.3820, 4.4998, 4.6052
                     , 4.7005, 5.0752, 5.2983, 5.7038, 5.9915
                     , 6.2146, 6.5221, 6.9078, 7.3132, 7.6009
                     , 8.0064, 8.5172, 8.6995, 8.9872, 9.2103
                     , 9.4727, 9.9035,10.3735,10.5966,10.8198
                     ,11.5129 };

  Float_t vye[kNv] = { 80.0  , 31.0  , 23.3  , 21.1  , 21.0
                     , 20.9  , 20.8  , 20.0  , 16.0  , 11.0
                     ,  8.0  ,  6.0  ,  5.2  ,  4.6  ,  4.0
                     ,  3.5  ,  3.0  ,  1.4  ,  0.67 ,  0.44
                     ,  0.3  ,  0.18 ,  0.12 ,  0.08 ,  0.056
                     ,  0.04 ,  0.023,  0.015,  0.011,  0.01
		     ,  0.004 };

  energy = x[0];

  // Find the position 
  pos1 = pos2 = 0;
  dpos = 0;
  do {
    dpos = energy - vxe[pos2++];
  } 
  while (dpos > 0);
  pos2--; 
  if (pos2 > kNv) pos2 = kNv - 1;
  pos1 = pos2 - 1;

  // Differentiate between the sampling points
  dnde = (vye[pos1] - vye[pos2]) / (vxe[pos2] - vxe[pos1]);

  return dnde;

}

//_____________________________________________________________________________
Double_t IntSpecGeant(Double_t *x, Double_t *)
{
  //
  // Integrated spectrum from Geant3
  //

  const Int_t npts = 83;
  Double_t arre[npts] = {
    2.421257,  2.483278,  2.534301,  2.592230,
    2.672067,  2.813299,  3.015059,  3.216819,
    3.418579,  3.620338,  3.868209,  3.920198,
    3.978284,  4.063923,  4.186264,  4.308605,
    4.430946,  4.553288,  4.724261,  4.837736,
    4.999842,  5.161949,  5.324056,  5.486163,
    5.679688,  5.752998,  5.857728,  5.962457,
    6.067185,  6.171914,  6.315653,  6.393674,
    6.471694,  6.539689,  6.597658,  6.655627,
    6.710957,  6.763648,  6.816338,  6.876198,
    6.943227,  7.010257,  7.106285,  7.252151,
    7.460531,  7.668911,  7.877290,  8.085670,
    8.302979,  8.353585,  8.413120,  8.483500,
    8.541030,  8.592857,  8.668865,  8.820485,
    9.037086,  9.253686,  9.470286,  9.686887,
    9.930838,  9.994655, 10.085822, 10.176990,
    10.268158, 10.359325, 10.503614, 10.627565,
    10.804637, 10.981709, 11.158781, 11.335854,
    11.593397, 11.781165, 12.049404, 12.317644,
    12.585884, 12.854123, 14.278421, 16.975889,
    20.829416, 24.682943, 28.536469
  };

//   Double_t arrdndx[npts] = {
//     19.344431, 18.664679, 18.136106, 17.567745,
//     16.836426, 15.677382, 14.281277, 13.140237,
//     12.207677, 11.445510, 10.697049, 10.562296,
//     10.414673, 10.182341,  9.775256,  9.172330,
//     8.240271,  6.898587,  4.808303,  3.889751,
//     3.345288,  3.093431,  2.897347,  2.692470,
//     2.436222,  2.340029,  2.208579,  2.086489,
//     1.975535,  1.876519,  1.759626,  1.705024,
//     1.656374,  1.502638,  1.330566,  1.200697,
//     1.101168,  1.019323,  0.943867,  0.851951,
//     0.755229,  0.671576,  0.570675,  0.449672,
//     0.326722,  0.244225,  0.188225,  0.149608,
//     0.121529,  0.116289,  0.110636,  0.103490,
//     0.096147,  0.089191,  0.079780,  0.063927,
//     0.047642,  0.036341,  0.028250,  0.022285,
//     0.017291,  0.016211,  0.014802,  0.013533,
//     0.012388,  0.011352,  0.009803,  0.008537,
//     0.007039,  0.005829,  0.004843,  0.004034,
//     0.003101,  0.002564,  0.001956,  0.001494,
//     0.001142,  0.000873,  0.000210,  0.000014,
//     0.000000,  0.000000,  0.000000
//   };

  // Differentiate
  //  dnde = (arrdndx[i-1] - arrdndx[i]) / (arre[i] - arre[i-1]);

  Double_t arrdnde[npts] = {
    10.960000, 10.960000, 10.359500,  9.811340,
    9.1601500,  8.206670,  6.919630,  5.655430,
    4.6221300,  3.777610,  3.019560,  2.591950,
    2.5414600,  2.712920,  3.327460,  4.928240,
    7.6185300, 10.966700, 12.225800,  8.094750,
    3.3586900,  1.553650,  1.209600,  1.263840,
    1.3241100,  1.312140,  1.255130,  1.165770,
    1.0594500,  0.945450,  0.813231,  0.699837,
    0.6235580,  2.260990,  2.968350,  2.240320,
    1.7988300,  1.553300,  1.432070,  1.535520,
    1.4429900,  1.247990,  1.050750,  0.829549,
    0.5900280,  0.395897,  0.268741,  0.185320,
    0.1292120,  0.103545,  0.0949525, 0.101535,
    0.1276380,  0.134216,  0.123816,  0.104557,
    0.0751843,  0.0521745, 0.0373546, 0.0275391,
    0.0204713,  0.0169234, 0.0154552, 0.0139194,
    0.0125592,  0.0113638, 0.0107354, 0.0102137,
    0.00845984, 0.00683338, 0.00556836, 0.00456874,
    0.0036227,  0.00285991, 0.00226664, 0.00172234,
    0.00131226, 0.00100284, 0.000465492, 7.26607e-05,
    3.63304e-06, 0.0000000, 0.0000000
  };

  Int_t i;
  Double_t energy = x[0];

  for( i = 0 ; i < npts ; i++ )
    if( energy < arre[i] ) break;

  if( i == 0 )
    IlcErrorGeneral("IlcDCHv1","Given energy value is too small or zero");

   return arrdnde[i];

}
 */
