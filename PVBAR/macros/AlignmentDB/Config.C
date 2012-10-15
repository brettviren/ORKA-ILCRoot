// One can use the configuration macro in compiled mode by
// root [0] gSystem->Load("libgeant321");
// root [0] gSystem->SetIncludePath("-I$ROOTSYS/include -I$ILC_ROOT/include\
//                   -I$ILC_ROOT -I$ILC/geant3/TGeant3");
// root [0] .x grun.C(1,"Config.C++")

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TVirtualMC.h>
#include <TGeant3TGeo.h>
#include <TPDGCode.h>
#include "STEER/IlcRunLoader.h"
#include "STEER/IlcRun.h"
#include "STEER/IlcConfig.h"
#include "PYTHIA6/IlcDecayerPythia.h"
#include "EVGEN/IlcGenCocktail.h"
#include "EVGEN/IlcGenHIJINGpara.h"
#include "STEER/IlcMagF.h"
#include "STRUCT/IlcBODY.h"
#include "STRUCT/IlcMAG.h"
#include "STRUCT/IlcABSOv0.h"
#include "STRUCT/IlcDIPOv2.h"
#include "STRUCT/IlcHALL.h"
#include "STRUCT/IlcFRAMEv2.h"
#include "STRUCT/IlcSHILv2.h"
#include "STRUCT/IlcPIPEv0.h"
#include "ITS/IlcITSvPPRasymmFMD.h"
#include "TPC/IlcTPCv2.h"
#include "TOF/IlcTOFv4T0.h"
#include "HMPID/IlcHMPIDv1.h"
#include "ZDC/IlcZDCv2.h"
#include "TRD/IlcTRDv1.h"
#include "FMD/IlcFMDv1.h"
#include "MUON/IlcMUONv1.h"
#include "MUON/IlcMUONSt1GeometryBuilderV2.h"
#include "MUON/IlcMUONSt2GeometryBuilderV2.h"
#include "MUON/IlcMUONSlatGeometryBuilder.h"
#include "MUON/IlcMUONTriggerGeometryBuilder.h"
#include "PVBAR/IlcPVBARv1.h"
#include "PMD/IlcPMDv1.h"
#include "T0/IlcT0v1.h"
#include "EMCAL/IlcEMCALv2.h"
#include "ACORDE/IlcACORDEv1.h"
#include "VZERO/IlcVZEROv5.h"
#endif

Float_t EtaToTheta(Float_t arg);
enum PprGeo_t 
{
    kHoles, kNoHoles
};
static PprGeo_t geo = kHoles;

