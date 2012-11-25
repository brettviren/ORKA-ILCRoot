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

#ifndef ORKALowEMPhysics_h
#define ORKALowEMPhysics_h 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"
#include "ORKALowEMPhysicsList.h"

class G4HadronElasticPhysics;

class ORKALowEMPhysics : public G4VPhysicsConstructor
{
public: 

  ORKALowEMPhysics(G4int verb = 1); 

  virtual ~ORKALowEMPhysics();

  // This method will be invoked in the Construct() method. 
  // each particle type will be instantiated
  virtual void ConstructParticle();
 
  // This method will be invoked in the Construct() method.
  // each physics process will be instantiated and
  // registered to the process manager of each particle type 
  virtual void ConstructProcess();

private:
  virtual void ConstructLocally();

private:

  G4int    fVerbose;
  G4bool   fWasActivated;
  ORKALowEMPhysicsList* fMainORKALowEnergyBuilder;
};


#endif








