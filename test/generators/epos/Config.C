// One can use the configuration macro in compiled mode by
// root [0] gSystem->Load("libgeant321");
// root [0] gSystem->SetIncludePath("-I$ROOTSYS/include -I$ILC_ROOT/include\
//                   -I$ILC_ROOT -I$ILC/geant3/TGeant3");
// root [0] .x grun.C(1,"ConfigPPR.C++")

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TVirtualMC.h>
#include <TGeant3TGeo.h>
#include <TPDGCode.h>
#include <TF1.h>
#include "STEER/IlcRunLoader.h"
#include "STEER/IlcRun.h"
#include "STEER/IlcConfig.h"
#include "STEER/IlcGenerator.h"
#include "STEER/IlcLog.h"
#include "PYTHIA6/IlcDecayerPythia.h"
#include "EVGEN/IlcGenHIJINGpara.h"
#include "THijing/IlcGenHijing.h"
#include "EVGEN/IlcGenCocktail.h"
#include "EVGEN/IlcGenSlowNucleons.h"
#include "EVGEN/IlcSlowNucleonModelExp.h"
#include "EVGEN/IlcGenParam.h"
#include "EVGEN/IlcGenMUONlib.h"
#include "EVGEN/IlcGenSTRANGElib.h"
#include "EVGEN/IlcGenMUONCocktail.h"
#include "EVGEN/IlcGenCocktail.h"
#include "EVGEN/IlcGenGeVSim.h"
#include "EVGEN/IlcGeVSimParticle.h"
#include "PYTHIA6/IlcGenPythia.h"
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
#include "FMD/IlcFMDv1.h"
#include "MUON/IlcMUONv1.h"
#include "PHOS/IlcPHOSv1.h"
#include "PMD/IlcPMDv1.h"
#include "T0/IlcT0v1.h"
#include "EMCAL/IlcEMCALv2.h"
#include "ACORDE/IlcACORDEv1.h"
#include "VZERO/IlcVZEROv7.h"
#include "EPOS/IlcGenEpos.h"
#endif

enum PprTrigConf_t
{
    kDefaultPPTrig, kDefaultPbPbTrig
};

const char * pprTrigConfName[] = {
    "p-p","Pb-Pb"
};

// This part for configuration    

static IlcMagF::BMap_t smag = IlcMagF::k5kG;
static Int_t    sseed = 0; //Set 0 to use the current time
static PprTrigConf_t strig = kDefaultPPTrig; // default pp trigger configuration
static Float_t energy = 900;

// Comment line 
static TString  comment;

// Functions
Float_t EtaToTheta(Float_t arg);
void ProcessEnvironmentVars();

