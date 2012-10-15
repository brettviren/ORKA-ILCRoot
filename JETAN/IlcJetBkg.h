#ifndef ILCJETBKG_H
#define ILCJETBKG_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 //---------------------------------------------------------------------
// Class to calculate the background per unit area
// manages the search for jets 
// Authors: Elena Bruna elena.bruna@yale.edu
//          Sevil Salur ssalur@lbl.gov
//          
//---------------------------------------------------------------------


class IlcJetFinder;
class IlcESDEvent;
class TTree;
class TChain;
class TString;
class IlcAODEvent;
class IlcJetHistos;
class IlcFastJetInput;

class IlcJetBkg : public TObject
{
 public:
    IlcJetBkg();
    IlcJetBkg(const IlcJetBkg &input);
    IlcJetBkg& operator=(const IlcJetBkg& source);
    virtual ~IlcJetBkg() {;}
    void SetHeader(IlcJetHeader *header)  {fHeader=header;}
    void SetReader(IlcJetReader *reader)  {fReader=reader;}
    void SetFastJetInput(IlcFastJetInput *fjinput)  {fInputFJ=fjinput;}
    void BkgFastJetb(Double_t& x,Double_t& y, Double_t& z);
    void BkgFastJetWoHardest(Double_t& x,Double_t& y, Double_t& z);
    Float_t BkgFastJet();
    Float_t BkgChargedFastJet();
    Float_t BkgStat();
    Float_t BkgFastJetCone(TClonesArray* fAODJets);
//    Float_t BkgRemoveJetLeading(TClonesArray* fAODJets);
    Float_t BkgRemoveJetLeadingFromUArray(TClonesArray* fAODJets);
    Float_t EtaToTheta(Float_t arg);
    Bool_t EmcalAcceptance(const Float_t eta, const Float_t phi, const Float_t radius) const;
    static Double_t BkgFunction(Double_t *x,Double_t *par);
    
 private:
    Double_t CalcRho(vector<fastjet::PseudoJet> input_particles,Double_t RparamBkg,TString method);
    void CalcRhob(Double_t& median, Double_t& sigma, Double_t& 
meanarea,vector<fastjet::PseudoJet> input_particles,Double_t RparamBkg,TString 
method);
    void CalcRhoWoHardest(Double_t& median, Double_t& sigma, Double_t& 
meanarea,vector<fastjet::PseudoJet> input_particles,Double_t RparamBkg,TString 
method);
    IlcJetReader *fReader;   //! reader
    IlcJetHeader *fHeader;   //! header
    IlcFastJetInput *fInputFJ; //! input particles

  ClassDef(IlcJetBkg, 1); // Analysis task for standard jet analysis
};
 
#endif
