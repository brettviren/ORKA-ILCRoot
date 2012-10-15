#ifndef IlcPVBARDA2_H
#define IlcPVBARDA2_H

#include "TNamed.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TObjArray.h"

class IlcPVBARDA2 : public TNamed {
  
 public:
  
  IlcPVBARDA2(Int_t module);
  IlcPVBARDA2(Int_t module, TObjArray* oldHistos);

  IlcPVBARDA2(const IlcPVBARDA2& );
  IlcPVBARDA2& operator= (const IlcPVBARDA2& );
  ~IlcPVBARDA2();
  
  void  FillQualityHistograms(Float_t quality[64][56][2]);
  void  FillFiredCellsHistogram(Int_t nCells);
  Int_t GetModule() { return fMod; }
  void  UpdateHistoFile();

  const TH1F* GetQualityHistogram(Int_t X, Int_t Z, Int_t gain) const
  { return fHQuality[X][Z][gain]; }

  const TH1I* GetFiredCellsHistogram() { return fFiredCells; }
  
 private:

  TFile* fHistoFile;            // root file to store histograms in
  TH1F* fHQuality[64][56][2];   // "quality" for high and low gains
  TH1I* fFiredCells;            // Number of fired cells pre event.
  Int_t fMod;                   // PVBAR module number (0..4)
  TH2F* fMaps[2];               // 2D quality map for low and high gains.
  
  ClassDef(IlcPVBARDA2,1)

};

#endif
