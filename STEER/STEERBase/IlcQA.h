#ifndef ILCQA_H
#define ILCQA_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
* See cxx source for full Copyright notice                               */

/* $Id: IlcQA.h 52430 2011-10-31 12:24:02Z schutz $ */

//
// Quality Assurance Object
//

#include <TNamed.h> 
class TFile ; 

#include "IlcLog.h"

class IlcQA : public TNamed {
public:
  
	enum DETECTORINDEX_t {
    kNULLDET=-1, kITS, kTPC, kTRD, kTOF, kPHOS, kHMPID, kEMCAL, kMUON, kFMD,
    kZDC, kPMD, kT0, kVZERO, kACORDE, kHLT, kGLOBAL, kCORR, kNDET
	#ifdef MFT_UPGRADE
	, kMFT
	#endif 
	};
	enum ILCTASK_t {
    kNULLTASK=-1, kRAW, kSIM, kREC, kESD, kANA, kNTASK };
	enum QABIT_t {
    kNULLBit=-1, kINFO, kWARNING, kERROR, kFATAL, kNBIT };
  enum RUNTYPE_t {
    kNULLTYPE=-1, kUNKOWN, kAUTOTEST, kCALIBRATION, kCALIBRATIONPULSER, kCHANNELDELAYTUNING, kCOSMIC, kCOSMICS, kDAQFOUNIFSCAN, 
    kDAQGENDACSCAN, kDAQMEANTHSCAN, kDAQMINTHSCAN, kDAQNOISYPIXSCAN, kDAQPIXDELAYSCAN, kDAQUNIFORMITYSCAN, 
    kDCSFOUNIFSCAN, kDCSMEANTHSCAN, kDCSMINTHSCAN, kDCSPIXDELAYSCAN, kDCSUNIFORMITYSCAN, kDDLTEST, kGAIN, 
    kPEDESTAL, kINJECTOR,  kLASER, kMONTECARLO, kNOISE, kNOISYPIXSCAN,  kPHYSICS, kPULSER, kSTANDALONE, kSTANDALONEBC, 
    kSTANDALONECENTRAL, kSTANDALONECOSMIC, kSTANDALONEEMD, kSTANDALONELASER, kSTANDALONEMB, kSTANDALONEPEDESTAL, 
    kSTANDALONESEMICENTRAL, kSTANDALONEPULSER, kNTYPE};
	
	enum TASKINDEX_t {
    kNULLTASKINDEX=-1, kRAWS, kHITS, kSDIGITS, kDIGITS, kRECPOINTS, kTRACKSEGMENTS, kRECPARTICLES, kESDS, kNTASKINDEX };
  
	// Creators - destructors
	IlcQA(); // beware singleton, not to be used
	IlcQA(const ILCTASK_t tsk) ;
	IlcQA(const DETECTORINDEX_t det) ;
	IlcQA(const IlcQA& qa) ;   
	IlcQA& operator = (const IlcQA& qa) ;
	virtual ~IlcQA();
  
	static  IlcQA *        Instance() ;
	static  IlcQA *        Instance(const DETECTORINDEX_t det) ;
	static  IlcQA *        Instance(const ILCTASK_t tsk) ;
	static  IlcQA *        Instance(const TASKINDEX_t tsk) ;
  Bool_t                 CheckFatal() const ;
	static void            Close() ; 
	static  char *         GetIlcTaskName(ILCTASK_t tsk) ;
  static const TString   GetExpert() { return fgkExpert ; }
  static  UInt_t         GetExpertBit() { return fgkExpertBit ; }
	static const TString   GetLabLocalFile() { return fgkLabLocalFile ; } 
	static const TString   GetLabLocalOCDB() { return fgkLabLocalOCDB ; } 
	static const TString   GetLabAliEnOCDB() { return fgkLabAliEnOCDB ; } 
	static  DETECTORINDEX_t GetDetIndex(const char * name) ; 
	static const TString   GetDetName(DETECTORINDEX_t det) { return fgDetNames[det] ; }
	static const char *    GetDetName(Int_t det) ;
	static const TString   GetGRPPath() { return fgGRPPath ; }  
  static  UInt_t         GetQABit() { return fgkQABit ; }
	static TFile *         GetQADataFile(const char * name, const Int_t run) ; 
	static TFile *	       GetQADataFile(const char * fileName) ;
	static const char *    GetQADataFileName(const char * name, const Int_t run) 
    {return Form("%s.%s.%d.root", name, fgQADataFileName.Data(), run)  ; }
	static const char *    GetQADataFileName() { return fgQADataFileName.Data() ; }
	static const char *    GetQAName() { return fgkQAName ; } 
  static const char *    GetQACorrName() { return fgkQACorrNtName ; }
	static TFile *         GetQAResultFile() ; 
	static const char  *   GetQAResultFileName() { return (fgQAResultDirName + fgQAResultFileName).Data() ; }
	static const char  *   GetQARefDefaultStorage() { return fgkQARefOCDBDefault.Data() ; }
	static const char  *   GetQARefFileName() { return fgQARefFileName ; }
	static const char  *   GetQARefStorage() { return fgQARefDirName.Data() ; }
	static const char  *   GetRefOCDBDirName() { return fgkRefOCDBDirName.Data() ; }
	static const char  *   GetRefDataDirName() { return fgkRefDataDirName.Data() ; }
	static const TString   GetRunTypeName(RUNTYPE_t rt = kNULLTYPE) ;
	static  TASKINDEX_t    GetTaskIndex(const char * name) ; 
	static const TString   GetTaskName(UInt_t tsk) { return fgTaskNames[tsk] ; }
  Bool_t                 IsSet(DETECTORINDEX_t det, ILCTASK_t tsk, QABIT_t bit) const ;
  Bool_t                 IsSetAny(DETECTORINDEX_t det, ILCTASK_t tsk) const ;
  Bool_t                 IsSetAny(DETECTORINDEX_t det) const ;
	Long64_t               Merge(const TCollection * list) ; 
	void                   Set(QABIT_t bit) ;
	static void			       SetQAResultDirName(const char * name) ; 
	static void            SetQARefStorage(const char * name) ; 
	static void            SetQARefDataDirName(RUNTYPE_t rt) { fgkRefDataDirName = GetRunTypeName(rt) ; }
	static void            SetQARefDataDirName(const char * name) ;
	void                   Show() const { ShowStatus(fDet, fTask) ; }
	void                   Show(DETECTORINDEX_t det) const { ShowStatus(det) ; }
	void                   ShowAll() const ;
	void                   UnSet(QABIT_t bit) ;
  
private:      
    
