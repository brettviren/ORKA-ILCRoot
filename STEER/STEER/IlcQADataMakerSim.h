#ifndef IlcQADataMakerSim_H
#define IlcQADataMakerSim_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcQADataMakerSim.h 50615 2011-07-16 23:19:19Z hristov $ */

//  Base Class:
//  Produces the data needed to calculate the quality assurance. 
//  All data must be mergeable objects.
//  Y. Schutz CERN July 2007


// --- ROOT system ---

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcQADataMaker.h"

class IlcQADataMakerSim: public IlcQADataMaker {
  
public:
	
	IlcQADataMakerSim(const char * name="", const char * title="") ;          // ctor
	IlcQADataMakerSim(const IlcQADataMakerSim& qadm) ;   
	IlcQADataMakerSim& operator = (const IlcQADataMakerSim& qadm) ;
	virtual ~IlcQADataMakerSim() ; // dtor
  
	virtual Int_t Add2DigitsList(TH1 * hist, const Int_t index, const Bool_t expert = kFALSE, const Bool_t image = kFALSE)                    
    { return Add2List(hist, index, fDigitsQAList, expert, image) ; }
	virtual Int_t Add2ESDsList(TH1 * /*hist*/, const Int_t /*index*/, const Bool_t /*expert = kFALSE*/, const Bool_t /*image = kFALSE*/)    
  { return -1 ; } 
	virtual Int_t Add2HitsList(TH1 * hist, const Int_t index, const Bool_t expert = kFALSE, const Bool_t image = kFALSE)                      
  { return Add2List(hist, index, fHitsQAList, expert, image) ; }
	virtual Int_t Add2RecPointsList(TH1 * /*hist*/, const Int_t /*index*/, const Bool_t /*expert = kFALSE*/, const Bool_t /*image = kFALSE*/) 
    { return -1 ; } 
  virtual Int_t Add2RawsList(TH1 * /*hist*/, const Int_t /*index*/, const Bool_t /*expert = kFALSE*/, const Bool_t /*saveForCorr = kFALSE*/, const Bool_t /*image = kFALSE*/)      
    { return -1 ; }  
 virtual Int_t Add2SDigitsList(TH1 * hist, const Int_t index, const Bool_t expert = kFALSE, const Bool_t image = kFALSE)   
    { return Add2List(hist, index, fSDigitsQAList, expert, image) ; }

