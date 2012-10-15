#ifndef ILCUA1JETFINDERV1_H
#define ILCUA1JETFINDERV1_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//---------------------------------------------------------------------
// UA1 Cone Algorithm Finder V1
// manages the search for jets
// Author: Rafael.Diaz.Valdes@cern.ch
// (version in c++)
//---------------------------------------------------------------------

#include "IlcJetFinder.h"
class IlcUA1JetHeaderV1;
class TH2F;
class TH1F;

class IlcUA1JetFinderV1 : public IlcJetFinder
{
 public:

  IlcUA1JetFinderV1();
  ~IlcUA1JetFinderV1();

  // others
  void FindJets();
  void RunAlgoritm(Float_t EtbgTotal, Double_t dEtTotal, Int_t& nJets,
                   Float_t* etJet,Float_t* etaJet, Float_t* phiJet,
                   Float_t* etallJet, Int_t* ncellsJet);

  void SubtractBackg(const Int_t& nIn, const Int_t&nJ, Float_t&EtbgTotalN,
		     const Float_t* ptT, const Float_t* etaT, const Float_t* phiT,
                     Float_t* etJet, const Float_t* etaJet, const Float_t* phiJet,
		     Float_t* etsigJet,Int_t* multJet, Int_t* injet);
  
  void SubtractBackgCone(const Int_t& nIn, const Int_t&nJ,Float_t& EtbgTotalN,
			 const Float_t* ptT, const Float_t* etaT, const Float_t* phiT,
			 Float_t* etJet, const Float_t* etaJet, const Float_t* phiJet,
			 Float_t* etsigJet, Int_t* multJet, Int_t* injet);
  
  void SubtractBackgRatio(const Int_t& nIn, const Int_t&nJ,Float_t& EtbgTotalN,
			  const Float_t* ptT, const Float_t* etaT, const Float_t* phiT,
			  Float_t* etJet, const Float_t* etaJet, const Float_t* phiJet,
			  Float_t* etsigJet, Int_t* multJet, Int_t* injet);
  
  void SubtractBackgStat(const Int_t& nIn, const Int_t&nJ,Float_t&EtbgTotalN,
			 const Float_t* ptT, const Float_t* etaT, const Float_t* phiT,
			 Float_t* etJet, const Float_t* etaJet, const Float_t* phiJet,
			 Float_t* etsigJet, Int_t* multJet, Int_t* injet);
  void Reset();
  void Init();
  void WriteJHeaderToFile() const;
  
  enum {kMaxJets = 30};

 protected:
  IlcUA1JetFinderV1(const IlcUA1JetFinderV1& rJetF1);
  IlcUA1JetFinderV1& operator = (const IlcUA1JetFinderV1& rhsf);
  TH2F*          fLego;           //Lego Histo
  // temporary histos for background, reset for each event, no need to stream
  TH1F*          fhEtJet[kMaxJets];   //! histogram for background subtraction
  TH1F*          fhAreaJet[kMaxJets]; //! histogram for background subtraction (store global not to create it with every event
  TH1F*          fhEtBackg;           //! histogram for background subtraction
  TH1F*          fhAreaBackg;           //! histogram for background subtraction

  //

  ClassDef(IlcUA1JetFinderV1,2)
};

#endif
