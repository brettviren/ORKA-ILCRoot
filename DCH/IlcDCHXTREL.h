#ifndef ILCDCHXTREL_H
#define ILCDCHXTREL_H

#include <TFile.h>
#include <TTree.h>
#include <TVector.h>
#include "IlcDCHXTDataVlGas.h"

class IlcDCHXTREL : public TObject{

public:
  IlcDCHXTREL(const char *XTFname = "$ILC_ROOT/DCH/XTtr_fromSigmaDiff_dw.0025_rc2_v2000_t25_p1.01325_b0_HELIUM-4_90_ISOBUTANE_10.root");
  ~IlcDCHXTREL();
  void OpenXT();
  Float_t* time2dist(Float_t time);
  Float_t* dist2time(Float_t dist);

private:
  char *filename;
  TString xtfilename;
  Float_t pos, minpos;
  Float_t drifttime, mindrifttime, maxdrifttime, firsttime;
  Float_t Dpos, Dminpos, Dfirstpos;
  Float_t Ddrifttime, Dmindrifttime;//, Dfirsttime;
  Int_t xtnbin;
  Int_t curentr;
  Int_t Dcurentr;
  Float_t InnerRtube, Rwire, Volts;
  TFile *trxtf;
  TTree *trdata, *trxt;
  TVectorF *radius, *integrTime, *integrDiffL, *integrDiffT, *coeffDiffL, *coeffDiffT;
  Float_t *Xxtx, *Xxttime, *XDiffL, *XDiffT, *XcoeffDiffL, *XcoeffDiffT;
  Float_t Emax, Emin, Econst;
  Float_t *E, *Vdrift, *DiffL, *DiffT;
  XTDataVlGas *xtData;
//  GasDataVl *gasdata;
  Float_t *x;

  ClassDef(IlcDCHXTREL,1)  //XT relation use
};

#endif // ILCDCHXTREL_H
