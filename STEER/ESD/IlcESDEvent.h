// -*- mode: C++ -*- 
#ifndef ILCESDEVENT_H
#define ILCESDEVENT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcESDEvent.h 55662 2012-04-10 15:18:49Z hristov $ */

//-------------------------------------------------------------------------
//                          Class IlcESDEvent
//   This is the class to deal with during the physics analysis of data.
//   It also ensures the backward compatibility with the old ESD format.
//      
// Origin: Christian Klein-Boesing, CERN, Christian.Klein-Boesing@cern.ch 
//-------------------------------------------------------------------------

#include <TClonesArray.h>
#include <TObject.h>
#include <TTree.h>
#include <TArrayF.h>


#include "IlcVEvent.h"
// some includes for delegated methods
#include "IlcESDCaloTrigger.h"
#include "IlcESDRun.h"
#include "IlcESDHeader.h"
#include "IlcESDTZERO.h"
#include "IlcESDZDC.h"
#include "IlcESDACORDE.h"

// IlcESDtrack has to be included so that the compiler 
// knows its inheritance tree (= that it is a IlcVParticle).
#include "IlcESDtrack.h"
// same for IlcESDVertex (which is a IlcVVertex)
#include "IlcESDVertex.h"
// same for CaloCells and CaloClusters (which is a IlcVCaloCells, IlcVCluster)
#include "IlcESDCaloCluster.h"
#include "IlcESDCaloCells.h"

#include "IlcTOFHeader.h"

#include "IlcESDVZERO.h"
#ifdef MFT_UPGRADE
//#include "IlcESDMFT.h"
#endif

class IlcESDfriend;
class IlcESDHLTtrack;
class IlcESDVertex;
class IlcESDPmdTrack;
class IlcESDFMD;
class IlcESDkink;
class IlcESDv0;
class IlcMultiplicity;
class IlcRawDataErrorLog;
class IlcESDRun;
class IlcESDTrdTrigger;
class IlcESDTrdTrack;
class IlcESDTrdTracklet;
class IlcESDMuonTrack;
class IlcESDMuonCluster;
class IlcESDMuonPad;
class IlcESD;
class IlcESDcascade;
class IlcCentrality;
class IlcEventplane;
class TRefArray;
class IlcESDACORDE;
class IlcESDHLTDecision;
class IlcESDCosmicTrack;

class TList;
class TString;


class IlcESDEvent : public IlcVEvent {
public:


  enum ESDListIndex   {kESDRun,
		       kHeader,
		       kESDZDC,
		       kESDFMD,
		       kESDVZERO,
		       kESDTZERO,
		       kTPCVertex,
		       kSPDVertex,
		       kPrimaryVertex,
		       kSPDMult,
		       kPHOSTrigger,
		       kEMCALTrigger,
		       kSPDPileupVertices,
		       kTrkPileupVertices,
		       kTracks,
		       kMuonTracks,
		       kMuonClusters,
		       kMuonPads,
		       kPmdTracks,
		       kTrdTrigger,
		       kTrdTracks,
		       kTrdTracklets,
		       kV0s,
		       kCascades,
		       kKinks,
		       kCaloClusters,
		       kEMCALCells,
		       kPHOSCells,
		       kErrorLogs,
                       kESDACORDE,
		       kTOFHeader,
                       kCosmicTracks,
		       kESDListN
	           #ifdef MFT_UPGRADE
	//           , kESDMFT
	           #endif
  };

  IlcESDEvent();
  virtual ~IlcESDEvent();
  IlcESDEvent &operator=(const IlcESDEvent& source); // or make private and use only copy? 
  virtual void Copy(TObject& obj) const;

  // RUN
  // move this to the UserData!!!
  const IlcESDRun*    GetESDRun() const {return fESDRun;}

