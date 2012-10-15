#ifndef ILCJETAODREADERHEADER_H
#define ILCJETAODREADERHEADER_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
// Jet AOD Reader Header
// Header for the AOD reader in the jet analysis
// Author: Davide Perrino (davide.perrino@cern.ch)

#include "IlcJetReaderHeader.h"
 
class IlcJetAODReaderHeader : public IlcJetReaderHeader
{

 public:
  IlcJetAODReaderHeader();
  virtual ~IlcJetAODReaderHeader();
  
  enum { kReadStdBranch = 1,
	 kReadNonStdBranch = 2 };

  // Getters
  Int_t   GetNaod()       const {return fNaod;}
  UInt_t  GetTestFilterMask()   const {return fTestFilterMask;}	    
  TString GetNonStdBranch()     const { return fNonStdBranch; }
  Bool_t  GetReadStdBranch()    const { return (fReadBranches & kReadStdBranch); }
  Bool_t  GetReadNonStdBranch() const { return (fReadBranches & kReadNonStdBranch); }

  // Setters
  virtual void SetNumberOfAOD(Int_t i=1) {fNaod = i;}    
  virtual void SetTestFilterMask(UInt_t i){fTestFilterMask = i;}
  virtual void SetReadAODMC(Short_t i){fReadMC = i;}
  virtual Short_t GetReadAODMC(){return fReadMC;}
  void SetNonStdBranch(TString name) { fNonStdBranch = name; }
  void SetReadBranches(UShort_t read) { fReadBranches = read; }

  enum { kDefault = 0, kAllMC = 1 , kChargedMC = 2};

 protected:
  Int_t   fNaod;           // number of aods
  UInt_t  fTestFilterMask; // Filter Mask for jets, not tested if 0
  Short_t fReadMC;      // Flag for reading the AODMC infomration, NB. this is not available on the flight...
  TString fNonStdBranch;       // non-standard branch to read additional tracks
  UShort_t fReadBranches;      // which branches to read from

  ClassDef(IlcJetAODReaderHeader,5);
};
 
#endif
