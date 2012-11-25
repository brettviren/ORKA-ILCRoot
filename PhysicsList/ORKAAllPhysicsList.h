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
// $Id: PhysicsList.hh,v 1.9 2010-11-19 20:12:32 vnivanch Exp $
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
//
////////////////////////////////////////////////////////////////////////
// 

#ifndef ORKAAllPhysicsList_h
#define ORKAAllPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4VPhysicsConstructor;
//class PhysicsListMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ORKAAllPhysicsList: public G4VModularPhysicsList
{
public:

  ORKAAllPhysicsList(const G4String& PhysicsList);
  virtual ~ORKAAllPhysicsList();
  void SetScifiCut(G4double val);

  void ConstructParticle();
    
  void BuildPhysicsListORKAv1();

  void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);
  void SetCutForProton(G4double);
        
  void ConstructProcess();
  void List();
  
private:

  G4double fcutForGamma;
  G4double fcutForElectron;
  G4double fcutForPositron;
  G4double fcutForProton;

  G4VPhysicsConstructor*  femPhysicsList;
  G4VPhysicsConstructor*  fparticleList;
  std::vector<G4VPhysicsConstructor*> fhadronPhys;
    
  //PhysicsListMessenger* pMessenger;
  //G4bool dump;

#ifdef G4_SCIFI_CUTS_USE
  G4ProductionCuts* fScifiDetectorCuts;
  //G4double fCutForScifiDetector;
  std::vector<double> fCutForScifiDetector; 
#endif

  G4ProductionCuts* fXtalDetectorCuts;
  //G4double fCutForXtalDetector;
  std::vector<double> fCutForXtalDetector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

