//
// Configuration for the first physics production 2008
//

// One can use the configuration macro in compiled mode by
// root [0] gSystem->Load("libgeant321");
// root [0] gSystem->SetIncludePath("-I$ROOTSYS/include -I$ILC_ROOT/include\
//                   -I$ILC_ROOT -I$ILC/geant3/TGeant3");
// root [0] .x grun.C(1,"Config.C++")

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TRandom.h>
#include <TDatime.h>
#include <TSystem.h>
#include <TVirtualMC.h>
#include <TGeant3TGeo.h>
#include "STEER/IlcRunLoader.h"
#include "STEER/IlcRun.h"
#include "STEER/IlcConfig.h"
#include "PYTHIA6/IlcDecayerPythia.h"
#include "PYTHIA6/IlcGenPythia.h"
#include "TDPMjet/IlcGenDPMjet.h"
#include "STEER/IlcMagF.h"
#include "STRUCT/IlcBODY.h"
#include "STRUCT/IlcMAG.h"
#include "STRUCT/IlcABSOv3.h"
#include "STRUCT/IlcDIPOv3.h"
#include "STRUCT/IlcHALLv3.h"
#include "STRUCT/IlcFRAMEv2.h"
#include "STRUCT/IlcSHILv3.h"
#include "STRUCT/IlcPIPEv3.h"
#include "ITS/IlcITSv11.h"
#include "TPC/IlcTPCv2.h"
#include "TOF/IlcTOFv6T0.h"
#include "HMPID/IlcHMPIDv3.h"
#include "ZDC/IlcZDCv4.h"
#include "TRD/IlcTRDv1.h"
#include "TRD/IlcTRDgeometry.h"
#include "FMD/IlcFMDv1.h"
#include "MUON/IlcMUONv1.h"
#include "PHOS/IlcPHOSv1.h"
#include "PHOS/IlcPHOSSimParam.h"
#include "PMD/IlcPMDv1.h"
#include "T0/IlcT0v1.h"
#include "EMCAL/IlcEMCALv2.h"
#include "ACORDE/IlcACORDEv1.h"
#include "VZERO/IlcVZEROv7.h"
#endif


enum PDC06Proc_t 
{
  kPythia6, kPhojet, kRunMax
};

const char * pprRunName[] = {
  "kPythia6", "kPhojet"
};

enum Mag_t
{
  kNoField, k5kG, kFieldMax
};

const char * pprField[] = {
  "kNoField", "k5kG"
};

//--- Functions ---
class IlcGenPythia;
IlcGenerator *MbPythia();
IlcGenerator *MbPhojet();
void ProcessEnvironmentVars();

// Geterator, field, beam energy
static PDC06Proc_t   proc     = kPhojet;
static Mag_t         mag      = k5kG;
static Float_t       energy   = 10000; // energy in CMS
//========================//
// Set Random Number seed //
//========================//
TDatime dt;
static UInt_t seed    = dt.Get();

// Comment line
static TString comment;

