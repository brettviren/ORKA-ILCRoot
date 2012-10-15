#ifndef ILCQADATAMAKER_H
#define ILCQADATAMAKER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcQADataMaker.h 50615 2011-07-16 23:19:19Z hristov $ */

//
//  Base Class:
//  Produces the data needed to calculate the quality assurance. 
//  All data must be mergeable objects.
//  Y. Schutz CERN July 2007
//


// --- ROOT system ---
#include <TH1.h>
#include <TObjArray.h>
#include <TNamed.h>  
#include <TArrayI.h>  
//class TCanvas ; 
class TClonesArray;
class TDirectory;
class TFile;  
class TObject; 
class TTree; 
class IlcESDEvent;
class IlcRawReader;
class IlcDetectorRecoParam;

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcQAv1.h"
#include "IlcRecoParam.h" 

class IlcQADataMaker: public TNamed {
  
 public:

  IlcQADataMaker(const Char_t * name="", const Char_t * title="") ;          // ctor
  IlcQADataMaker(const IlcQADataMaker& qadm) ;   
  virtual ~IlcQADataMaker() ; // dtor
  //
  void   ForbidCloning(TH1* h, Bool_t v=kTRUE)                       {h->SetBit(IlcQAv1::GetForbidCloningBit(),v);}
  Bool_t IsCloningForbidden(TH1* h)                            const {return h->TestBit(IlcQAv1::GetForbidCloningBit());}
  Char_t* GetHistoTrigger(const TH1* h)                        const;
  //
  virtual Int_t Add2DigitsList(TH1 * hist, const Int_t index, const Bool_t expert = kFALSE, const Bool_t image = kFALSE)          = 0 ; 
  virtual Int_t Add2ESDsList(TH1 * hist, const Int_t index, const Bool_t expert = kFALSE, const Bool_t image = kFALSE)            = 0 ; 
  virtual Int_t Add2HitsList(TH1 * hist, const Int_t index, const Bool_t expert = kFALSE, const Bool_t image = kFALSE)            = 0 ; 
  virtual Int_t Add2RecPointsList(TH1 * hist, const Int_t index, const Bool_t expert = kFALSE, const Bool_t image = kFALSE)       = 0 ; 
  virtual Int_t Add2RawsList(TH1 * hist, const Int_t index, const Bool_t expert = kFALSE, const Bool_t image = kFALSE, const Bool_t saveForCorr = kFALSE)            = 0 ; 
  virtual Int_t Add2SDigitsList(TH1 * hist, const Int_t index, const Bool_t expert = kFALSE, const Bool_t image = kFALSE)         = 0 ; 
  virtual void        Exec(IlcQAv1::TASKINDEX_t, TObject * data)             = 0 ;
  virtual void        EndOfCycle()                                           = 0 ;
  virtual void        EndOfCycle(IlcQAv1::TASKINDEX_t )                      = 0 ;
  virtual void        EndOfDetectorCycle(IlcQAv1::TASKINDEX_t task, TObjArray ** list ) = 0 ; 
  void                Finish() const ; 
  IlcRecoParam::EventSpecie_t GetEventSpecie() const { return fEventSpecie ; }
  //
  virtual TObject*    GetDigitsData(const Int_t index)                       = 0 ; 
  virtual TObject*    GetESDsData(const Int_t index)                         = 0 ; 
  virtual TObject*    GetHitsData(const Int_t index)                         = 0 ; 
  virtual TObject*    GetRecPointsData(const Int_t index)                    = 0 ; 
  virtual TObject*    GetRawsData(const Int_t index)                         = 0 ; 
  virtual TObject*    GetSDigitsData(const Int_t index)                      = 0 ; 
  //
  virtual TH1*        GetDigitsData(const Int_t index, int cloneID)         = 0 ; 
  virtual TH1*        GetESDsData(const Int_t index, int cloneID)           = 0 ; 
  virtual TH1*        GetHitsData(const Int_t index, int cloneID)           = 0 ; 
  virtual TH1*        GetRecPointsData(const Int_t index, int cloneID)      = 0 ; 
  virtual TH1*        GetRawsData(const Int_t index, int cloneID)           = 0 ; 
  virtual TH1*        GetSDigitsData(const Int_t index, int cloneID)        = 0 ; 
  //
  virtual TObjArray*  GetDigitsDataOfTrigClass(int cloneID, TObjArray *dest=0)      = 0;
  virtual TObjArray*  GetSDigitsDataOfTrigClass(int cloneID, TObjArray *dest=0)     = 0;
  virtual TObjArray*  GetESDsDataOfTrigClass(int cloneID, TObjArray *dest=0)        = 0;
  virtual TObjArray*  GetHitsDataOfTrigClass(int cloneID, TObjArray *dest=0)        = 0;
  virtual TObjArray*  GetRecPointsDataOfTrigClass(int cloneID, TObjArray *dest=0)   = 0;
  virtual TObjArray*  GetRawsDataOfTrigClass(int cloneID, TObjArray *dest=0)        = 0;