void Config()
{
    // ThetaRange is (0., 180.). It was (0.28,179.72) 7/12/00 09:00
    // Theta range given through pseudorapidity limits 22/6/2001

    // Get settings from environment variables
    ProcessEnvironmentVars();

    // Set Random Number seed
    gRandom->SetSeed(sseed);
    cout<<"Seed for random number generation= "<<gRandom->GetSeed()<<endl; 


   // libraries required by geant321
#if defined(__CINT__)
    gSystem->Load("liblhapdf");
    gSystem->Load("libEGPythia6");
    gSystem->Load("libpythia6");
    gSystem->Load("libIlcPythia6");
    gSystem->Load("libgeant321");
    gSystem->Load("libEPOS.so");
#endif

    new     TGeant3TGeo("C++ Interface to Geant3");

  // Output every 100 tracks
  ((TGeant3*)gMC)->SetSWIT(4,100);

    IlcRunLoader* rl=0x0;

    IlcLog::Message(IlcLog::kInfo, "Creating Run Loader", "", "", "Config()","ConfigPPR.C", __LINE__);

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

    // Set the trigger configuration
    IlcSimulation::Instance()->SetTriggerConfig(pprTrigConfName[strig]);
    cout<<"Trigger configuration is set to  "<<pprTrigConfName[strig]<<endl;

    //
    // Set External decayer
    IlcDecayer *decayer = new IlcDecayerPythia();
    decayer->Init();
    gMC->SetExternalDecayer(decayer);
    //
    //
    //=======================================================================
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

    // Generator Configuration

    IlcGenerator* gener = 0x0;

    IlcGenEpos *epos = new IlcGenEpos();
    epos->SetProjectile("p", 1, 1);
    epos->SetTarget("p", 1, 1);
    epos->SetEnergyCMS(energy);
    gener = epos;

    gener->SetOrigin(0, 0, 0);    // vertex position
    gener->SetSigma(0, 0, 5.3);   // Sigma in (X,Y,Z) (cm) on IP position
    gener->SetCutVertexZ(1.);     // Truncate at 1 sigma
    gener->SetVertexSmear(kPerEvent); 
    gener->SetTrackingFlag(1);
    gener->Init();
    
    IlcMagF* field = 0x0;
    if (smag == IlcMagF::k2kG) {
	comment = comment.Append(" | L3 field 0.2 T");
	field = new IlcMagF("Maps","Maps", -1., -1., IlcMagF::k5kG,IlcMagF::kBeamTypepp, energy/2.0);
    } else if (smag == IlcMagF::k5kG) {
	comment = comment.Append(" | L3 field 0.5 T");
	field = new IlcMagF("Maps","Maps", -1., -1., IlcMagF::k5kG,IlcMagF::kBeamTypepp, energy/2.0);
    }
    
    printf("\n \n Comment: %s \n \n", comment.Data());
    
    
// Field
    TGeoGlobalMagField::Instance()->SetField(field);

    rl->CdGAFile();
//
    Int_t   iABSO   = 1;
    Int_t   iDIPO   = 1;
    Int_t   iFMD    = 1;
    Int_t   iFRAME  = 1;
    Int_t   iHALL   = 1;
    Int_t   iITS    = 1;
    Int_t   iMAG    = 1;
    Int_t   iMUON   = 1;
    Int_t   iPHOS   = 1;
    Int_t   iPIPE   = 1;
    Int_t   iPMD    = 1;
    Int_t   iHMPID  = 1;
    Int_t   iSHIL   = 1;
    Int_t   iT0     = 1;
    Int_t   iTOF    = 1;
    Int_t   iTPC    = 1;
    Int_t   iTRD    = 1;
    Int_t   iZDC    = 1;
    Int_t   iEMCAL  = 1;
    Int_t   iVZERO  = 1;
    Int_t   iACORDE = 1;

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
        //============================ TPC parameters ===================
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
        IlcMUON *MUON = new IlcMUONv1("MUON","default");
    }
    //=================== PHOS parameters ===========================

    if (iPHOS)
    {
        IlcPHOS *PHOS = new IlcPHOSv1("PHOS", "IHEP");
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
        IlcEMCAL *EMCAL = new IlcEMCALv2("EMCAL", "EMCAL_COMPLETEV1");
    }

     if (iACORDE)
    {
        //=================== ACORDE parameters ============================
        IlcACORDE *ACORDE = new IlcACORDEv1("ACORDE", "normal ACORDE");
    }

     if (iVZERO)
    {
        //=================== VZERO parameters ============================
        IlcVZERO *VZERO = new IlcVZEROv7("VZERO", "normal VZERO");
    }
 
             
}

Float_t EtaToTheta(Float_t arg){
  return (180./TMath::Pi())*2.*atan(exp(-arg));
}




void ProcessEnvironmentVars()
{
    // Run type
    if (gSystem->Getenv("CONFIG_RUN_TYPE")) {
      for (Int_t iRun = 0; iRun < kRunMax; iRun++) {
	if (strcmp(gSystem->Getenv("CONFIG_RUN_TYPE"), pprRunName[iRun])==0) {
	  srun = (PprRun_t)iRun;
	  cout<<"Run type set to "<<pprRunName[iRun]<<endl;
	}
      }
    }

    // Random Number seed
    if (gSystem->Getenv("CONFIG_SEED")) {
      sseed = atoi(gSystem->Getenv("CONFIG_SEED"));
    }
}
