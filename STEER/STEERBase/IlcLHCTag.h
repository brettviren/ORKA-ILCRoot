#ifndef ILCLHCTAG_H
#define ILCLHCTAG_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcLHCTag.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//                          Class IlcLHCTag
//   This is the class to deal with the tags for the LHC level
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------

#include "TObject.h"
#include "TString.h"

//______________________________________________________________________________
class IlcLHCTag : public TObject {
 public:
  IlcLHCTag();
  IlcLHCTag(const IlcLHCTag &tag);
  virtual ~IlcLHCTag();
  
  IlcLHCTag &operator=(const IlcLHCTag &tag);

  //____________________________________________________//
  //  void SetLHCTag(Float_t lumin, TString type) {fLHCLuminosity = lumin; fLHCState = type; }
  void UpdateFromRunTable(IlcLHCTag &tag);

  void SetLHCState(TString type) {fLHCState = type;}
  void SetLuminosity(Float_t lumin) {fLHCLuminosity = lumin;}
  void SetNBunches(UShort_t nb) { fNBunches = nb; };
  void SetFillingScheme(TString sch) { fFillingScheme = sch; }
  void SetFillNo(Int_t fill) { fFillNo = fill; };
  void SetBeamEnergy(Float_t be) { fBeamEnergy = be; }
  void SetBunchIntensity(Float_t bi) { fBunchIntensity = bi; }
  
  
  //____________________________________________________//
  const char *GetLHCState() const {return fLHCState.Data();}
  Float_t     GetLuminosity() const {return fLHCLuminosity;}
  UShort_t    GetNBunches() const {return fNBunches; }
  TString     GetFillingScheme() const {return fFillingScheme; }
  Int_t       GetFillNo() const {return fFillNo; }
  Float_t     GetBeamEnergy() const {return fBeamEnergy; }
  Float_t     GetBunchIntensity() const {return fBunchIntensity; }

  //____________________________________________________//
 private:
  TString  fLHCState;      //LHC run conditions - comments
  Float_t  fLHCLuminosity; //the value of the luminosity
  UShort_t fNBunches;      //Number of bunches in beam
  TString  fFillingScheme; //Filling scheme name
  Int_t    fFillNo;        //Fill number
  Float_t  fBeamEnergy;    //Beam energy
  Float_t  fBunchIntensity;//Intensity per bunch

  ClassDef(IlcLHCTag,2)   //(ClassName, ClassVersion)
};
//______________________________________________________________________________

#endif
