#ifndef ILCFASTJETBKG_H
#define ILCFASTJETBKG_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//---------------------------------------------------------------------
// Class to calculate the background per unit area
// manages the search for jets 
// Authors: Elena Bruna elena.bruna@yale.edu
//          Sevil Salur ssalur@lbl.gov
//  
// 2011 :
// renamed from IlcJetBkg to IlcFastJetBkg as this class uses only FASTJET based algos        
//---------------------------------------------------------------------

class TString;
class IlcFastJetInput;

class IlcFastJetBkg : public TObject
{
 public:
  IlcFastJetBkg();
  IlcFastJetBkg(const IlcFastJetBkg &input);
  IlcFastJetBkg& operator=(const IlcFastJetBkg& source);
  virtual          ~IlcFastJetBkg() {;}
  void             SetHeader(IlcJetHeader *header)  {fHeader=header;}
  void             SetFastJetInput(IlcFastJetInput *fjinput)  {fInputFJ=fjinput;}
  void             BkgFastJetb(Double_t& x,Double_t& y, Double_t& z);
  void             BkgFastJetWoHardest(Double_t& x,Double_t& y, Double_t& z);
  Float_t          BkgFastJet();
  Float_t          BkgChargedFastJet();
  Float_t          BkgStat();
  Float_t          BkgFastJetCone(TClonesArray* fAODJets);

  Bool_t           EmcalAcceptance(const Float_t eta, const Float_t phi, const Float_t radius) const;
  static Double_t  BkgFunction(Double_t *x,Double_t *par);
    
 private:
  Double_t         CalcRho(vector<fastjet::PseudoJet> input_particles,Double_t RparamBkg,TString method);
  void             CalcRhob(Double_t& median, Double_t& sigma, Double_t& meanarea,
			    vector<fastjet::PseudoJet> input_particles,Double_t RparamBkg,TString method);
  void             CalcRhoWoHardest(Double_t& median, Double_t& sigma, Double_t& meanarea,
				    vector<fastjet::PseudoJet> input_particles,Double_t RparamBkg,TString method);

  IlcJetHeader*    fHeader;  //! header
  IlcFastJetInput* fInputFJ; //! input particles

  ClassDef(IlcFastJetBkg, 2)   //  Fastjet backgroud analysis
 
};
 
#endif