  // Delegated methods for fESDRun
  void     SetRunNumber(Int_t n) {if(fESDRun) fESDRun->SetRunNumber(n);}
  Int_t    GetRunNumber() const {return fESDRun?fESDRun->GetRunNumber():-1;}
  void     SetPeriodNumber(UInt_t n){
    if(fESDRun) fESDRun->SetPeriodNumber(n);
    if(fHeader) fHeader->SetPeriodNumber(n);
  }
  UInt_t   GetPeriodNumber() const {return fESDRun?fESDRun->GetPeriodNumber():0;}
  void     SetMagneticField(Double_t mf){if(fESDRun) fESDRun->SetMagneticField(mf);}
  Double_t GetMagneticField() const {return fESDRun?fESDRun->GetMagneticField():0;}
  void     SetDiamond(const IlcESDVertex *vertex) { if(fESDRun) fESDRun->SetDiamond(vertex);}
  Double_t  GetDiamondX() const {return fESDRun?fESDRun->GetDiamondX():0;}
  Double_t  GetDiamondY() const {return fESDRun?fESDRun->GetDiamondY():0;}
  Double_t  GetDiamondZ() const {return fESDRun?fESDRun->GetDiamondZ():0;}
  Double_t  GetSigma2DiamondX() const {return  fESDRun?fESDRun->GetSigma2DiamondX():0;}
  Double_t  GetSigma2DiamondY() const {return  fESDRun?fESDRun->GetSigma2DiamondY():0;}
  Double_t  GetSigma2DiamondZ() const {return  fESDRun?fESDRun->GetSigma2DiamondZ():0;}
  void      GetDiamondCovXY(Float_t cov[3]) const {if(fESDRun) fESDRun->GetDiamondCovXY(cov);}   
  void     SetTriggerClass(const char*name, Int_t index) {if(fESDRun) fESDRun->SetTriggerClass(name,index);}
  void     SetPHOSMatrix(TGeoHMatrix*matrix, Int_t i) {if(fESDRun) fESDRun->SetPHOSMatrix(matrix,i);}
  const TGeoHMatrix* GetPHOSMatrix(Int_t i) const {return fESDRun?fESDRun->GetPHOSMatrix(i):0x0;}
  void     SetEMCALMatrix(TGeoHMatrix*matrix, Int_t i) {if(fESDRun) fESDRun->SetEMCALMatrix(matrix,i);}
  const TGeoHMatrix* GetEMCALMatrix(Int_t i) const {return fESDRun?fESDRun->GetEMCALMatrix(i):0x0;}
  void     SetCaloTriggerType(const Int_t* type) {if (fESDRun) fESDRun->SetCaloTriggerType(type);}
  Int_t*   GetCaloTriggerType() const {return fESDRun?fESDRun->GetCaloTriggerType():0x0;}
  virtual const Float_t* GetVZEROEqFactors() const {return fESDRun?fESDRun->GetVZEROEqFactors():0x0;}
  virtual Float_t        GetVZEROEqMultiplicity(Int_t i) const;
	
  //
  void        SetCurrentL3(Float_t cur)           const  {if(fESDRun) fESDRun->SetCurrentL3(cur);}
  void        SetCurrentDip(Float_t cur)          const  {if(fESDRun) fESDRun->SetCurrentDip(cur);}
  void        SetBeamEnergy(Float_t be)           const  {if(fESDRun) fESDRun->SetBeamEnergy(be);}
  void        SetBeamType(const char* bt)         const  {if(fESDRun) fESDRun->SetBeamType(bt);}
  void        SetBeamParticle(Int_t az, Int_t ibeam)      {if(fESDRun) fESDRun->SetBeamParticle(az,ibeam);}
  void        SetUniformBMap(Bool_t val=kTRUE)    const  {if(fESDRun) fESDRun->SetBit(IlcESDRun::kUniformBMap,val);}
  void        SetBInfoStored(Bool_t val=kTRUE)    const  {if(fESDRun) fESDRun->SetBit(IlcESDRun::kBInfoStored,val);}
  //
  Float_t     GetCurrentL3()                      const  {return fESDRun?fESDRun->GetCurrentL3():0;}
  Float_t     GetCurrentDip()                     const  {return fESDRun?fESDRun->GetCurrentDip():0;}
  Float_t     GetBeamEnergy()                     const  {return fESDRun?fESDRun->GetBeamEnergy():0;}
  const char* GetBeamType()                       const  {return fESDRun?fESDRun->GetBeamType():0;}
  Int_t       GetBeamParticle(Int_t ibeam)        const  {return fESDRun?fESDRun->GetBeamParticle(ibeam):0;}
  Int_t       GetBeamParticleA(Int_t ibeam)       const  {return fESDRun?fESDRun->GetBeamParticleA(ibeam):0;}
  Int_t       GetBeamParticleZ(Int_t ibeam)       const  {return fESDRun?fESDRun->GetBeamParticleZ(ibeam):0;}
  Bool_t      IsUniformBMap()                     const  {return fESDRun?fESDRun->TestBit(IlcESDRun::kUniformBMap):kFALSE;}
  //
  Bool_t      InitMagneticField()                 const  {return fESDRun?fESDRun->InitMagneticField():kFALSE;} 
  void      SetT0spread(Float_t *t)               const  {if(fESDRun) fESDRun->SetT0spread(t);} 
  void      SetVZEROEqFactors(Float_t factors[64]) const {if(fESDRun) fESDRun->SetVZEROEqFactors(factors);}
  // HEADER
  IlcESDHeader* GetHeader() const {return fHeader;}

