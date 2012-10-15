#ifndef ILCESDMUONPAD_H
#define ILCESDMUONPAD_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
* See cxx source for full Copyright notice                               */

// $Id: IlcESDMuonPad.h 50615 2011-07-16 23:19:19Z hristov $

/// \class IlcESDMuonPad
/// \brief Class to describe the MUON pads in the Event Summary Data
//  Author Philippe Pillot, Subatech


#include <TObject.h>

class IlcESDMuonPad : public TObject {
public:
  IlcESDMuonPad(); // Constructor
  virtual ~IlcESDMuonPad() {} ///< Destructor
  IlcESDMuonPad(const IlcESDMuonPad& pad);
  IlcESDMuonPad& operator=(const IlcESDMuonPad& pad);
  
  /// Clear method (used by TClonesArray)
  virtual void Clear(Option_t* = "") {}
  
  /// Set the raw charge
  void     SetADC(Int_t adc) {fADC = adc;}
  /// Return the raw charge
  Int_t    GetADC() const {return fADC;}
  
  /// Set the calibrated charge
  void     SetCharge(Double_t charge) {fCharge = charge;}
  /// Return the calibrated charge
  Double_t GetCharge() const {return fCharge;}
  
  /// Return detection element id, part of the uniqueID
  Int_t    GetDetElemId() const   {return (GetUniqueID() & 0x00000FFF);}
  /// Return electronic card id, part of the uniqueID
  Int_t    GetManuId() const      {return (GetUniqueID() & 0x00FFF000) >> 12;}
  /// Return the channel within ManuId(), part of the uniqueID
  Int_t    GetManuChannel() const {return (GetUniqueID() & 0x3F000000) >> 24;}
  /// Return the cathode number, part of the uniqueID
  Int_t    GetCathode() const     {return (GetUniqueID() & 0x40000000) >> 30;}
  
  /// Set the pad as being calibrated or not
  void     SetCalibrated(Bool_t calibrated = kTRUE) {SetBit(BIT(14),calibrated);}
  /// return kTRUE if the pad is calibrated
  Bool_t   IsCalibrated() const {return TestBit(BIT(14));}
  /// Set the pad as being saturated or not
  void     SetSaturated(Bool_t saturated = kTRUE) {SetBit(BIT(15),saturated);}
  /// return kTRUE if the pad is saturated
  Bool_t   IsSaturated() const {return TestBit(BIT(15));}
  
  void     Print(Option_t */*option*/ = "") const;
  
  
protected:
  Int_t      fADC;    ///< ADC value
  Double32_t fCharge; ///< Calibrated charge
  
  
  ClassDef(IlcESDMuonPad, 1) // MUON ESD pad class
};

#endif

