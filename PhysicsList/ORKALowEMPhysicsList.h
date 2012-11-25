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
// $Id: ORKALowEMPhysicsList.hh,v 1.12 2008-09-22 16:41:20 maire Exp $
// GEANT4 tag $Name: geant4-09-05-beta-01 $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ORKALowEMPhysicsList_h
#define ORKALowEMPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
//#include "G4VModularPhysicsList.hh"


#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ORKALowEMPhysicsList: public G4VUserPhysicsList
//class ORKALowEMPhysicsList: public G4VModularPhysicsList  // G4VUserPhysicsList
{
  public:
    ORKALowEMPhysicsList();
   ~ORKALowEMPhysicsList();
   void SetScifiCut(G4double val);
    // Construct particle and physics
    void ConstructParticle();
    void ConstructProcess();

  protected:
 
    void SetCuts();

   
  protected:
    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    void ConstructOthers();

  protected:
  // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    void AddStepMax();


private:

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

 
