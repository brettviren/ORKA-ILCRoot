#ifndef ILCUA1JETFINDER_H
#define ILCUA1JETFINDER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//---------------------------------------------------------------------
// UA1 Cone Algorithm Finder 
// manages the search for jets
// Author: Rafael.Diaz.Valdes@cern.ch
// (version in c++)
// ** 2011
// Modified accordingly to reader/finder splitting and new handling of neutral information
// Versions V1 and V2 merged
//---------------------------------------------------------------------

#include "IlcJetFinder.h"

class TH2F;
class IlcJetBkg;

class IlcUA1JetFinder : public IlcJetFinder
{
 public:

  IlcUA1JetFinder();
  ~IlcUA1JetFinder();

  // others
  void FindJets();
  void RunAlgoritm(Float_t EtbgTotal, Double_t dEtTotal, Int_t& nJets,
		   Float_t* const etJet,Float_t* const etaJet, Float_t* const phiJet,
		   Float_t* const etallJet, Int_t* const ncellsJet);

  void Reset();
  void Init();
  void WriteJHeaderToFile() const;

  enum {kMaxJets = 60};

 protected:
  IlcUA1JetFinder(const IlcUA1JetFinder& rJetF1);
  IlcUA1JetFinder& operator = (const IlcUA1JetFinder& rhsf);

  TH2F*       fLego;          //  Lego Histo

  IlcJetBkg*  fJetBkg;        //! pointer to bkg class

  ClassDef(IlcUA1JetFinder,3) //  UA1 jet finder

};

#endif