  // Delegated methods for fHeader
  void      SetTriggerMask(ULong64_t n) {if(fHeader) fHeader->SetTriggerMask(n);}
  void      SetOrbitNumber(UInt_t n) {if(fHeader) fHeader->SetOrbitNumber(n);}
  void      SetTimeStamp(UInt_t timeStamp){if(fHeader) fHeader->SetTimeStamp(timeStamp);}
  void      SetEventType(UInt_t eventType){if(fHeader) fHeader->SetEventType(eventType);}
  void      SetEventSpecie(UInt_t eventSpecie){if(fHeader) fHeader->SetEventSpecie(eventSpecie);}
  void      SetEventNumberInFile(Int_t n) {if(fHeader) fHeader->SetEventNumberInFile(n);}
  //  void     SetRunNumber(Int_t n) {if(fHeader) fHeader->SetRunNumber(n);}
  void      SetBunchCrossNumber(UShort_t n) {if(fHeader) fHeader->SetBunchCrossNumber(n);}
  void      SetTriggerCluster(UChar_t n) {if(fHeader) fHeader->SetTriggerCluster(n);}
  
  ULong64_t GetTriggerMask() const {return fHeader?fHeader->GetTriggerMask():0;}
  TString   GetFiredTriggerClasses() const {return (fESDRun&&fHeader)?fESDRun->GetFiredTriggerClasses(fHeader->GetTriggerMask()):"";}
  Bool_t    IsTriggerClassFired(const char *name) const {return (fESDRun&&fHeader)?fESDRun->IsTriggerClassFired(fHeader->GetTriggerMask(),name):kFALSE;}
  Bool_t    IsEventSelected(const char *trigExpr) const;
  TObject*  GetHLTTriggerDecision() const;
  TString   GetHLTTriggerDescription() const;
  Bool_t    IsHLTTriggerFired(const char* name=NULL) const;
  UInt_t    GetOrbitNumber() const {return fHeader?fHeader->GetOrbitNumber():0;}
  UInt_t    GetTimeStamp()  const { return fHeader?fHeader->GetTimeStamp():0;}
  UInt_t    GetEventType()  const { return fHeader?fHeader->GetEventType():0;}
  UInt_t    GetEventSpecie()  const { return fHeader?fHeader->GetEventSpecie():0;}
  Int_t     GetEventNumberInFile() const {return fHeader?fHeader->GetEventNumberInFile():-1;}
  UShort_t  GetBunchCrossNumber() const {return fHeader?fHeader->GetBunchCrossNumber():0;}
  UChar_t   GetTriggerCluster() const {return fHeader?fHeader->GetTriggerCluster():0;}

  // ZDC CKB: put this in the header?
  IlcESDZDC*    GetESDZDC()  const {return fESDZDC;}
  IlcESDZDC*    GetZDCData() const {return fESDZDC;}

