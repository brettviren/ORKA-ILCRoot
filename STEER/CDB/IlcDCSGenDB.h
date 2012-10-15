
/////////////////////////////////////////////////////////////////
// Class to generate temperature sensor data base entries.
//
// Existing data base structure read at start of processsing.
// 20/12-2006 HH.
// Modification log:
/////////////////////////////////////////////////////////////////

#ifndef IlcDCSGenDB_h
#define IlcDCSGenDB_h

#include <TROOT.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TH1F.h>
#include <TFile.h>
#include <TObjArray.h>

#include "IlcCDBMetaData.h"
#include "IlcCDBManager.h"
#include "IlcCDBId.h"
#include "IlcCDBStorage.h"
#include "IlcDCSSensorArray.h"
#include "IlcLog.h"
#include "TSystem.h"

class IlcDCSGenDB : public TObject {

public:

// Constructors

  IlcDCSGenDB();
  IlcDCSGenDB(const char* defaultStorage, const char* specificStorage);
  ~IlcDCSGenDB();

// Functionality

  void            MakeCalib(const char *file, const char *fMap,
                            const TTimeStamp& startTime,
			    const TTimeStamp& endTime,
			    Int_t firstRun, Int_t lastRun, const char *calibDir);
  void            MakeConfig(const char *file, Int_t firstRun, Int_t lastRun, 
                             const char *confDir);
  IlcCDBMetaData* CreateMetaObject(const char *objectClassName);
  void            StoreObject(const char* cdbPath, TObject* object, IlcCDBMetaData* metaData);
  void            Init(Int_t run, const char *configDir, 
                       const char *specificDir, 
		       const char *sensorClass="IlcDCSSensorArray");
  static TClonesArray *  ReadList(const char* fname, const char *title="dcsConf");
  static TTree        *  ReadListTree(const char* fname, const char *title="dcsConf");

// Getters/Setters

  void            SetFirstRun(Int_t frun){fFirstRun=frun;}
  void            SetLastRun(Int_t lrun) {fLastRun=lrun;}
  TMap*           SetGraphFile(const char* fname);
  void            SetConfTree(TTree *tree) {fConfTree=tree;}
  TTree*          GetConfTree() const {return fConfTree;}
  const TString&  GetSpecificStorage() const { return fSpecificStorage;}
  void            SetSpecificStorage (const TString& specificStorage) { fSpecificStorage=specificStorage; }
  const TString&  GetDefaultStorage() const { return fDefaultStorage;}
  void            SetDefaultStorage (const TString& defaultStorage) { fDefaultStorage=defaultStorage; }
  const IlcDCSSensorArray* GetSensorArray() const {return fSensor;}
  void            SetSensorArray(IlcDCSSensorArray *arr) { fSensor=arr; }


protected:
  IlcDCSGenDB(const IlcDCSGenDB& org);
  IlcDCSGenDB& operator= (const IlcDCSGenDB& org);

   Int_t          fFirstRun;        // first run in validity period
   Int_t          fLastRun;         // last run in validity period
   TString        fSpecificStorage; // specific storage for data base
   TString        fDefaultStorage;  // default storage for data base
   IlcDCSSensorArray  *fSensor;     // array of DCS sensors
   IlcCDBStorage  *fStorLoc;        // pointer to CDB storage
   IlcCDBMetaData *fMetaData;       // data base metadata
   TTree          *fConfTree;	    // configuration tree

   ClassDef(IlcDCSGenDB,1)
 };
#endif