  //
  const Char_t *      GetDetectorDirName() const { return fDetectorDirName.Data() ; }
  TList *             GetParameterList() const { return fParameterList[IlcRecoParam::AConvert(fEventSpecie)] ; }
  virtual const IlcDetectorRecoParam * GetRecoParam() { return NULL ; }
  Int_t               GetRun() const { return fRun ; } 
  Int_t               Increment() { return ++fCycleCounter ; } 
  virtual TObjArray** Init(IlcQAv1::TASKINDEX_t, Int_t cycles = -1)                                 = 0 ;
  TObjArray*          Init(IlcQAv1::TASKINDEX_t, IlcRecoParam::EventSpecie_t es, Int_t cycles = -1) ;
  virtual void        Init(IlcQAv1::TASKINDEX_t, TObjArray ** list, Int_t run, Int_t cycles = -1)   = 0 ;
  virtual void        InitDigits()        = 0 ; 
  virtual void        InitESDs()          = 0 ; 
  virtual void        InitRaws()          = 0 ; 
  virtual void        InitRecPoints()     = 0 ; 
  Bool_t              IsCycleDone() const { return fCycleCounter > fCycle ? kTRUE : kFALSE ; }
  Bool_t              IsValidEventSpecie(Int_t eventSpecieIndex, TObjArray ** list) ; 
  void                Reset() { fCycleCounter = 0 ; }
  virtual void        ResetDetector(IlcQAv1::TASKINDEX_t task) = 0 ;
  void                SetCycle(Int_t nevts) { fCycle = nevts ; } 
  void                SetWriteExpert() { fWriteExpert = kTRUE ; }
  virtual void        StartOfCycle(Int_t run = -1)                                                   = 0 ;
  virtual void        StartOfCycle(IlcQAv1::TASKINDEX_t, Int_t run, const Bool_t sameCycle = kFALSE) = 0 ;
  void                UnSetWriteExpert() { fWriteExpert = kFALSE ; }
  Bool_t              WriteExpert() { return fWriteExpert ; }
  void                SetEventSpecie(IlcRecoParam::EventSpecie_t es) {fEventSpecie = es;}
  void                SetEventSpecie(Int_t es) { SetEventSpecie(IlcRecoParam::Convert(es)); }
  virtual void        SetRecoParam(const IlcDetectorRecoParam *) {;}

  virtual void        InitRecPointsForTracker() {;} // needed by IlcGlobalQADataMaker

