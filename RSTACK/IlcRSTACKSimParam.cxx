/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.                *
 * All rights reserved.                                                   *
 * Author: ILCroot Off-line Project.                                      *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.           *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

// Base class for the RSTACK simulation parameters.

// --- IlcRoot header files ---
#include "IlcRSTACKSimParam.h"
#include "IlcLog.h"

ClassImp(IlcRSTACKSimParam)

IlcRSTACKSimParam  * IlcRSTACKSimParam::fgSimParam = 0 ;
//-----------------------------------------------------------------------------
IlcRSTACKSimParam::IlcRSTACKSimParam() : TNamed(),
fLightYieldMean(0.),fBirks(0.),fAttenuationLength(0.),
fSiPMPDE(0.),fSiPMPixels(0.),fCollectEff(0.),fSiPMNoise(0.),fElectronicGain(0.),fConversionFactor(0.),
fENF(0.),fDigitsThreshold(0.),fPrimaryThreshold(0.),fADCchannel(0.),fADCbits(0),
fTOFa(0.),fTOFb(0.),fCellNonLinearityA(0.),fCellNonLinearityB(1.),fCellNonLinearityC(1.),
fDigitizeE(0),fCellNonLinearityOn(0)
{
  //Default constructor.
  for(Int_t i=0; i<10; i++) fDStream[i] = 0 ;
}

//-----------------------------------------------------------------------------
IlcRSTACKSimParam::IlcRSTACKSimParam(Int_t) : TNamed(),
fLightYieldMean(0.),fBirks(0.),fAttenuationLength(0.),
fSiPMPDE(0.),fSiPMPixels(0.),fCollectEff(0.),fSiPMNoise(0.),fElectronicGain(0.),fConversionFactor(0.),
fENF(0.),fDigitsThreshold(0.),fPrimaryThreshold(0.),fADCchannel(0.),fADCbits(0),
fTOFa(0.),fTOFb(0.),fCellNonLinearityA(0.),fCellNonLinearityB(1.),fCellNonLinearityC(1.),
fDigitizeE(0),fCellNonLinearityOn(0)
{
  //Real (private) constructor 
  //Set default parameters

  //Parameters describing energy deposition and light collection by SiPM, used in IlcRSTACKv1
  //Photoelectron statistics:
  // The light yield is a poissonian distribution of the number of
  // photons created in the scintillator, calculated using following formula
  // NumberOfPhotons = EnergyLost * LightYieldMean
  // NumberOfPhotoElectrons = NumberOfPhotons * IntrinsicSiPMEfficiency 
  // The number of electrons created in the SiPM is
  // NumberOfElectrons = SiPMGain * NumberOfPhotoElectrons
  fBirks = 9.0;                // Birk's constant for organic scintillator
  fLightYieldMean = 1.131e7;   //Average number of photons per GeV for BC408
  fAttenuationLength  = 200. ; //light attenuation length in BC408.



  //Parameters defining electronic noise calculation and Digits noise thresholds
  //used in IlcRSTACKDigitizer
  fSiPMPDE           = 0.2 ;    // SiPM Photon Detection Efficiency
  fSiPMPixels        = 6400.;   // Number of pixels in 1 SiPM
  fCollectEff        = 0.9 ;    // geometric collection efficiency
  fSiPMNoise         = 0.1 ;    // [photo-electron] SiPM dark current noise (shot noise)
  fElectronicGain    = 1. ;     // [photo-electron] SiPM Gain
  fENF               = 1.016 ;  // Excess Noise Factor
  fDigitsThreshold   = 1. ;     // [ADC counts] Threshold
  fPrimaryThreshold  = 10. ;    // [photon] minum number of photons to assign primary particle index to sdigit
  fADCchannel        = 5.e-5 ;  // [GeV/count] : 10bit ADC -> Max Val = 51.2 MeV
  fADCbits           = 10 ;     // ADC bits
  fConversionFactor  =  fLightYieldMean*fCollectEff*fSiPMPDE*fADCchannel; //[p.e./ADC count] factor to convert number of p.e. in ADC counts
  fTOFa              = 0.5e-9 ; // [s] constant term
  fTOFb              = 1.e-9 ;  // [s/sqrt(GeV)]] stochastic term
  fCellNonLinearityA = 0.18 ;   // Amp of non-linearity of cell responce
  fCellNonLinearityB = 0.109 ;  // Scale of non-linearity of cell responce
  fCellNonLinearityC = 0.976 ;  // Overall calibration

  //Imput streams for merging. If true => this stream contains digits (and thus noise) and not SDigits.
  for(Int_t i=0; i<10; i++){
    fDStream[i] = 0 ;
  }
  fgSimParam = this ;
}

//-----------------------------------------------------------------------------
IlcRSTACKSimParam::IlcRSTACKSimParam(const IlcRSTACKSimParam& ) : TNamed(),
fLightYieldMean(0.),fBirks(0.),fAttenuationLength(0.),
fSiPMPDE(0.),fSiPMPixels(0.),fCollectEff(0.),fSiPMNoise(0.),fElectronicGain(0.),fConversionFactor(0.),
fENF(0.),fDigitsThreshold(0.),fPrimaryThreshold(0.),fADCchannel(0.),fADCbits(0),
fTOFa(0.),fTOFb(0.),fCellNonLinearityA(0.),fCellNonLinearityB(1.),fCellNonLinearityC(1.),
fDigitizeE(0),fCellNonLinearityOn(0)
{
  //Copy constructor.
  IlcError("Should not use copy constructor for singleton") ;
  for(Int_t  i=0; i<10; i++){
    fDStream[i] = 0 ;
  }
  fgSimParam = this ;
}
//-----------------------------------------------------------------------------                                                            
IlcRSTACKSimParam * IlcRSTACKSimParam::GetInstance(){

  if(!fgSimParam)
    new IlcRSTACKSimParam(0) ;
  return fgSimParam ;
}
//-----------------------------------------------------------------------------
IlcRSTACKSimParam& IlcRSTACKSimParam::operator = (const IlcRSTACKSimParam& simParam)
{
  //Assignment operator.

  if(this != &simParam) {
    IlcError("Should not use operator= for singleton\n") ;
  }

  return *this;
}

