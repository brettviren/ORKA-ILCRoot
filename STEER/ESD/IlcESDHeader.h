// -*- mode: C++ -*- 
#ifndef ILCESDHEADER_H
#define ILCESDHEADER_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//                      Class IlcESDHeader
//   Header data
//   for the ESD   
//   Origin: Christian Klein-Boesing, CERN, Christian.Klein-Boesing@cern.ch 
//-------------------------------------------------------------------------

#include <TObjArray.h>
#include <TClonesArray.h>
#include "IlcVHeader.h"
#include "IlcTriggerScalersESD.h"
#include "IlcTriggerScalersRecordESD.h"

class IlcTriggerScalersESD;
class IlcTriggerScalersRecordESD;
class IlcTriggerIR;
class IlcTriggerConfiguration;

class IlcESDHeader: public IlcVHeader {
public:
  IlcESDHeader();
  virtual ~IlcESDHeader();
  IlcESDHeader(const IlcESDHeader& header);
  IlcESDHeader& operator=(const IlcESDHeader& header);
  virtual void Copy(TObject &obj) const;

  void      SetTriggerMask(ULong64_t n) {fTriggerMask=n;}
  void      SetOrbitNumber(UInt_t n) {fOrbitNumber=n;}
  void      SetTimeStamp(UInt_t timeStamp){fTimeStamp = timeStamp;}
  void      SetEventType(UInt_t eventType){fEventType = eventType;}
  void      SetEventSpecie(UInt_t eventSpecie){fEventSpecie = eventSpecie;}
  void      SetEventNumberInFile(Int_t n) {fEventNumberInFile=n;}
  void      SetBunchCrossNumber(UShort_t n) {fBunchCrossNumber=n;}
  void      SetPeriodNumber(UInt_t n) {fPeriodNumber=n;}
  void      SetTriggerCluster(UChar_t n) {fTriggerCluster = n;}
  Bool_t    AddTriggerIR(const IlcTriggerIR* ir);
  void      SetCTPConfig(IlcTriggerConfiguration* ctpConfig) {fCTPConfig=ctpConfig;};
//************Setters/Getters for Trigger Inputs and TriggerScalersRecordESD
  void SetL0TriggerInputs(UInt_t n) {fL0TriggerInputs=n;}
  void SetL1TriggerInputs(UInt_t n) {fL1TriggerInputs=n;}
  void SetL2TriggerInputs(UShort_t n) {fL2TriggerInputs=n;}
  UInt_t      GetL0TriggerInputs() const {return fL0TriggerInputs;}  
  UInt_t      GetL1TriggerInputs() const {return fL1TriggerInputs;} 
  UShort_t    GetL2TriggerInputs() const {return fL2TriggerInputs;} 
  void SetTriggerScalersRecord(IlcTriggerScalersESD *scalerRun) {fTriggerScalers.AddTriggerScalers(scalerRun); }
  void SetTriggerScalersDeltaEvent(const IlcTriggerScalersRecordESD *scalerRun) {fTriggerScalersDeltaEvent = *scalerRun; }
  void SetTriggerScalersDeltaRun(const IlcTriggerScalersRecordESD *scalerRun) {fTriggerScalersDeltaRun = *scalerRun; }
  const IlcTriggerScalersRecordESD* GetTriggerScalersRecord() const {return &fTriggerScalers; }
  const IlcTriggerScalersRecordESD* GetTriggerScalersDeltaEvent() const {return &fTriggerScalersDeltaEvent; }
  const IlcTriggerScalersRecordESD* GetTriggerScalersDeltaRun() const {return &fTriggerScalersDeltaRun; }
  const IlcTriggerIR* GetTriggerIR(Int_t i) const { return (const IlcTriggerIR*)fIRBufferArray[i]; }
  void SetActiveTriggerInputs(const char*name, Int_t index);
  const char* GetTriggerInputName(Int_t index, Int_t trglevel) const;
  TString     GetActiveTriggerInputs() const;
  TString     GetFiredTriggerInputs() const;
  Bool_t      IsTriggerInputFired(const char *name) const;
  const IlcTriggerConfiguration*  GetCTPConfig() const { return fCTPConfig;}
//**************************************************************************

  ULong64_t GetTriggerMask() const {return fTriggerMask;}
  UInt_t    GetOrbitNumber() const {return fOrbitNumber;}
  UInt_t    GetTimeStamp()  const { return fTimeStamp;}
  UInt_t    GetEventType()  const { return fEventType;}
  UInt_t    GetEventSpecie()  const { return fEventSpecie;}
  Int_t     GetEventNumberInFile() const {return fEventNumberInFile;}
  UShort_t  GetBunchCrossNumber() const {return fBunchCrossNumber;}
  UInt_t    GetPeriodNumber() const {return fPeriodNumber;}
  UChar_t   GetTriggerCluster() const {return fTriggerCluster;}
  Int_t     GetTriggerIREntries() const { return fIRBufferArray.GetEntriesFast();};
  Int_t     GetTriggerIREntries(Int_t int1, Int_t int2, Float_t deltaTime = 180.) const;
  TObjArray GetIRArray(Int_t int1, Int_t int2, Float_t deltaTime = 180.) const;
  void      Reset();
  void      Print(const Option_t *opt=0) const;

  enum {kNTriggerInputs = 60};   //24 L0, 24 L1 and 12 L2 inputs

private:

  // Event Identification
  ULong64_t    fTriggerMask;       // Trigger Type (mask)
  UInt_t       fOrbitNumber;       // Orbit Number
  UInt_t       fTimeStamp;         // Time stamp
  UInt_t       fEventType;         // Type of Event
  UInt_t       fEventSpecie;       // Reconstruction event specie (1-default,2-lowM,4-highM,8-cosmic,16-cal)
  UInt_t       fPeriodNumber;      // Period Number
  Int_t        fEventNumberInFile; // Running Event count in the file
  UShort_t     fBunchCrossNumber;  // Bunch Crossing Number
  UChar_t      fTriggerCluster;    // Trigger cluster (mask)
  UInt_t       fL0TriggerInputs;   // L0 Trigger Inputs (mask)
  UInt_t       fL1TriggerInputs;   // L1 Trigger Inputs (mask)
  UShort_t     fL2TriggerInputs;   // L2 Trigger Inputs (mask)
  IlcTriggerScalersRecordESD fTriggerScalers;  //Trigger counters of triggered classes in event, interpolated to the event time
  IlcTriggerScalersRecordESD fTriggerScalersDeltaEvent;  // Change in the trigger scalers between the two counter readings closest to the event time 
  IlcTriggerScalersRecordESD fTriggerScalersDeltaRun;  // Total number of counts in the trigger scalers for the duration of the run
  enum {kNMaxIR = 3};              // Max number of interaction records (IR)
  IlcTriggerIR*  fIRArray[kNMaxIR];// Array with trigger IRs 
  TObjArray    fTriggerInputsNames;// Array of TNamed of the active trigger inputs (L0,L1 and L2)
  IlcTriggerConfiguration*  fCTPConfig; // Trigger configuration for the run
  TObjArray    fIRBufferArray;// Array with interaction records before and after triggered event



  ClassDef(IlcESDHeader,10)
};

#endif
