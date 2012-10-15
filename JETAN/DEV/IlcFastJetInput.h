#ifndef ILCFASTJETINPUT_H
#define ILCFASTJETINPUT_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//---------------------------------------------------------------------
// Class for input particles
// manages the search for jets 
// Authors: Elena Bruna elena.bruna@yale.edu
//
// ** 2011 magali.estienne@subatech.in2p3.fr &  alexandre.shabetai@cern.ch             
// Modified accordingly to reader/finder splitting and new handling of neutral information
//---------------------------------------------------------------------

#include <TObject.h>

// FastJet classes 
#include "fastjet/PseudoJet.hh"

#include <vector> 

class IlcJetCalTrkEvent;
class IlcJetHeader;

class IlcFastJetInput : public TObject
{
 public:
  IlcFastJetInput();
  IlcFastJetInput(const IlcFastJetInput &input);
  IlcFastJetInput& operator=(const IlcFastJetInput& source);
  virtual                    ~IlcFastJetInput() {;}
  void                       SetHeader(IlcJetHeader *header)            {fHeader=header;}
  void                       SetCalTrkEvent(IlcJetCalTrkEvent *caltrk)  {fCalTrkEvent=caltrk;}
  void                       FillInput();
  vector<fastjet::PseudoJet> GetInputParticles()   const                {return fInputParticles;}
  vector<fastjet::PseudoJet> GetInputParticlesCh() const                {return fInputParticlesCh;}
  static Double_t            Thermalspectrum(const Double_t *x, const Double_t *par);

 private:
  IlcJetHeader *fHeader;                        //! header 
  IlcJetCalTrkEvent *fCalTrkEvent;              //! caltrkevent
   
  vector<fastjet::PseudoJet> fInputParticles;   //! input particles for FastJet
  vector<fastjet::PseudoJet> fInputParticlesCh; //! input charged particles for FastJet

  ClassDef(IlcFastJetInput, 2)                  //  fills input particles for FASTJET based analysis
    
};
 
#endif
