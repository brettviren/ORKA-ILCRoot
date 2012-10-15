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
  Double_t GetRadius   () const { return fRadius; }
  Double_t GetJetPtCut () const { return fJetPtCut ; }
  Int_t GetMinPartJet  () const { return fMinPartJet ; }

  // Setters
  void SetRadius         ( Double_t radius )          { fRadius = radius; }
  void SetJetPtCut       ( Double_t jetptcut )        { fJetPtCut = jetptcut; }
  void SetAODwrite       ( Bool_t aodwrite )          { fAODwrite = aodwrite ; }
  void SetAODtracksWrite ( Bool_t aodtrackswrite )    { fAODtracksWrite = aodtrackswrite ; }
  void SetMinPartJet     ( Int_t npart )              { fMinPartJet = npart ; }

//  void SetCDFJetHeader   () { fCDFheader = (IlcCdfJetHeader*)fHeader; }

  Bool_t IsAODwrite() const { return fAODwrite ; }
  Bool_t IsAODtracksWrite() const { return fAODtracksWrite ; }

//     void PrintParameters() const ;

  protected:

  IlcCdfJetHeader(const IlcCdfJetHeader &jh);
  IlcCdfJetHeader& operator=(const IlcCdfJetHeader &jh);

  // parameters of algorithm
  Double_t fRadius ;      //  Cone radius
  Int_t  fMinPartJet ;       // minimum number of particles in jet

  // JET Pt cut
  Double_t fJetPtCut ;  // pt cut of jets

  Bool_t fAODwrite ;         // flag for writing to AOD
  Bool_t fAODtracksWrite ;   // flag for writing tracks to AOD

//  IlcCdfJetHeader* fCDFheader ; // local pointer to CDF Jet Header

  ClassDef ( IlcCdfJetHeader, 1 )

  };
#endif
