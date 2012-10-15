#ifndef ILCCDFJETFINDER_H
#define ILCCDFJETFINDER_H

/*
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See cxx source for full Copyright notice
 *
*/

//  Definition of constants, structures and functions for jet finder

#include "IlcJetFinder.h"

using namespace std ;


// structure of jet and particles container
struct varContainer
  {
  Double_t  pt;  // pt of particle/jet
  Double_t eta;  // eta of particle/jet
  Double_t phi;  // phi of particle/jet
  Int_t    njet; // njet is multiplicity of jet or if particles are stored , njet is index number of jet 
  } ;

class IlcCdfJetHeader;

class IlcCdfJetFinder : public IlcJetFinder
  {
  public:

    IlcCdfJetFinder();
    virtual ~IlcCdfJetFinder();

    void           CreateOutputObjects(TList * const histos);
    void           FindJets();
    void           InitData();
    void           FindCones();
    void           ComputeConesWeight();
    void           WriteJets() ;
    void           AnalizeJets();
    void           Clean();
    
    virtual void   FinishRun();

    void           SetAnalyseJets(Bool_t flag = kTRUE) {fAnalyseJets = flag;}
    
  protected:
    IlcCdfJetFinder ( const IlcCdfJetFinder& jf );
    IlcCdfJetFinder& operator = ( const IlcCdfJetFinder& jf );

    TList         *fHistos;    // List of histograms

    Bool_t fFromAod ;          // is the data taken from AOD (or ESD filter) 
    Bool_t fAODwrite ;         // write jets to AOD
    Bool_t fAODtracksWrite ;   // write jet tracks to AOD
    Bool_t fAnalyseJets;       // analyse jets
	
    TRefArray *fRefArr ;       //! pointer to references array of tracks from AOD

    Int_t         fNJets;     //! counter of number of jets
    Int_t         fNPart;     //! number of particles in event

    Double_t      fRadius ;   // radius of jet 

    Int_t fMinJetParticles;   // leading jet must contain AT LEAST fMinJetParticles
    Double_t fJetPtCut;       // leading jet must have AT LEAST fJetPtCut

    varContainer **fVectParticle; //! container for Particles
    varContainer **fVectJet;      //! container for Jets

    Double_t *fPtArray;  //! momentum array
    Int_t    *fIdxArray;  //! index array of sorted pts


    ClassDef(IlcCdfJetFinder,2)
   };
#endif

