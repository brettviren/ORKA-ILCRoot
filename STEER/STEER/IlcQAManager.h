#ifndef ILCQAMANAGER_H
#define ILCQAMANAGER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcQAManager.h 30796 2009-01-28 11:05:10Z schutz $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// class for running the QA makers                                           //
//                                                                           //1
//   IlcQAManager qas;                                                //
//   qas.Run(IlcQAv1::kRAWS, rawROOTFileName);                                 //
//   qas.Run(IlcQAv1::kHITS);                                                  //
//   qas.Run(IlcQAv1::kSDIGITS);                                               //
//   qas.Run(IlcQAv1::kDIGITS);                                                //
//   qas.Run(IlcQAv1::kRECPOINTS);                                             //
//   qas.Run(IlcQAv1::kESDS);                                                  //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <TNamed.h>
#include "IlcQAv1.h"
#include "IlcLoader.h"
#include "IlcCDBManager.h"
#include "IlcRecoParam.h"
#include "IlcEventInfo.h"
 
class TCanvas ; 
class IlcESDEvent ; 
class IlcDetectorRecoParam ; 
class IlcESDEvent ; 
class IlcQADataMaker ;
class IlcRawReader ;  
class IlcRunLoader ; 
class IlcCorrQADataMakerRec ;

class IlcQAManager : public IlcCDBManager {
public:
  static void      Destroy() ;
  void             EndOfCycle(TObjArray * detArray=0x0) ; 
  void             EndOfCycle(TString detectors) ; 
	UInt_t           GetCurrentEvent() const { return fCurrentEvent ; }
	TObjArray *      GetFromOCDB(IlcQAv1::DETECTORINDEX_t det, IlcQAv1::TASKINDEX_t task, const Char_t * year) const ; 
  const IlcEventInfo *   GetEventInfo() const { return fEventInfo ; }
  IlcRecoParam::EventSpecie_t GetEventSpecieFromESD() ;
  TCanvas **       GetImage(Char_t * detName) ;
  const Char_t *   GetMode(){ return fMode.Data() ; }
  IlcQAv1     *    GetQA(UInt_t run, UInt_t evt) ; 
	IlcQADataMaker * GetQADataMaker(const Int_t iDet) ; 
	void             Increment(const IlcQAv1::TASKINDEX_t taskIndex = IlcQAv1::kNULLTASKINDEX) ;
	void             InitQADataMaker(UInt_t run, TObjArray * detArray=0x0) ;
  Bool_t           IsSaveData() { return fSaveData ; } 
	Bool_t           IsSelected(const Char_t * detName)  ;
	Bool_t           Merge(Int_t runNumber = -1, const char *fileName = NULL) const ;  
  void             MergeCustom() const ;
  Bool_t           MergeXML(const Char_t * collection, const Char_t * subFile = 0, const Char_t * outFile = 0) ; 
  static           IlcQAManager * QAManager(IlcQAv1::MODE_t = IlcQAv1::kNULLMODE, TMap *entryCache = NULL, Int_t run = -1) ;
  static           IlcQAManager * QAManager(IlcQAv1::TASKINDEX_t task) ;  
	void             Reset(const Bool_t sameCycle = kFALSE) ;  
  void             ResetDetectors(IlcQAv1::TASKINDEX_t task, IlcQAv1::DETECTORINDEX_t det=IlcQAv1::kNULLDET) ; 
	TString          Run(const Char_t * detectors, const IlcQAv1::TASKINDEX_t taskIndex=IlcQAv1::kNULLTASKINDEX, Bool_t const sameCycle = kFALSE, const Char_t * fileName = NULL) ; 
	TString          Run(const Char_t * detectors, IlcRawReader * rawReader, Bool_t const sameCycle = kFALSE) ; 
	TString          Run(const Char_t * detectors, const Char_t * filename, Bool_t const sameCycle = kFALSE) ;
	void             RunOneEvent(IlcRawReader * rawReader) ; 
	void             RunOneEventInOneDetector(Int_t det, TTree * tree) ; 
	void             RunOneEvent(IlcESDEvent *& esd, IlcESDEvent *& hltesd)  ;
	Bool_t           Save2OCDB(const Int_t runNumber, IlcRecoParam::EventSpecie_t es, const Char_t * year = "08", const Char_t * detectors = "ALL") const ; 
	void             SetActiveDetectors(TString aDet) { fDetectors = aDet ;  }
  void             SetCheckerExternParam(IlcQAv1::DETECTORINDEX_t det, TList * parameterList) ;  
	void             SetCycleLength(const IlcQAv1::DETECTORINDEX_t det, const Int_t cycle) { fQACycles[det] = cycle ; }
	void             SetWriteExpert(const IlcQAv1::DETECTORINDEX_t det) { fQAWriteExpert[det] = kTRUE ; }
  void             SetEventInfo(IlcEventInfo *info) { fEventInfo = info ;} 
	void             SetEventRange(UInt_t first, UInt_t last) { fFirstEvent = first ; fMaxEvents = last - first + 1 ; }    
  void             SetEventSpecie(IlcRecoParam::EventSpecie_t es) ; 
	void             SetFirsEvent(UInt_t first) { fFirstEvent = first ; }      
	void             SetMaxEvents(UInt_t max) { fMaxEvents = max ; }      
	void             SetNewCycle() { fCycleSame = kTRUE ; }
  void             SetPrintImage(Bool_t opt = kTRUE) { fPrintImage = opt ; }
	void             SetRecoParam(const Int_t det, const IlcDetectorRecoParam *par) ;
	void             SetRunLoader(IlcRunLoader * rl) { fRunLoader = rl ; }
  void             SetSaveData(Bool_t opt = kTRUE ) { fSaveData = opt ; }
	void             SetTasks(TString tasks) { fTasks = tasks ; }
  void             SetWriteExpert() ; 
  void             ShowQA() ; 
  
private: 
  IlcQAManager() ; 
	IlcQAManager(IlcQAv1::MODE_t mode, const Char_t * gIlcFilename = "gilc.root") ; 
	IlcQAManager(const IlcQAManager & qas) ; 
	IlcQAManager & operator = (const IlcQAManager & qas) ; 
  ~IlcQAManager() ; 
  
