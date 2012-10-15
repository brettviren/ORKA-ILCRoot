#ifndef ILCANALYSISGOODIES_H
#define ILCANALYSISGOODIES_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */
//______________________________________________________________________________
// Various utilities usefull for analysis
//
//*-- Yves Schutz 
//////////////////////////////////////////////////////////////////////////////

#include <TObject.h> 
#include <TStopwatch.h> 
#include <TString.h> 

#include "IlcLog.h"

class IlcAnalysisTask ; 
class TChain ; 
class TClass ; 
class IlcEventTagCuts ;  
class IlcRunTagCuts ;  
class IlcLHCTagCuts ;  
class IlcDetectorTagCuts ;  
class IlcAnalysisManager ; 
class IlcAnalysisDataContainer ;

class IlcAnalysisGoodies : public TObject {

public:
  IlcAnalysisGoodies() ; 
  IlcAnalysisGoodies(const IlcAnalysisGoodies& ag) ; 
  ~IlcAnalysisGoodies() {;} 
  IlcAnalysisGoodies& operator=(const IlcAnalysisGoodies& ag) ;

  virtual void Help() const; 
  Bool_t Alien2Local(const TString collectionNameIn, const TString localDir) ; 
  IlcAnalysisDataContainer * ConnectInput(IlcAnalysisTask * task, TClass * classin, UShort_t index) ; 
  void ConnectInput(IlcAnalysisTask * task, IlcAnalysisDataContainer * in, UShort_t index ) ;
  IlcAnalysisDataContainer * ConnectOuput(IlcAnalysisTask * task, TClass * classou, UShort_t index, TString opt = "") ; 
  void   ConnectOuput(IlcAnalysisTask * task, IlcAnalysisDataContainer * ou, UShort_t index) ; 
  Bool_t Make(IlcRunTagCuts *runCuts, IlcLHCTagCuts *lhcCuts, IlcDetectorTagCuts *detCuts, IlcEventTagCuts *evtCuts, const char * in, const char * out) const ;
  Bool_t Merge(const char * collection, const char * subFile = 0, const char * outFile = 0) ; 
  Bool_t Register( const char * lfndir, const char * pfndir, const char * file)  ;   
  Bool_t Process(TChain * chain) ;  
  Bool_t Process(const char * esdFile)  ;  
  Bool_t Process(const char * inFile, IlcRunTagCuts *runCuts, IlcLHCTagCuts *lhcCuts, IlcDetectorTagCuts *detCuts, IlcEventTagCuts * evtCuts ) ;
  Bool_t Process(const char * inFile, const char * runCuts, const char * lhcCuts, const char * detCuts, const char * evtCuts) ;  
  void   SetESDTreeName(const char * name) { fESDTreeName = name ; }
  Bool_t MakeEsdCollectionFromTagFile(IlcRunTagCuts *runCuts, IlcLHCTagCuts *lhcCuts, IlcDetectorTagCuts *detCuts, IlcEventTagCuts *evtCuts, const char * in, const char * out) const ; 

private:
  Bool_t MakeEsdCollectionFromTagFile(const char * , const char * , const char * , const char *) const ;
  Bool_t MakeEsdCollectionFromTagCollection(IlcRunTagCuts *runCuts, IlcLHCTagCuts *lhcCuts, IlcDetectorTagCuts *detCuts, IlcEventTagCuts *evtCuts, const char * in, const char * out) const ;
  Bool_t MakeEsdCollectionFromTagCollection(const char * runCuts, const char *lhcCuts, const char *detCuts, const char * evtCuts, const char * in, const char * out) const ;
  Bool_t ProcessChain(TChain * chain) const ; 
  Bool_t ProcessEsdFile(const char * esdFile) const ;
  Bool_t ProcessEsdXmlCollection(const char * xmlFile) const ;
  Bool_t ProcessTagFile(const char * tagFile, IlcRunTagCuts *runCuts,  IlcLHCTagCuts *lhcCuts, IlcDetectorTagCuts *detCuts, IlcEventTagCuts *evtCuts) const ;  
  Bool_t ProcessTagFile(const char * tagFile, const char * runCuts, const char * evtCuts) const ;
  Bool_t ProcessTagFile(const char * tagFile, const char * runCuts, const char * lhcCuts, const char * detCuts, const char * evtCuts) const ;   
  Bool_t ProcessTagXmlCollection(const char * xmlFile, IlcRunTagCuts *runCuts, IlcLHCTagCuts *lhcCuts, IlcDetectorTagCuts *detCuts, IlcEventTagCuts * evtCuts) const ;   
  Bool_t ProcessTagXmlCollection(const char * xmlFile, const char * runCuts, const char * lhcCuts, const char * detCuts, const char * evtCuts) const ; 

  TStopwatch        fTimer         ;  //! stopwatch
  TString           fESDTreeName   ;  //! name of the ESD TTree
  IlcAnalysisManager * fAmgr       ;  //! the analysis manager
  ClassDef(IlcAnalysisGoodies, 0); // an analysis utilities class
};
#endif // ILCANALYSISGOODIES_H