void Config()
{
    // ThetaRange is (0., 180.). It was (0.28,179.72) 7/12/00 09:00
    // Theta range given through pseudorapidity limits 22/6/2001

    // Set Random Number seed
  gRandom->SetSeed(123456); // Set 0 to use the currecnt time
  IlcLog::Message(IlcLog::kInfo, Form("Seed for random number generation = %d",gRandom->GetSeed()), "Config.C", "Config.C", "Config()","Config.C", __LINE__);


   // libraries required by geant321
#if defined(__CINT__)
    gSystem->Load("libgeant321");
#endif

    new     TGeant3TGeo("C++ Interface to Geant3");

    IlcRunLoader* rl=0x0;

    IlcLog::Message(IlcLog::kInfo, "Creating Run Loader", "Config.C", "Config.C", "Config()"," Config.C", __LINE__);

    rl = IlcRunLoader::Open("gilc.root",
			    IlcConfig::GetDefaultEventFolderName(),
			    "recreate");
    if (rl == 0x0)
      {
	gIlc->Fatal("Config.C","Can not instatiate the Run Loader");
	return;
      }
    rl->SetCompressionLevel(2);
    rl->SetNumberOfEventsPerFile(3);
    gIlc->SetRunLoader(rl);

    //
    // Set External decayer
    TVirtualMCDecayer *decayer = new IlcDecayerPythia();

    decayer->SetForceDecay(kAll);
    decayer->Init();
    gMC->SetExternalDecayer(decayer);
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


    int     nParticles = 100;
    if (gSystem->Getenv("CONFIG_NPARTICLES"))
    {
        nParticles = atoi(gSystem->Getenv("CONFIG_NPARTICLES"));
    }

    IlcGenCocktail *gener = new IlcGenCocktail();
    gener->SetPhiRange(220, 320);
    // Set pseudorapidity range from -0.13 to 0.13.
    Float_t thmin = EtaToTheta(+0.13);  // theta min. <---> eta max
    Float_t thmax = EtaToTheta(-0.13);  // theta max. <---> eta min 
    gener->SetThetaRange(thmin,thmax);
    gener->SetOrigin(0, 0, 0);  //vertex position
    gener->SetSigma(0, 0, 0);   //Sigma in (X,Y,Z) (cm) on IP position

    IlcGenBox *genbox = new IlcGenBox(nParticles);
    genbox->SetPart(kElectron);
    genbox->SetPtRange(0.3, 10.00);
    gener->AddGenerator(genbox,"GENBOX GAMMA for PVBAR",1);
    gener->Init();


    // 
    // Activate this line if you want the vertex smearing to happen
    // track by track
    //
    //gener->SetVertexSmear(perTrack); 
    // Field (L3 0.5 T)
    TGeoGlobalMagField::Instance()->SetField(new IlcMagF("Maps","Maps", -1., -1., IlcMagF::k5kG));

    Int_t   iABSO  =  0;
    Int_t   iDIPO  =  0;
    Int_t   iFMD   =  0;
    Int_t   iFRAME =  0;
    Int_t   iHALL  =  0;
    Int_t   iITS   =  0;
    Int_t   iMAG   =  0;
    Int_t   iMUON  =  0;
    Int_t   iPVBAR  =  1;
    Int_t   iPIPE  =  1;
    Int_t   iPMD   =  0;
    Int_t   iHMPID  =  0;
    Int_t   iSHIL  =  0;
    Int_t   iT0 =  0;
    Int_t   iTOF   =  0;
    Int_t   iTPC   =  0;
    Int_t   iTRD   =  0;
    Int_t   iZDC   =  0;
    Int_t   iEMCAL =  0;
    Int_t   iACORDE   =  0;
    Int_t   iVZERO =  0;
    rl->CdGAFile();
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
        IlcABSO *ABSO = new IlcABSOv0("ABSO", "Muon Absorber");
    }

    if (iDIPO)
    {
        //=================== DIPO parameters ============================

        IlcDIPO *DIPO = new IlcDIPOv2("DIPO", "Dipole version 2");
    }

    if (iHALL)
    {
        //=================== HALL parameters ============================

        IlcHALL *HALL = new IlcHALL("HALL", "Ilc Hall");
    }


    if (iFRAME)
    {
        //=================== FRAME parameters ============================

        IlcFRAMEv2 *FRAME = new IlcFRAMEv2("FRAME", "Space Frame");
	if (geo == kHoles) {
	    FRAME->SetHoles(1);
	} else {
	    FRAME->SetHoles(0);
	}
    }

    if (iSHIL)
    {
        //=================== SHIL parameters ============================

        IlcSHIL *SHIL = new IlcSHILv2("SHIL", "Shielding Version 2");
    }


    if (iPIPE)
    {
        //=================== PIPE parameters ============================

        IlcPIPE *PIPE = new IlcPIPEv0("PIPE", "Beam Pipe");
    }
 
    if(iITS) {

    //=================== ITS parameters ============================
    //
    // As the innermost detector in ILC, the Inner Tracking System "impacts" on
    // almost all other detectors. This involves the fact that the ITS geometry
    // still has several options to be followed in parallel in order to determine
    // the best set-up which minimizes the induced background. All the geometries
    // available to date are described in the following. Read carefully the comments
    // and use the default version (the only one uncommented) unless you are making
    // comparisons and you know what you are doing. In this case just uncomment the
    // ITS geometry you want to use and run Ilcroot.
    //
    // Detailed geometries:         
    //
    //
    //
	IlcITSvPPRasymmFMD *ITS  = new IlcITSvPPRasymmFMD("ITS","ITS PPR detailed version with asymmetric services");
	ITS->SetMinorVersion(2);  // don't touch this parameter if you're not an ITS developer
	ITS->SetReadDet(kTRUE);	  // don't touch this parameter if you're not an ITS developer
	//    ITS->SetWriteDet("$ILC_ROOT/ITS/ITSgeometry_vPPRasymm2.det");  // don't touch this parameter if you're not an ITS developer
	ITS->SetThicknessDet1(200.);   // detector thickness on layer 1 must be in the range [100,300]
	ITS->SetThicknessDet2(200.);   // detector thickness on layer 2 must be in the range [100,300]
	ITS->SetThicknessChip1(200.);  // chip thickness on layer 1 must be in the range [150,300]
	ITS->SetThicknessChip2(200.);  // chip thickness on layer 2 must be in the range [150,300]
	ITS->SetRails(0);	       // 1 --> rails in ; 0 --> rails out
	ITS->SetCoolingFluid(1);       // 1 --> water ; 0 --> freon

 
    //
    // Coarse geometries (warning: no hits are produced with these coarse geometries and they unuseful 
    // for reconstruction !):
    //                                                     
    //
    //IlcITSvPPRcoarseasymm *ITS  = new IlcITSvPPRcoarseasymm("ITS","New ITS PPR coarse version with asymmetric services");
    //ITS->SetRails(0);                // 1 --> rails in ; 0 --> rails out
    //ITS->SetSupportMaterial(0);      // 0 --> Copper ; 1 --> Aluminum ; 2 --> Carbon
    //
    //IlcITS *ITS  = new IlcITSvPPRcoarsesymm("ITS","New ITS PPR coarse version with symmetric services");
    //ITS->SetRails(0);                // 1 --> rails in ; 0 --> rails out
    //ITS->SetSupportMaterial(0);      // 0 --> Copper ; 1 --> Aluminum ; 2 --> Carbon
    //                      
    //
    //
    // Geant3 <-> EUCLID conversion
    // ============================
    //
    // SetEUCLID is a flag to output (=1) or not to output (=0) both geometry and
    // media to two ASCII files (called by default ITSgeometry.euc and
    // ITSgeometry.tme) in a format understandable to the CAD system EUCLID.
    // The default (=0) means that you dont want to use this facility.
    //
     ITS->SetEUCLID(0);  
    }

    if (iTPC)
    {
        //============================ TPC parameters ===================
        IlcTPC *TPC = new IlcTPCv2("TPC", "Default");
    }


    if (iTOF) {
        //=================== TOF parameters ============================
	IlcTOF *TOF = new IlcTOFv4T0("TOF", "normal TOF");
    }


    if (iHMPID)
    {
        //=================== HMPID parameters ===========================
        IlcHMPID *HMPID = new IlcHMPIDv1("HMPID", "normal HMPID");

    }


    if (iZDC)
    {
        //=================== ZDC parameters ============================

        IlcZDC *ZDC = new IlcZDCv2("ZDC", "normal ZDC");
    }

    if (iTRD)
    {
        //=================== TRD parameters ============================

        IlcTRD *TRD = new IlcTRDv1("TRD", "TRD slow simulator");

        // Select the gas mixture (0: 97% Xe + 3% isobutane, 1: 90% Xe + 10% CO2)
        TRD->SetGasMix(1);
	if (geo == kHoles) {
	    // With hole in front of PVBAR
	    TRD->SetPVBARhole();
	    // With hole in front of HMPID
	    TRD->SetHMPIDhole();
	}
	    // Switch on TR
	    IlcTRDsim *TRDsim = TRD->CreateTR();
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
	((IlcMUONv1*)MUON)->SetStepManagerVersionDE(true);
	MUON->AddGeometryBuilder(new IlcMUONSt1GeometryBuilderV2(MUON));
	MUON->AddGeometryBuilder(new IlcMUONSt2GeometryBuilderV2(MUON));
	MUON->AddGeometryBuilder(new IlcMUONSlatGeometryBuilder(MUON));
	MUON->AddGeometryBuilder(new IlcMUONTriggerGeometryBuilder(MUON));
    }
    //=================== PVBAR parameters ===========================

    if (iPVBAR)
    {
        IlcPVBAR *PVBAR = new IlcPVBARv1("PVBAR", "ORKA");
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
        //=================== ACORDE parameters ============================
        IlcVZERO *VZERO = new IlcVZEROv5("VZERO", "normal VZERO");
    }

     IlcLog::Message(IlcLog::kInfo, "End of Config", "Config.C", "Config.C", "Config()"," Config.C", __LINE__);

}

Float_t EtaToTheta(Float_t arg){
  return (180./TMath::Pi())*2.*atan(exp(-arg));
}
