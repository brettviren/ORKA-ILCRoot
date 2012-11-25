// $Id: ORKARunConfiguration.cxx 442 2010-01-21 15:47:00Z ivana $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file ORKARunConfiguration.cxx
/// \brief Implementation of the ORKARunConfiguration class 

#include "ORKARunConfiguration.h"

#include "TG4ComposedPhysicsList.h"
#include "TG4SpecialPhysicsList.h"

#include <QGSP_BERT_HP.hh>
// #include <QGSP_BERT.hh>
#include <G4EmLivermorePhysics.hh>
#include "G4LossTableManager.hh"
#include "G4RunManager.hh"
#include "ORKAAllPhysicsList.h"

//_____________________________________________________________________________
ORKARunConfiguration::ORKARunConfiguration(const TString& userGeometry,
					     const TString& physicsList,
                                             const TString& specialProcess)
  : TG4RunConfiguration(userGeometry, "", specialProcess),
  fPhysicsList(physicsList)
{
/// Standard constructor
/// \param userGeometry    Selection of geometry input and navigation
/// \param physicsList     Selection of the physicsList
/// \param specialProcess  Selection of the special processes
///
/// \see More on the available option in class TG4RunConfiguration:
/// http://ivana.home.cern.ch/ivana/g4vmc_html/classTG4RunConfiguration.html
}

//_____________________________________________________________________________
ORKARunConfiguration::~ORKARunConfiguration()
{
/// Destructor
}

//
// protected methods
//

//_____________________________________________________________________________
G4VUserPhysicsList*  ORKARunConfiguration::CreatePhysicsList()
{
/// Override the default physics list with user defined physics list;

  TG4ComposedPhysicsList* builder = new TG4ComposedPhysicsList();

  // User physics list
  G4cout << "Adding user physics list for ORKA" << G4endl;
  
  G4VModularPhysicsList* physics = new ORKAAllPhysicsList(fPhysicsList);
  // limit step size in fibers. Low energy EM physics + photonuclear process + hadrons +
  // G4EmExtraPhysics() + G4HadronElasticPhysicsHP() + G4QStoppingPhysics() + G4IonPhysics() +
  // G4NeutronTrackingCut() + QGSP_BERT_HP

  builder->AddPhysicsList(physics);


  G4cout << "Adding SpecialPhysicsList " << G4endl;

  builder->AddPhysicsList(new TG4SpecialPhysicsList(fSpecialProcessSelection.Data()));
  
  return builder;  
}
