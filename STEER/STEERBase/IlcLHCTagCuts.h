#ifndef ILCLHCTAGCUTS_H
#define ILCLHCTAGCUTS_H
/*  See cxx source for full Copyright notice */


/* $Id: IlcLHCTagCuts.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//                       Class IlcLHCTagCuts
//              This is the class for the cuts in run tags
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------

#include <TObject.h>

class IlcLHCTag;

//___________________________________________________________________________
class IlcLHCTagCuts : public TObject {
 public:
  IlcLHCTagCuts();
  ~IlcLHCTagCuts();
  void Reset();
  
 //____________________________________________________//
  void SetLHCState(TString state) {fLHCState = state; fLHCStateFlag = kTRUE;}
  void SetLHCLuminosityRange(Float_t low, Float_t high) {fLHCLuminosityMin = low; fLHCLuminosityMax = high; fLHCLuminosityFlag = kTRUE;}

  void SetNBunchesRange(UShort_t nbm, UShort_t nbx) { fNBunchesFlag = kTRUE; fNBunchesRange[0] = nbm; fNBunchesRange[1] = nbx; }
  void SetFillingScheme(TString sch) { fFillingSchemeFlag = kTRUE; fFillingScheme = sch; }
  void SetFillNoRange(Int_t fmin, Int_t fmax) { fFillNoFlag = kTRUE; fFillNoRange[0] = fmin; fFillNoRange[1] = fmax; }
  void SetBeamEnergyRange(Float_t bemin, Float_t bemax) { fBeamEnergyFlag = kTRUE; fBeamEnergyRange[0] = bemin; fBeamEnergyRange[1] = bemax; }
  void SetBunchIntensityRange(Float_t bimin, Float_t bimax) { fBunchIntensityFlag = kTRUE; fBunchIntensityRange[0] = bimin; fBunchIntensityRange[1] = bimax; }

  Bool_t IsAccepted(IlcLHCTag *lhcTag) const;

  //____________________________________________________//
 private:
  TString fLHCState;              //LHC State
  Bool_t  fLHCStateFlag;          //Shows whether this cut is used or
  Float_t fLHCLuminosityMin;      //LHC luminosity - min
  Float_t fLHCLuminosityMax;      //LHC luminosity - max
  Bool_t  fLHCLuminosityFlag;     //Shows whether this cut is used or
  UShort_t fNBunchesRange[2];     //Number of bunches in beam
  Bool_t   fNBunchesFlag;         //Shows whether this cut is used or
  TString  fFillingScheme;        //Rquired filling scheme name
  Bool_t   fFillingSchemeFlag;    //Shows whether this cut is used or
  Int_t    fFillNoRange[2];       //Required Fill no range
  Bool_t   fFillNoFlag;           //Shows whether this cut is used or
  Float_t  fBeamEnergyRange[2];   //Beam energy range
  Bool_t   fBeamEnergyFlag;       //Shows whether this cut is used or
  Float_t  fBunchIntensityRange[2];//Intensity per bunch range
  Bool_t   fBunchIntensityFlag;   //Shows whether this cut is used or

  ClassDef(IlcLHCTagCuts, 2)
};

#endif
