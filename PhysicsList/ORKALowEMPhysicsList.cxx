//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: ORKALowEMPhysicsList.cc,v 1.27 2009-11-15 14:27:30 maire Exp $
// GEANT4 tag $Name: geant4-09-05-beta-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "globals.hh"
#include "ORKALowEMPhysicsList.h"

#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4UserSpecialCuts.hh"




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4PhysicsListHelper.hh"
// gamma
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4ComptonScattering.hh"
#include "G4LivermoreComptonModel.hh"

#include "G4GammaConversion.hh"
#include "G4LivermoreGammaConversionModel.hh"

#include "G4RayleighScattering.hh" 
#include "G4LivermoreRayleighModel.hh"

// e-
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

// e+
#include "G4eIonisation.hh" 
#include "G4eBremsstrahlung.hh" 
#include "G4eplusAnnihilation.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"


#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"

#include "G4GammaNuclearReaction.hh"
#include "G4PhotoNuclearProcess.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ORKALowEMPhysicsList::ORKALowEMPhysicsList():  G4VUserPhysicsList()
{
  // The production threshold is fixed to 0.1 mm for all the particles
  // Secondary particles with a range bigger than 0.1 mm 
  // are generated; otherwise their energy is considered deposited locally

  defaultCutValue = 0.1001 * mm;

  SetVerboseLevel(1);

#ifdef G4_SCIFI_CUTS_USE
  fScifiDetectorCuts = 0;
  //fCutForScifiDetector = 0.012*mm;
  fCutForScifiDetector.push_back(0.00201*mm);
  fCutForScifiDetector.push_back(0.00202*mm);
  fCutForScifiDetector.push_back(0.00203*mm);
  fCutForScifiDetector.push_back(0.00204*mm);
  fCutForXtalDetector.push_back(0.00205*mm);
  fCutForXtalDetector.push_back(0.00206*mm);
  fCutForXtalDetector.push_back(0.00207*mm);
  fCutForXtalDetector.push_back(0.00208*mm);
  //fCutForXtalDetector = 0.011*mm;
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ORKALowEMPhysicsList::~ORKALowEMPhysicsList()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKALowEMPhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
  ConstructOthers();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKALowEMPhysicsList::ConstructBosons()
{
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  // gamma
  G4Gamma::GammaDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKALowEMPhysicsList::ConstructLeptons()
{
  // leptons
  //  e+/-
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  // mu+/-
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  // nu_e
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  // nu_mu
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKALowEMPhysicsList::ConstructMesons()
{
  //  mesons
  //    light mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4Eta::EtaDefinition();
  G4EtaPrime::EtaPrimeDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  G4KaonZero::KaonZeroDefinition();
  G4AntiKaonZero::AntiKaonZeroDefinition();
  G4KaonZeroLong::KaonZeroLongDefinition();
  G4KaonZeroShort::KaonZeroShortDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKALowEMPhysicsList::ConstructBaryons()
{
  //  barions
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();

  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
}

void ORKALowEMPhysicsList::ConstructOthers()
{

// ions
  G4Deuteron::DeuteronDefinition();
  G4Triton::TritonDefinition();
  G4Alpha::AlphaDefinition();
  G4He3::He3Definition();
  G4GenericIon::GenericIonDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKALowEMPhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();

  ConstructGeneral();
  AddStepMax();
    
#ifdef G4_NEUTRONSTIME_CUT
  // Needed to limit neutron tracking time
  G4ProcessManager* pmanager = G4Neutron::Neutron()->GetProcessManager();
  pmanager->AddProcess(new G4UserSpecialCuts(),-1,-1,1); 
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKALowEMPhysicsList::ConstructEM()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
	G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    
    if (particleName == "gamma") {
      // gamma         
#ifdef G4_HIGH_ENERGY
      // use EM process from N02 example
      ph->RegisterProcess(new G4PhotoElectricEffect, particle);
      ph->RegisterProcess(new G4ComptonScattering,   particle);
      ph->RegisterProcess(new G4GammaConversion,     particle);
#else
      // use EM process from BrachiTherapy example
      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      theRayleigh->SetModel(new G4LivermoreRayleighModel());  //not strictly necessary
      pmanager->AddDiscreteProcess(theRayleigh);

      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      thePhotoElectricEffect->SetModel(new G4LivermorePhotoElectricModel());
      pmanager->AddDiscreteProcess(thePhotoElectricEffect);
	
      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      theComptonScattering->SetModel(new G4LivermoreComptonModel());
      pmanager->AddDiscreteProcess(theComptonScattering);
	
      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      theGammaConversion->SetModel(new G4LivermoreGammaConversionModel());
      pmanager->AddDiscreteProcess(theGammaConversion);

      // Photo-nuclear process (only for ORKA)
      G4GammaNuclearReaction* lowEGammaModel = new G4GammaNuclearReaction();
      lowEGammaModel->SetMaxEnergy(3.5*GeV);
      G4PhotoNuclearProcess* thePhotoNuclearProcess = new G4PhotoNuclearProcess();
      thePhotoNuclearProcess->RegisterMe(lowEGammaModel);
      pmanager->AddDiscreteProcess(thePhotoNuclearProcess);

#endif
      
    } else if (particleName == "e-") {
      //electron
#ifdef G4_HIGH_ENERGY
      // use EM process from N02 example
      ph->RegisterProcess(new G4eMultipleScattering, particle);
      ph->RegisterProcess(new G4eIonisation,         particle);
      ph->RegisterProcess(new G4eBremsstrahlung,     particle);      
#else
      // use EM process from BrachiTherapy example
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      msc->SetStepLimitType(fUseDistanceToBoundary);
      pmanager->AddProcess(msc,-1, 1, 1);

      // Ionisation
      G4eIonisation* eIonisation = new G4eIonisation();
      eIonisation->SetEmModel(new G4LivermoreIonisationModel());
      eIonisation->SetStepFunction(0.2, 100*um); //improved precision in tracking  
      pmanager->AddProcess(eIonisation,-1, 2, 2);
	
      // Bremsstrahlung
      G4eBremsstrahlung* eBremsstrahlung = new G4eBremsstrahlung();
      eBremsstrahlung->SetEmModel(new G4LivermoreBremsstrahlungModel());
      pmanager->AddProcess(eBremsstrahlung, -1,-3, 3);
#endif

    } else if (particleName == "e+") {
      //positron
#ifdef G4_HIGH_ENERGY
      // use EM process from N02 example
      ph->RegisterProcess(new G4eMultipleScattering, particle);
      ph->RegisterProcess(new G4eIonisation,         particle);
      ph->RegisterProcess(new G4eBremsstrahlung,     particle);
      ph->RegisterProcess(new G4eplusAnnihilation,   particle);
#else
      // use EM process from BrachiTherapy example
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      msc->SetStepLimitType(fUseDistanceToBoundary);
      pmanager->AddProcess(msc,-1, 1, 1);
	
      // Ionisation (use default, no low-energy available)
      G4eIonisation* eIonisation = new G4eIonisation();
      eIonisation->SetStepFunction(0.2, 100*um);  //improved precision in tracking      
      pmanager->AddProcess(eIonisation, -1, 2, 2);

      //Bremsstrahlung (use default, no low-energy available)
      pmanager->AddProcess(new G4eBremsstrahlung(), -1,-1, 3);

      //Annihilation
      pmanager->AddProcess(new G4eplusAnnihilation(),0,-1, 4);     
#endif
    
    } else if( particleName == "mu+" || 
               particleName == "mu-"    ) {
      //muon  
      //ph->RegisterProcess(new G4MuMultipleScattering, particle);
      //ph->RegisterProcess(new G4MuIonisation,         particle);
      //ph->RegisterProcess(new G4MuBremsstrahlung,     particle);
      //ph->RegisterProcess(new G4MuPairProduction,     particle);
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation,        -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung,    -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction,    -1, 4, 4);
             
    } else if( particleName == "proton" || 
               particleName == "pi-" ||
               particleName == "pi+"    ) {
      //proton  
      //ph->RegisterProcess(new G4hMultipleScattering, particle);
      //ph->RegisterProcess(new G4hIonisation,         particle);
      //ph->RegisterProcess(new G4hBremsstrahlung,     particle);
      //ph->RegisterProcess(new G4hPairProduction,     particle);       
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4);       
     
    } else if( particleName == "alpha" || 
	       particleName == "He3" )     {
      //alpha 
      //ph->RegisterProcess(new G4hMultipleScattering, particle);
      //ph->RegisterProcess(new G4ionIonisation,       particle);
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);
     
    } else if( particleName == "GenericIon" ) { 
      //Ions 
      //ph->RegisterProcess(new G4hMultipleScattering, particle);
      //ph->RegisterProcess(new G4ionIonisation,       particle);     
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);
      
    } else if (particleName == "kaon+" ||
               particleName == "kaon-" ) {

      //ph->RegisterProcess(new G4hMultipleScattering(), particle);
      //ph->RegisterProcess(new G4hIonisation(), particle);
      //ph->RegisterProcess(new G4hBremsstrahlung(), particle);
      //ph->RegisterProcess(new G4hPairProduction(), particle);
      pmanager->AddProcess(new G4hMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation(), -1, 2, 2);
      pmanager->AddProcess(new G4hBremsstrahlung(), -1, 3, 3);
      pmanager->AddProcess(new G4hPairProduction(), -1, 4, 4);

    } else if (particleName == "B+" ||
	       particleName == "B-" ||
	       particleName == "D+" ||
	       particleName == "D-" ||
	       particleName == "Ds+" ||
	       particleName == "Ds-" ||
               particleName == "anti_He3" ||
               particleName == "anti_alpha" ||
               particleName == "anti_deuteron" ||
               particleName == "anti_lambda_c+" ||
               particleName == "anti_omega-" ||
               particleName == "anti_proton" ||
               particleName == "anti_sigma_c+" ||
               particleName == "anti_sigma_c++" ||
               particleName == "anti_sigma+" ||
               particleName == "anti_sigma-" ||
               particleName == "anti_triton" ||
               particleName == "anti_xi_c+" ||
               particleName == "anti_xi-" ||
               particleName == "deuteron" ||
	       particleName == "lambda_c+" ||
               particleName == "omega-" ||
               particleName == "sigma_c+" ||
               particleName == "sigma_c++" ||
               particleName == "sigma+" ||
               particleName == "sigma-" ||
               particleName == "tau+" ||
               particleName == "tau-" ||
               particleName == "triton" ||
               particleName == "xi_c+" ||
               particleName == "xi-" ) {

      //ph->RegisterProcess(new G4hMultipleScattering(), particle);
      //ph->RegisterProcess(new G4hIonisation(), particle);
      pmanager->AddProcess(new G4hMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation(), -1, 2, 2);

      } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino")) {
      //all others charged particles except geantino
      //ph->RegisterProcess(new G4hMultipleScattering, particle);
      //ph->RegisterProcess(new G4hIonisation,         particle);        
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);

	  }     
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Decay.hh"