	Bool_t			DoIt(const IlcQAv1::TASKINDEX_t taskIndex) ;
	IlcLoader * GetLoader(Int_t iDet) ; 
	Int_t       GetQACycles(const Int_t iDet) const { return fQACycles[iDet] ; }
	Bool_t 		  InitQA(const IlcQAv1::TASKINDEX_t taskIndex, const  Char_t * fileName = NULL) ;
	Bool_t      InitRunLoader() ; 
	Bool_t      Finish(const IlcQAv1::TASKINDEX_t taskIndex) ;
	Bool_t      MergeData(const Int_t runNumber, const char *fileName = NULL) const ;  
	Bool_t      MergeResults(const Int_t runNumber) const ;  
	Bool_t      SaveIt2OCDB(const Int_t runNumber, TFile * inputFile, const Char_t * year, IlcRecoParam::EventSpecie_t es) const ;  

 	static IlcQAManager*        fgQAInstance;                   // IlcQAManager instance
	UInt_t                      fCurrentEvent ;                 //! event counter
	Bool_t                      fCycleSame ;                    //! true if 2 consecutive data making for a same detector   
	TString                     fDetectors ;                    //! list of active detectors 
	TString                     fDetectorsW ;                   //! list of active detectors with QA implemented 
	IlcESDEvent *               fESD ;                          //! current ESD
	TTree *                     fESDTree ;                      //! current ESD Tree
  IlcEventInfo *              fEventInfo ;                    //! info on the current event  
	TString                     fGIlcFileName ;               //! name of the gilc file
	UInt_t                      fFirstEvent ;                   //! first event to process
	Long64_t                    fMaxEvents ;                    //! number of events to process
	TString                     fMode ;                         //! sim or rec
	Long64_t                    fNumberOfEvents ;               //! number of events in the run 
  IlcRecoParam                fRecoParam;                     //! container for the reco-param objects for detectors
	UInt_t                      fRunNumber ;                    //! current run number
	IlcRawReader *              fRawReader ;                    //! current raw reader object 
	Bool_t                      fRawReaderDelete ;              //! tells if the rawReader has been created by this
	IlcRunLoader *              fRunLoader ;                    //! current run loader object
	TString                     fTasks ;                        //! list of QA tasks to be performed
	static const UInt_t         fgkNDetectors = IlcQAv1::kNDET ;  //! number of detectors    
	IlcLoader      *            fLoader[fgkNDetectors];         //! array of detectors loader
	IlcQADataMaker *            fQADataMaker[fgkNDetectors];    //! array of QA data maker objects
	Int_t                       fQACycles[fgkNDetectors];       //! array of QA cycle length
	Bool_t                      fQAWriteExpert[fgkNDetectors];  //! array of QA cycle length
  IlcRecoParam::EventSpecie_t fEventSpecie ;                  //! type of event 
  Bool_t                      fPrintImage ;                   //! flag to print the images or not
  Bool_t                      fSaveData ;                     //! flag to sve the QA data or not   
    
  ClassDef(IlcQAManager, 2)      // class for running the QA makers
};

#endif
