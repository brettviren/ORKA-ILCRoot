#ifndef ILCLHCCLOCKPHASE_H
#define ILCLHCCLOCKPHASE_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//////////////////////////////////////////////////////////////////////////////
//                          Class IlcLHCClockPhase                          //
//   Container class for storing of the LHC clock phase.                    //
//   The source of the the data are BPTXs - they measure                    //
//   The beam pick-up time w.r.t to the LHC clock distributed by CTP.       //
//   The values stored by DCS are always relative to some fixed reference   //
//   moment.                                                                //
//                                                                          //
//   cvetan.cheshkov@cern.ch 21/07/2010                                     //
//////////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include <TObjArray.h>

class IlcDCSValue;

class IlcLHCClockPhase : public TObject {

 public:
  IlcLHCClockPhase();
  virtual ~IlcLHCClockPhase() {}

  Int_t    GetNumberOfPhaseB1DPs()   const {return fPhaseB1.GetEntries();}
  Int_t    GetNumberOfPhaseB2DPs()   const {return fPhaseB2.GetEntries();}
  const IlcDCSValue* GetPhaseB1DP(Int_t index) const;
  const IlcDCSValue* GetPhaseB2DP(Int_t index) const;

  Float_t  GetMeanPhaseB1() const;
  Float_t  GetMeanPhaseB2() const;
  Float_t  GetMeanPhase()   const;
  Float_t  GetPhaseB1(UInt_t timestamp) const;
  Float_t  GetPhaseB2(UInt_t timestamp) const;
  Float_t  GetPhase(UInt_t timestamp)   const;

  void     AddPhaseB1DP(UInt_t timestamp, Float_t phase);
  void     AddPhaseB2DP(UInt_t timestamp, Float_t phase);

  virtual void Print( const Option_t* opt ="" ) const;

 private:
  IlcLHCClockPhase(const IlcLHCClockPhase &phase);
  IlcLHCClockPhase& operator= (const IlcLHCClockPhase& phase);

  TObjArray fPhaseB1;              // Array of IlcDCSValue's containing the phase as measure by BPTX on beam1
  TObjArray fPhaseB2;              // Array of IlcDCSValue's containing the phase as measure by BPTX on beam2

  ClassDef(IlcLHCClockPhase,1)     // LHC-clock phase container class
};

#endif
