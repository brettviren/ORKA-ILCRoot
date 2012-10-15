#ifndef ILCQACHECKERBASE_H
#define ILCQACHECKERBASE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */


/* $Id: IlcQACheckerBase.h 54863 2012-02-28 12:10:29Z hristov $ */

//
//  Base class for detectors quality assurance checkers 
//  Compares Data made by QADataMakers with reference data
//  Y. Schutz CERN August 2007
//


// --- ROOT system ---
#include <TNamed.h>
#include "IlcQAv1.h"
class TCanvas ; 
class TFile ; 
class TH1 ; 
class TObjArray ; 
class TDirectory ; 
class TNtupleD ;
class IlcDetectorRecoParam ; 
class TList ; 

// --- Standard library ---

// --- IlcRoot header files ---

class IlcQACheckerBase: public TNamed {

public:
  IlcQACheckerBase(const char * name = "", const char * title = "") ;          // ctor
  virtual ~IlcQACheckerBase() ; // dtor
 
  void           DeleteImages() ;  
  TList *        GetExternParamlist() { return fExternParamList ;}
  TCanvas **     GetImage() { return fImage ; }
  TCanvas *      GetImage(IlcRecoParam::EventSpecie_t es) { return fImage[IlcRecoParam::AConvert(es)] ; }
  virtual void   Init(const IlcQAv1::DETECTORINDEX_t det)   { IlcQAv1::Instance(det) ; }
  virtual void   MakeImage( TObjArray ** list, IlcQAv1::TASKINDEX_t task, IlcQAv1::MODE_t mode) ; 
  void           PrintExternParam() ; 
  void           Run(IlcQAv1::ILCTASK_t tsk, const IlcDetectorRecoParam * recoParam = NULL); 
  void           Run(IlcQAv1::ILCTASK_t tsk, TObjArray ** list, const IlcDetectorRecoParam * recoParam = NULL); 
  void           Run(IlcQAv1::ILCTASK_t /*tsk*/, TNtupleD ** /*nt*/, const IlcDetectorRecoParam * /*recoParam*/) {;} 
  void           SetExternParamlist(TList * list) { fExternParamList = list ;}
  void           SetHiLo(Float_t * hiValue, Float_t * lowValue) ; 
  void           SetPrintImage(Bool_t opt = kTRUE) { fPrintImage = opt ; }

protected:
  virtual void Check(Double_t *rv, IlcQAv1::ILCTASK_t, TObjArray ** list, const IlcDetectorRecoParam * recoParam=0) ; 

  Double_t     DiffC(const TH1 * href, const TH1 * hin) const ;   
  Double_t     DiffK(const TH1 * href, const TH1 * hin) const ;   
  void         Finish() const ; 
  void         GetRefSubDir(const char * det, const char * task, TDirectory *& dirFile, TObjArray **& dirOCDB) ;
  virtual void SetQA(IlcQAv1::ILCTASK_t index, Double_t * value) const ;	

  TDirectory  * fDataSubDir     ; //! directory for the current task directory in the current detector directory in the data file
  TDirectory  * fRefSubDir      ; //! directory for the current task directory in the current detector directory in the reference file
  TObjArray   ** fRefOCDBSubDir ; //! Entry in OCDB for the current detector 
  Float_t     * fLowTestValue   ; // array of lower bounds for INFO, WARNING, ERROR, FATAL   
  Float_t     * fUpTestValue    ; // array of upper bounds for INFO, WARNING, ERROR, FATAL   
  TCanvas **    fImage          ; //[IlcRecoParam::kNSpecies] 
  Bool_t        fPrintImage     ; //! flag to print the images or not
  TList       * fExternParamList; //List of external parameters (TParameter<double>)  

private:
  IlcQACheckerBase(const IlcQACheckerBase& qac) ;   
  IlcQACheckerBase& operator = (const IlcQACheckerBase& qac) ;
   void PrivateCheck(Double_t * rv, IlcQAv1::ILCTASK_t index, const IlcDetectorRecoParam * recoParam) ;

  ClassDef(IlcQACheckerBase,3)  // description 

};

#endif // IlcQUALASSCHECKERBASE_H
