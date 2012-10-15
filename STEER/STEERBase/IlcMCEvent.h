// -*- mode: C++ -*- 
#ifndef ILCMCEVENT_H
#define ILCMCEVENT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcMCEvent.h 52237 2011-10-20 19:26:08Z hristov $ */

//-------------------------------------------------------------------------
//                          Class IlcMCEvent
//      
// Origin: Andreas.Morsch, CERN, andreas.morsch@cern.ch 
//-------------------------------------------------------------------------


#include <TTree.h>
#include <TRefArray.h>
#include <TClonesArray.h>

#include <IlcVEvent.h>
#include "IlcVHeader.h"
#include "IlcVParticle.h"
#include "IlcVVertex.h"
#include "IlcMCParticle.h"

class IlcCentrality;
class IlcEventplane;
class IlcStack;
class IlcHeader;
class IlcGenEventHeader;

class TClonesArray;
class TList;

class IlcMCEvent : public IlcVEvent {

public:

    IlcMCEvent();
    virtual ~IlcMCEvent() {;} 
    IlcMCEvent(const IlcMCEvent& mcEvnt); 
    IlcMCEvent& operator=(const IlcMCEvent& mcEvnt);
    //
    // Methods implementing the interface
    //
    // Services
    virtual void AddObject(TObject* /*obj*/)               {;}
    virtual TObject* FindListObject(const char */*name*/)  const {return 0;}
    virtual TList* GetList() const                         {return 0;}
    virtual void CreateStdContent()                        {;} 
    virtual void GetStdContent()                           {;}
    virtual void ReadFromTree(TTree * /*tree*/, Option_t* /*opt*/) {;}
    virtual void WriteToTree(TTree* /*tree*/)  const       {;}

    virtual void SetStdNames()                             {;}
    virtual void Print(Option_t */*option=""*/)  const     {;}
    virtual void PreReadAll();
    virtual void Reset()                                   {;}

    // Header
    virtual IlcVHeader* GetHeader()          const         {return 0;}

    // Delegated methods for fESDRun or AODHeader
  
    virtual void     SetRunNumber(Int_t /*n*/)             {;}
    virtual void     SetPeriodNumber(UInt_t /*n*/)         {;}
    virtual void     SetMagneticField(Double_t /*mf*/)     {;}
    
  
    virtual Int_t    GetRunNumber()          const         {return 0;}
    virtual UInt_t   GetPeriodNumber()       const         {return 0;}
    virtual Double_t GetMagneticField()      const         {return 0.;}

    // Setters not needed
    virtual void      SetOrbitNumber(UInt_t /*n*/)         {;}
    virtual void      SetBunchCrossNumber(UShort_t /*n*/)  {;}
    virtual void      SetEventType(UInt_t /*eventType*/)   {;}
    virtual void      SetTriggerMask(ULong64_t /*n*/)      {;}
    virtual void      SetTriggerCluster(UChar_t /*n*/)     {;} 

    virtual UInt_t    GetOrbitNumber()        const {return 0;}
    virtual UShort_t  GetBunchCrossNumber()   const {return 0;}
    
    virtual UInt_t    GetEventType()          const {return 0;}

    virtual ULong64_t GetTriggerMask()        const {return 0;}
    virtual UChar_t   GetTriggerCluster()     const {return 0;}
    virtual Double_t  GetZDCN1Energy()        const {return 0.;}
    virtual Double_t  GetZDCP1Energy()        const {return 0.;}
    virtual Double_t  GetZDCN2Energy()        const {return 0.;}
    virtual Double_t  GetZDCP2Energy()        const {return 0.;}
    virtual Double_t  GetZDCEMEnergy(Int_t /*i*/) 
                                              const {return 0.;}
    // Tracks
    virtual IlcVParticle *GetTrack(Int_t i) const;
    virtual Int_t     GetNumberOfTracks()    const {return fNparticles;}
    virtual Int_t     GetNumberOfV0s()       const {return -1;}
    virtual Int_t     GetNumberOfCascades()  const {return -1;}
    // Vertex
    virtual const IlcVVertex *GetPrimaryVertex() const;
    
