#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TPDGCode.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TVirtualMC.h>
#include <TGeant3.h>
#include "STEER/IlcRunLoader.h"
#include "STEER/IlcRun.h"
#include "STEER/IlcConfig.h"
#include "PYTHIA6/IlcDecayerPythia.h"
#include "EVGEN/IlcGenCocktail.h"
#include "EVGEN/IlcGenFixed.h"
#include "EVGEN/IlcGenBox.h"
#include "EVGEN/IlcGenScan.h"
#include "EVGEN/IlcGenHIJINGpara.h"
#include "THijing/IlcGenHijing.h"
#include "PYTHIA6/IlcGenPythia.h"
#include "THerwig/IlcGenHerwig.h"
#include "TIsajet/IlcGenIsajet.h"
#include "TDPMjet/IlcGenDPMjet.h"
#include "EVGEN/IlcGenParam.h"
#include "EVGEN/IlcGenMUONlib.h"
#include "EVGEN/IlcGenPHOSlib.h"
#include "EVGEN/IlcGenGSIlib.h"
#include "EVGEN/IlcGenFLUKAsource.h"
#include "EVGEN/IlcGenExtFile.h"
#include "EVGEN/IlcGenHalo.h"
#include "EVGEN/IlcGenReaderTreeK.h"
#include "STRUCT/IlcBODY.h"
#include "STRUCT/IlcMAG.h"

#endif

enum gentype_t {hijing, hijingParam, gun, box, pythia, herwig, 
		param1, param2, param3, param4, 
		cocktail, fluka, halo, ntuple, scan, isajet, dpmjet};

gentype_t gentype = dpmjet;

Int_t ntracks=1;

