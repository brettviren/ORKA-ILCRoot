#ifndef ILCFASTJETFINDER_H
#define ILCFASTJETFINDER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//---------------------------------------------------------------------
// FastJet v2.3.4 finder algorithm interface
//
// Author: Rafael.Diaz.Valdes@cern.ch
//
// ** 2011 magali.estienne@subatech.in2p3.fr &  alexandre.shabetai@cern.ch
// new implementation of background subtraction
// allowing to subtract bkg using a different algo than the one used for signal jets  
//---------------------------------------------------------------------

// FastJet classes 
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/JetDefinition.hh"

#include "IlcJetFinder.h"

class IlcFastJetInput;
class IlcFastJetBkg;

using namespace std;

class IlcFastJetFinder : public IlcJetFinder
{
 public:

  IlcFastJetFinder();
  ~IlcFastJetFinder();

  virtual void      FindJets(); 
  void              RunTest(const char* datafile); // a simple test
  virtual void      WriteJHeaderToFile() const;
  virtual Bool_t    ProcessEvent();
      
  protected:
  IlcFastJetFinder(const IlcFastJetFinder& rfj);
  IlcFastJetFinder& operator = (const IlcFastJetFinder& rsfj);
  IlcFastJetInput*  fInputFJ;  //! input particles array
  IlcFastJetBkg*    fJetBkg;   //! pointer to bkg class

  ClassDef(IlcFastJetFinder,3) //  Fastjet analysis class

};

#endif