  virtual void        Exec(IlcQAv1::TASKINDEX_t task, TObject * data) ;
  virtual void        EndOfCycle() ;
  virtual void        EndOfCycle(IlcQAv1::TASKINDEX_t task) ;
  virtual void        EndOfDetectorCycle(IlcQAv1::TASKINDEX_t, TObjArray ** ) {IlcInfo("To be implemented by detectors");} 
  //
  virtual TObject*    GetDigitsData(const Int_t index)        { return GetData(fDigitsQAList, index); }
  virtual TObject*    GetESDsData(const Int_t /*index*/)      { return NULL; }
  virtual TObject*    GetHitsData(const Int_t index)          { return GetData(fHitsQAList, index); }
  virtual TObject*    GetRecPointsData(const Int_t /*index*/) { return NULL; }
  virtual TObject*    GetRawsData(const Int_t /*index*/)      { return NULL; } 
  virtual TObject*    GetSDigitsData(const Int_t index)       { return GetData(fSDigitsQAList, index); }
  //
  virtual TH1*        GetDigitsData(const Int_t index, int cloneID)  { return GetData(fDigitsQAList, index, cloneID); }
  virtual TH1*        GetESDsData(const Int_t /*index*/, int)        { return NULL; }
  virtual TH1*        GetHitsData(const Int_t index, int cloneID)    { return GetData(fHitsQAList, index, cloneID); }
  virtual TH1*        GetRecPointsData(const Int_t /*index*/, int)   { return NULL; }
  virtual TH1*        GetRawsData(const Int_t /*index*/, int)        { return NULL; } 
  virtual TH1*        GetSDigitsData(const Int_t index, int cloneID) { return GetData(fSDigitsQAList, index, cloneID); }
  //
  virtual TObjArray*  GetDigitsDataOfTrigClass(int cloneID, TObjArray *dest=0)              {return GetDataOfTrigClass(fDigitsQAList,cloneID,dest);}
  virtual TObjArray*  GetSDigitsDataOfTrigClass(int cloneID, TObjArray *dest=0)             {return GetDataOfTrigClass(fSDigitsQAList,cloneID,dest);}
  virtual TObjArray*  GetESDsDataOfTrigClass(int /*cloneID*/, TObjArray */*dest*/=0)        {return NULL;}
  virtual TObjArray*  GetHitsDataOfTrigClass(int cloneID, TObjArray *dest=0)                {return GetDataOfTrigClass(fHitsQAList,cloneID,dest);}
  virtual TObjArray*  GetRecPointsDataOfTrigClass(int /*cloneID*/, TObjArray */*dest*/=0)   {return NULL;}
  virtual TObjArray*  GetRawsDataOfTrigClass(int /*cloneID*/, TObjArray */*dest*/=0)        {return NULL;}
  //
  virtual TObjArray** Init(IlcQAv1::TASKINDEX_t task, Int_t cycles = -1) ;
  virtual void        Init(IlcQAv1::TASKINDEX_t task, TObjArray ** list, Int_t run, Int_t cycles = -1) ;
  virtual void        InitRaws() {IlcWarning("Call not valid") ; }
  virtual void        InitRecPoints()                  {IlcWarning("Call not valid") ; } 
  virtual void        ResetDetector(IlcQAv1::TASKINDEX_t task) ;
  virtual void        StartOfCycle(Int_t run = -1) ;
  virtual void        StartOfCycle(IlcQAv1::TASKINDEX_t task, Int_t run, const Bool_t sameCycle = kFALSE) ;
  //
  virtual TH1*       GetMatchingHitsHisto(Int_t index, Int_t trigId)       {return GetMatchingHisto(fHitsQAList,index,trigId);}
  virtual TH1*       GetMatchingSDigitsHisto(Int_t index, Int_t trigId)    {return GetMatchingHisto(fSDigitsQAList,index,trigId);}
  virtual TH1*       GetMatchingDigitsHisto(Int_t index, Int_t trigId)     {return GetMatchingHisto(fDigitsQAList,index,trigId);}
  virtual TH1*       GetMatchingRawsHisto(Int_t, Int_t)                    {return 0;}
  virtual TH1*       GetMatchingRecPointsHisto(Int_t, Int_t)               {return 0;}
  virtual TH1*       GetMatchingESDsHisto(Int_t, Int_t)                    {return 0;}
  //
  virtual TObjArray* GetMatchingHitsHistosSet(const Int_t* indexList, Int_t nHist,Int_t trigId)    {return GetMatchingHistosSet(fHitsQAList,indexList,nHist,trigId);}
  virtual TObjArray* GetMatchingSDigitsHistosSet(const Int_t* indexList, Int_t nHist,Int_t trigId) {return GetMatchingHistosSet(fSDigitsQAList,indexList,nHist,trigId);}
  virtual TObjArray* GetMatchingDigitsHistosSet(const Int_t* indexList, Int_t nHist,Int_t trigId)  {return GetMatchingHistosSet(fDigitsQAList,indexList,nHist,trigId);}
  virtual TObjArray* GetMatchingRawsHistosSet(const Int_t*, Int_t,Int_t)                           {return 0;}
  virtual TObjArray* GetMatchingRecPointsHistosSet(const Int_t*,Int_t,Int_t)                       {return 0;}
  virtual TObjArray* GetMatchingESDsHistosSet(const Int_t*, Int_t,Int_t)                           {return 0;}
  //
  virtual TObjArray* GetMatchingHitsData(const Int_t index, TObjArray* optDest=0)    {return GetMatchingHistos(fHitsQAList,index,optDest);}
  virtual TObjArray* GetMatchingSDigitsData(const Int_t index, TObjArray* optDest=0) {return GetMatchingHistos(fSDigitsQAList,index,optDest);}
  virtual TObjArray* GetMatchingDigitsData(const Int_t index, TObjArray* optDest=0)  {return GetMatchingHistos(fDigitsQAList,index,optDest);}
  virtual TObjArray* GetMatchingRawsData(const Int_t, TObjArray*)                    {return 0;}
  virtual TObjArray* GetMatchingRecPointsData(const Int_t, TObjArray*)               {return 0;}
  virtual TObjArray* GetMatchingESDsData(const Int_t, TObjArray*)                    {return 0;}
  //
  virtual Int_t  FillHitsData(Int_t index, double x)                   {return FillData(fHitsQAList, index, x);}
  virtual Int_t  FillSDigitsData(Int_t index, double x)                {return FillData(fSDigitsQAList, index, x);}
  virtual Int_t  FillDigitsData(Int_t index, double x)                 {return FillData(fDigitsQAList, index, x);}
  virtual Int_t  FillRawsData(Int_t, double)                           {return -1;}
  virtual Int_t  FillRecPointsData(Int_t, double)                      {return -1;}
  virtual Int_t  FillESDsData(Int_t, double)                           {return -1;}
  //
  virtual Int_t  FillHitsData(Int_t index, double x, double y)         {return FillData(fHitsQAList, index, x, y);}
  virtual Int_t  FillSDigitsData(Int_t index, double x, double y)      {return FillData(fSDigitsQAList, index, x, y);}
  virtual Int_t  FillDigitsData(Int_t index, double x, double y)       {return FillData(fDigitsQAList, index, x, y);}
  virtual Int_t  FillRawsData(Int_t, double, double)                   {return -1;}
  virtual Int_t  FillRecPointsData(Int_t, double, double)              {return -1;}
  virtual Int_t  FillESDsData(Int_t, double, double)                   {return -1;}
  //
  virtual Int_t  FillHitsData(Int_t index, double x, double y, double z)         {return FillData(fHitsQAList, index, x,y,z);}
  virtual Int_t  FillSDigitsData(Int_t index, double x, double y, double z)      {return FillData(fSDigitsQAList, index, x,y,z);}
  virtual Int_t  FillDigitsData(Int_t index, double x, double y, double z)       {return FillData(fDigitsQAList, index, x,y,z);}
  virtual Int_t  FillRawsData(Int_t, double, double, double)                     {return -1;}
  virtual Int_t  FillRecPointsData(Int_t, double, double, double)                {return -1;}
  virtual Int_t  FillESDsData(Int_t, double, double, double)                     {return -1;}
  //
  virtual Int_t  SetHitsDataBinContent(Int_t index, int bin, double w)           {return SetDataBinContent(fHitsQAList, index,bin,w);}
  virtual Int_t  SetDigitsDataBinContent(Int_t index, int bin, double w)         {return SetDataBinContent(fDigitsQAList, index,bin,w);}
  virtual Int_t  SetSDigitsDataBinContent(Int_t index, int bin, double w)        {return SetDataBinContent(fSDigitsQAList, index,bin,w);}
  virtual Int_t  SetRawsDataBinContent(Int_t, int, double)                       {return -1;}
  virtual Int_t  SetRecPointsDataBinContent(Int_t, int, double)                  {return -1;}
  virtual Int_t  SetESDsDataBinContent(Int_t, int, double)                       {return -1;}
  //
  virtual Int_t  SetHitsDataBinContent(Int_t index, int binX, int binY, double w)      {return SetDataBinContent(fHitsQAList, index,binX,binY,w);}
  virtual Int_t  SetDigitsDataBinContent(Int_t index, int binX, int binY, double w)    {return SetDataBinContent(fDigitsQAList, index,binX,binY,w);}
  virtual Int_t  SetSDigitsDataBinContent(Int_t index, int binX, int binY, double w)   {return SetDataBinContent(fSDigitsQAList, index,binX,binY,w);}
  virtual Int_t  SetRawsDataBinContent(Int_t, int, int, double)                        {return -1;}
  virtual Int_t  SetRecPointsDataBinContent(Int_t, int, int, double)                   {return -1;}
  virtual Int_t  SetESDsDataBinContent(Int_t, int, int, double)                        {return -1;}
  //
  virtual Int_t  SetHitsDataBinError(Int_t index, int bin, double err)           {return SetDataBinError(fHitsQAList, index,bin,err);}
  virtual Int_t  SetDigitsDataBinError(Int_t index, int bin, double err)         {return SetDataBinError(fDigitsQAList, index,bin,err);}
  virtual Int_t  SetSDigitsDataBinError(Int_t index, int bin, double err)        {return SetDataBinError(fSDigitsQAList, index,bin,err);}
  virtual Int_t  SetRawsDataBinError(Int_t, int, double)                         {return -1;}
  virtual Int_t  SetRecPointsDataBinError(Int_t, int, double)                    {return -1;}
  virtual Int_t  SetESDsDataBinError(Int_t, int, double)                         {return -1;}
  //
  virtual Int_t  SetHitsDataBinError(Int_t index, int binX, int binY, double err)       {return SetDataBinError(fHitsQAList, index,binX,binY,err);}
  virtual Int_t  SetDigitsDataBinError(Int_t index, int binX, int binY, double err)     {return SetDataBinError(fDigitsQAList, index,binX,binY,err);}
  virtual Int_t  SetSDigitsDataBinError(Int_t index, int binX, int binY, double err)    {return SetDataBinError(fSDigitsQAList, index,binX,binY,err);}
  virtual Int_t  SetRawsDataBinError(Int_t, int, int, double)                           {return -1;}
  virtual Int_t  SetRecPointsDataBinError(Int_t, int, int, double)                      {return -1;}
  virtual Int_t  SetESDsDataBinError(Int_t, int, int, double)                           {return -1;}
  //
  virtual Int_t  ResetHitsData(Int_t index, Option_t* opt="")                    {return ResetData(fHitsQAList, index, opt);}
  virtual Int_t  ResetSDigitsData(Int_t index, Option_t* opt="")                 {return ResetData(fSDigitsQAList, index, opt);}
  virtual Int_t  ResetDigitsData(Int_t index, Option_t* opt="")                  {return ResetData(fDigitsQAList, index, opt);}
  virtual Int_t  ResetRawsData(Int_t, Option_t*)                                 {return -1;}
  virtual Int_t  ResetRecPointsData(Int_t, Option_t*)                            {return -1;}
  virtual Int_t  ResetESDsData(Int_t, Option_t*)                                 {return -1;}
  //
  virtual Int_t  ResetStatsHitsData(Int_t index)                      {return ResetStatsData(fHitsQAList, index);}
  virtual Int_t  ResetStatsSDigitsData(Int_t index)                   {return ResetStatsData(fSDigitsQAList, index);}
  virtual Int_t  ResetStatsDigitsData(Int_t index)                    {return ResetStatsData(fDigitsQAList, index);}
  virtual Int_t  ResetStatsRawsData(Int_t)                            {return -1;}
  virtual Int_t  ResetStatsRecPointsData(Int_t)                       {return -1;}
  virtual Int_t  ResetStatsESDsData(Int_t)                            {return -1;}
  //
  virtual void ClonePerTrigClass(IlcQAv1::TASKINDEX_t task);  
  //
 protected: 
  //
  virtual void   InitDigits()                     {IlcInfo("To be implemented by detectors");}
  virtual void   InitESDs()                       {IlcWarning("Call not valid") ; } 
  virtual void   InitHits()                       {IlcInfo("To be implemented by detectors");}
  virtual void   InitSDigits()                    {IlcInfo("To be implemented by detectors");}
  virtual void   MakeESDs(IlcESDEvent * )         {IlcWarning("Call not valid") ; }
  virtual void   MakeHits()                       {IlcInfo("To be implemented by detectors");} 
  virtual void   MakeHits(TTree * )               {IlcInfo("To be implemented by detectors");} 
  virtual void   MakeDigits()                     {IlcInfo("To be implemented by detectors");} 
  virtual void   MakeDigits(TTree * )             {IlcInfo("To be implemented by detectors");} 
  virtual void   MakeRaws(IlcRawReader *)         {IlcWarning("Call not valid") ; } 
  virtual void   MakeRecPoints(TTree * )          {IlcWarning("Call not valid") ; } 
  virtual void   MakeSDigits()                    {IlcInfo("To be implemented by detectors");} 
  virtual void   MakeSDigits(TTree * )            {IlcInfo("To be implemented by detectors");} 
  virtual void   StartOfDetectorCycle()           {IlcInfo("To be implemented by detectors");} 
  
  TObjArray * *    fDigitsQAList ;     //! list of the digits QA data objects
  TObjArray * *    fHitsQAList ;       //! list of the hits QA data objects
  TObjArray * *    fSDigitsQAList ;    //! list of the sdigits QA data objects
  TClonesArray *   fHitsArray ;       //! array to hold the hits
  TClonesArray *   fSDigitsArray ;     //! array to hold the digits

	  
 ClassDef(IlcQADataMakerSim,2)  // description 

};

#endif // IlcQADataMakerSim_H

