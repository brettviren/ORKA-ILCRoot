#ifndef ILCRUNTAG_H
#define ILCRUNTAG_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcRunTag.h 53510 2011-12-10 22:29:56Z shahoian $ */

//-------------------------------------------------------------------------
//                          Class IlcRunTag
//   This is the class to deal with the tags for the run level
//
//    Origin: Panos Christakoglou, UOA-CERN, Panos.Christakoglou@cern.ch
//-------------------------------------------------------------------------

#include <TObject.h>
#include <TString.h>
#include <TClonesArray.h>
#include "IlcLHCTag.h"
#include "IlcDetectorTag.h"
#include "IlcFileTag.h"
#include "IlcQA.h"

class IlcEventTag;
//class IlcDetectorTag;


//___________________________________________________________________________
class IlcRunTag : public TObject {
 public:
  IlcRunTag();
  virtual ~IlcRunTag();
  IlcRunTag(const IlcRunTag& qa) ;   
  IlcRunTag& operator = (const IlcRunTag& tag) ;
  Bool_t Add(const IlcRunTag* tag) ;
  //____________________________________________________//
  void SetRunId(Int_t Pid) {fIlcRunId = Pid;}
  void SetMagneticField(Float_t Pmag) {fIlcMagneticField = Pmag;}
  void SetDipoleField(Float_t Pmag) {fIlcDipoleField = Pmag;}
  void SetRunStartTime(Int_t Pt0) {fIlcRunStartTime = Pt0;}
  void SetRunStopTime(Int_t Pt1) {fIlcRunStopTime = Pt1;}
  void SetIlcrootVersion(TString v) {fIlcrootVersion = v;}
  void SetRootVersion(TString v) {fRootVersion = v;}
  void SetGeant3Version(TString v) {fGeant3Version = v;}
  void SetLHCPeriod(TString v) {fLHCPeriod = v; }
  void SetReconstructionPass(TString v) {fRecPass = v; }
  void SetProductionName(TString v) {fProductionName = v; }
  void SetRunValidation(Bool_t val) {fIlcRunValidated = val; }
  void SetRunQuality(Int_t Pn) {fIlcRunGlobalQuality = Pn;}
  void SetBeamEnergy(Float_t PE) {fIlcBeamEnergy = PE;}
  void SetBeamType(TString Ptype) {fIlcBeamType = Ptype;}
  void SetCalibVersion(Int_t Pn) {fIlcCalibrationVersion = Pn;}
  void SetDataType(Int_t i) {fIlcDataType = i;}
/*   void SetNEvents(Int_t Pn) { fNumEvents = Pn; } */
  void SetBeamTriggers(ULong_t bt) { fBeamTriggers = bt; }
  void SetCollisionTriggers(ULong_t ct) { fCollisionTriggers = ct; }
  void SetEmptyTriggers(ULong_t et) {fEmptyTriggers = et; }
  void SetASideTriggers(ULong_t at) {fASideTriggers = at; }
  void SetCSideTriggers(ULong_t ct) {fCSideTriggers = ct; }
  void SetHMTriggers(ULong_t ht) {fHMTriggers = ht; }
  void SetMuonTriggers(ULong_t mt) {fMuonTriggers = mt; }
  void SetCollisionRate(Float_t rate) {fCollisionRate = rate; }
  void SetMeanVertex(Float_t mv) {fMeanVertex = mv; }
  void SetVertexQuality(Float_t vq) {fVertexQuality = vq; }
  void SetLHCTag(Float_t Plumin, TString type);
  void SetDetectorTag(UInt_t mask, UInt_t maskReco=0);
  void SetQA(const IlcQA &qa) { fQA=qa; }  	
  void SetQAArray(ULong_t * qa, Int_t qalength) ; 
  void SetEventSpecies(Bool_t * es, Int_t eslength) ;
  void AddEventTag(const IlcEventTag &t);
  void Clear(const char * opt = "");
  void AddFileTag(IlcFileTag *t);

  void SetActiveTriggerClasses(TString str) { fActiveTriggerClasses = str; }

  void CopyStandardContent(IlcRunTag *oldtag);
  void UpdateFromRunTable(IlcRunTag *tabtag);