    //
    // MC Specific methods
    //
    // Getters
    IlcStack*    Stack()   {return fStack;}
    IlcHeader*   Header()  {return fHeader;}
    IlcGenEventHeader* GenEventHeader() const;
    // Services
    virtual void      ConnectTreeE (TTree* tree);
    virtual void      ConnectTreeK (TTree* tree);
    virtual void      ConnectTreeTR(TTree* tree);
    virtual void      Clean();
    virtual void      InitEvent();
    virtual void      FinishEvent();
    virtual Int_t     GetParticleAndTR(Int_t i, TParticle*& particle, TClonesArray*& trefs);
    virtual void      DrawCheck(Int_t i, Int_t search);
    virtual void      AddSubsidiaryEvent(IlcMCEvent* event);
    virtual Int_t     GetNumberOfPrimaries() {return fNprimaries;}
    virtual Int_t     GetPrimaryOffset()    const {return fPrimaryOffset;}
    virtual Int_t     GetSecondaryOffset()  const {return fSecondaryOffset;}    
    virtual void      SetPrimaryOffset(Int_t ioff)    {fPrimaryOffset = ioff;}
    virtual void      SetSecondaryOffset(Int_t ioff)  {fSecondaryOffset = ioff;}    
    virtual Bool_t    IsPhysicalPrimary(Int_t i) const;
    virtual Int_t     BgLabelToIndex(Int_t label);
    static  Int_t     BgLabelOffset() {return fgkBgLabelOffset;}
    virtual Bool_t    IsFromBGEvent(Int_t index);
    
    // External particle array
    virtual void      SetParticleArray(TClonesArray* mcParticles) 
	{fMCParticles = mcParticles; fNparticles = fMCParticles->GetEntries(); fExternal = kTRUE;}
    
    //Following needed only for mixed event
  virtual Int_t        EventIndex(Int_t)       const {return 0;}
  virtual Int_t        EventIndexForCaloCluster(Int_t) const {return 0;}
  virtual Int_t        EventIndexForPHOSCell(Int_t)    const {return 0;}
  virtual Int_t        EventIndexForEMCALCell(Int_t)   const {return 0;} 
  IlcCentrality*       GetCentrality()  {return 0;} 
  IlcEventplane*       GetEventplane()  {return 0;} 

  virtual IlcVVZERO    *GetVZEROData() const {return 0;}
  virtual IlcVZDC      *GetZDCData()   const {return 0;}
    
//   #ifdef MFT_UPGRADE
//   virtual IlcVMFT *GetMFTData() const {return 0;}
//   #endif
  virtual IlcVMFT *GetMFTData() const {return 0;}    // AU
	
private:
    virtual void      ReorderAndExpandTreeTR();
    virtual Int_t     FindIndexAndEvent(Int_t oldidx, IlcMCEvent*& event) const;
    
private: 
    // Stanndard implementation for ESD production
    IlcStack         *fStack;            // Current pointer to stack
    TClonesArray     *fMCParticles;      // Pointer to list of particles
    TObjArray        *fMCParticleMap;    // Map of MC Particles
    IlcHeader        *fHeader;           // Current pointer to header
    TClonesArray     *fTRBuffer;         // Track reference buffer    
    TClonesArray     *fTrackReferences;  // Array of track references
    TTree            *fTreeTR;           // Pointer to Track Reference Tree
    TTree            *fTmpTreeTR;        // Temporary tree TR to read old format
    TFile            *fTmpFileTR;        // Temporary file with TreeTR to read old format
    Int_t             fNprimaries;       // Number of primaries
    Int_t             fNparticles;       // Number of particles
    TList            *fSubsidiaryEvents; // List of possible subsidiary events (for example merged underlying event) 
    Int_t             fPrimaryOffset;    // Offset for primaries
    Int_t             fSecondaryOffset;  // Offset for secondaries
    Bool_t            fExternal;         // True if external particle array
    static   Int_t        fgkBgLabelOffset;  // Standard branch name    
    mutable  IlcVVertex*  fVertex;           // MC Vertex
    Int_t             fNBG;              //! Background particles in current event
    
    ClassDef(IlcMCEvent, 2)              // IlcVEvent realisation for MC data
};


#endif 

