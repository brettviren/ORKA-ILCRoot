// Function converting pseudorapidity
// interval to polar angle interval. It is used to set 
// the limits in the generator
Float_t EtaToTheta(Float_t arg){
  return (180./TMath::Pi())*2.*atan(exp(-arg));
}

// Set Random Number seed using the current time
TDatime dat;
static UInt_t sseed = dat.Get();

void Config()
{
  gRandom->SetSeed(sseed);
  cout<<"Seed for random number generation= "<<gRandom->GetSeed()<<endl; 
    
  // Load GEANT 3 library. It has to be in LD_LIBRARY_PATH
  gSystem->Load("libgeant321");

  // Instantiation of the particle transport package. gMC is set internaly
  new TGeant3TGeo("C++ Interface to Geant3");

  // Create run loader and set some properties
  IlcRunLoader* rl =  IlcRunLoader::Open("gilc.root",
					 IlcConfig::GetDefaultEventFolderName(),
					 "recreate");
  if (!rl) Fatal("Config.C","Can not instatiate the Run Loader");
  rl->SetCompressionLevel(2);
  rl->SetNumberOfEventsPerFile(3);

  // Register the run loader in gIlc
  gIlc->SetRunLoader(rl);

  // Set external decayer
  TVirtualMCDecayer *decayer = new IlcDecayerPythia();
  decayer->SetForceDecay(kAll); // kAll means no specific decay is forced
  decayer->Init();

  // Register the external decayer in the transport package
  gMC->SetExternalDecayer(decayer);

  // STEERING parameters FOR ILC SIMULATION
  // Specify event type to be transported through the ILC setup
  // All positions are in cm, angles in degrees, and P and E in GeV
  // For the details see the GEANT 3 manual

  // Switch on/off the physics processes (global)
  // Please consult the file data/gilc.cuts for detector
  // specific settings, i.e. DRAY
  gMC->SetProcess("DCAY",1); // Particle decay
  gMC->SetProcess("PAIR",1); // Pair production
  gMC->SetProcess("COMP",1); // Compton scattering
  gMC->SetProcess("PHOT",1); // Photo effect
  gMC->SetProcess("PFIS",0); // Photo fission
  gMC->SetProcess("DRAY",0); // Delta rays
  gMC->SetProcess("ANNI",1); // Positron annihilation
  gMC->SetProcess("BREM",1); // Bremstrahlung
  gMC->SetProcess("MUNU",1); // Muon nuclear interactions
  gMC->SetProcess("CKOV",1); // Cerenkov production
  gMC->SetProcess("HADR",1); // Hadronic interactions
  gMC->SetProcess("LOSS",2); // Energy loss (2=complete fluct.)
  gMC->SetProcess("MULS",1); // Multiple scattering
  gMC->SetProcess("RAYL",1); // Rayleigh scattering
  
  // Set the transport package cuts
  Float_t cut = 1.e-3;        // 1MeV cut by default
  Float_t tofmax = 1.e10;

  gMC->SetCut("CUTGAM", cut); // Cut for gammas
  gMC->SetCut("CUTELE", cut); // Cut for electrons
  gMC->SetCut("CUTNEU", cut); // Cut for neutral hadrons
  gMC->SetCut("CUTHAD", cut); // Cut for charged hadrons
  gMC->SetCut("CUTMUO", cut); // Cut for muons
  gMC->SetCut("BCUTE",  cut); // Cut for electron brems.
  gMC->SetCut("BCUTM",  cut); // Cut for muon brems.
  gMC->SetCut("DCUTE",  cut); // Cut for electron delta-rays
  gMC->SetCut("DCUTM",  cut); // Cut for muon delta-rays
  gMC->SetCut("PPCUTM", cut); // Cut for e+e- pairs by muons
  gMC->SetCut("TOFMAX", tofmax); // Time of flight cut
  
  // Set up the particle generation

  // IlcGenCocktail permits to combine several different generators
  IlcGenCocktail *gener = new IlcGenCocktail();

  // The phi range is always inside 0-360
  gener->SetPhiRange(0, 360);

  // Set pseudorapidity range from -8 to 8.
  Float_t thmin = EtaToTheta(8);   // theta min. <---> eta max
  Float_t thmax = EtaToTheta(-8);  // theta max. <---> eta min 
  gener->SetThetaRange(thmin,thmax);

  gener->SetOrigin(0, 0, 0);  // vertex position
  gener->SetSigma(0, 0, 5.3);   // Sigma in (X,Y,Z) (cm) on IP position
  gener->SetCutVertexZ(1.);     // Truncate at 1 sigma
  gener->SetVertexSmear(kPerEvent); 

  // First cocktail component: 100 ``background'' particles 
  IlcGenHIJINGpara *hijingparam = new IlcGenHIJINGpara(100);
  hijingparam->SetMomentumRange(0.2, 999);
  gener->AddGenerator(hijingparam,"HIJING PARAM",1);

  // Second cocktail component: one gamma in PHOS direction
  IlcGenBox *genbox = new IlcGenBox(1);
  genbox->SetMomentumRange(10,11.);
  genbox->SetPhiRange(270.5,270.7);
  genbox->SetThetaRange(90.5,90.7);
  genbox->SetPart(kGamma);
  gener->AddGenerator(genbox,"GENBOX GAMMA for PHOS",1);

  gener->Init(); // Initialization of the coctail generator

  // Field (the last parameter is 1 => L3 0.4 T)
  TGeoGlobalMagField::Instance()->SetField(new IlcMagF("Maps","Maps", -1., -1., IlcMagF::k5kG));

  // Make sure the current ROOT directory is in gilc.root 
  rl->CdGAFile();

  // Build the setup and set some detector parameters

  // ILC BODY parameters. BODY is always present
  IlcBODY *BODY = new IlcBODY("BODY", "ILC envelop");

  // Start with Magnet since detector layouts may be depending
  // on the selected Magnet dimensions
  IlcMAG *MAG = new IlcMAG("MAG", "Magnet");

  IlcABSO *ABSO = new IlcABSOv0("ABSO", "Muon Absorber");       // Absorber

  IlcDIPO *DIPO = new IlcDIPOv2("DIPO", "Dipole version 2");    // Dipole magnet

  IlcHALL *HALL = new IlcHALL("HALL", "ILC Hall");            // Hall
    
  IlcFRAMEv2 *FRAME = new IlcFRAMEv2("FRAME", "Space Frame");   // Space frame

  IlcSHIL *SHIL = new IlcSHILv2("SHIL", "Shielding Version 2"); // Shielding

  IlcPIPE *PIPE = new IlcPIPEv0("PIPE", "Beam Pipe");           // Beam pipe
    
  // ITS parameters
  IlcITSvPPRasymmFMD *ITS  = new IlcITSvPPRasymmFMD("ITS",
	       "ITS PPR detailed version with asymmetric services");
  ITS->SetMinorVersion(2);      // don't change it if you're not an ITS developer
  ITS->SetReadDet(kFALSE);      // don't change it if you're not an ITS developer
  ITS->SetThicknessDet1(200.);  // detector thickness on layer 1:[100,300] mkm
  ITS->SetThicknessDet2(200.);  // detector thickness on layer 2:[100,300] mkm
  ITS->SetThicknessChip1(150.); // chip thickness on layer 1: [150,300] mkm
  ITS->SetThicknessChip2(150.); // chip thickness on layer 2: [150,300]
  ITS->SetRails(0);	        // 1 --> rails in ; 0 --> rails out
  ITS->SetCoolingFluid(1);      // 1 --> water ; 0 --> freon
  ITS->SetEUCLID(0);            // no output for the EUCLID CAD system 

  
  IlcTPC *TPC = new IlcTPCv2("TPC", "Default");                 // TPC

  IlcTOF *TOF = new IlcTOFv5T0("TOF", "normal TOF");            // TOF

  IlcHMPID *HMPID = new IlcHMPIDv1("HMPID", "normal HMPID");         // HMPID

  IlcZDC *ZDC = new IlcZDCv2("ZDC", "normal ZDC");              // ZDC

  IlcTRD *TRD = new IlcTRDv1("TRD", "TRD slow simulator");      // TRD

  IlcFMD *FMD = new IlcFMDv1("FMD", "normal FMD");              // FMD

  IlcMUON *MUON = new IlcMUONv1("MUON", "default");             // MUON

  IlcPHOS *PHOS = new IlcPHOSv1("PHOS", "IHEP");                // PHOS

  IlcPMD *PMD = new IlcPMDv1("PMD", "normal PMD");              // PMD

  IlcT0 *T0 = new IlcT0v1("T0", "T0 Detector");  // T0

  // EMCAL
  IlcEMCAL *EMCAL = new IlcEMCALv2("EMCAL", "EMCAL_COMPLETEV1");

  IlcVZERO *VZERO = new IlcVZEROv7("VZERO", "normal VZERO");    // VZERO
}