void Config()
{
    

  // Get settings from environment variables
  ProcessEnvironmentVars();

  gRandom->SetSeed(seed);
  cerr<<"Seed for random number generation= "<<seed<<endl; 

  // Libraries required by geant321
#if defined(__CINT__)
  gSystem->Load("liblhapdf");      // Parton density functions
  gSystem->Load("libEGPythia6");   // TGenerator interface
  gSystem->Load("libpythia6");     // Pythia
  gSystem->Load("libIlcPythia6");  // ILC specific implementations
  gSystem->Load("libgeant321");
#endif

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
  rl->SetNumberOfEventsPerFile(1000);
  gIlc->SetRunLoader(rl);
  // gIlc->SetGeometryFromFile("geometry.root");
  // gIlc->SetGeometryFromCDB();
  
  // Set the trigger configuration: proton-proton
  IlcSimulation::Instance()->SetTriggerConfig("p-p");

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




  //======================//
  // Set External decayer //
  //======================//
  TVirtualMCDecayer* decayer = new IlcDecayerPythia();
  decayer->SetForceDecay(kAll);
  decayer->Init();
  gMC->SetExternalDecayer(decayer);

  //=========================//
  // Generator Configuration //
  //=========================//
  // Create pileup generator
  IlcGenPileup *pileup = new IlcGenPileup();

  IlcGenerator* gener = 0x0;
  
  if (proc == kPythia6) {
      gener = MbPythia();
  } else if (proc == kPhojet) {
      gener = MbPhojet();
  }
  
  // Set the pileup interaction generator
  // The second argument is the pileup rate
  // in terms of event rate per bunch crossing
  pileup->SetGenerator(gener,0.01);
  // Set the beam time structure
  // Details on the syntax in STEER/IlcTriggerBCMask
  pileup->SetBCMask("72(1H1L)3420L");
  // Examples of the pileup rate and beam structure settings
  // Most of the information is taken from the LHC commissionning page
  // rate from 0.01 (at 900GeV) to 0.76 (at 14TeV)
  // 1 bunch/orbit     - bc-mask = "1H3563L"
  // 43 bunches/orbit  - bc-mask = "43(1H80L)81L"
  // 72 bunches/orbit  - bc-mask = "72(1H1L)3420L" (50ns mode)
  // Please note that most of these setting should be cross-checked because
  // for example the 43 bunches mode is taken at CMS IP and not the ILC one.

  // Generate the trigger interaction
  pileup->GenerateTrigInteraction(kTRUE);

  // PRIMARY VERTEX
  //
  pileup->SetOrigin(0., 0., 0.);    // vertex position
  //
  //
  // Size of the interaction diamond
  // Longitudinal
  Float_t sigmaz  = 5.4 / TMath::Sqrt(2.); // [cm]
  if (energy == 900)
    sigmaz  = 10.5 / TMath::Sqrt(2.); // [cm]
  //
  // Transverse
  Float_t betast  = 10;                 // beta* [m]
  Float_t eps     = 3.75e-6;            // emittance [m]
  Float_t gamma   = energy / 2.0 / 0.938272;  // relativistic gamma [1]
  Float_t sigmaxy = TMath::Sqrt(eps * betast / gamma) / TMath::Sqrt(2.) * 100.;  // [cm]
  printf("\n \n Diamond size x-y: %10.3e z: %10.3e\n \n", sigmaxy, sigmaz);
    
  pileup->SetSigma(sigmaxy, sigmaxy, sigmaz);      // Sigma in (X,Y,Z) (cm) on IP position
  pileup->SetCutVertexZ(3.);        // Truncate at 3 sigma
  pileup->SetVertexSmear(kPerEvent);

  pileup->Init();

  // FIELD
  //
  IlcMagF* field = 0x0;
  if (mag == kNoField) {
    comment = comment.Append(" | L3 field 0.0 T");
    field = new IlcMagF("Maps","Maps", 0., 0., IlcMagF::k5kGUniform);
  } else if (mag == k5kG) {
    comment = comment.Append(" | L3 field 0.5 T");
    field = new IlcMagF("Maps","Maps", -1., -1., IlcMagF::k5kG);
  }
  printf("\n \n Comment: %s \n \n", comment.Data());
  TGeoGlobalMagField::Instance()->SetField(field);
    
  rl->CdGAFile();

  Int_t iABSO  = 1;
  Int_t iACORDE= 0;
  Int_t iDIPO  = 1;
  Int_t iEMCAL = 0;
  Int_t iFMD   = 1;
  Int_t iFRAME = 1;
  Int_t iHALL  = 1;
  Int_t iITS   = 1;
  Int_t iMAG   = 1;
  Int_t iMUON  = 1;
  Int_t iPHOS  = 1;
  Int_t iPIPE  = 1;
  Int_t iPMD   = 0;
  Int_t iHMPID = 1;
  Int_t iSHIL  = 1;
  Int_t iT0    = 1;
  Int_t iTOF   = 1;
  Int_t iTPC   = 1;
  Int_t iTRD   = 1;
  Int_t iVZERO = 1;
  Int_t iZDC   = 1;
  

    //=================== Ilc BODY parameters =============================
    IlcBODY *BODY = new IlcBODY("BODY", "Ilc envelop");


    if (iMAG)
    {
        //=================== MAG parameters ============================
        // --- Start with Magnet since detector layouts may be depending ---
        // --- on the selected Magnet dimensions ---
        IlcMAG *MAG = new IlcMAG("MAG", "Magnet");
    }


    if (iABSO)
    {
        //=================== ABSO parameters ============================
        IlcABSO *ABSO = new IlcABSOv3("ABSO", "Muon Absorber");
    }

    if (iDIPO)
    {
        //=================== DIPO parameters ============================

        IlcDIPO *DIPO = new IlcDIPOv3("DIPO", "Dipole version 3");
    }

    if (iHALL)
    {
        //=================== HALL parameters ============================

        IlcHALL *HALL = new IlcHALLv3("HALL", "Ilc Hall");
    }


    if (iFRAME)
    {
        //=================== FRAME parameters ============================

        IlcFRAMEv2 *FRAME = new IlcFRAMEv2("FRAME", "Space Frame");
	FRAME->SetHoles(1);
    }

    if (iSHIL)
    {
        //=================== SHIL parameters ============================

        IlcSHIL *SHIL = new IlcSHILv3("SHIL", "Shielding Version 3");
    }


    if (iPIPE)
    {
        //=================== PIPE parameters ============================

        IlcPIPE *PIPE = new IlcPIPEv3("PIPE", "Beam Pipe");
    }
 
    if (iITS)
    {
        //=================== ITS parameters ============================

	IlcITS *ITS  = new IlcITSv11("ITS","ITS v11");
    }

    if (iTPC)
    {
      //============================ TPC parameters =====================

        IlcTPC *TPC = new IlcTPCv2("TPC", "Default");
    }


    if (iTOF) {
        //=================== TOF parameters ============================

	IlcTOF *TOF = new IlcTOFv6T0("TOF", "normal TOF");
    }


    if (iHMPID)
    {
        //=================== HMPID parameters ===========================

        IlcHMPID *HMPID = new IlcHMPIDv3("HMPID", "normal HMPID");

    }


    if (iZDC)
    {
        //=================== ZDC parameters ============================

        IlcZDC *ZDC = new IlcZDCv4("ZDC", "normal ZDC");
    }

    if (iTRD)
    {
        //=================== TRD parameters ============================

        IlcTRD *TRD = new IlcTRDv1("TRD", "TRD slow simulator");
        IlcTRDgeometry *geoTRD = TRD->GetGeometry();
	// Partial geometry: modules at 0,1,7,8,9,10,17
	// starting at 3h in positive direction
	geoTRD->SetSMstatus(2,0);
	geoTRD->SetSMstatus(3,0);
	geoTRD->SetSMstatus(4,0);
        geoTRD->SetSMstatus(5,0);
	geoTRD->SetSMstatus(6,0);
        geoTRD->SetSMstatus(11,0);
        geoTRD->SetSMstatus(12,0);
        geoTRD->SetSMstatus(13,0);
        geoTRD->SetSMstatus(14,0);
        geoTRD->SetSMstatus(15,0);
        geoTRD->SetSMstatus(16,0);
    }

    if (iFMD)
    {
        //=================== FMD parameters ============================

	IlcFMD *FMD = new IlcFMDv1("FMD", "normal FMD");
   }

    if (iMUON)
    {
        //=================== MUON parameters ===========================
        // New MUONv1 version (geometry defined via builders)

        IlcMUON *MUON = new IlcMUONv1("MUON", "default");
    }

    if (iPHOS)
    {
        //=================== PHOS parameters ===========================

        IlcPHOS *PHOS = new IlcPHOSv1("PHOS", "noCPV_Modules123");
    }


    if (iPMD)
    {
        //=================== PMD parameters ============================

        IlcPMD *PMD = new IlcPMDv1("PMD", "normal PMD");
    }

    if (iT0)
    {
        //=================== T0 parameters ============================
        IlcT0 *T0 = new IlcT0v1("T0", "T0 Detector");
    }

    if (iEMCAL)
    {
        //=================== EMCAL parameters ============================

        IlcEMCAL *EMCAL = new IlcEMCALv2("EMCAL", "EMCAL_FIRSTYEAR");
    }

     if (iACORDE)
    {
        //=================== ACORDE parameters ============================

        IlcACORDE *ACORDE = new IlcACORDEv1("ACORDE", "normal ACORDE");
    }

     if (iVZERO)
    {
        //=================== ACORDE parameters ============================

        IlcVZERO *VZERO = new IlcVZEROv7("VZERO", "normal VZERO");
    }
}
//
//           PYTHIA
//

