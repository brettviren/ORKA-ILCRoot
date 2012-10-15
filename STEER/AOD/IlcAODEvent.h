#ifndef IlcAODEvent_H
#define IlcAODEvent_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAODEvent.h 58340 2012-08-28 15:56:24Z hristov $ */

//-------------------------------------------------------------------------
//     AOD base class
//     Author: Markus Oldenburg, CERN
//-------------------------------------------------------------------------

#include <TBuffer.h>
#include <TClonesArray.h>
#include <TList.h>
#include <TTree.h>
#include <TNamed.h>

#include "IlcVEvent.h"
#include "IlcVParticle.h"
#include "IlcAODHeader.h"
#include "IlcAODTrack.h"
#include "IlcAODVertex.h"
#include "IlcAODv0.h"
#include "IlcAODcascade.h"
#include "IlcAODTracklets.h"
#include "IlcAODJet.h"
#include "IlcAODCaloCells.h"
#include "IlcAODCaloCluster.h"
#include "IlcAODCaloTrigger.h"
#include "IlcAODPmdCluster.h"
#include "IlcAODFmdCluster.h"
#include "IlcAODDimuon.h"
#include "IlcAODTZERO.h"
#include "IlcAODVZERO.h"
#include "IlcAODHMPIDrings.h"
#include "IlcAODZDC.h"
#include "IlcTOFHeader.h"
#ifdef MFT_UPGRADE
#include "IlcAODMFT.h"
#endif

class TTree;
class TFolder;
class IlcCentrality;
class IlcEventplane;

class IlcAODEvent : public IlcVEvent {

 public :
  enum AODListIndex_t {kAODHeader,
		       kAODTracks,
		       kAODVertices,
		       kAODv0,
		       kAODcascade,
		       kAODTracklets,
		       kAODJets,
		       kAODEmcalCells,
		       kAODPhosCells,
		       kAODCaloClusters,
	           kAODEMCALTrigger,
	           kAODPHOSTrigger,
		       kAODFmdClusters,
		       kAODPmdClusters,
                       kAODHMPIDrings,
		       kAODDimuons,
		       kAODTZERO,
		       kAODVZERO,
		       kAODZDC,
		       kTOFHeader,                       
		       kAODListN
	           #ifdef MFT_UPGRADE
	           ,kAODVZERO
			   #endif
  };

  IlcAODEvent();
  virtual ~IlcAODEvent();

  IlcAODEvent(const IlcAODEvent& aodevent);
  IlcAODEvent& operator=(const IlcAODEvent& aodevent);

  void          AddObject(TObject *obj);
  void          RemoveObject(TObject *obj);
  TObject      *FindListObject(const char *objName) const;
  TList        *GetList()                const { return fAODObjects; }
  void          SetConnected(Bool_t conn=kTRUE) {fConnected=conn;}
  Bool_t        GetConnected() const {return fConnected;}

  // -- Header
  IlcAODHeader *GetHeader()              const { return fHeader; }
  void          AddHeader(const IlcAODHeader* hdx)
    {
	delete fHeader; fHeader = new IlcAODHeader(*hdx);
	(fAODObjects->FirstLink())->SetObject(fHeader);
    }

