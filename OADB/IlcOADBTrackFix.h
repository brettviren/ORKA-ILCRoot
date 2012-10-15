#ifndef ILCOADBTRACKKIX_H
#define ILCOADBTRACKFIX_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


//-------------------------------------------------------------------------
//     OADB class for run dependent track fixing parameters
//     Convention for phi-dependent data: 0 : 2pi
//     Author: ruben.shahoyan@cern.ch
//-------------------------------------------------------------------------

#include <TNamed.h>
class TGraph;

class IlcOADBTrackFix : public TNamed 
{
 public :
  enum CorMode_t {kCorModeGlob, kCorModeTPCInner, kNCorModes};
  //
  IlcOADBTrackFix();
  IlcOADBTrackFix(const char* name);
  virtual ~IlcOADBTrackFix();
  //
  Double_t GetPtInvCorr(int mode, double sideAfrac, double phi=0) const;
  //
  TGraph*  GetPtInvCorrGraph(int mode,int side)             const {return (TGraph*)fPtInvCor[mode][side];}
  Double_t GetXIniPtInvCorr(int mode)                       const {return fXIniPtInvCorr[mode];}
  //
  void     SetPtInvCorr(int mode,int side, const TGraph* gr);
  void     SetXIniPtInvCorr(int mode, double x=0)                 {fXIniPtInvCorr[mode] = x;}
  //
 private:
  IlcOADBTrackFix(const IlcOADBTrackFix& cont); 
  IlcOADBTrackFix& operator=(const IlcOADBTrackFix& cont);

 protected:
  const TGraph   *fPtInvCor[kNCorModes][2];    // graphs with 1/pt correction vs phi for A,C sides
  Double_t        fXIniPtInvCorr[kNCorModes];  // if >0 use as the reper X for slope,position correction of corresponding mode
  //
  ClassDef(IlcOADBTrackFix, 1);
};

#endif
