#ifndef ILCJETHISTOS_H
#define ILCJETHISTOS_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//---------------------------------------------------------------------
// Jet Histos class
// Creates and fills a few cummon histograms for jet analysis
//
//---------------------------------------------------------------------

class TList;
class TClonesArray;
class TH1I;
class TH1F;

class IlcJetHistos : public TObject {
 public:
  IlcJetHistos();
  ~IlcJetHistos();

  void CreateHistos();
  void AddHistosToList(TList *list) const;
  void FillHistos(TClonesArray *jets);
  
 private:
  void SetProperties(TH1* h,const char* x, const char* y) const;
  IlcJetHistos(const IlcJetHistos &det);
  IlcJetHistos &operator=(const IlcJetHistos &det);

 private:

  TH1I *fNJetsH;           // distribution of number of jets
  TH1F *fPtH;              // pt spectra
  TH1F *fEtaH;             // eta distribution
  TH1F *fEneH;             // energy distribution
  TH1F *fPhiH;             // phi distribution

  ClassDef(IlcJetHistos,2) // some jet histos

};

#endif

    
