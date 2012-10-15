#ifndef ILCPVBARRAWDIGIPRODUCER_H
#define ILCPVBARRAWDIGIPRODUCER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: IlcPVBARRawDigiProducer.h 43671 2010-09-19 18:45:08Z kharlov $ */

//This class produces PVBAR digits of one event
//using IlcPVBARRawFitter. See cxx source for use case.

class IlcPVBARCalibData ;
class IlcPVBARDigit ;
class IlcPVBARGeometry ;
class IlcPVBARPulseGenerator;
class IlcRawReader;
class IlcCaloRawStreamV3;
class IlcPVBARRawFitterv0;

#include "IlcAltroMapping.h"
#include "TObject.h"

class IlcPVBARRawDigiProducer: public TObject {

public:

  IlcPVBARRawDigiProducer() ;
  IlcPVBARRawDigiProducer(IlcRawReader *rawReader, IlcAltroMapping **mapping = NULL);
  IlcPVBARRawDigiProducer(const IlcPVBARRawDigiProducer &dp);
  IlcPVBARRawDigiProducer& operator= (const IlcPVBARRawDigiProducer &dp);
 
  virtual ~IlcPVBARRawDigiProducer(); 

  void MakeDigits(TClonesArray *digits, IlcPVBARRawFitterv0* fitter);
  void MakeDigits(TClonesArray *digits, TClonesArray *tmpDigLG, IlcPVBARRawFitterv0* fitter);

  void SetEmcMinAmp(Float_t emcMin) { fEmcMinE=emcMin; }
  void SetCpvMinAmp(Float_t cpvMin) { fCpvMinE=cpvMin; }
  void SetSampleQualityCut(Float_t qcut) { fSampleQualityCut=qcut; }

protected:

  void GetCalibrationParameters() ; //Extract calibration parameters from DB
  void CleanDigits(TClonesArray* digits) ; //remove digits below threshold and bad ones
  
  Bool_t IsInEMC(IlcPVBARDigit* digit) const ; //tests if digit belongs to EMC
  Bool_t IsInCPV(IlcPVBARDigit* digit) const ;

  Double_t CalibrateE(Double_t amp, Int_t* relId, Bool_t isLowGain) ; //calibrate energy 
  Double_t CalibrateT(Double_t amp, Int_t* relId, Bool_t isLowGain) ; //calibrate time

private:
  Float_t fEmcMinE ;                 // minimum energy of digit (ADC)
  Float_t fCpvMinE ;                 // minimum energy of digit (ADC)
  Float_t fSampleQualityCut;         // Cut on sample shapes: 0: no samples; 1: default parameterization; 999: accept even obviously bad
  Float_t fSampleToSec ;             // Conversion coeff from sample time step to seconds
  Int_t fEmcCrystals ;               //  number of EMC crystals
  IlcPVBARGeometry * fGeom ;          //! PVBAR geometry
  static IlcPVBARCalibData * fgCalibData ;     //! Calibration database if avalable
  IlcPVBARPulseGenerator   * fPulseGenerator ; //! Class with pulse shape parameters
  IlcRawReader            * fRawReader;       //! Raw data reader
  IlcCaloRawStreamV3      * fRawStream;       //! Calorimeter decoder of ALTRO format
  Int_t *fADCValuesLG;               //! Array og low-gain ALTRO samples
  Int_t *fADCValuesHG;               //! Array og high-gain ALTRO samples

  ClassDef(IlcPVBARRawDigiProducer,7)
};

#endif
