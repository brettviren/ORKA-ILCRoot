//Config.C macro
//Read comments in the macro to congifure your simulation

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Riostream.h>
#include <TPDGCode.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TVirtualMC.h>
#include <TGeant3TGeo.h>
#include "STEER/IlcRunLoader.h"
#include "STEER/IlcRun.h"
#include "STEER/IlcConfig.h"
#include "PYTHIA6/IlcDecayerPythia.h"
#include "EVGEN/IlcGenCocktail.h"
#include "EVGEN/IlcGenHIJINGpara.h"
#include "EVGEN/IlcGenFixed.h"
#include "EVGEN/IlcGenBox.h"
#include "STEER/IlcMagF.h"
#include "STEER/IlcPID.h"
#include "STRUCT/IlcBODY.h"
#endif

// ************* STEERING parameters FOR SIMULATION **************
// --- Specify event type to be tracked through the detector setup
// --- All positions are in cm, angles in degrees, and P and E in GeV


//____________________________________________________________________
// 
// Generator types 
//
enum PprRun_t 
{
  kGenBox, kGenFixed,
  kCocktail
};


//____________________________________________________________________
//
// MonteCarlo types
//
enum PprMC_t 
{
  kGEANT4, 
  kGEANT3TGEO
};


//____________________________________________________________________
//
// GeneratorFactory prototype
//
void GeneratorFactory(PprRun_t srun);

// Definitions 
static TString  comment;



