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
// $Id: PhysicsList.cc,v 1.37 2010-11-19 20:12:32 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-05-beta-01 $
//
/////////////////////////////////////////////////////////////////////////
//
// PhysicsList
//
// Created: 31.04.2006 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
// 26.04.2007 Physics according to 8.3 Physics List (V.Ivanchenko)
//
////////////////////////////////////////////////////////////////////////
// 

#include "ORKAAllPhysicsList.h"
#include "ORKALowEMPhysics.h"
//#include "PhysicsListMessenger.hh"

#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"

#include "G4DecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsLHEP.hh"
#include "G4HadronQElasticPhysics.hh"
#include "G4ChargeExchangePhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4NeutronCrossSectionXS.hh"
#include "G4QStoppingPhysics.hh"
#include "G4LHEPStoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonFTFPBinaryCascadePhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4EmProcessOptions.hh"

#include "HadronPhysicsQGSP_BERT_HP.hh"

#include "G4IonPhysics.hh"

#include "G4LossTableManager.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

ORKAAllPhysicsList::ORKAAllPhysicsList(const G4String& PhysicsList) : G4VModularPhysicsList(),
fcutForGamma(0.),  
fcutForElectron(0.),
fcutForPositron(0.),
fcutForProton(0.),
femPhysicsList(0x0),
fparticleList(0x0),
fhadronPhys(0x0)
{
  
   if (PhysicsList == "ORKA_PL_v1") {

     BuildPhysicsListORKAv1();

  } else {
    G4cout << "ORKAAllPhysicsList: <" << PhysicsList << ">"
           << " is not defined"
           << G4endl;
	   List();

  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

ORKAAllPhysicsList::~ORKAAllPhysicsList()
{
  //delete pMessenger;
  delete fparticleList;
  delete femPhysicsList;
  for(size_t i=0; i<fhadronPhys.size(); i++) {
    delete fhadronPhys[i];
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void ORKAAllPhysicsList::ConstructParticle()
{
  fparticleList->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void ORKAAllPhysicsList::ConstructProcess()
{
  AddTransportation();
  femPhysicsList->ConstructProcess();
  fparticleList->ConstructProcess();
  for(size_t i=0; i<fhadronPhys.size(); i++) {
    fhadronPhys[i]->ConstructProcess();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
void ORKAAllPhysicsList::BuildPhysicsListORKAv1()
{
    G4LossTableManager::Instance();

  // The production threshold is fixed to 0.1 mm for all the particles
  // Secondary particles with a range bigger than 0.1 mm 
  // are generated; otherwise their energy is considered deposited locally

  SetDefaultCutValue(0.1 * mm);

  fcutForGamma = GetDefaultCutValue();
  fcutForElectron = GetDefaultCutValue();
  fcutForPositron = GetDefaultCutValue();
  fcutForPositron = GetDefaultCutValue();

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

  verboseLevel    = 1;

  // Particles
  fparticleList = new G4DecayPhysics("decays");

  // EM physics
  femPhysicsList = new ORKALowEMPhysics();
  //ReplacePhysics(new G4EmLivermorePhysics(verboseLevel)); 

  // Hadron  physics
  fhadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
  fhadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
  fhadronPhys.push_back( new G4QStoppingPhysics(verboseLevel));
  fhadronPhys.push_back( new G4IonPhysics(verboseLevel));
  fhadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));

  
  fhadronPhys.push_back( new HadronPhysicsQGSP_BERT_HP());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void ORKAAllPhysicsList::SetCuts()
{

  if (verboseLevel >0){
    G4cout << "ORKAAllPhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }



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
#else
   // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(fcutForGamma, "gamma");
  SetCutValue(fcutForElectron, "e-");
  SetCutValue(fcutForPositron, "e+");
  SetCutValue(fcutForProton, "proton");
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void ORKAAllPhysicsList::SetCutForGamma(G4double cut)
{
  fcutForGamma = cut;
  SetParticleCuts(fcutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKAAllPhysicsList::SetCutForElectron(G4double cut)
{
  fcutForElectron = cut;
  SetParticleCuts(fcutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKAAllPhysicsList::SetCutForPositron(G4double cut)
{
  fcutForPositron = cut;
  SetParticleCuts(fcutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKAAllPhysicsList::SetCutForProton(G4double cut)
{
  fcutForProton = cut;
  SetParticleCuts(fcutForProton, G4Proton::Proton());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ORKAAllPhysicsList::SetScifiCut(G4double cut)
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

void ORKAAllPhysicsList::List()
{
  G4cout << "### PhysicsLists available: ORKA_PL_v1"
	 << G4endl;
//   G4cout << "                            LHEP LHEP_EMV QBBC QGS_BIC QGSP"
// 	 << G4endl; 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

