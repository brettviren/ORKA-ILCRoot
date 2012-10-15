#ifndef ILCRUNTAGCUTS_H
#define ILCRUNTAGCUTS_H
/*  See cxx source for full Copyright notice */


/* $Id: IlcRunTagCuts.h 50615 2011-07-16 23:19:19Z hristov $ */

//-------------------------------------------------------------------------
//                       Class IlcRunTagCuts
//              This is the class for the cuts in run tags
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------

#include <TObject.h>

class IlcRunTag;

//___________________________________________________________________________
class IlcRunTagCuts : public TObject {
 public:
  IlcRunTagCuts();
  ~IlcRunTagCuts();
  void Reset();
  
 //____________________________________________________//
  void SetRunId(Int_t Pid) {fIlcRunId = Pid; fIlcRunIdFlag = kTRUE;}
  void SetMagneticField(Float_t Pmag) {fIlcMagneticField = Pmag; fIlcMagneticFieldFlag = kTRUE;}
  void SetDipoleField(Float_t Pmag) {fIlcDipoleField = Pmag; fIlcDipoleFieldFlag = kTRUE;}
  void SetRunStartTimeRange(Int_t t0, Int_t t1) {fIlcRunStartTimeMin = t0; fIlcRunStartTimeMax = t1; fIlcRunStartTimeFlag = kTRUE;}
  void SetRunStopTimeRange(Int_t t0, Int_t t1) {fIlcRunStopTimeMin = t0; fIlcRunStopTimeMax = t1; fIlcRunStartTimeFlag = kTRUE;}
  void SetIlcrootVersion(TString v) {fIlcrootVersion = "VO_ILC@IlcRoot::"; fIlcrootVersion += v; fIlcrootVersionFlag = kTRUE;}
  void SetRootVersion(TString v) {fRootVersion = "VO_ILC@ROOT::"; fRootVersion += v; fRootVersionFlag = kTRUE;}
  void SetGeant3Version(TString v) {fGeant3Version = "VO_ILC@GEANT3::"; fGeant3Version += v; fGeant3VersionFlag = kTRUE;}
  void SetLHCPeriod(TString v) {fLHCPeriod = v; fLHCPeriodFlag = kTRUE; }
  void SetReconstructionPass(TString v) {fRecPass = v; fRecPassFlag = kTRUE; }
  void SetProductionName(TString v) {fProdName = v; fProdNameFlag = kTRUE; }
  void SetRunValidation(Int_t Pn) {fIlcRunValidation = Pn; fIlcRunValidationFlag = kTRUE;}
  void AddRunQualityValue(Int_t qval);
  void SetBeamEnergy(Float_t PE) {fIlcBeamEnergy = PE; fIlcBeamTypeFlag = kTRUE;}
  void SetBeamType(TString Ptype) {fIlcBeamType = Ptype; fIlcCalibrationVersionFlag = kTRUE;}
  void SetCalibVersion(Int_t Pn) {fIlcCalibrationVersion = Pn; fIlcCalibrationVersionFlag = kTRUE;}
  void SetDataType(Int_t i) {fIlcDataType = i; fIlcDataTypeFlag = kTRUE;}
  void SetBeamTriggersRange(ULong_t tmin, ULong_t tmax) { fBeamTriggerRange[0] = tmin; fBeamTriggerRange[1] = tmax; fBeamTriggerFlag = kTRUE; }
  void SetCollisionTriggersRange(ULong_t tmin, ULong_t tmax) { fCollisionTriggerRange[0] = tmin; fCollisionTriggerRange[1] = tmax; fCollisionTriggerFlag = kTRUE; }
  void SetEmptyTriggersRange(ULong_t tmin, ULong_t tmax) { fEmptyTriggerRange[0] = tmin; fEmptyTriggerRange[1] = tmax; fEmptyTriggerFlag = kTRUE; }
  void SetASideTriggersRange(ULong_t tmin, ULong_t tmax) { fASideTriggerRange[0] = tmin; fASideTriggerRange[1] = tmax; fASideTriggerFlag = kTRUE; }
  void SetCSideTriggersRange(ULong_t tmin, ULong_t tmax) { fCSideTriggerRange[0] = tmin; fCSideTriggerRange[1] = tmax; fCSideTriggerFlag = kTRUE; }
  void SetHMTriggersRange(ULong_t tmin, ULong_t tmax) { fHMTriggerRange[0] = tmin; fHMTriggerRange[1] = tmax; fHMTriggerFlag = kTRUE; }
  void SetMuonTriggersRange(ULong_t tmin, ULong_t tmax) { fMuonTriggerRange[0] = tmin; fMuonTriggerRange[1] = tmax; fMuonTriggerFlag = kTRUE; }
  void SetCollisionRatesRange(ULong_t tmin, ULong_t tmax) { fCollisionRateRange[0] = tmin; fCollisionRateRange[1] = tmax; fCollisionRateFlag = kTRUE; }
  void SetMeanVertexsRange(ULong_t tmin, ULong_t tmax) { fMeanVertexRange[0] = tmin; fMeanVertexRange[1] = tmax; fMeanVertexFlag = kTRUE; }
  void SetVertexQualitysRange(ULong_t tmin, ULong_t tmax) { fVertexQualityRange[0] = tmin; fVertexQualityRange[1] = tmax; fVertexQualityFlag = kTRUE; }