void Config()
{
  
  //____________________________________________________________________
  // 
  // ***** This part is for configuration:                   ******
  // ***** please, select the following 6 fields as you need ******
  //
  
  //1) Select your generator
  static PprRun_t srun = kGenFixed; //particle gun with fixed momentum, phi, theta
//   static PprRun_t srun = kGenBox;    //particle gun with const. distribution in theta
//   static PprRun_t srun = kCocktail; //EXAMPLE of Cocktail. From this example improve your own Cocktail of particles/generators.
  
  
  //2) Select your MC here
  static PprMC_t  mc   = kGEANT4;
//   static PprMC_t  mc   = kGEANT3TGEO;
  
  
  //3) Magnetic field in Tesla
  static Float_t MagField = 1.25;
  
  
  //4) Select the seed
  static Int_t  sseed = 123456; //Set 0 to use the current time 
  
  
  //5) Select the number of events per file root
  static Int_t evntperfile = 100;
  
  
  //6) Select SubDetectors to use
  
  Int_t   iPVBAR  = 1;  //Photon Veto Barrel
  Int_t   iPVEC   = 1;  //Photon Veto EndCap
  Int_t   iRSTACK = 1;  //Range Stack
  Int_t   iDCH    = 1;  //Drift Chamber
  Int_t   iTARGET = 1;  //Target
  
  
  // Set Random Number seed
  gRandom->SetSeed(sseed);
  
  IlcLog::Message(IlcLog::kInfo, Form("Seed for random number generation = %d",gRandom->GetSeed()), "Config.C", "Config.C", "Config()","Config.C", __LINE__);
  
  
  
  //____________________________________________________________________
  IlcRunLoader* rl=0x0;
  
  cout<<"Config.C: Creating Run Loader ..."<<endl;
  
  rl = IlcRunLoader::Open("gilc.root", IlcConfig::GetDefaultEventFolderName(), "recreate");
  if (!rl)
  {
    gIlc->Fatal("Config.C","Can not instatiate the Run Loader");
    return;
  }
  
  // Set compression level for gIlc.root file
  rl->SetCompressionLevel(2);
  
  // Set number of events per file to "evntperfile" selected above
  rl->SetNumberOfEventsPerFile(evntperfile);
  
  // Set run loader to "rl"
  gIlc->SetRunLoader(rl);
  
  
  
  //generator configuration
  GeneratorFactory(srun);
  
  
  // Field (1.25 T)
  TGeoGlobalMagField::Instance()->SetField(new IlcMagF("MagField","MagField", MagField, 1., IlcMagF::kConst, IlcMagF::kNoBeamField));
  rl->CdGAFile();
  TGeoGlobalMagField::Instance()->GetField()->Write();
  
  comment = comment.Append(Form("*** MAGNETIC FIELD: Solenoidal %f Tesla\n", MagField));
  
  
  
  //=================== Ilc BODY parameters =============================
  IlcBODY *BODY = new IlcBODY("BODY", "Ilc envelop");
  
  comment = comment.Append("*** DETECTORS ON: ");
  
  
  if(iTARGET)
  {
    
    //=================== TARGET parameters ============================
    //TARGET version for ORKA
    //Mionor version: 1=square fibers; 2=poligonal shape
    Int_t MinorVersion = 1;
    
    IlcTARGETvORKA *TARGET  = new IlcTARGETvORKA("TARGET","TARGET version for ORKA",MinorVersion);
    
    comment = comment.Append(" TARGET ");
    
  }
  
  if (iDCH)
  {
    //=================== DCH parameters ===========================
    IlcDCH *DCH = new IlcDCHv1("DCH", "default");
    
    comment = comment.Append(" DCH ");
    
  }
  
  
  if (iPVBAR)
  {
    //=================== Photon Veto Barrel ===========================
    IlcPVBAR *PVBAR = new IlcPVBARv1("PVBAR", "ORKA");
    
    comment = comment.Append(" PVBAR ");
    
  }
  
  if (iPVEC)
  {
    //=================== Photon Veto EndCap ===========================
    IlcPVEC *PVEC = new IlcPVECv1("PVEC", "ORKA");
    
    comment = comment.Append(" PVEC ");
    
  }
  
  if (iRSTACK)
  {
    //=================== Range Stack ===========================
    IlcRSTACK *RSTACK = new IlcRSTACKv1("RSTACK", "ORKA");
    
    comment = comment.Append(" RSTACK ");
    
  }
  
  comment = comment.Append("\n*** MC: ");
  
  
  //MC section
  //__________________________________________________________________
  switch (mc) 
  {
    case kGEANT4:
      // 
      // libraries required by Geant4
      //       
      if (gClassTable->GetID("TGeant4") == -1) {
	
	if (!gInterpreter->IsLoaded("$G4VMC/examples/macro/g4libs.C")) {
	  gROOT->LoadMacro("$G4VMC/examples/macro/g4libs.C");
	  gInterpreter->ProcessLine("g4libs()");
	}
	
      }
      //
      // Create Geant4 VMC
      //  
      TGeant4 *geant4 = 0;
      if (!gMC) {
	
	
	/********************************************************************************
	 *          
	 * "stepLimit" permits to choose the stepping in the step manager of the detector
	 * "specialCuts" permits to set your own cuts like:  gMC->SetCut("CUTGAM", cut);
	 * "specialControls" permits to set your own processes like:  gMC->SetProcess("DCAY",1);
	 *  
	 ********************************************************************************/
	
	
	//QGSP_BERT HP neutrons Physics list
	//TG4RunConfiguration* runConfiguration = new TG4RunConfiguration("geomVMCtoRoot","QGSP_BERT_HP");//,"stepLimiter+specialCut",true);
	
	//QGSP_BERT Physics list
	TG4RunConfiguration* runConfiguration = new TG4RunConfiguration("geomVMCtoRoot","QGSP_BERT");//,"stepLimiter+specialCut",true);
	geant4 = new TGeant4("TGeant4", "The Geant4 Monte Carlo",runConfiguration);        
	
	cout << "Geant4 has been created." << endl;
      }      
      else{ 
	cout << "Monte Carlo has been already created." << endl;
      }
      
     /****************************************************************************************/
	
	
	// Switch on special cuts process
	//geant4->ProcessGeantCommand("/run/setCut 1.e-5");  
	//geant4->ProcessGeantCommand("/tracking/verbose 0");  
	
	//geant4->ProcessGeantCommand("/mcVerbose/all 0");
	//geant4->ProcessGeantCommand("/mcVerbose/runAction 1");
	//geant4->ProcessGeantCommand("/mcVerbose/geometryManager 2");
	//geant4->ProcessGeantCommand("/mcVerbose/stackingAction 0");
	//geant4->ProcessGeantCommand("/mcVerbose/eventAction 2");
	
	//geant4->ProcessGeantCommand("/run/particle/applyCuts");
	//geant4->ProcessGeantCommand("/random/setSavingFlag 1");
	//geant4->ProcessGeantCommand("/random/saveThisRun");
	
	//geant4->ProcessGeantCommand("/run/particle/applyCuts");  
	//geant4->ProcessGeantCommand("/mcVerbose/geometryManager 2");  
	//geant4->ProcessGeantCommand("/mcDet/volNameSeparator #");
	//geant4->ProcessGeantCommand("/mcDet/useVGM "); 
	//geant4->ProcessGeantCommand("/mcControl/g3Defaults");
	//geant4->ProcessGeantCommand("/mcPhysics/setStackPopperSelection e+ e- pi+ pi- kaon+ kaon- gamma");
	
	comment = comment.Append(" GEANT4 ");
	
	break;
	
	case kGEANT3TGEO:
	  
	  ///////////////////////////////////////////////////
	  // libraries required by geant321
	  #if defined(__CINT__)
	  gSystem->Load("libEGPythia6");
	  gSystem->Load("libpythia6");
	  gSystem->Load("libIlcPythia6");
	  gSystem->Load("libgeant321");
	  #endif
	  
	  
	  // 
	  // GEANT 3.21 MC 
	  // 
	  new TGeant3TGeo("C++ Interface to Geant3");
	  Printf("Making a TGeant3TGeo objet");
	  
	  // Set External decayer
	  TVirtualMCDecayer *decayer = new IlcDecayerPythia();
	  
	  decayer->SetForceDecay(kAll);
	  decayer->Init();
	  gMC->SetExternalDecayer(decayer);
	  
	  //************************************************************************
	  
	  //CUTGAM threshold for gamma transport;
	  //CUTELE threshold for electron and positron transport;
	  //CUTNEU threshold for neutral hadron transport;
	  //CUTHAD threshold for charged hadron and ion transport;
	  //CUTMUO threshold for muon transport;
	  //BCUTE threshold for photons produced by electron bremsstrahlung;
	  //BCUTM threshold for photons produced by muon bremsstrahlung;
	  //DCUTE threshold for electrons produced by electron delta-rays;
	  //DCUTM threshold for electrons produced by muon or hadron delta-rays;
	  //PPCUTM threshold for electron-positron direct pair production by muon;
	  //TOFMAX threshold on time of flight counted from primary interaction time;
	  //GCUTS free for user applications.
	  
	  //*********************************************************************
	  
	  
	  Float_t cut = 1.e-3;        // 1 MeV  cut by default for ILC
	  Float_t cutn = 0.;
	  Float_t tofmax = 1.e10;
	  
	  gMC->SetCut("CUTGAM", cut);
	  gMC->SetCut("CUTELE", cut);
	  gMC->SetCut("CUTNEU", cutn);
	  gMC->SetCut("CUTHAD", cut);
	  gMC->SetCut("CUTMUO", cut);
	  gMC->SetCut("BCUTE",  cut); 
	  gMC->SetCut("BCUTM",  cut); 
	  gMC->SetCut("DCUTE",  cut); 
	  gMC->SetCut("DCUTM",  cut); 
	  gMC->SetCut("PPCUTM", cut);
	  gMC->SetCut("TOFMAX", tofmax); 
	  
	  
	  
	  comment = comment.Append(" GEANT3TGEO ");
	  
	  break;
	  
	  default:
	    gIlc->Fatal("Config.C", "No MC type chosen");
	    return;
  }
  
  
  
  
  gMC->SetProcess("DCAY",1); //Decay in flight with generation of secondaries.
  gMC->SetProcess("PAIR",1); //Pair production with generation of the electron-positron.
  gMC->SetProcess("COMP",1); //Compton scattering with generation of the electron.
  gMC->SetProcess("PHOT",1); //Photo-electric effect with generation of the electron.
  gMC->SetProcess("PFIS",0); //No photo-fission.
  gMC->SetProcess("DRAY",0); //No delta-rays production.
  gMC->SetProcess("ANNI",1); //Positron annihilation with generation of photons.
  gMC->SetProcess("BREM",1); //bremsstrahlung with generation of gamma.
  gMC->SetProcess("MUNU",1); //Muon-nucleus interactions with generation of secondaries.
  gMC->SetProcess("CKOV",1); //Cherenkov photon.
  gMC->SetProcess("HADR",1); //Hadronic interactions with generation of secondaries.
  gMC->SetProcess("LOSS",2); //Continuous energy loss without generation of delta-rays and full Landau-Vavilov-Gauss fluctuations. 
  //In this case "DRAY" is forced to 0 to avoid double
  //counting of fluctuations.
  gMC->SetProcess("MULS",1); //Multiple scattering according to Moliere theory.
  gMC->SetProcess("RAYL",1); //Rayleigh effect.  
  
  printf("\n \n");
  
  printf("***************** WITH THIS CONFIG YOU HAVE CHOOSEN ***************** ");  
  printf("\n \n%s \n \n", comment.Data());
  printf("\n*********************************************************************\n \n");
  
  IlcLog::Message(IlcLog::kInfo, "END OF CONFIG", "Config.C", "Config.C", "Config()"," Config.C", __LINE__);
  
  printf("\n \n");
  
  
  gMC->SetMaxNStep(1000000);  
  
}


