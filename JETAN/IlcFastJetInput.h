#ifndef ILCFASTJETINPUT_H
#define ILCFASTJETINPUT_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
  //---------------------------------------------------------------------
// Class for input particles
// manages the search for jets 
// Authors: Elena Bruna elena.bruna@yale.edu
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


#include<sstream>  // needed for internal io
#include <vector> 
#include <cmath> 
#endif
class IlcJetFinder;
class IlcJetHeader;
class IlcJetReader;


class IlcFastJetInput : public TObject
{
 public:
    IlcFastJetInput();
    IlcFastJetInput(const IlcFastJetInput &input);
    IlcFastJetInput& operator=(const IlcFastJetInput& source);
    virtual ~IlcFastJetInput() {;}
    void SetHeader(IlcJetHeader *header)  {fHeader=header;}
    void SetReader(IlcJetReader *reader)  {fReader=reader;}
    void FillInput();
    vector<fastjet::PseudoJet> GetInputParticles()   const {return fInputParticles;}
    vector<fastjet::PseudoJet> GetInputParticlesCh() const {return fInputParticlesCh;}
    Float_t  EtaToTheta(Float_t arg);
    static Double_t Thermalspectrum(const Double_t *x, const Double_t *par);

 private:
   IlcJetReader *fReader;  //! reader 
   IlcJetHeader *fHeader;  //! header 
   
    vector<fastjet::PseudoJet> fInputParticles;     //! input particles for FastJet
    vector<fastjet::PseudoJet> fInputParticlesCh;   //! input charged particles for FastJet

  ClassDef(IlcFastJetInput, 1); // Analysis task for standard jet analysis
};
 
#endif
