#ifndef ILCRECONSTRUCTOR_H
#define ILCRECONSTRUCTOR_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcReconstructor.h 50895 2011-08-02 13:03:20Z hristov $ */

//
// base class for reconstruction algorithm
// Derived classes should implement a default constructor and
// the virtual methods
//

#include <TObject.h>
#include <TString.h>

class TTree;
class IlcRawReader;
class IlcVertexer;
class IlcTracker;
class IlcTrackleter;
class IlcESDEvent;
class IlcDetectorRecoParam;
class IlcRunInfo;
class IlcEventInfo;
class IlcESDpid;

#include "IlcReconstruction.h"

class IlcReconstructor: public TObject {
public:
  IlcReconstructor(): TObject(), fOption(), fRunInfo(0x0), fEventInfo(0x0) {};
  virtual ~IlcReconstructor() {};

  virtual void         Init() {};

  virtual Bool_t       HasDigitConversion() const {return kFALSE;};
  virtual void         ConvertDigits(IlcRawReader* rawReader, TTree* digitsTree) const;

  virtual void         Reconstruct(TTree* digitsTree, TTree* clustersTree) const;
  virtual void         Reconstruct(IlcRawReader* rawReader, TTree* clustersTree) const;

  virtual IlcVertexer* CreateVertexer() const 
    {return NULL;}
  virtual IlcTracker*  CreateTracker() const 
    {return NULL;}
  virtual IlcTracker*  CreateTrackleter() const 
    {return NULL;}
  virtual IlcTrackleter* CreateMultFinder() const 
    {return NULL;}

  virtual void         FillESD(TTree* digitsTree, TTree* clustersTree, 
			       IlcESDEvent* esd) const;
  virtual void         FillESD(IlcRawReader* rawReader, TTree* clustersTree, 
			       IlcESDEvent* esd) const;

  virtual const char*  GetDetectorName() const;

  void                 SetOption(Option_t* option) {fOption = option;};
  virtual Option_t*    GetOption() const {return fOption.Data();};

  void                 SetRunInfo(IlcRunInfo *runInfo) {fRunInfo = runInfo;}
  const IlcRunInfo*    GetRunInfo() const {return fRunInfo;}
  void                 SetEventInfo(IlcEventInfo *evInfo) {fEventInfo = evInfo;}
  const IlcEventInfo*  GetEventInfo() const {return fEventInfo;}

  void                               SetRecoParam(const IlcDetectorRecoParam *par);
  static const IlcDetectorRecoParam* GetRecoParam(Int_t iDet);
  virtual void                 GetPidSettings(IlcESDpid *esdPID);

  virtual void FillEventTimeWithTOF(IlcESDEvent *, IlcESDpid *)
    {return;}
  virtual void Terminate() const {return; }

private:

  IlcReconstructor(const IlcReconstructor &); // Not implemented
  IlcReconstructor& operator=(const IlcReconstructor &); // Not implemented
  
  TString                            fOption;                                       //! option for reconstruction
  static const IlcDetectorRecoParam* fgRecoParam[IlcReconstruction::kNDetectors]; //! event reconstruction parameters for all detectors
  IlcRunInfo*                        fRunInfo;                                    //! pointer to the run info object
  IlcEventInfo*                      fEventInfo;                                  //! pointer to the event info object

  ClassDef(IlcReconstructor, 0)   // base class for reconstruction algorithms
};

#endif