  void SetZDCData(const IlcESDZDC * obj);

  // Delegated methods for fESDZDC
  Double_t GetZDCN1Energy() const {return fESDZDC?fESDZDC->GetZDCN1Energy():0;}
  Double_t GetZDCP1Energy() const {return fESDZDC?fESDZDC->GetZDCP1Energy():0;}
  Double_t GetZDCN2Energy() const {return fESDZDC?fESDZDC->GetZDCN2Energy():0;}
  Double_t GetZDCP2Energy() const {return fESDZDC?fESDZDC->GetZDCP2Energy():0;}
  Double_t GetZDCEMEnergy(Int_t i=0) const {return fESDZDC?fESDZDC->GetZDCEMEnergy(i):0;}
  Int_t    GetZDCParticipants() const {return fESDZDC?fESDZDC->GetZDCParticipants():0;}
  IlcCentrality* GetCentrality();
  IlcEventplane* GetEventplane();
    

  void     SetZDC(Float_t n1Energy, Float_t p1Energy, Float_t em1Energy, Float_t em2Energy,
                  Float_t n2Energy, Float_t p2Energy, Int_t participants, Int_t nPartA,
	 	  Int_t nPartC, Double_t b, Double_t bA, Double_t bC, UInt_t recoflag)
  {if(fESDZDC) fESDZDC->SetZDC(n1Energy, p1Energy, em1Energy, em2Energy, n2Energy, p2Energy, 
            participants, nPartA, nPartC, b, bA, bC,  recoflag);}
    // FMD
  void SetFMDData(IlcESDFMD * obj);
  IlcESDFMD *GetFMDData() const { return fESDFMD; }


  // TZERO CKB: put this in the header?
  const IlcESDTZERO*    GetESDTZERO() const {return fESDTZERO;}
  void SetTZEROData(const IlcESDTZERO * obj);
 // delegetated methods for fESDTZERO

  Double32_t GetT0zVertex() const {return fESDTZERO?fESDTZERO->GetT0zVertex():0;}
  void SetT0zVertex(Double32_t z) {if(fESDTZERO) fESDTZERO->SetT0zVertex(z);}
  Double32_t GetT0() const {return fESDTZERO?fESDTZERO->GetT0():0;}
  void SetT0(Double32_t timeStart) {if(fESDTZERO) fESDTZERO->SetT0(timeStart);}
  Double32_t GetT0clock() const {return fESDTZERO?fESDTZERO->GetT0clock():0;}
  void SetT0clock(Double32_t timeStart) {if(fESDTZERO) fESDTZERO->SetT0clock(timeStart);}
  Double32_t GetT0TOF(Int_t icase) const {return fESDTZERO?fESDTZERO->GetT0TOF(icase):0;}
  const Double32_t * GetT0TOF() const {return fESDTZERO?fESDTZERO->GetT0TOF():0x0;}
  void SetT0TOF(Int_t icase,Double32_t timeStart) {if(fESDTZERO) fESDTZERO->SetT0TOF(icase,timeStart);}
  const Double32_t * GetT0time() const {return fESDTZERO?fESDTZERO->GetT0time():0x0;}
  void SetT0time(Double32_t time[24]) {if(fESDTZERO) fESDTZERO->SetT0time(time);}
  const Double32_t * GetT0amplitude() const {return fESDTZERO?fESDTZERO->GetT0amplitude():0x0;}
  void SetT0amplitude(Double32_t amp[24]){if(fESDTZERO) fESDTZERO->SetT0amplitude(amp);}
  Int_t GetT0Trig() const { return fESDTZERO?fESDTZERO->GetT0Trig():0;}
  void SetT0Trig(Int_t tvdc) {if(fESDTZERO) fESDTZERO->SetT0Trig(tvdc);}

  // VZERO 
  IlcESDVZERO *GetVZEROData() const { return fESDVZERO; }
  void SetVZEROData(const IlcESDVZERO * obj);
	
  #ifdef MFT_UPGRADE
  // MFT 
//  IlcESDMFT *GetMFTData() const { return fESDMFT; }
//  void SetMFTData(IlcESDMFT * obj);
  #endif
	
