#ifndef ORKA_RUN_CONFIGURATION2_H
#define ORKA_RUN_CONFIGURATION2_H

// $Id: ORKARunConfiguration.h 341 2008-05-26 11:04:57Z ivana $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007, 2008 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file ORKARunConfiguration.h 
/// \brief Definition of the ORKARunConfiguration class 
///
/// Geant4 ExampleN03 adapted to Virtual Monte Carlo
///
/// \author I. Hrivnacova; IPN, Orsay
#include <globals.hh>

#include "TG4RunConfiguration.h"

/// \ingroup E03
/// \brief User Geant4 VMC run configuration
///
/// This class overrides setting of the default Geant4 VMC physics list
/// with the LHEP_BERT hadronics physics list
///
/// \author I. Hrivnacova; IPN, Orsay

class ORKARunConfiguration : public TG4RunConfiguration
{
  public:
    ORKARunConfiguration(const TString& userGeometry,
			  const TString& physicsList = "emStandard",
                          const TString& specialProcess = "stepLimiter" );
    virtual ~ORKARunConfiguration();

    // methods
    virtual G4VUserPhysicsList*  CreatePhysicsList();
    
  private:
    G4String fPhysicsList;

};

#endif //ORKA_RUN_CONFIGURATION_H

