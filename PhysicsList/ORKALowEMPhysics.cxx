//---------------------------------------------------------------------------
//
// ClassName:   ORKALowEMPhysics
//
// Author: 9 June 2012 C. Gatto
//
// Modified:
//
//----------------------------------------------------------------------------
//

#include "ORKALowEMPhysics.h"
#include "ORKALowEMPhysicsList.h"

#include "G4LossTableManager.hh"
#include "G4EmProcessOptions.hh"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
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
#include "G4CoulombScattering.hh"
#include "G4WentzelVIModel.hh"
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4GammaNuclearReaction.hh"
#include "G4PhotoNuclearProcess.hh"



ORKALowEMPhysics::ORKALowEMPhysics(G4int verb)
  : G4VPhysicsConstructor("ORKALowEM"), fVerbose(verb), 
    fWasActivated(false)
{
  if(fVerbose > 1) { 
    G4cout << "### ORKALowEMPhysics: " << GetPhysicsName() 
	   << G4endl; 
  }
  G4LossTableManager::Instance();
  SetPhysicsType(bElectromagnetic);
  fMainORKALowEnergyBuilder = new ORKALowEMPhysicsList();
}

ORKALowEMPhysics::~ORKALowEMPhysics()
{
  delete fMainORKALowEnergyBuilder;
}

void ORKALowEMPhysics::ConstructParticle()
{
  // G4cout << "G4HadronElasticPhysics::ConstructParticle" << G4endl;
  fMainORKALowEnergyBuilder->ConstructParticle();
}

void ORKALowEMPhysics::ConstructProcess()
{
  if(fWasActivated) { return; }
  fWasActivated = true;

  //fMainORKALowEnergyBuilder->ConstructProcess();	// Unclear if this methods of registering Physics works correctly
  ConstructLocally();

  if(fVerbose > 1) {
    G4cout << "### ORKALowEM Physics is constructed " 
	   << G4endl;
  }
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKALowEMPhysics::ConstructLocally()
{
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();

  // Add standard EM Processes
  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4String particleName = particle->GetParticleName();
    if(fVerbose > 1)
      G4cout << "### " << GetPhysicsName() << " instantiates for " 
	     << particleName << G4endl;

    if (particleName == "gamma") {


	 // use EM process from BrachiTherapy example
      G4RayleighScattering* theRayleigh = new G4RayleighScattering();
      theRayleigh->SetModel(new G4LivermoreRayleighModel());  //not strictly necessary
      //pmanager->AddDiscreteProcess(theRayleigh);
      ph->RegisterProcess(theRayleigh, particle);

      G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
      thePhotoElectricEffect->SetModel(new G4LivermorePhotoElectricModel());
      //pmanager->AddDiscreteProcess(thePhotoElectricEffect);
      ph->RegisterProcess(thePhotoElectricEffect, particle);
	
      G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
      theComptonScattering->SetModel(new G4LivermoreComptonModel());
      //pmanager->AddDiscreteProcess(theComptonScattering);
      ph->RegisterProcess(theComptonScattering, particle);
	
      G4GammaConversion* theGammaConversion = new G4GammaConversion();
      theGammaConversion->SetModel(new G4LivermoreGammaConversionModel());
      //pmanager->AddDiscreteProcess(theGammaConversion);
      ph->RegisterProcess(theGammaConversion, particle);


    } else if (particleName == "e-") {

	  // use EM process from BrachiTherapy example
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      msc->SetStepLimitType(fUseDistanceToBoundary);
      //pmanager->AddProcess(msc,-1, 1, 1);
      ph->RegisterProcess(msc, particle);

      // Ionisation
      G4eIonisation* eIonisation = new G4eIonisation();
      eIonisation->SetEmModel(new G4LivermoreIonisationModel());
      eIonisation->SetStepFunction(0.2, 100*um); //improved precision in tracking  
      //pmanager->AddProcess(eIonisation,-1, 2, 2);
      ph->RegisterProcess(eIonisation, particle);
	
      // Bremsstrahlung
      G4eBremsstrahlung* eBremsstrahlung = new G4eBremsstrahlung();
      eBremsstrahlung->SetEmModel(new G4LivermoreBremsstrahlungModel());
      //pmanager->AddProcess(eBremsstrahlung, -1,-3, 3);
      ph->RegisterProcess(eBremsstrahlung, particle);
	 
    } else if (particleName == "e+") {

	  // use EM process from BrachiTherapy example
      G4eMultipleScattering* msc = new G4eMultipleScattering();
      msc->SetStepLimitType(fUseDistanceToBoundary);
      //pmanager->AddProcess(msc,-1, 1, 1);
      ph->RegisterProcess(msc, particle);

      // Ionisation
      G4eIonisation* eIonisation = new G4eIonisation();
      //eIonisation->SetEmModel(new G4LivermoreIonisationModel());
      eIonisation->SetStepFunction(0.2, 100*um); //improved precision in tracking  
      //pmanager->AddProcess(eIonisation,-1, 2, 2);
      ph->RegisterProcess(eIonisation, particle);

      //Bremsstrahlung (use default, no low-energy available)
      ph->RegisterProcess(new G4eBremsstrahlung(), particle);
      ph->RegisterProcess(new G4eplusAnnihilation(), particle);

    } else if (particleName == "mu+" ||
               particleName == "mu-"    ) {

      G4MuMultipleScattering* msc = new G4MuMultipleScattering();
      msc->AddEmModel(0, new G4WentzelVIModel());

      ph->RegisterProcess(msc, particle);
      ph->RegisterProcess(new G4MuIonisation(), particle);
      ph->RegisterProcess(new G4MuBremsstrahlung(), particle);
      ph->RegisterProcess(new G4MuPairProduction(), particle);
      ph->RegisterProcess(new G4CoulombScattering(), particle);

    } else if (particleName == "alpha" ||
               particleName == "He3") {

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(new G4ionIonisation(), particle);

    } else if (particleName == "GenericIon") {

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(new G4ionIonisation(), particle);

    } else if (particleName == "pi+" ||
               particleName == "pi-" ||
	       particleName == "kaon+" ||
               particleName == "kaon-" ||
               particleName == "proton" ) {

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(new G4hIonisation(), particle);
      ph->RegisterProcess(new G4hBremsstrahlung(), particle);
      ph->RegisterProcess(new G4hPairProduction(), particle);

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

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(new G4hIonisation(), particle);

    } else if ((!particle->IsShortLived()) &&
	       (particle->GetPDGCharge() != 0.0) && 
	       (particle->GetParticleName() != "chargedgeantino") ) {

      ph->RegisterProcess(new G4hMultipleScattering(), particle);
      ph->RegisterProcess(new G4hIonisation(), particle);

    }
  }
  G4EmProcessOptions opt;
  opt.SetVerbose(fVerbose);
  opt.SetPolarAngleLimit(0.2);

  // Deexcitation
  //
  G4VAtomDeexcitation* de = new G4UAtomicDeexcitation();
  G4LossTableManager::Instance()->SetAtomDeexcitation(de);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