void ORKALowEMPhysicsList::ConstructGeneral()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  
  // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    if (theDecayProcess->IsApplicable(*particle)) { 
      ph->RegisterProcess(theDecayProcess, particle);    
    }
  }
}
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4StepLimiter.hh"

void ORKALowEMPhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  G4StepLimiter* stepLimiter = new G4StepLimiter();
  ////G4UserSpecialCuts* userCuts = new G4UserSpecialCuts();
  
  theParticleIterator->reset();
  while ((*theParticleIterator)()){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (particle->GetPDGCharge() != 0.0)
        {
	  pmanager ->AddDiscreteProcess(stepLimiter);
	  ////pmanager ->AddDiscreteProcess(userCuts);
        }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKALowEMPhysicsList::SetCuts()
{
  //G4VUserPhysicsList::SetCutsWithDefault method sets 
  //the default cut value for all particle types 
  //
  SetCutsWithDefault();

  //SetCutValue(cutForGamma, "gamma", "DefaultRegionForTheWorld");
  //SetCutValue(cutForElectron, "e-", "DefaultRegionForTheWorld");
  //SetCutValue(cutForPositron, "e+", "DefaultRegionForTheWorld");
  //  G4cout << "PhysicsList: world cuts are set cutG= " << cutForGamma/mm 
  //	 << " mm    cutE= " << cutForElectron/mm << " mm " << G4endl;

  //G4cout << " cutV= " << fCutForScifiDetector 
  //     << " cutM= " << cutForMuonDetector<<G4endl;


  // Set the secondary production cut (for SetCut calculation)lower than 990. eV
  // Very important for high precision of lowenergy processes at low energies
 
  G4double lowLimit = 250. * eV;
  G4double highLimit = 100. * GeV;
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowLimit, highLimit);

 #ifdef G4_SCIFI_CUTS_USE
  G4Region* scifiRegion = (G4RegionStore::GetInstance())->GetRegion("ScifiDetector");
  //SetScifiCut(fCutForScifiDetector);
  scifiRegion->SetProductionCuts(new G4ProductionCuts());
  fScifiDetectorCuts = scifiRegion->GetProductionCuts();
  fScifiDetectorCuts->SetProductionCuts(fCutForScifiDetector);
  G4cout << "Scifi cuts are set" << G4endl;
 #endif
  
  G4Region* Xtalregion = (G4RegionStore::GetInstance())->GetRegion("XtalDetector");
  fXtalDetectorCuts = Xtalregion->GetProductionCuts();
  //fXtalDetectorCuts->SetProductionCut(fCutForXtalDetector, idxG4GammaCut);
  //fXtalDetectorCuts->SetProductionCut(fCutForXtalDetector, idxG4ElectronCut);
  //fXtalDetectorCuts->SetProductionCut(fCutForXtalDetector, idxG4PositronCut);
  Xtalregion->SetProductionCuts(new G4ProductionCuts());
  fXtalDetectorCuts = Xtalregion->GetProductionCuts();
  fXtalDetectorCuts->SetProductionCuts(fCutForXtalDetector);
  G4cout << "XtalDetector cuts are set" << G4endl;
 
  
  if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKALowEMPhysicsList::SetScifiCut(G4double cut)
{
#ifdef G4_SCIFI_CUTS_USE
  ////fCutForScifiDetector = cut;
  
  if( fScifiDetectorCuts ) {
    fScifiDetectorCuts->SetProductionCut(cut, idxG4GammaCut);
    fScifiDetectorCuts->SetProductionCut(cut, idxG4ElectronCut);
    fScifiDetectorCuts->SetProductionCut(cut, idxG4PositronCut);
  } 
#endif
}