  // setters and getters for header information
  void     SetRunNumber(Int_t n) {if (fHeader) fHeader->SetRunNumber(n);}
  void     SetPeriodNumber(UInt_t n){if (fHeader) fHeader->SetPeriodNumber(n);}
  void     SetOrbitNumber(UInt_t n) {if (fHeader) fHeader->SetOrbitNumber(n);}
  void     SetBunchCrossNumber(UShort_t n) {if (fHeader) fHeader->SetBunchCrossNumber(n);}
  void     SetMagneticField(Double_t mf){if (fHeader) fHeader->SetMagneticField(mf);}
  void     SetMuonMagFieldScale(Double_t mf){if (fHeader) fHeader->SetMuonMagFieldScale(mf);}
  void     SetDiamond(Float_t xy[2],Float_t cov[3]){if (fHeader) fHeader->SetDiamond(xy,cov);}
  void     SetDiamondZ(Float_t z, Float_t sig2z){if (fHeader) fHeader->SetDiamondZ(z,sig2z);}
  Int_t    GetRunNumber() const {return fHeader ? fHeader->GetRunNumber() : -999;}
  UInt_t   GetPeriodNumber() const {return fHeader ? fHeader->GetPeriodNumber() : 0;}
  UInt_t   GetOrbitNumber() const {return fHeader ? fHeader->GetOrbitNumber() : 0;}
  UShort_t GetBunchCrossNumber() const {return fHeader ? fHeader->GetBunchCrossNumber() : 0;}
  Double_t GetMagneticField() const {return fHeader ? fHeader->GetMagneticField() : -999.;}
  Double_t GetMuonMagFieldScale() const {return fHeader ? fHeader->GetMuonMagFieldScale() : -999.;}
  Double_t GetDiamondX() const {return fHeader ? fHeader->GetDiamondX() : -999.;}
  Double_t GetDiamondY() const {return fHeader ? fHeader->GetDiamondY() : -999.;}
  Double_t GetDiamondZ() const {return fHeader ? fHeader->GetDiamondZ() : -999.;}
  void     GetDiamondCovXY(Float_t cov[3]) const {cov[0]=-999.; if(fHeader) fHeader->GetDiamondCovXY(cov);}
  Double_t GetSigma2DiamondX() const {return fHeader ? fHeader->GetSigma2DiamondX() : -999.;}
  Double_t GetSigma2DiamondY() const {return fHeader ? fHeader->GetSigma2DiamondY() : -999.;}
  Double_t GetSigma2DiamondZ() const {return fHeader ? fHeader->GetSigma2DiamondZ() : -999.;}
  
  void      SetEventType(UInt_t eventType){fHeader->SetEventType(eventType);}
  void      SetTriggerMask(ULong64_t n) {fHeader->SetTriggerMask(n);}
  void      SetTriggerCluster(UChar_t n) {fHeader->SetTriggerCluster(n);}

  UInt_t    GetEventType()          const { return fHeader ? fHeader->GetEventType() : 0;}
  ULong64_t GetTriggerMask()        const { return fHeader ? fHeader->GetTriggerMask() : 0;}
  UChar_t   GetTriggerCluster()     const { return fHeader ? fHeader->GetTriggerCluster() : 0;}
  TString   GetFiredTriggerClasses()const { return fHeader->GetFiredTriggerClasses();};
  Double_t  GetZDCN1Energy()        const { return fHeader ? fHeader->GetZDCN1Energy() : -999.; }
  Double_t  GetZDCP1Energy()        const { return fHeader ? fHeader->GetZDCP1Energy() : -999.; }
  Double_t  GetZDCN2Energy()        const { return fHeader ? fHeader->GetZDCN2Energy() : -999.; }
  Double_t  GetZDCP2Energy()        const { return fHeader ? fHeader->GetZDCP2Energy() : -999.; }
  Double_t  GetZDCEMEnergy(Int_t i) const { return fHeader ? fHeader->GetZDCEMEnergy(i) : -999.; }

  void SetTOFHeader(const IlcTOFHeader * tofEventTime);
  const IlcTOFHeader *GetTOFHeader() const {return fTOFHeader;}
  Float_t GetEventTimeSpread() const {if (fTOFHeader) return fTOFHeader->GetT0spread(); else return 0.;}
  Float_t GetTOFTimeResolution() const {if (fTOFHeader) return fTOFHeader->GetTOFResolution(); else return 0.;}


  // -- Tracks
  TClonesArray *GetTracks()              const { return fTracks; }
  Int_t         GetNTracks()             const { return fTracks? fTracks->GetEntriesFast() : 0; }
  Int_t         GetNumberOfTracks()      const { return GetNTracks(); }
  IlcAODTrack  *GetTrack(Int_t nTrack)   const { if(fTracks && fTracks->UncheckedAt(nTrack)) ((IlcAODTrack*)fTracks->UncheckedAt(nTrack))->SetAODEvent(this);return fTracks ? (IlcAODTrack*)fTracks->UncheckedAt(nTrack):0; }
  Int_t         AddTrack(const IlcAODTrack* trk)
    {new((*fTracks)[fTracks->GetEntriesFast()]) IlcAODTrack(*trk); return fTracks->GetEntriesFast()-1;}
  Int_t         GetMuonTracks(TRefArray *muonTracks) const;
  Int_t         GetNumberOfMuonTracks() const;

