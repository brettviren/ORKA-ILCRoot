#ifndef ILCSISCONEJETFINDER_H
#define ILCSISCONEJETFINDER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */



//---------------------------------------------------------------------
// SISCone (FastJet v2.3.4) finder algorithm interface
//
// Author: swensy.jangal@ires.in2p3.fr
//  
//---------------------------------------------------------------------

// FastJet classes 
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"

// Get info on how fastjet was configured
#include "fastjet/config.h"
#ifdef ENABLE_PLUGIN_SISCONE
#include "fastjet/SISConePlugin.hh"
#endif

#include<sstream>  // needed for internal io
#include <vector> 
#include <cmath> 

#include "IlcFastJetHeaderV1.h"
#include "IlcJetFinder.h"

using namespace std;

class IlcSISConeJetFinder : public IlcJetFinder
{
 public:

  IlcSISConeJetFinder();
  ~IlcSISConeJetFinder();

  void    FindJets(); 

  // others
 
  void    WriteJHeaderToFile() const;
  Float_t EtaToTheta(Float_t arg);
  void    InitTask(TChain* tree);

  protected:
  IlcSISConeJetFinder(const IlcSISConeJetFinder& rfj);
  IlcSISConeJetFinder& operator = (const IlcSISConeJetFinder& rsfj);

  ClassDef(IlcSISConeJetFinder,2)
};

#endif
