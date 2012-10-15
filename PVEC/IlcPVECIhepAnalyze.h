#ifndef IlcPVECIhepAnalyze_H
#define IlcPVECIhepAnalyze_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//_________________________________________________________________________
// Algorythm class to analyze PVECv1 events:
// Construct histograms and displays them.
// Used the IHEP CPV/PVEC reconstruction algorithm.
//*--
//*-- Author : Boris Polichtchouk (IHEP)

// --- ROOT system ---
#include "TObject.h"

// --- Standard library ---

// --- IlcRoot header files ---
class IlcRunLoader;

class IlcPVECIhepAnalyze : public TObject {

 public:

  IlcPVECIhepAnalyze() ;              // ctor
  IlcPVECIhepAnalyze(Text_t * name) ; // ctor
  IlcPVECIhepAnalyze(const IlcPVECIhepAnalyze & obj) : TObject(obj), fRunLoader(0), fFileName()
  {
    // cpy ctor: no implementation yet
    // requested by the Coding Convention
    Fatal("cpy ctor", "not implemented") ;
  }
  virtual ~IlcPVECIhepAnalyze() {}  ; // dtor
  IlcPVECIhepAnalyze & operator = (const IlcPVECIhepAnalyze & /*rvalue*/)  {
    Fatal("operator =", "not implemented") ; return *this ; }

  void AnalyzeCPV1(Int_t Nevents); // resolutions, mult and cluster lengths for CPV
  void AnalyzeEMC1(Int_t Nevents); // resolutions, mult and cluster lengths for EMC
  void AnalyzeCPV2(Int_t Nevents); // delta(gen)/delta(rec) between hits 
  void CpvSingle(Int_t Nevents); // signle particle analysis
  virtual void HitsCPV(Int_t event); 
  const TString GetFileName() const { return fFileName; }

 private:

  Bool_t IsCharged(Int_t pdgCode);

 private:
 
  IlcRunLoader *fRunLoader; // run loader
  TString fFileName;        // filename with headers (e.g. gilc.root)

ClassDef(IlcPVECIhepAnalyze,1)  // PVECv1 event analyzis algorithm

};

#endif // IlcPVECIhepAnalyze_H