 // ACORDE
  IlcESDACORDE *GetACORDEData() const { return fESDACORDE;}
  void SetACORDEData(IlcESDACORDE * obj);

  void SetESDfriend(const IlcESDfriend *f) const;
  void GetESDfriend(IlcESDfriend *f) const;



  void SetPrimaryVertexTPC(const IlcESDVertex *vertex); 
  const IlcESDVertex *GetPrimaryVertexTPC() const {return fTPCVertex;}

  void SetPrimaryVertexSPD(const IlcESDVertex *vertex); 
  const IlcESDVertex *GetPrimaryVertexSPD() const {return fSPDVertex;}
  const IlcESDVertex *GetVertex() const {
    //For the backward compatibily only
     return GetPrimaryVertexSPD();
  }

  void SetPrimaryVertexTracks(const IlcESDVertex *vertex);
  const IlcESDVertex *GetPrimaryVertexTracks() const {return fPrimaryVertex;}
  IlcESDVertex *PrimaryVertexTracksUnconstrained() const;

  const IlcESDVertex *GetPrimaryVertex() const;



  void SetTOFHeader(const IlcTOFHeader * tofEventTime);
  const IlcTOFHeader *GetTOFHeader() const {return fTOFHeader;}
  Float_t GetEventTimeSpread() const {if (fTOFHeader) return fTOFHeader->GetT0spread(); else return 0.;}
  Float_t GetTOFTimeResolution() const {if (fTOFHeader) return fTOFHeader->GetTOFResolution(); else return 0.;}


  void SetMultiplicity(const IlcMultiplicity *mul);

  const IlcMultiplicity *GetMultiplicity() const {return fSPDMult;}
  void   EstimateMultiplicity(Int_t &tracklets,Int_t &trITSTPC,Int_t &trITSSApure,
			      Double_t eta=1.,Bool_t useDCAFlag=kTRUE,Bool_t useV0Flag=kTRUE) const;

  Bool_t Clean(Float_t *cleanPars);
  Bool_t RemoveKink(Int_t i)   const;
  Bool_t RemoveV0(Int_t i)     const;
  Bool_t RemoveTrack(Int_t i)  const;

  const IlcESDVertex *GetPileupVertexSPD(Int_t i) const {
    return (const IlcESDVertex *)(fSPDPileupVertices?fSPDPileupVertices->At(i):0x0);
  }
  Char_t  AddPileupVertexSPD(const IlcESDVertex *vtx);
  const IlcESDVertex *GetPileupVertexTracks(Int_t i) const {
    return (const IlcESDVertex *)(fTrkPileupVertices?fTrkPileupVertices->At(i):0x0);
  }
  Char_t  AddPileupVertexTracks(const IlcESDVertex *vtx);
  TClonesArray* GetPileupVerticesTracks() const {return (TClonesArray*)fTrkPileupVertices;}
  TClonesArray* GetPileupVerticesSPD()    const {return (TClonesArray*)fSPDPileupVertices;}

  virtual Bool_t  IsPileupFromSPD(Int_t minContributors=3, 
				  Double_t minZdist=0.8, 
				  Double_t nSigmaZdist=3., 
				  Double_t nSigmaDiamXY=2., 
				  Double_t nSigmaDiamZ=5.) const;
  
  virtual Bool_t IsPileupFromSPDInMultBins() const;

  IlcESDtrack *GetTrack(Int_t i) const {
    if (!fTracks) return 0;
    IlcESDtrack* track = (IlcESDtrack*) fTracks->At(i);
    if (track) track->SetESDEvent(this);
    return track;
  }
  Int_t  AddTrack(const IlcESDtrack *t);

  /// add new track at the end of tracks array and return instance
  IlcESDtrack* NewTrack();
  