  Bool_t IsAccepted(IlcRunTag *RunTag) const;

  //____________________________________________________//
 private:
  Int_t   fIlcRunId;                  //the run id
  Bool_t  fIlcRunIdFlag;              //Shows whether this cut is used or not
  Float_t fIlcMagneticField;          //value of the magnetic field
  Bool_t  fIlcMagneticFieldFlag;      //Shows whether this cut is used or not
  Float_t fIlcDipoleField;            //value of the dipole field
  Bool_t  fIlcDipoleFieldFlag;        //Shows whether this cut is used or not
  Int_t   fIlcRunStartTimeMin;        //minimum run start date
  Int_t   fIlcRunStartTimeMax;        //maximum run start date
  Bool_t  fIlcRunStartTimeFlag;       //Shows whether this cut is used or not
  Int_t   fIlcRunStopTimeMin;         //minmum run stop date
  Int_t   fIlcRunStopTimeMax;         //maximum run stop date
  Bool_t  fIlcRunStopTimeFlag;        //Shows whether this cut is used or not
  TString fIlcrootVersion;              //ilcroot version
  Bool_t  fIlcrootVersionFlag;          //Shows whether this cut is used or not
  TString fRootVersion;                 //root version
  Bool_t  fRootVersionFlag;             //Shows whether this cut is used or not
  TString fGeant3Version;               //geant3 version
  Bool_t  fGeant3VersionFlag;           //Shows whether this cut is used or not
  TString fLHCPeriod;                   //LHC period version
  Bool_t  fLHCPeriodFlag;               //Shows whether this cut is used or not
  TString fRecPass;                     //Reconstruction pass
  Bool_t  fRecPassFlag;                 //Shows whether this cut is used or not
  TString fProdName;                    //Production Name
  Bool_t  fProdNameFlag;                //Shows whether this cut is used or not
  Bool_t  fIlcRunValidation;          //validation script
  Bool_t  fIlcRunValidationFlag;      //Shows whether this cut is used or not
  TString fIlcRunQualities;           //selected qualities
  Bool_t  fIlcRunQualitiesFlag;       //Shows whether this cut is used or not
  Float_t fIlcBeamEnergy;             //beam energy cm
  Bool_t  fIlcBeamEnergyFlag;         //Shows whether this cut is used or not
  TString fIlcBeamType;               //run type (pp, AA, pA)
  Bool_t  fIlcBeamTypeFlag;           //Shows whether this cut is used or not
  Int_t   fIlcCalibrationVersion;     //calibration version  
  Bool_t  fIlcCalibrationVersionFlag; //Shows whether this cut is used or not
  Int_t   fIlcDataType;               //0: simulation -- 1: data  
  Bool_t  fIlcDataTypeFlag;           //Shows whether this cut is used or not
  ULong_t fBeamTriggerRange[2];         //Beam trigger maximum and minimum
  Bool_t  fBeamTriggerFlag;             //Shows whether this cut is used or not
  ULong_t fCollisionTriggerRange[2];    //Collision trigger maximum and minimum
  Bool_t  fCollisionTriggerFlag;        //Shows whether this cut is used or not
  ULong_t fEmptyTriggerRange[2];        //Empty trigger maximum and minimum
  Bool_t  fEmptyTriggerFlag;            //Shows whether this cut is used or not
  ULong_t fASideTriggerRange[2];        //ASide trigger maximum and minimum
  Bool_t  fASideTriggerFlag;            //Shows whether this cut is used or not
  ULong_t fCSideTriggerRange[2];        //CSide trigger maximum and minimum
  Bool_t  fCSideTriggerFlag;            //Shows whether this cut is used or not
  ULong_t fHMTriggerRange[2];           //High Multiplicity trigger maximum and minimum
  Bool_t  fHMTriggerFlag;               //Shows whether this cut is used or not
  ULong_t fMuonTriggerRange[2];         //Muon trigger maximum and minimum
  Bool_t  fMuonTriggerFlag;             //Shows whether this cut is used or not
  Float_t fCollisionRateRange[2];       //Collision rate range
  Bool_t  fCollisionRateFlag;           //Shows whether this cut is used or not
  Float_t fMeanVertexRange[2];          //Mean Vertex Postion
  Bool_t  fMeanVertexFlag;              //Shows whether this cut is used or not
  Float_t fVertexQualityRange[2];       //Mean Vertex quality
  Bool_t  fVertexQualityFlag;           //Shows whether this cut is used or not

  ClassDef(IlcRunTagCuts, 2)
};

#endif
