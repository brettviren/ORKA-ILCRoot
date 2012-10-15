#ifndef ILCQACHECKER_H
#define ILCQACHECKER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcQAChecker.h 54214 2012-01-29 17:56:54Z shahoian $ */

//
// class for running the Quality Assurance Checker
// to run type:
//   IlcQAChecker qac;
//   qac.Run();
//   qac.SelectDetectors("list of detectors") ; 
//   qac.SelectTargets("list of targets among Hits, Digits, ESD") ; 
//

#include <TNamed.h>
#include <TFile.h>  
class TNtupleD ;

#include "IlcQAv1.h"
#include "IlcRecoParam.h"
class IlcCDBEntry ; 
class IlcRunInfo ;
class IlcQACheckerBase ; 
class IlcDetectorRecoParam ; 

class IlcQAChecker: public TNamed {
public:
  IlcQAChecker(const char* name = "IlcQAChecker", 
		    const char* title = "Quality Assurance checker for Raws, Hits, Digits and ESDs");
  IlcQAChecker(const IlcQAChecker& qac);
  IlcQAChecker& operator = (const IlcQAChecker& qac);
  virtual  ~IlcQAChecker();

  static  IlcQAChecker *     Instance() ;
  IlcQACheckerBase *         GetDetQAChecker(Int_t det) ; 
  Bool_t Run(const char * fileName = NULL, IlcDetectorRecoParam * recoParam = NULL) ;
  Bool_t Run(IlcQAv1::DETECTORINDEX_t det, IlcQAv1::TASKINDEX_t task, TObjArray ** list, IlcDetectorRecoParam * recoParam = NULL);
  Bool_t Run(IlcQAv1::DETECTORINDEX_t det, IlcQAv1::TASKINDEX_t task, TNtupleD ** list, IlcDetectorRecoParam * recoParam = NULL);
  void   SetRunInfo(IlcRunInfo * ei) {fRunInfo = ei;}
  Int_t  GetRunNumber() const { return fRun ; } 
  void   SetRunNumber(Int_t run) { fRun = run ; } 

private:

  void LoadRunInfoFromGRP() ; 

  static IlcQAChecker *       fgQAChecker ;             // pointer to the instance of the singleton
  TFile *                     fDataFile ;               //! Data file to check
  IlcRunInfo *                fRunInfo ;                //! Event info object 
  Bool_t                      fRunInfoOwner;            //! owns fRunInfo or not
  TFile *                     fRefFile ;                //! Reference Data file 
  TString                     fFoundDetectors ;         //! detectors for which the Quality assurance could be done
  IlcQACheckerBase *          fCheckers[IlcQAv1::kNDET] ; //! list of detectors checkers
  IlcRecoParam::EventSpecie_t fEventSpecie ;            //! event specie deduced from the GRP data
	Int_t                       fRun ;                    //! run number
  ClassDef(IlcQAChecker, 1)  // class for running generation, simulation and digitization
};
#endif