  Bool_t               CheckRange(DETECTORINDEX_t det) const ;
  Bool_t               CheckRange(ILCTASK_t tsk) const ;
  Bool_t               CheckRange(QABIT_t bit) const ;
  char *               GetBitName(QABIT_t bit) const ;
  ULong_t              GetStatus(DETECTORINDEX_t det) const  { return fQA[det] ;}
	void                 Finish() const ;  
  ULong_t              Offset(ILCTASK_t tsk) const ;
	void                 ShowStatus(DETECTORINDEX_t det, ILCTASK_t tsk=kNULLTASK) const ;
	void                 ShowASCIIStatus(DETECTORINDEX_t det, ILCTASK_t tsk, ULong_t status) const ; 
	void                 ResetStatus(DETECTORINDEX_t det) { fQA[det] = 0 ; }
	void                 Set(DETECTORINDEX_t det) { fDet = det ;}
	void                 Set(ILCTASK_t tsk) { fTask = tsk ; IlcDebug(1, Form("Ready to set QA status in %s", GetIlcTaskName(tsk) )) ; }
	void                 SetStatus(DETECTORINDEX_t det, UShort_t status) { fQA[det] = status ; }
	void                 SetStatusBit(DETECTORINDEX_t det, ILCTASK_t tsk, QABIT_t bit) ;
	void                 UnSetStatusBit(DETECTORINDEX_t det, ILCTASK_t tsk, QABIT_t bit) ;
  
	static IlcQA *       fgQA		                ; // pointer to the instance of the singleton
	Int_t                fNdet     	            ; // number of detectors
	ULong_t    *         fQA		                ; //[fNdet] the status word 4 bits for SIM, REC, ESD, ANA each
	DETECTORINDEX_t      fDet		                ; //!  the current detector (ITS, TPC, ....)
	ILCTASK_t            fTask	                ; //!  the current environment (SIM, REC, ESD, ANA)
	static TString       fgDetNames[]	          ; //! list of detector names   
	static TString       fgGRPPath              ; //! path of the GRP object in OCDB
	static TFile *       fgQADataFile	          ; //! the output file where the quality assurance maker store their results
	static TString       fgQADataFileName       ; //! the name of the file where the quality assurance maker store their results
	static TFile *       fgQARefFile	          ; //! the output file where the quality assurance maker store their results
	static TString       fgQARefDirName	        ; //! name of directory where to find the reference data file
	static TString       fgQARefFileName        ; //! file name where to find the reference data
	static TFile *       fgQAResultFile         ; //! File where to find the QA result
	static TString       fgQAResultDirName      ; //! the location of the output file where the QA results are stored  
	static TString       fgQAResultFileName     ; //! the output file where the QA results are stored  
	static TString       fgRTNames[]	          ; //! list of Run Type names   
	static TString       fgTaskNames[]	        ; //! list of tasks names   
  static const TString fgkExpert              ; //! name for the expert directory
  static const UInt_t  fgkExpertBit           ; //! TObject bit identifing the object as "expert"
	static const TString fgkLabLocalFile        ; //! label to identify a file as local 
	static const TString fgkLabLocalOCDB        ; //! label to identify a file as local OCDB 
	static const TString fgkLabAliEnOCDB        ; //! label to identify a file as AliEn OCDB 
	static const TString fgkRefFileName         ; //! name of Reference File Name 
	static const UInt_t  fgkQABit               ; //! bit in the QA data object which is set when Checker does not return 0
	static const TString fgkQAName              ; //! name of QA object 
	static const TString fgkQACorrNtName        ; //! name of QA Correlation Ntuple
	static const TString fgkRefOCDBDirName      ; //! name of Reference directory name in OCDB  	
	static       TString fgkRefDataDirName      ; //! name of Reference directory name in OCDB for data  	
	static const TString fgkQARefOCDBDefault    ; //! default storage for QA in OCDB 
  
  ClassDef(IlcQA,1)  //ILC Quality Assurance Object
};
#endif