void Config()
{

  // Set Random Number seed
  gRandom->SetSeed(12345); //Set 0 to use the current time
  cout<<"Seed for random number generation= "<<gRandom->GetSeed()<<endl; 


  // libraries required by geant321
#if defined(__CINT__)
  gSystem->Load("libgeant321");
#endif
gSystem->Load("libherwig.so");
gSystem->Load("libTHerwig.so");
gSystem->Load("libisajet.so");
gSystem->Load("libTIsajet.so");
gSystem->Load("libdpmjet.so");
gSystem->Load("libTDPMjet.so");

  new TGeant3TGeo("C++ Interface to Geant3");

//=======================================================================
//  Create the output file
   
  IlcRunLoader* rl=0x0;

  cout<<"Config.C: Creating Run Loader ..."<<endl;
  rl = IlcRunLoader::Open("gilc.root",
			  IlcConfig::GetDefaultEventFolderName(),
			  "recreate");
  if (rl == 0x0)
    {
      gIlc->Fatal("Config.C","Can not instatiate the Run Loader");
      return;
    }
  rl->SetCompressionLevel(2);
  rl->SetNumberOfEventsPerFile(100);
  gIlc->SetRunLoader(rl);

//
// Set External decayer
  IlcDecayer* decayer = new IlcDecayerPythia();
  decayer->SetForceDecay(kAll);
  decayer->Init();
  gMC->SetExternalDecayer(decayer);


//
//=======================================================================
// ************* STEERING parameters FOR ILC SIMULATION **************
// --- Specify event type to be tracked through the ILC setup
// --- All positions are in cm, angles in degrees, and P and E in GeV


  gMC->SetProcess("DCAY",1);
  gMC->SetProcess("PAIR",1);
  gMC->SetProcess("COMP",1);
  gMC->SetProcess("PHOT",1);
  gMC->SetProcess("PFIS",0);
  gMC->SetProcess("DRAY",0);
  gMC->SetProcess("ANNI",1);
  gMC->SetProcess("BREM",1);
  gMC->SetProcess("MUNU",1);
  gMC->SetProcess("CKOV",1);
  gMC->SetProcess("HADR",1);
  gMC->SetProcess("LOSS",2);
  gMC->SetProcess("MULS",1);
  gMC->SetProcess("RAYL",1);
  
  Float_t cut = 1.e-3;        // 1MeV cut by default
  Float_t tofmax = 1.e10;
  
  gMC->SetCut("CUTGAM", cut);
  gMC->SetCut("CUTELE", cut);
  gMC->SetCut("CUTNEU", cut);
  gMC->SetCut("CUTHAD", cut);
  gMC->SetCut("CUTMUO", cut);
  gMC->SetCut("BCUTE",  cut); 
  gMC->SetCut("BCUTM",  cut); 
  gMC->SetCut("DCUTE",  cut); 
  gMC->SetCut("DCUTM",  cut); 
  gMC->SetCut("PPCUTM", cut);
  gMC->SetCut("TOFMAX", tofmax); 
  

  IlcGenerator * gGener = 0x0;
  switch(gentype)
    {
    case gun:
//*********************************************
// Example for Fixed Particle Gun             
//*********************************************
      {
	IlcGenFixed *gener = new IlcGenFixed(ntracks);
	gener->SetMomentum(50);
	gener->SetPhi(180.);
	gener->SetTheta(5.);
	gener->SetOrigin(0,0,0);        //vertex position
	gener->SetPart(kNeutron);
	gGener = gener;
      }
      break;
    case box:  
//*********************************************
// Example for Moving Particle Gun            *
//*********************************************
      {
	IlcGenBox *gener = new IlcGenBox(ntracks);
	gener->SetMomentumRange(3,4);
	gener->SetPhiRange(0,360);
	gener->SetThetaRange(90, 180. );
	gener->SetOrigin(0,0,0);   
	//vertex position
	gener->SetSigma(0,0,0);         //Sigma in (X,Y,Z) (cm) on IP position
	gener->SetPart(kMuonPlus);
	gGener = gener;
      }
      break;
    case scan:  
//*********************************************
// Scanning on a grid                         *
//*********************************************
      {
	IlcGenScan *gener = new IlcGenScan(-1);
	gener->SetMomentumRange(4,4);
	gener->SetPhiRange(0,360);
	gener->SetThetaRange(9,9);
	//vertex position
	gener->SetSigma(6,6,0);         //Sigma in (X,Y,Z) (cm) on IP position
	gener->SetPart(kMuonPlus); 
	gener->SetRange(20, -100, 100, 20, -100, 100, 1, 500, 500);
	gGener = gener;
      }
      break;
     
    case hijingParam:
      {
	IlcGenHIJINGpara *gener = new IlcGenHIJINGpara(ntracks);
	gener->SetMomentumRange(0,999);
	gener->SetPhiRange(0,360);
	gener->SetThetaRange(2,10);
	gener->SetOrigin(0,0,0);        //vertex position
	gener->SetSigma(0,0,0);         //Sigma in (X,Y,Z) (cm) on IP position
	gGener = gener;
      }
      break;
    case hijing:
      {
	IlcGenHijing *gener = new IlcGenHijing(-1);
// centre of mass energy 
	gener->SetEnergyCMS(5500);
// reference frame
	gener->SetReferenceFrame("CMS     ");
// projectile
	gener->SetProjectile("A       ", 208, 82);
	gener->SetTarget    ("A       ", 208, 82);
// impact parameter range
	gener->SetImpactParameterRange(0, 3.);
// evaluate cross section before run
	gener->SetEvaluate(0);
// tell hijing to keep the full parent child chain
	gener->KeepFullEvent();
// enable jet quenching
	gener->SetJetQuenching(1);
// enable shadowing
	gener->SetShadowing(1);
// neutral pion and heavy particle decays switched off
	gener->SetDecaysOff(1);
// trigger
	gener->SetTrigger(0);
// kinematic selection
	gener->SetSelectAll(0);
// momentum range
	gener->SetMomentumRange(0,999);
// phi range
	gener->SetPhiRange(0.,360.);
// theta range 
	gener->SetThetaRange(0,180.);
// select flavor (0: no, 4: charm+beauty, 5:beauty)
	gener->SetFlavor(0);
//     
	gener->SetOrigin(0., 0.0 ,0);
	gener->SetSigma(0,0,5.3);
	gener->SetVertexSmear(kPerEvent); 
// no tracking
	gener->SetTrackingFlag(0);
	gGener = gener;
      }
      break;
     
    case pythia:
//********************************************
// Example for Charm  Production with Pythia *
//********************************************
      {
	IlcGenPythia *gener = new IlcGenPythia(-1);
//   final state kinematic cuts
	gener->SetMomentumRange(0,999);
	gener->SetPhiRange(0. ,360.);
	gener->SetThetaRange(0., 180.);
	gener->SetYRange(-10,10);
	gener->SetPtRange(0,100);
//   vertex position and smearing 
	gener->SetOrigin(0,0,0);       // vertex position
	gener->SetVertexSmear(kPerEvent);
	gener->SetSigma(0,0,5.6);      // Sigma in (X,Y,Z) (cm) on IP position
//   Structure function. See the list in EVGEN/IlcStructFuncType.h
	gener->SetStrucFunc(kGRVHO);
// Select corection for nuclear structure functions
//     gener->SetNuclei(208,208);
//
//   Process type. See the list in PYTHIA6/IlcPythia.h
	gener->SetProcess(kPyBeauty);
//   
//   Pt transfer of the hard scattering
	gener->SetPtHard(0.,5.);
//   Decay type (semielectronic, semimuonic, nodecay)
	gener->SetForceDecay(kSemiElectronic);
//   Centre of mass energy 
	gener->SetEnergyCMS(5500.);
//   No Tracking 
	gener->SetTrackingFlag(0);
	gGener = gener;
      }
      break;              

    case herwig:
//********************************************
// Example for Charm  Production with Pythia *
//********************************************
      {
	IlcGenHerwig *gener = new IlcGenHerwig(-1);
//   final state kinematic cuts
	gener->SetMomentumRange(0,7000);
	gener->SetPhiRange(0. ,360.);
	gener->SetThetaRange(0., 180.);
	gener->SetYRange(-10,10);
	gener->SetPtRange(0,7000);
//   vertex position and smearing 
	gener->SetOrigin(0,0,0);       // vertex position
	gener->SetVertexSmear(kPerEvent);
	gener->SetSigma(0,0,5.6);      // Sigma in (X,Y,Z) (cm) on IP position
//   Beam momenta
	gener->SetBeamMomenta(7000,7000);
//   Beams
	gener->SetProjectile("P");
	gener->SetTarget("P");
//   Structure function
	gener->SetStrucFunc(kGRVHO);
//   Hard scatering
	gener->SetPtHardMin(200);
	gener->SetPtRMS(20);
//   Min bias
	gener->SetProcess(8000);
//   No Tracking 
	gener->SetTrackingFlag(0);
	gGener = gener;
      }
      break;              
    case isajet:
	IlcGenIsajet *gener = new IlcGenIsajet(-1);
        gGener = gener;
      break;
   case dpmjet:
	IlcGenDPMjet *gener = new IlcGenDPMjet(-1);
	//kDpmSingleDiffr, kDpmDoubleDiffr, kDpmDiffr, kDpmNonDiffr
		gener->SetProcess(kDpmMb);
	gGener = gener;
      break;



    case param1:
//*******************************************************
// Example for J/psi  Production from  Parameterisation 
// using default library (IlcMUONlib)                                       
//*******************************************************
      {
	IlcGenParam *gener =
	  new IlcGenParam(ntracks, IlcGenMUONlib::kUpsilon);
	gener->SetMomentumRange(0,999);
	gener->SetPtRange(0,999);     
	gener->SetPhiRange(0. , 360.);
	gener->SetYRange(2.5,4);
	gener->SetCutOnChild(1);
	gener->SetChildThetaRange(2,9);
	gener->SetOrigin(0,0,0);        //vertex position
	gener->SetSigma(0,0,5.3);       //Sigma in (X,Y,Z) (cm) on IP position
	gener->SetForceDecay(kDiMuon);
	gener->SetTrackingFlag(0);
	gGener = gener;
      }
      break;

    case param2:
//*******************************************************
// Example for Omega  Production from  Parameterisation 
// specifying library.                                       
//*******************************************************
      {
	IlcGenParam *gener = new IlcGenParam(1000,new IlcGenPHOSlib(), 
					     IlcGenPHOSlib::kOmega);
	gener->SetWeighting(kNonAnalog);
	gener->SetForceDecay(kNoDecay);
	gener->SetPtRange(0,100);
	gener->SetThetaRange(45,135);
	gener->SetTrackingFlag(0);
	gGener = gener;
      }
      break;

    case param3:
//*******************************************************
// Example for Upsilon  Production from  Parameterisation 
// specifying library.                                       
// GSI style
//*******************************************************
      {
	IlcGenParam *gener = new IlcGenParam(1000,new IlcGenGSIlib(), 
					     IlcGenGSIlib::kUpsilon, "MUON");
	gener->SetMomentumRange(0,999);
	gener->SetPtRange(0,999);     
	gener->SetPhiRange(0., 360.);
	gener->SetYRange(2.5,4);
	gener->SetCutOnChild(1);
	gener->SetChildThetaRange(2,9);
	gener->SetOrigin(0,0,0);        //vertex position
	gener->SetSigma(0,0,5.3);       //Sigma in (X,Y,Z) (cm) on IP position
	gener->SetForceDecay(kDiMuon);
	gener->SetTrackingFlag(0);
	gGener = gener;
      }
      break;
     
    case param4:
//*******************************************************
// Example for Omega  Production from  Parameterisation 
// specifying library.
// The alternative way.                                       
//*******************************************************
      {
	IlcGenLib* Lib=new IlcGenPHOSlib();
	Int_t iOmega = IlcGenPHOSlib::kOmega;
	IlcGenParam *gener = new IlcGenParam(50, iOmega,            
					     Lib->GetPt(iOmega, ""),
					     Lib->GetY (iOmega, ""),
					     Lib->GetIp(iOmega, ""));
	gener->SetPtRange(0,999);     
	gener->SetWeighting(kNonAnalog);
	gener->SetForceDecay(kNoDecay);
	gener->SetTrackingFlag(0);
	gGener = gener;
      }
      break;
      
    case fluka:
//*******************************************************
// Example for a FLUKA Boundary Source                  *
//*******************************************************
      {
	IlcGenFLUKAsource *gener = new IlcGenFLUKAsource(-1);
	gener->SetFileName("$(ILC_ROOT)/data/all32.root"); 
	gener->SetPartFlag(9);
	gener->SetAgeMax(1.e-5);
//  31.7 events     
	gener->SetFraction(0.0315);     
//     gener->SetFraction(0.75*0.0315);     
	rl->CdGAFile();
//     gener->SetPartFlag(10);
	gener->SetMomentumRange(0,999);
	gener->SetPhiRange(0.,360.);
	gener->SetThetaRange(0., 180.); 
	gener->SetAgeMax(1.e-5);
     
//  31.7 events     
//     gener->SetFraction(0.0315);     
	gGener = gener;
      }
      break;

    case ntuple:
//*******************************************************
// Example for reading from a external file                  *
//*******************************************************
      {
	IlcGenExtFile *gener = new IlcGenExtFile(-1); 
	gener->SetVertexSmear(kPerEvent); 
	gener->SetTrackingFlag(1);
	
	IlcGenReaderTreeK * reader = new IlcGenReaderTreeK();
	reader->SetFileName("$(ILC_ROOT)/data/dtujet93.root");
	gener->SetReader(reader);
	gGener = gener;
      }
      break;

    case halo:
//*******************************************************
// Example for Tunnel Halo Source                       *
//*******************************************************
      {
	IlcGenHalo *gener = new IlcGenHalo(ntracks); 
	gener->SetFileName("/h1/morsch/marsip/marsip5.mu");
	gGener = gener;
      }
      break;
      
    case cocktail:
//*******************************************************
// Example for a Cocktail                               *
//*******************************************************
      {
	IlcGenCocktail *gener = new IlcGenCocktail(); 

	gener->SetPhiRange(0,360);
	gener->SetYRange(2.5,4);
	gener->SetThetaRange(2,9);
	gener->SetPtRange(0,10);
	gener->SetOrigin(0,0,0);        //vertex position
	gener->SetSigma(0,0,0);         //Sigma in (X,Y,Z) (cm) on IP position
	gener->SetMomentumRange(0,999);

	IlcGenParam *jpsi = new IlcGenParam(1,IlcGenMUONlib::kJpsi);
	jpsi->SetForceDecay(kDiMuon);
	jpsi->SetCutOnChild(1);

     
	IlcGenFLUKAsource *bg = new IlcGenFLUKAsource(-1);
	bg->AddFile("$(ILC_ROOT)/data/all32.root"); 
	rl->CdGAFile();
	bg->SetPartFlag(9);
	bg->SetAgeMax(1.e-5);
//  31.7 events     
//     gener->SetFraction(0.0315);     
	bg->SetFraction(0.01*0.0315);     
      
	gener->AddGenerator(jpsi,"J/Psi", 1);
	gener->AddGenerator(bg,"Background",1);

	gGener = gener;
      }
      break;
    }
 
// Activate this line if you want the vertex smearing to happen
// track by track
//
// gener->SetVertexSmear(kPerTrack); 

  gGener->Init();

  gIlc->SetField(-999,2);    //Specify maximum magnetic field in Tesla (neg. ==> default field)

  Int_t iMAG=1;
  rl->CdGAFile();

//=================== Ilc BODY parameters =============================
  IlcBODY *BODY = new IlcBODY("BODY","Ilc envelop");


  if(iMAG) {
//=================== MAG parameters ============================
// --- Start with Magnet since detector layouts may be depending ---
// --- on the selected Magnet dimensions ---
    IlcMAG *MAG  = new IlcMAG("MAG","Magnet");
  }
}
