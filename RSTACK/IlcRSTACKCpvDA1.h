#ifndef IlcRSTACKCPVDA1_H
#define IlcRSTACKCPVDA1_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////////
// Class IlcRSTACKCpvDA1 accumulates histograms with amplitudes per CPV channel.
// It is intended to run at DAQ or HLT computers.
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "TH1.h"
#include "TFile.h"

class IlcRSTACKCpvDA1 : public TNamed {
  
 public:
  
  IlcRSTACKCpvDA1(Int_t module);
  IlcRSTACKCpvDA1(const IlcRSTACKCpvDA1& );
  IlcRSTACKCpvDA1& operator= (const IlcRSTACKCpvDA1& );
  ~IlcRSTACKCpvDA1();
  
  void  FillHistograms(Float_t e[128][56]);
  Int_t GetModule() { return fMod; }
  void  UpdateHistoFile();
  
 private:

  TFile* fHistoFile;            // root file to store histograms in
  TH1F*  fCharge[128][56];      // charge deposited on CPV pads
  Int_t  fMod;                  // RSTACK module number (0..4)
  
  ClassDef(IlcRSTACKCpvDA1,1)

};

#endif
