#ifndef ILCPVBARSIMPARAM_H
#define ILCPVBARSIMPARAM_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                          */

/* $Id: IlcPVBARSimParam.h 23530 2008-01-25 06:46:13Z prsnko $ */
                                              
// Base class for the PVBAR simulation parameters.
// Do not use in the reconstruction; use derivative classes instead.

#include "TNamed.h"
#include "TMath.h"

class IlcPVBARSimParam : public TNamed {

public:

  IlcPVBARSimParam();
  IlcPVBARSimParam(const IlcPVBARSimParam& recoParam);
  IlcPVBARSimParam& operator = (const IlcPVBARSimParam& recoParam);
  virtual ~IlcPVBARSimParam() {}

  static IlcPVBARSimParam * GetInstance() ;

  //Parameters used in conversion of deposited energy to photons [see IlcPVBARv1 for details]
  void SetBirks(Float_t Birks=9.0){fBirks=Birks;}
  Float_t GetBirks(void)  const {return fBirks;}
  
  void SetAttenuationLength(Float_t AttenuationLength=210.){fAttenuationLength=AttenuationLength;}
  Float_t GetAttenuationLength(void)  const {return fAttenuationLength;}
  
  void SetLightYieldMean(Float_t ly=619000.){fLightYieldMean=ly;}
  Float_t GetLightYieldMean(void)  const {return fLightYieldMean;}

  //Parameters used to convert photons in electronic signal [see IlcPVBARDigitizer for details]
  void SetSiPMPDE(Float_t PDE=0.02){fSiPMPDE=PDE;}  // SiPM Photon Detection Efficiency
  Float_t GetSiPMPDE(void)  const {return fSiPMPDE;}

  void SetSiPMPixels(Float_t Npixels=6400.){fSiPMPixels=Npixels;} //Number of pixels in a SiPM
  Float_t GetSiPMPixels(void)  const {return fSiPMPixels;}

  void SetCollectEff(Float_t CollectEff=0.9){fCollectEff = CollectEff;  }
  Float_t GetCollectEff() const { return fCollectEff;  } //geometric collection efficiency

  void SetSiPMNoise(Float_t noise=0.1){fSiPMNoise = noise;  }
  Float_t GetSiPMNoise() const { return fSiPMNoise;  }          //RMS of SiPM noise

  void SetElectronicGain(Float_t gain=1.){fElectronicGain = gain;  }
  Float_t GetElectronicGain() const { return fElectronicGain;  }    //Electronic Gain

  void SetConversionFactor(Float_t factor=7.e-6){fConversionFactor = factor;  }
  Float_t GetConversionFactor() const { return fConversionFactor;  }      //Convert p.e. in energy

  void SetENF(Float_t ENF=1.016){fENF = ENF;  }
  Float_t GetENF() const { return fENF;  }          //Excess Noise Factor

  void SetDigitsThreshold(Float_t thresh=0.01){fDigitsThreshold=thresh;} 
  Float_t GetDigitsThreshold() const { return fDigitsThreshold; }  //Minimal energy to keep digit

  void SetPrimaryThreshold(Float_t thresh=1.){fPrimaryThreshold=thresh;} 
  Float_t GetPrimaryThreshold() const { return fPrimaryThreshold; }  //Minimal number of photons to assign primary particle index to digit

  //Parameters for energy digitization [see IlcPVBARDigitizer for details]
  void SetADCchannelW(Float_t width=0.005){fADCchannel=width ;} //ADC channel width
  Float_t GetADCchannelW(void) const {return fADCchannel; }

  void SetADCbits(Int_t bits=10){fADCbits=bits;} //ADC bits
  Int_t GetADCbits(void) const {return fADCbits; }
  
  void SetEDigitizationOn(Bool_t on=kTRUE){fDigitizeE=on ;}   //Use digitization in simulation or left it
  Bool_t IsEDigitizationOn(void)const {return fDigitizeE ;}   //for Digits2Raw->Digits procedure



  //Parameters to apply non-lineary on cell level
  Bool_t IsCellNonlinearityOn() const {return fCellNonLinearityOn;}
  void SetCellNonLinearity(Bool_t on=kTRUE){fCellNonLinearityOn=on;} //default: on=kFALSE
  Double_t GetCellNonLineariryA(void) const {return fCellNonLinearityA; }
  Double_t GetCellNonLineariryB(void) const {return fCellNonLinearityB; }
  Double_t GetCellNonLineariryC(void) const {return fCellNonLinearityC; }
  void SetCellNonLineariryA(Double_t a=0.30) {fCellNonLinearityA = a; }
  void SetCellNonLineariryB(Double_t b=0.109){fCellNonLinearityB = b; }
  void SetCellNonLineariryC(Double_t c=0.955){fCellNonLinearityC = c; }



  //Parameters for EMC TOF resolution [see IlcPVBARDigitizer::TimeResolution()]
  Float_t GetTOFa()const{return fTOFa ;}  //constant term
  Float_t GetTOFb()const{return fTOFb ;}  //stohastic term
  void SetTOFparameters(Float_t a=0.5e-9, Float_t b=1.5e-9){fTOFa=a; fTOFb=b; }

  //Mark streams for mixing as streams contaning Digits (true) or SDigits (false)
  //Streamt numbering the same as in StreamManager
  void    SetStreamDigits(Int_t i){if(i<10)fDStream[i]=kTRUE;}
  Bool_t  IsStreamDigits(Int_t i){return fDStream[i]; }


private:

  IlcPVBARSimParam(Int_t i); //True constructor which should be called by GetInstance()

private:

  //Parameters used in conversion of deposited energy to photons (IlcPVBARv1)
  Float_t  fLightYieldMean ;        //Average number of photoelectrons per GeV
  Float_t  fBirks ;                //Birk's constant
  Float_t  fAttenuationLength ;

  
  //Parameters used in electronic noise calculation and thresholds (IlcPVBARDigitizer)
  Float_t fSiPMPDE;             //SiPM Photon Detection Efficiency
  Float_t fSiPMPixels;         //Number of pixels in a SiPM
  Float_t fCollectEff;         //geometric collection efficiency
  Float_t fSiPMNoise;          //RMS of SiPM noise (shot noise)
  Float_t fElectronicGain;     //Electronic Gain
  Float_t fConversionFactor;   //factor to convert number of p.e. in ADC counts
  Float_t fENF;                // Excess Noise Factor
  Float_t fDigitsThreshold ;   //minimal energy to keep digit 
  Float_t fPrimaryThreshold ;  //Minimal number of photons to assign primary particle index to sdigit
  Float_t fADCchannel ;        //width of ADC channel in GeV
  Int_t   fADCbits ;           //ADC bits
  Float_t fTOFa  ;             //constant term of TOF resolution 
  Float_t fTOFb  ;             //stohastic term of TOF resolution 
  Float_t fCellNonLinearityA ; //Amp of cel non-linearity
  Float_t fCellNonLinearityB ; //Energy scale of cel non-linearity
  Float_t fCellNonLinearityC ; //Overall calibration

 
  Bool_t fDStream[10] ;   //Mark mixing stream contains digits or SDigits
  Bool_t fDigitizeE ;     //Use energy digitization in simulation or left to Digits2Raw()
  Bool_t fCellNonLinearityOn ;  //Model scintillator non-linearity in IlcPVBARDigitizer
  
  static IlcPVBARSimParam * fgSimParam ; // pointer to the unique instance of the class

  ClassDef(IlcPVBARSimParam,3)
};

#endif
