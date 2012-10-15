#ifndef ILC_PREPROCESSOR_H
#define ILC_PREPROCESSOR_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPreprocessor.h 51748 2011-09-26 15:52:43Z zampolli $ */

//
// This class is the CDBPreProcessor interface,
// supposed to be implemented by any detector
// interested in immediate processing of data 
// which is retrieved from DCS, DAQ or HLT.
//

#include <TNamed.h>
#include <TList.h>

class TMap;

class IlcCDBMetaData;
class IlcCDBEntry;
class IlcShuttleInterface;

class IlcPreprocessor : public TNamed
{
  public:

	enum { kDAQ = 0, kDCS, kHLT, kDQM };

    IlcPreprocessor(const char* detector, IlcShuttleInterface* shuttle);
    virtual ~IlcPreprocessor();

    virtual void Initialize(Int_t run, UInt_t startTime, UInt_t endTime);
    virtual UInt_t Process(TMap* dcsAliasMap) = 0;
   
    virtual Bool_t ProcessDCS() { return kTRUE; }
    Bool_t ProcessRunType();
  
  protected:
    Bool_t Store(const char* pathLevel2, const char* pathLevel3, TObject* object,
    		IlcCDBMetaData* metaData, Int_t validityStart = 0, Bool_t validityInfinite = kFALSE);
    Bool_t StoreReferenceData(const char* pathLevel2, const char* pathLevel3, TObject* object,
    		IlcCDBMetaData* metaData);
    Bool_t StoreReferenceFile(const char* localFile, const char* gridFileName);
    Bool_t StoreRunMetadataFile(const char* localFile, const char* gridFileName);
    
    const char* GetFile(Int_t system, const char* id, const char* source);
    TList* GetFileSources(Int_t system, const char* id = 0);
    const char* GetForeignFile(const char* detector, Int_t system, const char* id, const char* source);
    TList* GetForeignFileSources(const char* detector, Int_t system, const char* id);
    TList* GetFileIDs(Int_t system, const char* source);
    const char* GetRunParameter(const char* param);
    IlcCDBEntry* GetFromOCDB(const char* pathLevel2, const char* pathLevel3);
    IlcCDBEntry* GetGeometryFromOCDB();
    const char* GetRunType();
    Bool_t GetHLTStatus();
    const char* GetTriggerConfiguration();
    const char* GetCTPTimeParams();
    const char* GetTriggerDetectorMask();
    void Log(const char* message, UInt_t level=3);
    UInt_t GetStartTimeDCSQuery();
    UInt_t GetEndTimeDCSQuery();
    
    void AddRunType(const char* runType);

    void SendToML(const char*);
    TString* GetLTUConfig(const char* det);

    Int_t fRun;         // current run
    UInt_t fStartTime;  // starttime of current run
    UInt_t fEndTime;    // endtime of current run

  private:
    IlcPreprocessor(const IlcPreprocessor & source);
    IlcPreprocessor & operator=(const IlcPreprocessor & source);
    IlcShuttleInterface* fShuttle;   // link to Shuttle
    
    TList fRunTypes;    // list of run types that are processed by this preprocessor

    ClassDef(IlcPreprocessor, 0);
};

#endif