  // -- Vertex
  TClonesArray *GetVertices()            const { return fVertices; }
  Int_t         GetNumberOfVertices()    const { return fVertices?fVertices->GetEntriesFast():0; }
  IlcAODVertex *GetVertex(Int_t nVertex) const { return fVertices?(IlcAODVertex*)fVertices->At(nVertex):0; }
  Int_t         AddVertex(const IlcAODVertex* vtx)
  {new((*fVertices)[fVertices->GetEntriesFast()]) IlcAODVertex(*vtx); return fVertices->GetEntriesFast()-1;}
  
  // primary vertex
  virtual IlcAODVertex *GetPrimaryVertex() const { return GetVertex(0); }
  virtual IlcAODVertex *GetPrimaryVertexSPD() const;

  // -- Pileup vertices 
  Int_t         GetNumberOfPileupVerticesTracks()   const;
  Int_t         GetNumberOfPileupVerticesSPD()    const;
  virtual IlcAODVertex *GetPileupVertexSPD(Int_t iV=0) const;
  virtual IlcAODVertex *GetPileupVertexTracks(Int_t iV=0) const;
  virtual Bool_t  IsPileupFromSPD(Int_t minContributors=3, Double_t minZdist=0.8, Double_t nSigmaZdist=3., Double_t nSigmaDiamXY=2., Double_t nSigmaDiamZ=5.) const;
  virtual Bool_t IsPileupFromSPDInMultBins() const;


  // V0
  TClonesArray *GetV0s()                 const { return fV0s; }
  Int_t         GetNumberOfV0s()         const { return fV0s->GetEntriesFast(); }
  IlcAODv0     *GetV0(Int_t nV0)         const { return (IlcAODv0*)fV0s->UncheckedAt(nV0); }
  Int_t         AddV0(const IlcAODv0* v0)
  {new((*fV0s)[fV0s->GetEntriesFast()]) IlcAODv0(*v0); return fV0s->GetEntriesFast()-1;}

  // Cascades
  TClonesArray  *GetCascades()            const { return fCascades; }
  Int_t          GetNumberOfCascades()    const { return fCascades->GetEntriesFast(); }
  IlcAODcascade *GetCascade(Int_t nCasc)  const { return (IlcAODcascade*)fCascades->UncheckedAt(nCasc); }
  Int_t          AddCascade(const IlcAODcascade* cascade)
  {new((*fCascades)[fCascades->GetEntriesFast()]) IlcAODcascade(*cascade); return fCascades->GetEntriesFast()-1;}

  // -- EMCAL and PHOS Cluster
  TClonesArray *GetCaloClusters()          const { return fCaloClusters; }
  Int_t         GetNumberOfCaloClusters()  const { return fCaloClusters->GetEntriesFast(); }
  IlcAODCaloCluster *GetCaloCluster(Int_t nCluster) const { return (IlcAODCaloCluster*)fCaloClusters->UncheckedAt(nCluster); }
  Int_t         AddCaloCluster(const IlcAODCaloCluster* clus)
  {new((*fCaloClusters)[fCaloClusters->GetEntriesFast()]) IlcAODCaloCluster(*clus); return fCaloClusters->GetEntriesFast()-1;}
  IlcAODCaloTrigger *GetCaloTrigger(TString calo) const 
  { 	  
     if (calo.Contains("EMCAL")) return fEMCALTrigger;
     else
     return fPHOSTrigger; 
  }	
	
  Int_t GetEMCALClusters(TRefArray *clusters) const;
  Int_t GetPHOSClusters(TRefArray *clusters) const;


