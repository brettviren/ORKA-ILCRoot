#ifndef ILC_SHUTTLE_INTERFACE_H
#define ILC_SHUTTLE_INTERFACE_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcShuttleInterface.h 58464 2012-09-05 15:46:16Z hristov $ */

//
// abstract interface class to IlcShuttle
//

#include <TObject.h>
#include <TString.h>

class TList;
class IlcPreprocessor;
class IlcCDBMetaData;
class IlcCDBPath;
class IlcCDBEntry;

class IlcShuttleInterface : public TObject
{
  public:
	enum System { kDAQ = 0, kDCS, kHLT, kDQM };
    enum { kNDetectors = 21+1 }; // number of subdetectors in ILC

    virtual Bool_t Store(const IlcCDBPath& path, TObject* object, IlcCDBMetaData* metaData,
    				Int_t validityStart = 0, Bool_t validityInfinite = kFALSE) = 0;
    virtual Bool_t StoreReferenceData(const IlcCDBPath& path, TObject* object, IlcCDBMetaData* metaData) = 0;
    virtual Bool_t StoreReferenceFile(const char* detector, const char* localFile, const char* gridFileName) = 0;
    virtual Bool_t StoreRunMetadataFile(const char* localFile, const char* gridFileName) = 0;
    
    virtual const char* GetFile(Int_t system, const char* detector, const char* id, const char* source) = 0;
    
    virtual TList* GetFileSources(Int_t system, const char* detector, const char* id = 0) = 0;
    virtual TList* GetFileIDs(Int_t system, const char* detector, const char* source) = 0;
    
    virtual const char* GetRunParameter(const char* lbEntry) = 0;
    virtual UInt_t GetStartTimeDCSQuery() = 0;
    virtual UInt_t GetEndTimeDCSQuery() = 0;
    virtual const char* GetRunType() = 0;
    virtual Bool_t GetHLTStatus() = 0;
    virtual const char* GetTriggerConfiguration() = 0;
    virtual const char* GetCTPTimeParams() = 0;
    virtual const char* GetTriggerDetectorMask() = 0;

    virtual IlcCDBEntry* GetFromOCDB(const char* detector, const IlcCDBPath& path) = 0;
    
    virtual void Log(const char* detector, const char* message, UInt_t level=3) = 0;

    virtual void RegisterPreprocessor(IlcPreprocessor* preprocessor) = 0;

    static const char* GetSystemName(UInt_t system) {return (system < 4) ? fkSystemNames[system] : 0;}

    static const char* GetOfflineDetName(const char* detName);
    static const char* GetDetName(UInt_t detPos);
    static Int_t GetDetPos(const char* detName);
    static UInt_t NDetectors() {return kNDetectors;}

    static TString GetMainCDB () {return fgkMainCDB;}
    static TString GetLocalCDB () {return fgkLocalCDB;}
    static TString GetMainRefStorage() {return fgkMainRefStorage;}
    static TString GetLocalRefStorage() {return fgkLocalRefStorage;}
    static TString GetMirrorSEs () {return fgkMirrorSEs;}

    static const char* GetShuttleLogDir() {return fgkShuttleLogDir.Data();}
    static const char* GetShuttleTempDir() {return fgkShuttleTempDir.Data();}

    virtual void SendMLFromDet(const char* value) = 0;
    virtual TString* GetLTUConfig(const char* det) =0;

  protected:

    static const char* fkSystemNames[4];  		// names of the systems providing data to the shuttle
    static const char* fgkDetName[kNDetectors]; 	// names of detectors' preprocessors (3-letter code convention)
    static const char* fgkOfflineDetName[kNDetectors];  // names of detectors in OCDB (IlcRoot naming convention)

    static TString fgkMainCDB;		// URI of the main (Grid) CDB storage
    static TString fgkLocalCDB;		// URI of the local backup CDB storage
    static TString fgkMainRefStorage;	// URI of the main (Grid) REFERENCE storage
    static TString fgkLocalRefStorage;	// URI of the local REFERENCE storage
    static TString fgkMirrorSEs;	// mirror SEs for the main (Grid) CDB storage

    static TString fgkShuttleTempDir;	// path of SHUTTLE temp folder
    static TString fgkShuttleLogDir;	// path of SHUTTLE log folder

  private:
    ClassDef(IlcShuttleInterface, 0);
};

#endif