  IlcESDHLTtrack *GetHLTConfMapTrack(Int_t /*i*/) const {
    //    return (IlcESDHLTtrack *)fHLTConfMapTracks->At(i);
    return 0;
  }
  void AddHLTConfMapTrack(const IlcESDHLTtrack */*t*/) {
    printf("ESD:: AddHLTConfMapTrack do nothing \n");
    //    TClonesArray &fhlt = *fHLTConfMapTracks;
    //  new(fhlt[fHLTConfMapTracks->GetEntriesFast()]) IlcESDHLTtrack(*t);
  }
  

  IlcESDHLTtrack *GetHLTHoughTrack(Int_t /*i*/) const {
    //    return (IlcESDHLTtrack *)fHLTHoughTracks->At(i);
    return 0;
  }
  void AddHLTHoughTrack(const IlcESDHLTtrack */*t*/) {
    printf("ESD:: AddHLTHoughTrack do nothing \n");
    //    TClonesArray &fhlt = *fHLTHoughTracks;
    //     new(fhlt[fHLTHoughTracks->GetEntriesFast()]) IlcESDHLTtrack(*t);
  }
  
  Bool_t MoveMuonObjects();
  
  IlcESDMuonTrack* GetMuonTrack(Int_t i);
  IlcESDMuonTrack* NewMuonTrack();
  
  IlcESDMuonCluster* GetMuonCluster(Int_t i);
  IlcESDMuonCluster* FindMuonCluster(UInt_t clusterId);
  IlcESDMuonCluster* NewMuonCluster();
  
  IlcESDMuonPad* GetMuonPad(Int_t i);
  IlcESDMuonPad* FindMuonPad(UInt_t padId);
  IlcESDMuonPad* NewMuonPad();
  
  IlcESDPmdTrack *GetPmdTrack(Int_t i) const {
    return (IlcESDPmdTrack *)(fPmdTracks?fPmdTracks->At(i):0x0);
  }

  void AddPmdTrack(const IlcESDPmdTrack *t);


  IlcESDTrdTrack *GetTrdTrack(Int_t i) const {
    return (IlcESDTrdTrack *)(fTrdTracks?fTrdTracks->At(i):0x0);
  }

  
  void SetTrdTrigger(const IlcESDTrdTrigger *t);

  IlcESDTrdTrigger* GetTrdTrigger() const {
    return (IlcESDTrdTrigger*)(fTrdTrigger);
  }

  void AddTrdTrack(const IlcESDTrdTrack *t);

  IlcESDTrdTracklet* GetTrdTracklet(Int_t idx) const {
    return (IlcESDTrdTracklet*)(fTrdTracklets?fTrdTracklets->At(idx):0x0);
  }

  void AddTrdTracklet(const IlcESDTrdTracklet *trkl);

  IlcESDv0 *GetV0(Int_t i) const {
    return (IlcESDv0*)(fV0s?fV0s->At(i):0x0);
  }
  Int_t AddV0(const IlcESDv0 *v);

  IlcESDcascade *GetCascade(Int_t i) const {
    return (IlcESDcascade *)(fCascades?fCascades->At(i):0x0);
  }

  void AddCascade(const IlcESDcascade *c);

  IlcESDkink *GetKink(Int_t i) const {
    return (IlcESDkink *)(fKinks?fKinks->At(i):0x0);
  }
  Int_t AddKink(const IlcESDkink *c);

  IlcESDCaloCluster *GetCaloCluster(Int_t i) const {
    return (IlcESDCaloCluster *)(fCaloClusters?fCaloClusters->At(i):0x0);
  }

  Int_t AddCaloCluster(const IlcESDCaloCluster *c);

  IlcESDCaloCells *GetEMCALCells() const {return fEMCALCells; }  
  IlcESDCaloCells *GetPHOSCells() const {return fPHOSCells; }  

  IlcESDCaloTrigger* GetCaloTrigger(TString calo) const 
  {
	  if (calo.Contains("EMCAL")) return fEMCALTrigger;
	  else
		  return fPHOSTrigger;
  }

  IlcESDCosmicTrack *GetCosmicTrack(Int_t i) const {
    return fCosmicTracks ? (IlcESDCosmicTrack*) fCosmicTracks->At(i) : 0;
  }
  const TClonesArray * GetCosmicTracks() const{ return fCosmicTracks;}

