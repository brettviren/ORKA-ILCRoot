#ifndef IlcRSTACKDA2_H
#define IlcRSTACKDA2_H

#include "TNamed.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TObjArray.h"

class IlcRSTACKDA2 : public TNamed {
  
 public:
  
  IlcRSTACKDA2(Int_t module);
  IlcRSTACKDA2(Int_t module, TObjArray* oldHistos);

  IlcRSTACKDA2(const IlcRSTACKDA2& );
  IlcRSTACKDA2& operator= (const IlcRSTACKDA2& );
  ~IlcRSTACKDA2();
  
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
  Int_t fMod;                   // RSTACK module number (0..4)
  TH2F* fMaps[2];               // 2D quality map for low and high gains.
  
  ClassDef(IlcRSTACKDA2,1)

};

#endif
