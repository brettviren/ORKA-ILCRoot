#ifndef ILCRUNINFO_H
#define ILCRUNINFO_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////////
//                          Class IlcRunInfo                                //
//   Container class for all the information related to LHCstate, run type, //
//   active detectors, beam energy etc.                                     //
//   It is used together with the IlcEventInfo in order to provide          //
//   the IlcRecoParam object with                                           //
//   the necessary information so that it can decide which instance of      //
//   IlcDetectorRecoParam objects to use in reconstruction one particular   //
//   event.                                                                 //
//                                                                          //
//   cvetan.cheshkov@cern.ch 12/06/2008                                     //
//////////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TObjString.h>

#include "IlcDAQ.h"

class IlcRunInfo : public TObject {

 public:
  IlcRunInfo();
  IlcRunInfo(const char *lhcState,
	     const char *beamType,
	     Float_t beamEnergy,
	     const char *runType,
	     UInt_t activeDetectors);
  virtual ~IlcRunInfo() {}

  virtual void Print(Option_t */*option=""*/) const { Dump(); }

  const char *GetLHCState() const { return fLHCState.Data(); }
  const char *GetBeamType() const { return fBeamType.Data(); }
  Float_t     GetBeamEnergy() const { return fBeamEnergy; }
  const char *GetRunType() const { return fRunType.Data(); }
  UInt_t      GetDetectorMask() const { return fActiveDetectors; }
  const char *GetActiveDetectors() const { return IlcDAQ::ListOfTriggeredDetectors(fActiveDetectors); }

  IlcRunInfo(const IlcRunInfo &evInfo);
  IlcRunInfo& operator= (const IlcRunInfo& evInfo);

 private:

  TString  fLHCState;       // state of the machine as provided by DCS and DAQ log-book (per run)
  TString  fBeamType;       // beam type for Ilc
  Float_t  fBeamEnergy;     // beam energy (in GeV)
  TString  fRunType;        // run type accoring to ECS (per run)
  UInt_t   fActiveDetectors;// list of active detectors (per run)

  ClassDef(IlcRunInfo,1)     // Run info class
};

#endif
