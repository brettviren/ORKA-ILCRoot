#ifndef ILCSISCONEJETFINDER_H
#define ILCSISCONEJETFINDER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//---------------------------------------------------------------------
// SISCone (FastJet v2.3.4) finder algorithm interface
//
// Author: swensy.jangal@ires.in2p3.fr
//  
// ** 2011 magali.estienne@subatech.in2p3.fr &  alexandre.shabetai@cern.ch
// Modified accordingly to reader/finder splitting and new handling of neutral information (via FastJetInput)
//---------------------------------------------------------------------

#include "IlcJetFinder.h"

class IlcFastJetHeaderV1;
class IlcFastJetInput;
class IlcFastJetBkg;
using namespace std;

class IlcSISConeJetFinder : public IlcJetFinder
{
 public:
  IlcSISConeJetFinder();
  ~IlcSISConeJetFinder();

  void    FindJets(); 

  // others
  Bool_t  ProcessEvent(); 
  void    WriteJHeaderToFile() const;

  protected:
  IlcSISConeJetFinder(const IlcSISConeJetFinder& rfj);
  IlcSISConeJetFinder& operator = (const IlcSISConeJetFinder& rsfj);

  IlcFastJetInput*  fInputFJ;     //! input particles array
  IlcFastJetBkg*    fJetBkg;      //! pointer to bkg class

  ClassDef(IlcSISConeJetFinder,3) // SISCONE analysis class

};

#endif
