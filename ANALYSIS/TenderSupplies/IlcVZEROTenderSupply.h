#ifndef ILCVZEROTENDERSUPPLY_H
#define ILCVZEROTENDERSUPPLY_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

////////////////////////////////////////////////////////////////////////
//                                                                    //
//  Recalculate VZERO timing and decision using the tender            //
//  (in case the LHC phase drift is updated in OCDB)                  //
//                                                                    //
////////////////////////////////////////////////////////////////////////



#include <IlcTenderSupply.h>

class TF1;
class IlcVZEROCalibData;
class IlcVZERORecoParam;

class IlcVZEROTenderSupply: public IlcTenderSupply {
  
public:
  IlcVZEROTenderSupply();
  IlcVZEROTenderSupply(const char *name, const IlcTender *tender=NULL);
  
  virtual ~IlcVZEROTenderSupply(){;}

  virtual void              Init();
  virtual void              ProcessEvent();
  
  void GetPhaseCorrection();

  void SetDebug(Bool_t flag) { fDebug = flag; }

private:
  IlcVZEROCalibData* fCalibData;      //! calibration data
  TF1*               fTimeSlewing;    //! Function for time slewing correction
  IlcVZERORecoParam* fRecoParam;      //! pointer to reco-param object
  Float_t            fLHCClockPhase;  //! the correction to the LHC-clock phase
  Bool_t             fDebug;          //  debug on/off
  
  IlcVZEROTenderSupply(const IlcVZEROTenderSupply&c);
  IlcVZEROTenderSupply& operator= (const IlcVZEROTenderSupply&c);
  
  ClassDef(IlcVZEROTenderSupply, 2)  // VZERO tender task
};


#endif