  void  AddCosmicTrack(const IlcESDCosmicTrack *t);
	
  IlcRawDataErrorLog *GetErrorLog(Int_t i) const {
    return (IlcRawDataErrorLog *)(fErrorLogs?fErrorLogs->At(i):0x0);
  }
  void  AddRawDataErrorLog(const IlcRawDataErrorLog *log) const;

  Int_t GetNumberOfErrorLogs()   const {return fErrorLogs?fErrorLogs->GetEntriesFast():0;}

  Int_t GetNumberOfPileupVerticesSPD() const {
    return (fSPDPileupVertices?fSPDPileupVertices->GetEntriesFast():0);
  }
  Int_t GetNumberOfPileupVerticesTracks() const {
    return (fTrkPileupVertices?fTrkPileupVertices->GetEntriesFast():0);
  }
  Int_t GetNumberOfTracks()     const {return fTracks?fTracks->GetEntriesFast():0;}
  Int_t GetNumberOfHLTConfMapTracks()     const {return 0;} 
  // fHLTConfMapTracks->GetEntriesFast();}
  Int_t GetNumberOfHLTHoughTracks()     const {return  0;  }
  //  fHLTHoughTracks->GetEntriesFast();  }

  Int_t GetNumberOfMuonTracks() const {return fMuonTracks?fMuonTracks->GetEntriesFast():0;}
  Int_t GetNumberOfMuonClusters();
  Int_t GetNumberOfMuonPads();
  Int_t GetNumberOfPmdTracks() const {return fPmdTracks?fPmdTracks->GetEntriesFast():0;}
  Int_t GetNumberOfTrdTracks() const {return fTrdTracks?fTrdTracks->GetEntriesFast():0;}
  Int_t GetNumberOfTrdTracklets() const {return fTrdTracklets?fTrdTracklets->GetEntriesFast():0;}
  Int_t GetNumberOfV0s()      const {return fV0s?fV0s->GetEntriesFast():0;}
  Int_t GetNumberOfCascades() const {return fCascades?fCascades->GetEntriesFast():0;}
  Int_t GetNumberOfKinks() const {return fKinks?fKinks->GetEntriesFast():0;}

  Int_t GetNumberOfCosmicTracks() const {return fCosmicTracks ? fCosmicTracks->GetEntriesFast():0;}  
  Int_t GetEMCALClusters(TRefArray *clusters) const;
  Int_t GetPHOSClusters(TRefArray *clusters) const;
  Int_t GetNumberOfCaloClusters() const {return fCaloClusters?fCaloClusters->GetEntriesFast():0;}

  void SetUseOwnList(Bool_t b){fUseOwnList = b;}
  Bool_t GetUseOwnList() const {return fUseOwnList;}

  void ResetV0s() { if(fV0s) fV0s->Clear(); }
  void ResetCascades() { if(fCascades) fCascades->Clear(); }
  void Reset();

  void  Print(Option_t *option="") const;

  void AddObject(TObject* obj);
  void ReadFromTree(TTree *tree, Option_t* opt = "");
  TObject* FindListObject(const char *name) const;
  IlcESD *GetIlcESDOld(){return fESDOld;}
  void WriteToTree(TTree* tree) const;
  void GetStdContent();
  void ResetStdContent();
  void CreateStdContent();
  void CreateStdContent(Bool_t bUseThisList);
  void CompleteStdContent();
  void SetStdNames();
  void CopyFromOldESD();
  TList* GetList() const {return fESDObjects;}
  
    //Following needed only for mixed event
  virtual Int_t        EventIndex(Int_t)       const {return 0;}
  virtual Int_t        EventIndexForCaloCluster(Int_t) const {return 0;}
  virtual Int_t        EventIndexForPHOSCell(Int_t)    const {return 0;}
  virtual Int_t        EventIndexForEMCALCell(Int_t)   const {return 0;} 
  
