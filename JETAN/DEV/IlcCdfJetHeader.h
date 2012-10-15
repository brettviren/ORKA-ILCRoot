#ifndef ILCCDFJETHEADER_H
#define ILCCDFJETHEADER_H

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

// settings for jet finder process

#include "IlcJetHeader.h"

class IlcCdfJetHeader : public IlcJetHeader
{
 public:
  IlcCdfJetHeader();
  virtual ~IlcCdfJetHeader() { }

  // Getters
  Double_t GetJetPtCut() const      { return fJetPtCut; }
  Int_t    GetMinPartJet() const    { return fMinPartJet; }
  Bool_t   GetAnalyseJets() const   { return fkAnalyseJets; }

  // Setters
  void     SetJetPtCut(Double_t jetptcut)          { fJetPtCut = jetptcut; }
  void     SetAODwrite(Bool_t aodwrite)            { fAODwrite = aodwrite; }
  void     SetAODtracksWrite(Bool_t aodtrackswrite){ fAODtracksWrite = aodtrackswrite; }
  void     SetMinPartJet(Int_t npart)              { fMinPartJet = npart; }
  void     SetAnalyseJets(Bool_t flag = kTRUE)     { fkAnalyseJets = flag; }

  Bool_t   IsAODwrite() const       { return fAODwrite; }
  Bool_t   IsAODtracksWrite() const { return fAODtracksWrite; }

 protected:
  // Parameters of algorithm
  Int_t    fMinPartJet;           // minimum number of particles in jet
  Double_t fJetPtCut;             // pt cut of jets

  Bool_t   fAODwrite;             // flag for writing to AOD
  Bool_t   fAODtracksWrite;       // flag for writing tracks to AOD

  Bool_t   fkAnalyseJets;         // analyse jets 

  ClassDef ( IlcCdfJetHeader, 2 ) // CDF jet header class

};

#endif