  //____________________________________________________//
  Int_t       GetRunId() const {return fIlcRunId;}
  Float_t     GetMagneticField() const {return fIlcMagneticField;}
  Float_t     GetDipoleField() const {return fIlcDipoleField;}
  Int_t       GetRunStartTime() const {return fIlcRunStartTime;}
  Int_t       GetRunStopTime() const {return fIlcRunStopTime;}
  const char* GetIlcrootVersion() const {return fIlcrootVersion.Data();}
  const char* GetRootVersion() const {return fRootVersion.Data();}
  const char* GetGeant3Version() const {return fGeant3Version.Data();}
  const char* GetLHCPeriod() const {return fLHCPeriod.Data();}
  const char* GetReconstructionPass() const {return fRecPass.Data();}
  const char* GetProductionName() const {return fProductionName.Data();}
  Bool_t      GetRunValidation() const {return fIlcRunValidated;}
  Int_t       GetRunQuality() const {return fIlcRunGlobalQuality;}
  Float_t     GetBeamEnergy() const {return fIlcBeamEnergy;}
  const char *GetBeamType() const {return fIlcBeamType.Data();}
  Int_t       GetCalibVersion() const {return fIlcCalibrationVersion;}
  Int_t       GetDataType() const {return fIlcDataType;}
  Int_t       GetNEvents() const;
  ULong_t     GetBeamTriggers() const {return fBeamTriggers;}
  ULong_t     GetCollisionTriggers() const {return fCollisionTriggers;}
  ULong_t     GetEmptyTriggers() const {return fEmptyTriggers;}
  ULong_t     GetASideTriggers() const {return fASideTriggers;}
  ULong_t     GetCSideTriggers() const {return fCSideTriggers;}
  ULong_t     GetHMTriggers() const {return fHMTriggers;}
  ULong_t     GetMuonTriggers() const {return fMuonTriggers;}
  Float_t     GetCollisionRate() const {return fCollisionRate;}
  Float_t     GetMeanVertex() const {return fMeanVertex;}
  Float_t     GetVertexQuality() const {return fVertexQuality;}
  IlcLHCTag  *GetLHCTag() {return &fLHCTag; } 
  IlcDetectorTag *GetDetectorTags() {return &fDetectorTag;}
  //  const TClonesArray *GetEventTags() const {return &fEventTag;}
  const IlcEventTag* GetEventTag(int evt) const;
  IlcFileTag *GetFileTagForEvent(int evt);
  Int_t       GetNFiles() const { return fFileTags.GetEntries(); }
  IlcFileTag *GetFileTag(Int_t ifile) const {return (IlcFileTag *) fFileTags.At(ifile);}
  const IlcQA *GetQA() const {return &fQA;}
  ULong_t *  GetQAArray() const {return fQAArray;}	
  Int_t      GetQALength() const { return fQALength ; }
  Bool_t *   GetEventSpecies() const {return fEventSpecies;}	
  Int_t      GetESLength() const { return fESLength ; }
  Int_t      GetFileId(const char *guid);
  Int_t      GetNumFiles() const {return fNumFiles;}
  TString    GetActiveTriggerClasses() const {return fActiveTriggerClasses; }

  //____________________________________________________//
 private:
  Int_t        fIlcRunId;              //the run id
  Float_t      fIlcMagneticField;      //value of the magnetic field
  Float_t      fIlcDipoleField;        //value of the magnetic field in dipole
  Int_t        fIlcRunStartTime;       //run start date
  Int_t        fIlcRunStopTime;        //run stop date
  TString      fIlcrootVersion;          //ilcroot version
  TString      fRootVersion;             //root version
  TString      fGeant3Version;           //geant3 version
  TString      fLHCPeriod;               //datataking period
  TString      fRecPass;                 //Reconstruction pass number
  TString      fProductionName;          //production name
  Bool_t       fIlcRunValidated;       //validation script
  Int_t        fIlcRunGlobalQuality;   //validation script
  Float_t      fIlcBeamEnergy;         //beam energy cm
  TString      fIlcBeamType;           //run type (pp, AA, pA)
  Int_t        fIlcCalibrationVersion; //calibration version  
  Int_t        fIlcDataType;           //0: simulation -- 1: data  
/*   Int_t        fNumEvents;               //number of events per file */
  Int_t        fNumFiles;                //number of files in the run
  ULong_t      fBeamTriggers;            //number of beam triggers in run (CBEAMB)
  ULong_t      fCollisionTriggers;       //number of collision triggers in run (CINT1-B)
  ULong_t      fEmptyTriggers;           //number of empty triggers in run (CINT1-E)
  ULong_t      fASideTriggers;           //number of A-side triggers in run (CINT1-A)
  ULong_t      fCSideTriggers;           //number of C-side triggers in run (CINT1-C)
  ULong_t      fHMTriggers;              //number of High-Mult triggers
  ULong_t      fMuonTriggers;            //number of Muon Triggers
  Float_t      fCollisionRate;           //Average collision rate
  Float_t      fMeanVertex;              //mean vertex position
  Float_t      fVertexQuality;           //vertex quality
  Int_t        fNumDetectors;            //number of detector configs per file
  //  TClonesArray fEventTag;                //array with all event tags
  //  TClonesArray fFileTags;                //array of file tags
  //  IlcFileTag **fFileTags;                //array of file tags
  TObjArray    fFileTags;                //array of file tags
  IlcDetectorTag fDetectorTag;           //array with all the detector tags
  IlcLHCTag    fLHCTag;                  //LHC tag object
  TString      fActiveTriggerClasses;    //Active trigger classes for run
  IlcQA        fQA;                      //needed for backward compaibility
  Int_t        fQALength;                // Length of the fQA array  
  ULong_t *    fQAArray ;                //[fQALength] QA objects's data	
  Int_t        fESLength;                // Length of the Event Specie Length
  Bool_t *     fEventSpecies;            //[fESLength] EventSpecies in this run	
  

  ClassDef(IlcRunTag,9)  //(ClassName, ClassVersion)
};
//___________________________________________________________________________

#endif