  void SetDetectorStatus(ULong_t detMask) {fDetectorStatus|=detMask;}
  void ResetDetectorStatus(ULong_t detMask) {fDetectorStatus&=~detMask;}
  ULong_t GetDetectorStatus() const {return fDetectorStatus;}
  Bool_t IsDetectorOn(ULong_t detMask) const {return (fDetectorStatus&detMask)>0;}

protected:
  IlcESDEvent(const IlcESDEvent&);
  static Bool_t ResetWithPlacementNew(TObject *pObject);

  void AddMuonTrack(const IlcESDMuonTrack *t);
  
  TList *fESDObjects;             // List of esd Objects

  IlcESDRun       *fESDRun;           //! Run information tmp put in the Userdata
  IlcESDHeader    *fHeader;           //! ESD Event Header
  IlcESDZDC       *fESDZDC;           //! ZDC information
  IlcESDFMD       *fESDFMD;           //! FMD object containing rough multiplicity
  IlcESDVZERO     *fESDVZERO;         //! VZERO object containing rough multiplicity
  IlcESDTZERO     *fESDTZERO;         //! TZEROObject
  IlcESDVertex    *fTPCVertex;        //! Primary vertex estimated by the TPC
  IlcESDVertex    *fSPDVertex;        //! Primary vertex estimated by the SPD
  IlcESDVertex    *fPrimaryVertex;    //! Primary vertex estimated using ESD tracks
  IlcMultiplicity *fSPDMult;          //! SPD tracklet multiplicity
  IlcESDCaloTrigger* fPHOSTrigger;     //! PHOS Trigger information
  IlcESDCaloTrigger* fEMCALTrigger;    //! PHOS Trigger information
  IlcESDACORDE    *fESDACORDE;        //! ACORDE ESD object caontaining bit pattern
  IlcESDTrdTrigger *fTrdTrigger;      //! TRD trigger information

  TClonesArray *fSPDPileupVertices;//! Pileup primary vertices reconstructed by SPD 
  TClonesArray *fTrkPileupVertices;//! Pileup primary vertices reconstructed using the tracks 
  TClonesArray *fTracks;           //! ESD tracks 
  TClonesArray *fMuonTracks;       //! MUON ESD tracks
  TClonesArray *fMuonClusters;     //! MUON ESD clusters
  TClonesArray *fMuonPads;         //! MUON ESD pads
  TClonesArray *fPmdTracks;        //! PMD ESD tracks
  TClonesArray *fTrdTracks;        //! TRD ESD tracks (triggered)
  TClonesArray *fTrdTracklets;     //! TRD tracklets (for trigger)
  TClonesArray *fV0s;              //! V0 vertices
  TClonesArray *fCascades;         //! Cascade vertices
  TClonesArray *fKinks;            //! Kinks
  TClonesArray *fCaloClusters;     //! Calorimeter clusters for PHOS/EMCAL
  IlcESDCaloCells *fEMCALCells;     //! EMCAL cell info
  IlcESDCaloCells *fPHOSCells;     //! PHOS cell info
  TClonesArray *fCosmicTracks;     //! Tracks created by cosmics finder
  TClonesArray *fErrorLogs;        //! Raw-data reading error messages
 
  Bool_t fOldMuonStructure;        //! Flag if reading ESD with old MUON structure

  IlcESD       *fESDOld;           //! Old esd Structure
  IlcESDfriend *fESDFriendOld;     //! Old friend esd Structure
  Bool_t    fConnected;            //! flag if leaves are alreday connected
  Bool_t    fUseOwnList;           //! Do not use the list from the esdTree but use the one created by this class 

  static const char* fgkESDListName[kESDListN]; //!

  IlcTOFHeader *fTOFHeader;  //! event times (and sigmas) as estimated by TOF
			     //  combinatorial algorithm.
                             //  It contains also TOF time resolution
                             //  and T0spread as written in OCDB
  IlcCentrality *fCentrality; //! Centrality for AA collision
  IlcEventplane *fEventplane; //! Event plane for AA collision

  ULong_t fDetectorStatus; // set detector event status bit for good event selection

  ClassDef(IlcESDEvent,17)  //ESDEvent class 
};
#endif 

