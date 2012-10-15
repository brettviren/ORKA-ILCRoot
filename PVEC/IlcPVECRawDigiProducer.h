#ifndef ILCPVECRAWDIGIPRODUCER_H
#define ILCPVECRAWDIGIPRODUCER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: IlcPVECRawDigiProducer.h 43671 2010-09-19 18:45:08Z kharlov $ */

//This class produces PVEC digits of one event
//using IlcPVECRawFitter. See cxx source for use case.

class IlcPVECCalibData ;
class IlcPVECDigit ;
class IlcPVECGeometry ;
class IlcPVECPulseGenerator;
class IlcRawReader;
class IlcCaloRawStreamV3;
class IlcPVECRawFitterv0;

#include "IlcAltroMapping.h"
#include "TObject.h"

class IlcPVECRawDigiProducer: public TObject {

public:

  IlcPVECRawDigiProducer() ;
  IlcPVECRawDigiProducer(IlcRawReader *rawReader, IlcAltroMapping **mapping = NULL);
  IlcPVECRawDigiProducer(const IlcPVECRawDigiProducer &dp);
  IlcPVECRawDigiProducer& operator= (const IlcPVECRawDigiProducer &dp);
 
  virtual ~IlcPVECRawDigiProducer(); 

  void MakeDigits(TClonesArray *digits, IlcPVECRawFitterv0* fitter);
  void MakeDigits(TClonesArray *digits, TClonesArray *tmpDigLG, IlcPVECRawFitterv0* fitter);

  void SetEmcMinAmp(Float_t emcMin) { fEmcMinE=emcMin; }
  void SetCpvMinAmp(Float_t cpvMin) { fCpvMinE=cpvMin; }
  void SetSampleQualityCut(Float_t qcut) { fSampleQualityCut=qcut; }

protected:

  void GetCalibrationParameters() ; //Extract calibration parameters from DB
  void CleanDigits(TClonesArray* digits) ; //remove digits below threshold and bad ones
  
  Bool_t IsInEMC(IlcPVECDigit* digit) const ; //tests if digit belongs to EMC
  Bool_t IsInCPV(IlcPVECDigit* digit) const ;

  Double_t CalibrateE(Double_t amp, Int_t* relId, Bool_t isLowGain) ; //calibrate energy 
  Double_t CalibrateT(Double_t amp, Int_t* relId, Bool_t isLowGain) ; //calibrate time

private:
  Float_t fEmcMinE ;                 // minimum energy of digit (ADC)
  Float_t fCpvMinE ;                 // minimum energy of digit (ADC)
  Float_t fSampleQualityCut;         // Cut on sample shapes: 0: no samples; 1: default parameterization; 999: accept even obviously bad
  Float_t fSampleToSec ;             // Conversion coeff from sample time step to seconds
  Int_t fEmcCrystals ;               //  number of EMC crystals
  IlcPVECGeometry * fGeom ;          //! PVEC geometry
  static IlcPVECCalibData * fgCalibData ;     //! Calibration database if avalable
  IlcPVECPulseGenerator   * fPulseGenerator ; //! Class with pulse shape parameters
  IlcRawReader            * fRawReader;       //! Raw data reader
  IlcCaloRawStreamV3      * fRawStream;       //! Calorimeter decoder of ALTRO format
  Int_t *fADCValuesLG;               //! Array og low-gain ALTRO samples
  Int_t *fADCValuesHG;               //! Array og high-gain ALTRO samples

  ClassDef(IlcPVECRawDigiProducer,7)
};

#endif
