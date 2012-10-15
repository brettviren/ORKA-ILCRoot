#ifndef ILCDBPPRS_H
#define ILCDBPPRS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice     */
/* $Id: IlcDBPPRS.h 3276 2001-04-04 13:30:37Z schutz $ */
//_________________________________________________________________________
//  Class for the DB table PPRS
//
//*-- Author: Yves Schutz (SUBATECH)
 
#include <assert.h> 
// --- ROOT system ---
class TDatime ; 
 
// --- IlcRoot header files ---
 
class IlcDBPPRS : public TObject {
 
 public:
  IlcDBPPRS() ;
  IlcDBPPRS(const IlcDBPPRS & obj) {assert(0==1);}
  ~IlcDBPPRS() ;
  const char * GetField(Int_t index) const { return fFields[index].Data() ; } 
  const TString * GetFields() const { return fFields ; } 
  const Int_t GetNfields() const { return fNfields ; } 
  void GetEntry(Option_t * opt ="next") ;  
  const Int_t GetRUN() const {return fRUN ;}  
  const Int_t GetEVENT() const {return fEVENT ;}  
  const TString GetWORKER() const {return fWORKER ;}  
  const TString GetSTATUS() const {return fSTATUS ;}  
  const TDatime GetDATE() const {return fDATE ;}  
  const TString GetSTORAGE() const {return fSTORAGE ;}  
  const TString GetID() const {return fID ;}  
  const Int_t GetPOS() const {return fPOS ;}  
  const Int_t GetSIZE() const {return fSIZE ;}  
  const TString GetFTP() const {return fFTP ;}  
  const TString GetLOG() const {return fLOG ;}  
  const TString GetCOMMENT() const {return fCOMMENT ;}  
 
 private:
  Int_t fCurrentEntry ; // Current entry to be retrieved from table
  Int_t fNfields ; // number of fields in the table 
  TString * fFields ; //[fNfields] list of fields in the table 
  Int_t fRUN ; // RUN 
  Int_t fEVENT ; // EVENT 
  TString fWORKER ; // WORKER 
  TString fSTATUS ; // STATUS 
  TDatime fDATE ; // DATE 
  TString fSTORAGE ; // STORAGE 
  TString fID ; // ID 
  Int_t fPOS ; // POS 
  Int_t fSIZE ; // SIZE 
  TString fFTP ; // FTP 
  TString fLOG ; // LOG 
  TString fCOMMENT ; // COMMENT 
 
ClassDef(IlcDBPPRS,1) // DB class for table PPRS 
} ;
#endif
