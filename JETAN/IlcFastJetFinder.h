#ifndef ILCFASTJETFINDER_H
#define ILCFASTJETFINDER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */



//---------------------------------------------------------------------
// FastJet v2.3.4 finder algorithm interface
//
// Author: Rafael.Diaz.Valdes@cern.ch
//  
//---------------------------------------------------------------------

//FastJet classes 
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/JetDefinition.hh"
// get info on how fastjet was configured
#include "fastjet/config.h"
#ifdef ENABLE_PLUGIN_SISCONE
#include "fastjet/SISConePlugin.hh"
#endif


#include<sstream>  // needed for internal io
#include <vector> 
#include <cmath> 

#include "IlcJetFinder.h"
#include "IlcFastJetHeaderV1.h"

using namespace std;
class IlcFastJetInput;
class IlcJetBkg;

class IlcFastJetFinder : public IlcJetFinder
{
 public:

  IlcFastJetFinder();
  ~IlcFastJetFinder();

  virtual void    FindJets(); 
  void    RunTest(const char* datafile); // a simple test
  virtual void    WriteJHeaderToFile() const;
  Float_t EtaToTheta(Float_t arg);
  void    InitTask(TChain* tree);
  virtual Bool_t ProcessEvent();
  virtual Bool_t  ProcessEvent2();
  
      
  protected:
  IlcFastJetFinder(const IlcFastJetFinder& rfj);
  IlcFastJetFinder& operator = (const IlcFastJetFinder& rsfj);
  IlcFastJetInput*                fInputFJ;     //! input particles array
  IlcJetBkg*                      fJetBkg;      //! pointer to bkg class
  ClassDef(IlcFastJetFinder,2)
};

#endif
