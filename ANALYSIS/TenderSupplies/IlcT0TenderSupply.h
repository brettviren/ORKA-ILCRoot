#ifndef ILCT0TENDERSUPPLY_H
#define ILCT0TENDERSUPPLY_H

/* 2012-2013 Infrastructure for Large Collider Experiment, All rights reserved. *
 * See cxx source for full Copyright notice                               */

///////////////////////////////////////////////////////////////////////////
//                                                                       //
//      //
//   //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include <IlcTenderSupply.h>

class IlcT0TenderSupply: public IlcTenderSupply {
  
 public:
  IlcT0TenderSupply();
  IlcT0TenderSupply(const char *name, const IlcTender *tender=NULL);
  virtual ~IlcT0TenderSupply();

  virtual void          Init();
  virtual void          ProcessEvent();
  void SetCorrectMeanTime (Bool_t flag=kFALSE){fCorrectMeanTime=flag;};
  void SetAmplutudeCorrection (Bool_t flag=kFALSE){fCorrectStartTimeOnAmplSatur=flag;};
  void SetPass4LHC11aCorrection (Bool_t flag=kFALSE){fPass4LHC11aCorrection=flag;};

 private:
  
  IlcT0TenderSupply(const IlcT0TenderSupply&c);
  IlcT0TenderSupply& operator= (const IlcT0TenderSupply&c);


  Bool_t  fCorrectMeanTime; //! mean time shift will be corrected
  Float_t fTimeOffset[4]; //! time offset to be used for fCorrectMeanTime
  Bool_t  fCorrectStartTimeOnAmplSatur; //!  fix start times suffering from saturated amplitude in pmts
  Float_t fAmplitudeThreshold; //! above this value pmt suffer from saturation
  Bool_t fPass4LHC11aCorrection; //! above this value pmt suffer from saturation

  ClassDef(IlcT0TenderSupply, 2);  // T0 tender supply
};

#endif