void GeneratorFactory(PprRun_t srun) {
  
  IlcGenerator * gGener = 0x0;
  typedef enum { kNoSmear, kPerEvent, kPerTrack }  VertexSmear_t;
  
  comment = comment.Append("*** GENERATOR: ");
  
  switch (srun) {
    
    
      //Particle Type specified  by user  
      
      /****************************************************
       *     // Particles type
       *     //  PDG    Name
       *     //  22     // photon
       *     //  -11    // positron
       *     //  11     // electron
       *     //  12     // neutrino e
       *     //  -13    // muon +
       *     //  13     // muon -
       *     //  111    // pi0
       *     //  211    // pi+
       *     //  -211   // pi-
       *     //  130    // Kaon Long
       *     //  321    // Kaon +
       *     //  -321   // Kaon -
       *     //  2112   // Neutron
       *     //  2212   // Proton
       *     //  -2212  // Anti Proton
       *     //  310    // Kaon Short
       ****************************************************/
      
      
      case kGenFixed:
	//*******************************************************
	// Example of fixed particle gun                        *
	//*******************************************************
	
	//Fixed momentum, phi, theta and vertex position specified by the user:
	//SetMomentum(momentum)
	//SetPhi(phi)
	//SetTheta(theta)
	
	//Particle Type specified  by user:
	//SetPart(ipart)
	
	{
	  Int_t nParticles=1;
	  IlcGenFixed *gener = new IlcGenFixed(nParticles);
	  Int_t PDG=321;     //set the PDG code of particle
	  gener->SetPart(PDG);
	  Double_t pmom = 1.e-5;
	  gener->SetMomentum(pmom);
	  Double_t phi = 90.;
	  gener->SetPhi(phi);
	  Double_t theta = 2.;
	  gener->SetTheta(theta);
	  const TParticlePDG *ap = TDatabasePDG::Instance()->GetParticle(PDG);
	  char *name = ap->GetName();
	  
	  comment = comment.Append(Form ("IlcGenFixed for %d %s (%f GeV/c) with phi %f and theta %f\n", nParticles,name,pmom,phi,theta));
	  
	  gGener=gener;
	  
	  // gGener->SetOrigin(0., 0., -20.);    // vertex position
	  // gGener->SetSigma(0, 0, 2.);   // Sigma in (X,Y,Z) (cm) on IP position
	  // gGener->SetCutVertexZ(1.);     // Truncate at 1 sigma
	  // gGener->SetVertexSmear(kPerTrack); 
	  
	}
	break;  
	
	
    case kGenBox:
      //*******************************************************
      // Example moving  particle gun                        *
      //*******************************************************
      
      //Random generation of  momentum (transverse momentum), phi and theta in user specified range.
      //Gaussian smearing of vertex either per event or per track.
      {      
	Int_t nParticles=1;
	IlcGenBox *gener = new IlcGenBox(nParticles);
	Int_t PDG=321;     //set the PDG code of particle
	gener->SetPart(PDG);
	Double_t pmin = 0.;
	Double_t pmax = 1.e-7; 
	gener->SetMomentumRange(pmin,pmax);
	Double_t phimin = 0.0;
	Double_t phimax = 360.0; 
	gener->SetPhiRange(phimin,phimax);
	Float_t thmin = 0.;
	Float_t thmax = 3.;
	gener->SetThetaRange(thmin,thmax);
	const TParticlePDG *ap = TDatabasePDG::Instance()->GetParticle(PDG);
	char *name = ap->GetName();
	
	comment = comment.Append(Form ("IlcGenBox for %d %s (%f GeV/c-%f GeV/c) within phi %f-%f and theta %f-%f\n", nParticles,name,pmin,pmax,phimin,phimax,thmin,thmax));
	
	gGener=gener;

	// gGener->SetOrigin(0., 0., -10.);    // vertex position
	  // gGener->SetSigma(0, 0, 2.);   // Sigma in (X,Y,Z) (cm) on IP position
	  // gGener->SetCutVertexZ(1.);     // Truncate at 1 sigma
	  // gGener->SetVertexSmear(kPerTrack); 
	  
	}
	break;  
	
	
	
      case kCocktail:
	//*******************************************************
	// Example of Cocktail of particles                     *
	//*******************************************************      
	{
	  comment = comment.Append(": Cocktail of one muon and one pion from GenFixed generator\n");
	  
	  IlcGenCocktail *gener  = new IlcGenCocktail();
	  // gener->SetOrigin(0., 0., -10.);    // vertex position
	  // gener->SetSigma(0, 0, 1.);   // Sigma in (X,Y,Z) (cm) on IP position
	  // gener->SetCutVertexZ(1.);     // Truncate at 1 sigma
	  // gener->SetVertexSmear(kPerTrack); 
	  
	  IlcGenFixed *muon = new IlcGenFixed(1);
	  muon->SetPart(-13);
	  muon->SetMomentum(2.);
	  muon->SetPhi(90.);
	  muon->SetTheta(80.);
	  
	  IlcGenFixed *pion = new IlcGenFixed(1);
	  pion->SetPart(211);
	  pion->SetMomentum(2.);
	  pion->SetPhi(35.);
	  pion->SetTheta(85.);
	  
	  gener->AddGenerator(muon,"muon",1);
	  gener->AddGenerator(pion,"pion",1);
	  
	  
	  gGener=gener;
	  
	}
	break; 
	
      default: break;
  }
  
  gGener->Init();
  
}

