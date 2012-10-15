/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
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

/* $Id: IlcPVECRawFitterv0.cxx 53738 2011-12-20 16:49:27Z kharlov $ */

// This class extracts the signal parameters (energy, time, quality)
// from ALTRO samples. Energy is in ADC counts, time is in time bin units.
// A coarse algorithm takes the energy as the maximum
// sample, time is the first sample index, and the quality is the number
// of bunches in the signal.
// 
//     IlcPVECRawFitterv0 *fitterv0=new IlcPVECRawFitterv0();
//     fitterv0->SetChannelGeo(module,cellX,cellZ,caloFlag);
//     fitterv0->SetCalibData(fgCalibData) ;
//     fitterv0->Eval(sig,sigStart,sigLength);
//     Double_t amplitude = fitterv0.GetEnergy();
//     Double_t time      = fitterv0.GetTime();
//     Bool_t   isLowGain = fitterv0.GetCaloFlag()==0;

// Author: Yuri Kharlov

// --- ROOT system ---
#include "TArrayI.h"
#include "TMath.h"
#include "TObject.h"

// --- IlcRoot header files ---
#include "IlcPVECRawFitterv0.h"
#include "IlcPVECCalibData.h"
#include "IlcLog.h"

ClassImp(IlcPVECRawFitterv0)

//-----------------------------------------------------------------------------
IlcPVECRawFitterv0::IlcPVECRawFitterv0():
  TObject(),
  fModule(0),
  fCellX(0),
  fCellZ(0),
  fCaloFlag(0),
  fNBunches(0),
  fPedSubtract(kFALSE),
  fEnergy(-111),
  fTime(-111),
  fQuality(0.),
  fPedestalRMS(0.),
  fAmpOffset(0),
  fAmpThreshold(0),
  fOverflow(kFALSE),
  fCalibData(0)
{
  //Default constructor
}

//-----------------------------------------------------------------------------
IlcPVECRawFitterv0::~IlcPVECRawFitterv0()
{
  //Destructor
}

//-----------------------------------------------------------------------------
IlcPVECRawFitterv0::IlcPVECRawFitterv0(const IlcPVECRawFitterv0 &PVECFitter ):
  TObject(),
  fModule      (PVECFitter.fModule),
  fCellX       (PVECFitter.fCellX),
  fCellZ       (PVECFitter.fCellZ),
  fCaloFlag    (PVECFitter.fCaloFlag),
  fNBunches    (PVECFitter.fNBunches),
  fPedSubtract (PVECFitter.fPedSubtract),
  fEnergy      (PVECFitter.fEnergy),
  fTime        (PVECFitter.fTime),
  fQuality     (PVECFitter.fQuality),
  fPedestalRMS (PVECFitter.fPedestalRMS),
  fAmpOffset   (PVECFitter.fAmpOffset),
  fAmpThreshold(PVECFitter.fAmpThreshold),
  fOverflow    (PVECFitter.fOverflow),
  fCalibData   (PVECFitter.fCalibData)
{
  //Copy constructor
}

//-----------------------------------------------------------------------------
IlcPVECRawFitterv0& IlcPVECRawFitterv0::operator = (const IlcPVECRawFitterv0 &PVECFitter)
{
  //Assignment operator.

  if(this != &PVECFitter) {
    fModule       = PVECFitter.fModule;
    fCellX        = PVECFitter.fCellX;
    fCellZ        = PVECFitter.fCellZ;
    fCaloFlag     = PVECFitter.fCaloFlag;
    fNBunches     = PVECFitter.fNBunches;
    fPedSubtract  = PVECFitter.fPedSubtract;
    fEnergy       = PVECFitter.fEnergy;
    fTime         = PVECFitter.fTime;
    fQuality      = PVECFitter.fQuality;
    fPedestalRMS  = PVECFitter.fPedestalRMS;
    fAmpOffset    = PVECFitter.fAmpOffset;
    fAmpThreshold = PVECFitter.fAmpThreshold;
    fOverflow     = PVECFitter.fOverflow;
    fCalibData    = PVECFitter.fCalibData;
  }

  return *this;
}

//-----------------------------------------------------------------------------

void IlcPVECRawFitterv0::SetChannelGeo(const Int_t module, const Int_t cellX,
				     const Int_t cellZ,  const Int_t caloFlag)
{
  // Set geometry address of the channel
  // (for a case if fitting parameters are different for different channels)
  
  fModule   = module;
  fCellX    = cellX;
  fCellZ    = cellZ;
  fCaloFlag = caloFlag;
}
//-----------------------------------------------------------------------------

