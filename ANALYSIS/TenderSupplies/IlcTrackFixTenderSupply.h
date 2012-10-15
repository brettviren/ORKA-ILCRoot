#ifndef ILCTRACKFIXTENDERSUPPLY_H
#define ILCTRACKFIXTENDERSUPPLY_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////
//                                                                    //
//  Apply on-the-fly fix to tracks                                    //
//                                                                    //
//  19/06/2012: RS: Add 1/pt shift from AODB to TPC and TPC-ITS       //
//                  Optionally correct also track coordinate          //
//                                                                    //
////////////////////////////////////////////////////////////////////////

#include <TString.h>
#include "IlcTenderSupply.h"


class IlcESDVertex;
class IlcExternalTrackParam;
class IlcOADBContainer;
class IlcESDtrack;
class IlcOADBTrackFix;

class IlcTrackFixTenderSupply: public IlcTenderSupply {
  
public:
  //
  IlcTrackFixTenderSupply();
  IlcTrackFixTenderSupply(const char *name, const IlcTender *tender=NULL);
  virtual ~IlcTrackFixTenderSupply();
  virtual  void ProcessEvent();
  virtual  void Init() {}
  //
  Double_t GetSideAFraction(const IlcESDtrack* track) const;
  void     CorrectTrackPtInv(IlcExternalTrackParam* trc, int mode, double sideAfraction, double phi) const;
  Bool_t   GetRunCorrections(int run);
  Bool_t   LoadOADBObjects();
  //
  void     SetOADBObjPath(const char* path)        { fOADBObjPath = path; }
  void     SetOADBObjName(const char* name)        { fOADBObjName = name; }
  TString& GetOADBObjPath()                 const  { return (TString&)fOADBObjPath; }
  TString& GetOADBObjName()                 const  { return (TString&)fOADBObjName; }
  //
  void     SetDebugLevel(Int_t l=1)                {fDebug = l;}
  Int_t    GetDebugLevel()                  const  {return fDebug;}
  //
private:
  
  IlcTrackFixTenderSupply(const IlcTrackFixTenderSupply&c);
  IlcTrackFixTenderSupply& operator= (const IlcTrackFixTenderSupply&c);
  //
  Int_t             fDebug;                  // Debug level
  Double_t          fBz;                     // mag field from ESD
  IlcOADBTrackFix*  fParams;                 // parameters for current run
  TString           fOADBObjPath;            // path of file with parameters to use, starting from OADB dir
  TString           fOADBObjName;            // name of the corrections object in the OADB container
  IlcOADBContainer* fOADBCont;               // OADB container with parameters collection
  //
  ClassDef(IlcTrackFixTenderSupply, 1);  // track fixing tender task 
};


#endif