IlcGenerator* MbPythia()
{
      comment = comment.Append(" pp at 14 TeV: Pythia low-pt");
//
//    Pythia
      IlcGenPythia* pythia = new IlcGenPythia(-1); 
      pythia->SetMomentumRange(0, 999999.);
      pythia->SetThetaRange(0., 180.);
      pythia->SetYRange(-12.,12.);
      pythia->SetPtRange(0,1000.);
      pythia->SetProcess(kPyMb);
      pythia->SetEnergyCMS(energy);
      
      return pythia;
}

IlcGenerator* MbPhojet()
{
      comment = comment.Append(" pp at 14 TeV: Pythia low-pt");
//
//    DPMJET
#if defined(__CINT__)
  gSystem->Load("libdpmjet");      // Parton density functions
  gSystem->Load("libTDPMjet");      // Parton density functions
#endif
      IlcGenDPMjet* dpmjet = new IlcGenDPMjet(-1); 
      dpmjet->SetMomentumRange(0, 999999.);
      dpmjet->SetThetaRange(0., 180.);
      dpmjet->SetYRange(-12.,12.);
      dpmjet->SetPtRange(0,1000.);
      dpmjet->SetProcess(kDpmMb);
      dpmjet->SetEnergyCMS(energy);

      return dpmjet;
}

void ProcessEnvironmentVars()
{
    // Run type
    if (gSystem->Getenv("CONFIG_RUN_TYPE")) {
      for (Int_t iRun = 0; iRun < kRunMax; iRun++) {
	if (strcmp(gSystem->Getenv("CONFIG_RUN_TYPE"), pprRunName[iRun])==0) {
	  proc = (PDC06Proc_t)iRun;
	  cout<<"Run type set to "<<pprRunName[iRun]<<endl;
	}
      }
    }

    // Field
    if (gSystem->Getenv("CONFIG_FIELD")) {
      for (Int_t iField = 0; iField < kFieldMax; iField++) {
	if (strcmp(gSystem->Getenv("CONFIG_FIELD"), pprField[iField])==0) {
	  mag = (Mag_t)iField;
	  cout<<"Field set to "<<pprField[iField]<<endl;
	}
      }
    }

    // Energy
    if (gSystem->Getenv("CONFIG_ENERGY")) {
      energy = atoi(gSystem->Getenv("CONFIG_ENERGY"));
      cout<<"Energy set to "<<energy<<" GeV"<<endl;
    }

    // Random Number seed
    if (gSystem->Getenv("CONFIG_SEED")) {
      seed = atoi(gSystem->Getenv("CONFIG_SEED"));
    }
}