Bool_t IlcPVECRawFitterv0::Eval(const UShort_t *signal, Int_t sigStart, Int_t sigLength)
{
  // Calculate signal parameters (energy, time, quality) from array of samples
  // Energy is a maximum sample minus pedestal 9
  // Time is the first time bin
  // Signal overflows is there are at least 3 samples of the same amplitude above 900

  fOverflow= kFALSE ;
  fEnergy  = 0;
  if (fNBunches > 1) {
    fQuality = 1000;
    return kTRUE;
  }
  
  const Float_t kBaseLine   = 1.0;
  const Int_t   kPreSamples = 10;

  Float_t  pedMean   = 0;
  Float_t  pedRMS    = 0;
  Int_t    nPed      = 0;
  UShort_t maxSample = 0;
  Int_t    nMax      = 0;

  for (Int_t i=0; i<sigLength; i++) {
    if (i>sigLength-kPreSamples) { //inverse signal time order
      nPed++;
      pedMean += signal[i];
      pedRMS  += signal[i]*signal[i] ;
    }
    if(signal[i] >  maxSample){ maxSample = signal[i]; nMax=0;}
    if(signal[i] == maxSample) nMax++;

  }
  
  
  fEnergy = (Double_t)maxSample;
  if (maxSample > 900 && nMax > 2) fOverflow = kTRUE;

  Double_t pedestal = 0 ;
  if (fPedSubtract) {
    if (nPed > 0) {
      fPedestalRMS=(pedRMS - pedMean*pedMean/nPed)/nPed ;
      if(fPedestalRMS > 0.) 
	fPedestalRMS = TMath::Sqrt(fPedestalRMS) ;
      pedestal = (Double_t)(pedMean/nPed);
    }
    else
      return kFALSE;
  }
  else {
    //take pedestals from DB
    pedestal = (Double_t) fAmpOffset ;
    if (fCalibData) {
      Float_t truePed       = fCalibData->GetADCpedestalEmc(fModule, fCellZ, fCellX) ;
      Int_t   altroSettings = fCalibData->GetAltroOffsetEmc(fModule, fCellZ, fCellX) ;
      pedestal += truePed - altroSettings ;
    }
    else{
      IlcDebug(2,Form("Pedestal and offset are not read from OCDB. Use 0 for their values.")) ;
    }
  }
  fEnergy-=pedestal ;
  if (fEnergy < kBaseLine) fEnergy = 0;

  //Evaluate time
  fTime = sigStart-sigLength-3; 
  const Int_t nLine= 6 ;        //Parameters of fitting
  const Float_t eMinTOF = 10. ; //Choosed from beam-test and cosmic analyis
  const Float_t kAmp=0.35 ;     //Result slightly depends on them, so no getters
  // Avoid too low peak:
  if(fEnergy < eMinTOF){
     return kTRUE;
  }

  // Find index posK (kLevel is a level of "timestamp" point Tk):
  Int_t posK =sigLength-1 ; //last point before crossing k-level
  Double_t levelK = pedestal + kAmp*fEnergy;
  while(signal[posK] <= levelK && posK>=0){
     posK-- ;
  }
  posK++ ;

  if(posK == 0 || posK==sigLength-1){
    return kTRUE; 
  }

  // Find crosing point by solving linear equation (least squares method)
  Int_t np = 0;
  Int_t iup=posK-1 ;
  Int_t idn=posK ;
  Double_t sx = 0., sy = 0., sxx = 0., sxy = 0.;
  Double_t x,y ;

  while(np<nLine){
    //point above crossing point
    if(iup>=0){
      x = sigLength-iup-1;
      y = signal[iup];
      sx += x;
      sy += y;
      sxx += (x*x);
      sxy += (x*y);
      np++ ;
      iup-- ;
    }
    //Point below crossing point
    if(idn<sigLength){
      if(signal[idn]<pedestal){
        idn=sigLength-1 ; //do not scan further
	idn++ ;
        continue ;
      }
      x = sigLength-idn-1;
      y = signal[idn];
      sx += x;
      sy += y;
      sxx += (x*x);
      sxy += (x*y);
      np++;
      idn++ ;
    }
    if(idn>=sigLength && iup<0){
      break ; //can not fit futher
    }
  }

  Double_t det = np*sxx - sx*sx;
  if(det == 0){
    return kTRUE;
  }
  if(np == 0){
    return kFALSE;
  }
  Double_t c1 = (np*sxy - sx*sy)/det;  //slope
  Double_t c0 = (sy-c1*sx)/np; //offset
  if(c1 == 0){
    return kTRUE;
  }

  // Find where the line cross kLevel:
  fTime += (levelK - c0)/c1-5. ; //5: mean offset between k-Level and start times
  return kTRUE;

}