  // -- FMD Cluster
  TClonesArray *GetFmdClusters()        const { return fFmdClusters; }
  Int_t         GetNFmdClusters()       const { return fFmdClusters->GetEntriesFast(); }
  IlcAODFmdCluster *GetFmdCluster(Int_t nCluster) const { return (IlcAODFmdCluster*)fFmdClusters->UncheckedAt(nCluster); }
  Int_t         AddFmdCluster(const IlcAODFmdCluster* clus)
  {new((*fFmdClusters)[fFmdClusters->GetEntriesFast()]) IlcAODFmdCluster(*clus); return fFmdClusters->GetEntriesFast()-1;}

  // -- PMD Cluster
  TClonesArray *GetPmdClusters()        const { return fPmdClusters; }
  Int_t         GetNPmdClusters()       const { return fPmdClusters->GetEntriesFast(); }
  IlcAODPmdCluster *GetPmdCluster(Int_t nCluster) const { return (IlcAODPmdCluster*)fPmdClusters->UncheckedAt(nCluster); }
  Int_t         AddPmdCluster(const IlcAODPmdCluster* clus)
  {new((*fPmdClusters)[fPmdClusters->GetEntriesFast()]) IlcAODPmdCluster(*clus); return fPmdClusters->GetEntriesFast()-1;}

  // -- HMPID objects 
  TClonesArray *GetHMPIDrings()       const {return fHMPIDrings; } 
  Int_t         GetNHMPIDrings();
  IlcAODHMPIDrings *GetHMPIDring(Int_t nRings);
  Int_t         AddHMPIDrings(const  IlcAODHMPIDrings* ring) 
  {new((*fHMPIDrings)[fHMPIDrings->GetEntriesFast()]) IlcAODHMPIDrings(*ring); return fHMPIDrings->GetEntriesFast()-1;}
  
  IlcAODHMPIDrings *GetHMPIDringForTrackID(Int_t trackID);
  
  
  // -- Jet
  TClonesArray *GetJets()            const { return fJets; }
  Int_t         GetNJets()           const { return fJets?fJets->GetEntriesFast():0; }
  IlcAODJet    *GetJet(Int_t nJet) const { return fJets?(IlcAODJet*)fJets->UncheckedAt(nJet):0; }
  Int_t         AddJet(const IlcAODJet* vtx)
    {new((*fJets)[fJets->GetEntriesFast()]) IlcAODJet(*vtx); return fJets->GetEntriesFast()-1;}

  // -- Tracklets
  IlcAODTracklets *GetTracklets() const { return fTracklets; }

  // -- Calorimeter Cells
  IlcAODCaloCells *GetEMCALCells() const { return fEmcalCells; }
  IlcAODCaloCells *GetPHOSCells() const { return fPhosCells; }
  const TGeoHMatrix* GetPHOSMatrix(Int_t /*i*/) const { return NULL; }
  const TGeoHMatrix* GetEMCALMatrix(Int_t /*i*/)const { return NULL; }


  // -- Dimuons
  TClonesArray *GetDimuons()              const { return fDimuons; }
  Int_t         GetNDimuons()             const { return fDimuons->GetEntriesFast(); }
  Int_t         GetNumberOfDimuons()      const { return GetNDimuons(); }
  IlcAODDimuon *GetDimuon(Int_t nDimu)    const { return (IlcAODDimuon*)fDimuons->UncheckedAt(nDimu); }
  Int_t         AddDimuon(const IlcAODDimuon* dimu)
    {new((*fDimuons)[fDimuons->GetEntriesFast()]) IlcAODDimuon(*dimu); return fDimuons->GetEntriesFast()-1;}
  
  // -- Services
  void    CreateStdContent();
  void    SetStdNames();
  void    GetStdContent();
  void    CreateStdFolders();
  void    ResetStd(Int_t trkArrSize = 0, 
		   Int_t vtxArrSize = 0, 
		   Int_t v0ArrSize = 0, 
		   Int_t cascadeArrSize = 0,
		   Int_t jetSize = 0, 
		   Int_t caloClusSize = 0, 
		   Int_t fmdClusSize = 0, 
		   Int_t pmdClusSize = 0,
                   Int_t hmpidRingsSize = 0,
		   Int_t dimuonArrsize =0
		   );
  void    ClearStd();
  void    Reset(); 
  void    ReadFromTree(TTree *tree, Option_t* opt = "");
  void    WriteToTree(TTree* tree) const {tree->Branch(fAODObjects);}