  Int_t               IsClonedPerTrigClass(Int_t index, TObjArray ** list);
  Bool_t              IsOrigHistoKept(Int_t index, TObjArray ** list);
  Bool_t              MatchesToTriggers(UInt_t hpos) const;
  //
  virtual TObjArray* GetMatchingDigitsData(const Int_t index, TObjArray* optDest=0)     = 0 ; 
  virtual TObjArray* GetMatchingESDsData(const Int_t index, TObjArray* optDest=0)       = 0 ; 
  virtual TObjArray* GetMatchingHitsData(const Int_t index, TObjArray* optDest=0)       = 0 ; 
  virtual TObjArray* GetMatchingRecPointsData(const Int_t index, TObjArray* optDest=0)  = 0 ; 
  virtual TObjArray* GetMatchingRawsData(const Int_t index, TObjArray* optDest=0)       = 0 ; 
  virtual TObjArray* GetMatchingSDigitsData(const Int_t index, TObjArray* optDest=0)    = 0 ; 
  //
  virtual TH1*       GetMatchingRawsHisto(Int_t index, Int_t trigId)       = 0;
  virtual TH1*       GetMatchingDigitsHisto(Int_t index, Int_t trigId)     = 0;
  virtual TH1*       GetMatchingSDigitsHisto(Int_t index, Int_t trigId)    = 0;
  virtual TH1*       GetMatchingHitsHisto(Int_t index, Int_t trigId)       = 0;
  virtual TH1*       GetMatchingRecPointsHisto(Int_t index, Int_t trigId)  = 0;
  virtual TH1*       GetMatchingESDsHisto(Int_t index, Int_t trigId)       = 0;
  //
  virtual TObjArray* GetMatchingRawsHistosSet(const Int_t* indexList, Int_t nHist,Int_t trigId)       = 0;
  virtual TObjArray* GetMatchingDigitsHistosSet(const Int_t* indexList, Int_t nHist,Int_t trigId)     = 0;
  virtual TObjArray* GetMatchingSDigitsHistosSet(const Int_t* indexList, Int_t nHist,Int_t trigId)    = 0;
  virtual TObjArray* GetMatchingHitsHistosSet(const Int_t* indexList, Int_t nHist,Int_t trigId)       = 0;
  virtual TObjArray* GetMatchingRecPointsHistosSet(const Int_t* indexList, Int_t nHist,Int_t trigId)  = 0;
  virtual TObjArray* GetMatchingESDsHistosSet(const Int_t* indexList, Int_t nHist,Int_t trigId)       = 0;
  //
  virtual Int_t  FillHitsData(Int_t index, double x)                   = 0;
  virtual Int_t  FillDigitsData(Int_t index, double x)                 = 0;
  virtual Int_t  FillSDigitsData(Int_t index, double x)                = 0;
  virtual Int_t  FillRawsData(Int_t index, double x)                   = 0;
  virtual Int_t  FillRecPointsData(Int_t index, double x)              = 0;
  virtual Int_t  FillESDsData(Int_t index, double x)                   = 0;
  //
  virtual Int_t  FillHitsData(Int_t index, double x, double y)         = 0;
  virtual Int_t  FillDigitsData(Int_t index, double x, double y)       = 0;
  virtual Int_t  FillSDigitsData(Int_t index, double x, double y)      = 0;
  virtual Int_t  FillRawsData(Int_t index, double x, double y)         = 0;
  virtual Int_t  FillRecPointsData(Int_t index, double x, double y)    = 0;
  virtual Int_t  FillESDsData(Int_t index, double x, double y)         = 0;
  //
  virtual Int_t  FillHitsData(Int_t index, double x, double y, double z)         = 0;
  virtual Int_t  FillDigitsData(Int_t index, double x, double y, double z)       = 0;
  virtual Int_t  FillSDigitsData(Int_t index, double x, double y, double z)      = 0;
  virtual Int_t  FillRawsData(Int_t index, double x, double y, double z)         = 0;
  virtual Int_t  FillRecPointsData(Int_t index, double x, double y, double z)    = 0;
  virtual Int_t  FillESDsData(Int_t index, double x, double y, double z)         = 0;
  //
  virtual Int_t  SetHitsDataBinContent(Int_t index, int bin, double w)        = 0;
  virtual Int_t  SetDigitsDataBinContent(Int_t index, int bin, double w)      = 0;
  virtual Int_t  SetSDigitsDataBinContent(Int_t index, int bin, double w)     = 0;
  virtual Int_t  SetRawsDataBinContent(Int_t index, int bin, double w)        = 0;
  virtual Int_t  SetRecPointsDataBinContent(Int_t index, int bin, double w)   = 0;
  virtual Int_t  SetESDsDataBinContent(Int_t index, int bin, double w)        = 0;
  //
  virtual Int_t  SetHitsDataBinContent(Int_t index, int binX, int binY, double w)        = 0;
  virtual Int_t  SetDigitsDataBinContent(Int_t index, int binX, int binY, double w)      = 0;
  virtual Int_t  SetSDigitsDataBinContent(Int_t index, int binX, int binY, double w)     = 0;
  virtual Int_t  SetRawsDataBinContent(Int_t index, int binX, int binY, double w)        = 0;
  virtual Int_t  SetRecPointsDataBinContent(Int_t index, int binX, int binY, double w)   = 0;
  virtual Int_t  SetESDsDataBinContent(Int_t index, int binX, int binY, double w)        = 0;
  //
  virtual Int_t  SetHitsDataBinError(Int_t index, int bin, double err)        = 0;
  virtual Int_t  SetDigitsDataBinError(Int_t index, int bin, double err)      = 0;
  virtual Int_t  SetSDigitsDataBinError(Int_t index, int bin, double err)     = 0;
  virtual Int_t  SetRawsDataBinError(Int_t index, int bin, double err)        = 0;
  virtual Int_t  SetRecPointsDataBinError(Int_t index, int bin, double err)   = 0;
  virtual Int_t  SetESDsDataBinError(Int_t index, int bin, double err)        = 0;
  //
  virtual Int_t  SetHitsDataBinError(Int_t index, int binX, int binY, double err)        = 0;
  virtual Int_t  SetDigitsDataBinError(Int_t index, int binX, int binY, double err)      = 0;
  virtual Int_t  SetSDigitsDataBinError(Int_t index, int binX, int binY, double err)     = 0;
  virtual Int_t  SetRawsDataBinError(Int_t index, int binX, int binY, double err)        = 0;
  virtual Int_t  SetRecPointsDataBinError(Int_t index, int binX, int binY, double err)   = 0;
  virtual Int_t  SetESDsDataBinError(Int_t index, int binX, int binY, double err)        = 0;
  //
  virtual Int_t  ResetHitsData(Int_t index, Option_t *opt="")                 = 0;
  virtual Int_t  ResetDigitsData(Int_t index, Option_t *opt="")               = 0;
  virtual Int_t  ResetSDigitsData(Int_t index, Option_t *opt="")              = 0;
  virtual Int_t  ResetRawsData(Int_t index, Option_t *opt="")                 = 0;
  virtual Int_t  ResetRecPointsData(Int_t index, Option_t *opt="")            = 0;
  virtual Int_t  ResetESDsData(Int_t index, Option_t *opt="")                 = 0;
  //
  virtual Int_t  ResetStatsHitsData(Int_t index)                              = 0;
  virtual Int_t  ResetStatsDigitsData(Int_t index)                            = 0;
  virtual Int_t  ResetStatsSDigitsData(Int_t index)                           = 0;
  virtual Int_t  ResetStatsRawsData(Int_t index)                              = 0;
  virtual Int_t  ResetStatsRecPointsData(Int_t index)                         = 0;
  virtual Int_t  ResetStatsESDsData(Int_t index)                              = 0;
  //
  void           ResetEvCountCycle(IlcRecoParam::EventSpecie_t isp, IlcQAv1::TASKINDEX_t task);
  void           ResetEvCountTotal(IlcRecoParam::EventSpecie_t isp, IlcQAv1::TASKINDEX_t task);
  void           ResetEvCountCycle(IlcRecoParam::EventSpecie_t isp)     {fEvCountCycle[IlcRecoParam::AConvert(isp)].Reset();}
  void           ResetEvCountTotal(IlcRecoParam::EventSpecie_t isp)     {fEvCountTotal[IlcRecoParam::AConvert(isp)].Reset();}
  void           ResetEvCountCycle()                                    {for (int isp=IlcRecoParam::kNSpecies;isp--;) ResetEvCountCycle(IlcRecoParam::ConvertIndex(isp));}
  void           ResetEvCountTotal()                                    {for (int isp=IlcRecoParam::kNSpecies;isp--;) ResetEvCountTotal(IlcRecoParam::ConvertIndex(isp));}
  //
  Int_t          GetEvCountCycle(IlcRecoParam::EventSpecie_t sp, IlcQAv1::TASKINDEX_t task, Int_t trCl=-1) const;
  Int_t          GetEvCountTotal(IlcRecoParam::EventSpecie_t sp, IlcQAv1::TASKINDEX_t task, Int_t trCl=-1) const;
  Int_t          GetEvCountCycle(IlcQAv1::TASKINDEX_t task,Int_t trCl=-1)          const {return GetEvCountCycle(fEventSpecie,task,trCl);}
  Int_t          GetEvCountTotal(IlcQAv1::TASKINDEX_t task,Int_t trCl=-1)          const {return GetEvCountCycle(fEventSpecie,task,trCl);}
  //
  Int_t          GetEvCountCycleRaws(Int_t trCl=-1)                       const {return GetEvCountCycle(IlcQAv1::kRAWS,trCl);}
  Int_t          GetEvCountCycleHits(Int_t trCl=-1)                       const {return GetEvCountCycle(IlcQAv1::kHITS,trCl);}
  Int_t          GetEvCountCycleSDigits(Int_t trCl=-1)                    const {return GetEvCountCycle(IlcQAv1::kSDIGITS,trCl);}
  Int_t          GetEvCountCycleDigits(Int_t trCl=-1)                     const {return GetEvCountCycle(IlcQAv1::kDIGITS,trCl);}
  Int_t          GetEvCountCycleRecPoints(Int_t trCl=-1)                  const {return GetEvCountCycle(IlcQAv1::kRECPOINTS,trCl);}
  Int_t          GetEvCountCycleESDS(Int_t trCl=-1)                       const {return GetEvCountCycle(IlcQAv1::kESDS,trCl);}
  //
  Int_t          GetEvCountTotalRaws(Int_t trCl=-1)                       const {return GetEvCountTotal(IlcQAv1::kRAWS,trCl);}
  Int_t          GetEvCountTotalHits(Int_t trCl=-1)                       const {return GetEvCountTotal(IlcQAv1::kHITS,trCl);}
  Int_t          GetEvCountTotalSDigits(Int_t trCl=-1)                    const {return GetEvCountTotal(IlcQAv1::kSDIGITS,trCl);}
  Int_t          GetEvCountTotalDigits(Int_t trCl=-1)                     const {return GetEvCountTotal(IlcQAv1::kDIGITS,trCl);}
  Int_t          GetEvCountTotalRecPoints(Int_t trCl=-1)                  const {return GetEvCountTotal(IlcQAv1::kRECPOINTS,trCl);}
  Int_t          GetEvCountTotalESDS(Int_t trCl=-1)                       const {return GetEvCountTotal(IlcQAv1::kESDS,trCl);}
  //
  void           IncEvCountCycle(IlcQAv1::TASKINDEX_t task, Int_t diff=1);
  void           IncEvCountTotal(IlcQAv1::TASKINDEX_t task, Int_t diff=1);
  //
  void           IncEvCountCycleRaws(Int_t diff=1)                              {IncEvCountCycle(IlcQAv1::kRAWS, diff);}
  void           IncEvCountCycleHits(Int_t diff=1)                              {IncEvCountCycle(IlcQAv1::kHITS, diff);}
  void           IncEvCountCycleSDigits(Int_t diff=1)                           {IncEvCountCycle(IlcQAv1::kSDIGITS, diff);}
  void           IncEvCountCycleDigits(Int_t diff=1)                            {IncEvCountCycle(IlcQAv1::kDIGITS, diff);}
  void           IncEvCountCycleRecPoints(Int_t diff=1)                         {IncEvCountCycle(IlcQAv1::kRECPOINTS, diff);}
  void           IncEvCountCycleTrackSegments(Int_t diff=1)                     {IncEvCountCycle(IlcQAv1::kTRACKSEGMENTS, diff);}
  void           IncEvCountCycleRecParticles(Int_t diff=1)                      {IncEvCountCycle(IlcQAv1::kRECPARTICLES, diff);}
  void           IncEvCountCycleESDs(Int_t diff=1)                              {IncEvCountCycle(IlcQAv1::kESDS, diff);}
  //
  void           IncEvCountTotalRaws(Int_t diff=1)                              {IncEvCountTotal(IlcQAv1::kRAWS, diff);}
  void           IncEvCountTotalHits(Int_t diff=1)                              {IncEvCountTotal(IlcQAv1::kHITS, diff);}
  void           IncEvCountTotalSDigits(Int_t diff=1)                           {IncEvCountTotal(IlcQAv1::kSDIGITS, diff);}
  void           IncEvCountTotalDigits(Int_t diff=1)                            {IncEvCountTotal(IlcQAv1::kDIGITS, diff);}
  void           IncEvCountTotalRecPoints(Int_t diff=1)                         {IncEvCountTotal(IlcQAv1::kRECPOINTS, diff);}
  void           IncEvCountTotalTrackSegments(Int_t diff=1)                     {IncEvCountTotal(IlcQAv1::kTRACKSEGMENTS, diff);}
  void           IncEvCountTotalRecParticles(Int_t diff=1)                      {IncEvCountTotal(IlcQAv1::kRECPARTICLES, diff);}
  void           IncEvCountTotalESDs(Int_t diff=1)                              {IncEvCountTotal(IlcQAv1::kESDS, diff);}
  //
  virtual void   ClonePerTrigClass(IlcQAv1::TASKINDEX_t task) = 0;
  //
  static Int_t   SetEventTrigClasses(const char* triggers);
  static const   TObjArray& GetEventTrigClasses()                           {return fgEventTrigClasses;}
  static TNamed* GetTrigClass(Int_t i)                                      {return fgTrigClasses ? (TNamed*)fgTrigClasses->At(i) : 0;}
  static const char* GetTrigClassName(Int_t i)                              {return fgTrigClasses ? (i<0||i>=GetNTrigClasses() ? "TriggerBlind":GetTrigClass(i)->GetName()) : 0;}
  static TNamed* GetEventTrigClass(Int_t i)                                 {return (TNamed*)fgEventTrigClasses.At(i);}
  static Int_t   GetNEventTrigClasses()                                     {return fgEventTrigClasses.GetEntriesFast();}
  static void    ResetEventTrigClasses()                                    {fgEventTrigClasses.Clear(); fgEventTrigClasses.SetUniqueID(0);}
  static Int_t   GetNTrigClasses()                                          {return fgTrigClasses ? fgTrigClasses->GetEntriesFast() : 0;}
  static Bool_t  IsEventTriggerSet()                                        {return fgEventTrigClasses.GetUniqueID()>0;}
  static void    SetCloningRequest(TObjArray* aliases, TObjArray* histos);
  //
  static TObjArray*  GetDataOfTrigClass(TObjArray *  arr,  Int_t cloneID, TObjArray *dest=0);
  static TObjArray*  GetDataOfTrigClass(TObjArray ** list, Int_t specieIndex, int cloneID, TObjArray *dest);
  //
  static const char* GetTriggerPrefix() {return fgkTriggerPrefix;}
  //
 protected: 
  virtual void   ClonePerTrigClassL(TObjArray ** list, IlcQAv1::TASKINDEX_t task);
  virtual Int_t  ClonePerTrigClassA(TObjArray * list, const char* hnm, const char* triggers, Bool_t keepOriginal=kFALSE);
  virtual Int_t  ClonePerTrigClassH(Int_t index, TObjArray* list, TObjArray* trigArr, Bool_t keepOriginal=kFALSE);
  //
  Int_t          Add2List(TH1 * hist, const Int_t index, TObjArray ** list, const Bool_t expert = kFALSE, const Bool_t image = kFALSE, const Bool_t saveForCorr = kFALSE) ;
  TH1 *          CloneMe(TH1 * hist, Int_t specie) const ; 
  virtual void   DefaultEndOfDetectorCycle(IlcQAv1::TASKINDEX_t task ) ; 
  TObject *      GetData(TObjArray ** list, const Int_t index);
  TH1*           GetData(TObjArray ** list, const Int_t index, Int_t cloneID);
  TObjArray*     GetDataOfTrigClass(TObjArray ** list, Int_t cloneID, TObjArray *dest=0);
  virtual void   InitHits()          = 0 ; 
  //virtual void   InitRecParticles()  = 0 ; 
  virtual void   InitSDigits()       = 0 ; 
  //virtual void   InitTrackSegments()  = 0 ; 
  virtual void   MakeESDs(IlcESDEvent * )          = 0 ; 
  virtual void   MakeHits()         = 0 ; 
  virtual void   MakeHits(TTree * )                = 0 ;  
  virtual void   MakeDigits()       = 0 ;  
  virtual void   MakeDigits(TTree * )              = 0 ; 
  //virtual void   MakeRecParticles( ) = 0 ; 
  virtual void   MakeRaws(IlcRawReader *)          = 0 ; 
  virtual void   MakeRecPoints(TTree * )           = 0 ; 
  virtual void   MakeSDigits()      = 0 ;  
  virtual void   MakeSDigits(TTree * )             = 0 ;  
  //virtual void   MakeTrackSegments(TTree * )		 = 0 ;  
  void           ResetCycle() { fCurrentCycle++ ; fCycleCounter = 0 ; } 
  virtual void   StartOfDetectorCycle()            = 0 ;
  //
  TH1*           GetMatchingHisto(TObjArray ** list, Int_t index, Int_t trigId);
  TObjArray*     GetMatchingHistosSet(TObjArray ** list, const Int_t* indexList, Int_t nHist, Int_t trigId);
  TObjArray*     GetMatchingHistos(TObjArray ** list, Int_t index, TObjArray* optDest=0);
  Bool_t         CheckCloningConsistency(TObjArray ** list, const Int_t* indexList, Int_t nHist, Bool_t checkTriggers=kFALSE);
  Int_t          FillData(TObjArray ** list, Int_t index, double x);
  Int_t          FillData(TObjArray ** list, Int_t index, double x, double y);
  Int_t          FillData(TObjArray ** list, Int_t index, double x, double y, double z);
  Int_t          SetDataBinContent(TObjArray ** list, Int_t index, int bin, double w);
  Int_t          SetDataBinContent(TObjArray ** list, Int_t index, int binX, int binY, double w);
  Int_t          SetDataBinError(TObjArray ** list, Int_t index, int bin, double err);
  Int_t          SetDataBinError(TObjArray ** list, Int_t index, int binX, int binY, double err);
  Int_t          ResetData(TObjArray ** list, Int_t index, Option_t* option = "");
  Int_t          ResetStatsData(TObjArray ** list, Int_t index);
  //
  TFile *        fOutput ;          //! output root file
  TDirectory *   fDetectorDir ;     //! directory for the given detector in the file
  TString        fDetectorDirName ; //! detector directory name in the quality assurance data file
  Int_t          fCurrentCycle ;    //! current cycle number
  Int_t          fCycle ;           //! length (# events) of the QA data acquisition cycle  
  Int_t          fCycleCounter ;    //! cycle counter
  Bool_t         fWriteExpert ;     //! flag to write or not the expert QA data
  TList **       fParameterList ;   //! list of QA data parameters
  Int_t          fRun ;             //! run number
  IlcRecoParam::EventSpecie_t fEventSpecie ; //! event specie, see IlcRecoParam
  TClonesArray * fDigitsArray ;    //! array to hold the sdigits
  TArrayI        fEvCountCycle[IlcRecoParam::kNSpecies];  // cycle event counters per specie and per task and eventually per trigger class
  TArrayI        fEvCountTotal[IlcRecoParam::kNSpecies];  // total event counters per specie and per task and eventually per trigger class
  //
  static TObjArray     fgEventTrigClasses;   // array of indices of fired trigger classes for current event (indices of fgTriggeClasses)
  static TObjArray*    fgCloningRequest;     // set of histogram names and trigger aliases
  static TObjArray*    fgTrigClasses;        // set of trigger_aliases and corresponding trigger names 
  static const Char_t* fgkTriggerPrefix;     // special prefix to separate the trigger name from histo name  
 private:
  IlcQADataMaker& operator = (const IlcQADataMaker& /*qadm*/); // Not implemented

