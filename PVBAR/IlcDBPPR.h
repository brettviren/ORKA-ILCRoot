#ifndef ILCDBPPR_H
#define ILCDBPPR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */
/* $Id: IlcDBPPR.h 3276 2001-04-04 13:30:37Z schutz $ */
//_________________________________________________________________________
//  Class for the DB table PPR
//
//*-- Author: Yves Schutz (SUBATECH)
 
#include <assert.h> 
// --- ROOT system ---
class TDatime ; 
 
// --- IlcRoot header files ---
 
class IlcDBPPR : public TObject {
 
 public:
  IlcDBPPR() ;
  IlcDBPPR(const IlcDBPPR & obj) {assert(0==1);}
  ~IlcDBPPR() ;
  const char * GetField(Int_t index) const { return fFields[index].Data() ; } 
  const TString * GetFields() const { return fFields ; } 
  const Int_t GetNfields() const { return fNfields ; } 
  void GetEntry(Option_t * opt ="next") ;  
  const Int_t GetRUN() const {return fRUN ;}  
  const Int_t GetEVENT() const {return fEVENT ;}  
  const TDatime GetDATE() const {return fDATE ;}  
  const TString GetSIMULATION() const {return fSIMULATION ;}  
  const TString GetDIGITIZATION() const {return fDIGITIZATION ;}  
  const TString GetRECONSTRUCTION() const {return fRECONSTRUCTION ;}  
 
 private:
  Int_t fCurrentEntry ; // Current entry to be retrieved from table
  Int_t fNfields ; // number of fields in the table 
  TString * fFields ; //[fNfields] list of fields in the table 
  Int_t fRUN ; // RUN 
  Int_t fEVENT ; // EVENT 
  TDatime fDATE ; // DATE 
  TString fSIMULATION ; // SIMULATION 
  TString fDIGITIZATION ; // DIGITIZATION 
  TString fRECONSTRUCTION ; // RECONSTRUCTION 
 
ClassDef(IlcDBPPR,1) // DB class for table PPR 
} ;
#endif