  void  Print(Option_t *option="") const;
  void  MakeEntriesReferencable();
  static void AssignIDtoCollection(const TCollection* col);
  
    //Following needed only for mixed event
  virtual Int_t        EventIndex(Int_t)       const {return 0;}
  virtual Int_t        EventIndexForCaloCluster(Int_t) const {return 0;}
  virtual Int_t        EventIndexForPHOSCell(Int_t)    const {return 0;}
  virtual Int_t        EventIndexForEMCALCell(Int_t)   const {return 0;} 
  IlcCentrality*       GetCentrality() {return fHeader->GetCentralityP();} 
  IlcEventplane*       GetEventplane() {return fHeader->GetEventplaneP();}

  // TZERO 
  IlcAODTZERO *GetTZEROData() const { return fAODTZERO; }
  Double32_t GetT0TOF(Int_t icase) const { return fAODTZERO?fAODTZERO->GetT0TOF(icase):999999;}
  const Double32_t * GetT0TOF() const { return fAODTZERO?fAODTZERO->GetT0TOF():0x0;}
 
  // VZERO 
  IlcAODVZERO *GetVZEROData() const { return fAODVZERO; }
  virtual const Float_t* GetVZEROEqFactors() const {return fHeader?fHeader->GetVZEROEqFactors():0x0;}
  virtual Float_t        GetVZEROEqMultiplicity(Int_t i) const;
  void           SetVZEROEqFactors(const Float_t *factors) const {
    if(fHeader && factors)
      fHeader->SetVZEROEqFactors(factors);}

  //ZDC
  IlcAODZDC   *GetZDCData() const { return fAODZDC; }

#ifdef MFT_UPGRADE
  // MFT 
  IlcAODMFT *GetMFTData() const { return fAODMFT; }
#endif

  private :

  TList   *fAODObjects; //  list of AODObjects
  TFolder *fAODFolder;  //  folder structure of branches
  Bool_t   fConnected;  //! flag if leaves are alreday connected 
  // standard content
  IlcAODHeader    *fHeader;       //! event information
  TClonesArray    *fTracks;       //! charged tracks
  TClonesArray    *fVertices;     //! vertices
  TClonesArray    *fV0s;          //! V0s
  TClonesArray    *fCascades;     //! Cascades
  IlcAODTracklets *fTracklets;    //! SPD tracklets
  TClonesArray    *fJets;         //! jets
  IlcAODCaloCells *fEmcalCells;   //! EMCAL calorimenter cells
  IlcAODCaloCells *fPhosCells;    //! PHOS calorimenter cells
  TClonesArray    *fCaloClusters; //! calorimeter clusters
  IlcAODCaloTrigger *fEMCALTrigger; //! EMCAL Trigger information
  IlcAODCaloTrigger *fPHOSTrigger;  //! PHOS Trigger information
  TClonesArray    *fFmdClusters;  //! FMDclusters
  TClonesArray    *fPmdClusters;  //! PMDclusters
  TClonesArray    *fHMPIDrings;   //! HMPID signals
  TClonesArray    *fDimuons;      //! dimuons
  IlcAODTZERO     *fAODTZERO;     //! TZERO AOD
  IlcAODVZERO     *fAODVZERO;     //! VZERO AOD
  IlcAODZDC       *fAODZDC;       //! ZDC AOD
  IlcTOFHeader    *fTOFHeader;  //! event times (and sigmas) as estimated by TOF
			     //  combinatorial algorithm.
                             //  It contains also TOF time resolution
                             //  and T0spread as written in OCDB
#ifdef MFT_UPGRADE
  IlcAODMFT       *fAODMFT;       //! VZERO AOD
#endif
  
  static const char* fAODListName[kAODListN]; //!

  ClassDef(IlcAODEvent,91);
};

#endif