  ClassDef(IlcQADataMaker,5)  // description 
    
};

//____________________________________________________________________________
inline Bool_t IlcQADataMaker::IsOrigHistoKept(Int_t index, TObjArray ** list) {
  // check if the histogram was cloned and the original is kept
  TObject* h = GetData(list,index); if (!h) return kFALSE; 
  return (!h->TestBit(IlcQAv1::GetClonedBit())) || h->TestBit(IlcQAv1::GetOrigHistoKeptBit());
}   

//____________________________________________________________________________
inline Int_t IlcQADataMaker::IsClonedPerTrigClass(Int_t index, TObjArray ** list)
{
  // if the historgam was cloned per trigger, return number of clones, otherwise, return 0
  TObject* obj = GetData(list, index);
  if (!obj || !obj->TestBit(IlcQAv1::GetClonedBit())) return 0;
  return ((TObjArray*)obj)->GetEntriesFast();
}

//____________________________________________________________________________
inline Bool_t IlcQADataMaker::MatchesToTriggers(UInt_t hpos) const
{
  // check if the position hpos of clones TObjArray matches to one of event triggers classes
  // if the event trigger was not set, every position is matching
  if (!IsEventTriggerSet()) return kTRUE; // if triggers are not set, then this is a call from EndOfCycle
  for (int itc=GetNEventTrigClasses();itc--;) if (fgEventTrigClasses.UncheckedAt(itc)->GetUniqueID()==hpos) return kTRUE;
  return kFALSE;
}

#endif // IlcQADataMaker_H
