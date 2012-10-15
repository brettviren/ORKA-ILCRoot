#ifndef ILCJETESDFILLUNITARRAYEMCALDIGITS_H
#define ILCJETESDFILLUNITARRAYEMCALDIGITS_H
 
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
 
//---------------------------------------------------------------------
// Jet Fill Unit Array 
// Called by ESD Reader for jet analysis
// Author: Magali Estienne (magali.estienne@subatech.in2p3.fr)
//---------------------------------------------------------------------

#include "IlcJetFillUnitArray.h"

class IlcESDCaloCluster;
class IlcJetReader;
class IlcJetESDReader;
class IlcJetESDReaderHeader;
//class IlcEMCALCalibData ;

class IlcJetESDFillUnitArrayEMCalDigits : public IlcJetFillUnitArray
{
 public: 
  IlcJetESDFillUnitArrayEMCalDigits();
  IlcJetESDFillUnitArrayEMCalDigits(IlcESDEvent *fESD);
  virtual ~IlcJetESDFillUnitArrayEMCalDigits();
  
  // Setter
  void SetApplyElectronCorrection(Int_t flag = 1)     {fApplyElectronCorrection = flag;}
  void SetApplyFractionHadronicCorrection(Bool_t val) {fApplyFractionHadronicCorrection = val;}
  void SetFractionHadronicCorrection(Double_t val)    {fFractionHadronicCorrection = val;}
  void SetESD(IlcESDEvent* const esd)                 {fESD = esd;}
  void SetInitMult(Int_t mult)                        {fNDigitEmcal = mult;}
  void SetInitMultCut(Int_t multcut)                  {fNDigitEmcalCut = multcut;}

  // Getter
  Int_t         GetMult()    const {return fNDigitEmcal;}
  Int_t         GetMultCut() const {return fNDigitEmcalCut;}

  // Other
  void Exec(Option_t* const option);
  // For calibration 
  //  virtual Float_t Calibrate(Int_t amp, Int_t cellId) ;  // Tranforms Amp to energy

 protected:
  IlcESDEvent  *fESD;                 // ESD
  Int_t        fNIn;                  // Number of Array filled in UnitArray
  Int_t        fOpt;                  // Detector to be used for jet reconstruction
  Int_t        fCluster;              // Use all cells or cells in clusters for jet finding 
  Int_t        fDebug;                // Debug option
  Int_t        fNCEMCAL;              // Number of clusters in EMCAL
  Int_t        fNCPHOS;               // Number of clusters in PHOS
  Int_t        fNCCalo;               // Number of cluster in EMCAL + PHOS calorimeters

  Bool_t       fApplyElectronCorrection;          // Electron correction flag
  Bool_t       fApplyFractionHadronicCorrection;  // Fraction hadronic correction flag
  Bool_t       fFractionHadronicCorrection;       // Fraction hadronic correction 

  IlcESDCaloCluster *fClus;           //! 
  Int_t              fNDigitEmcal;    //!
  Int_t              fNDigitEmcalCut; //!
  // Calibration parameters... to be replaced by database
/*   IlcEMCALCalibData *fCalibData;      //! Calibration database if aval */
/*   Float_t            fADCchannelECA;  // width of one ADC channel for EC section (GeV) */
/*   Float_t            fADCpedestalECA; // pedestal of ADC for EC section (GeV) */


 private:
  IlcJetESDFillUnitArrayEMCalDigits(const IlcJetESDFillUnitArrayEMCalDigits &det);
  IlcJetESDFillUnitArrayEMCalDigits &operator=(const IlcJetESDFillUnitArrayEMCalDigits &det);

//  void    GetCalibrationParameters(void) ;
  
  ClassDef(IlcJetESDFillUnitArrayEMCalDigits,1) // Fill Unit Array with tpc and/or emcal information
};

#endif
