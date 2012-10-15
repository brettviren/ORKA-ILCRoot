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

/* $Id: IlcPVECRawDigiProducer.cxx 53421 2011-12-05 15:19:38Z prsnko $ */

//This class produces PVEC digits of one event
//using IlcPVECRawFitter. 
//
//   For example:
//   TClonesArray *digits = new TClonesArray("IlcPVECDigit",100);
//   IlcRawReader* rawReader = new IlcRawReaderDate("2006run2211.raw");
//   IlcPVECRawDecoder dc(rawReader);
//   while (rawReader->NextEvent()) {
//     IlcPVECRawDigiProducer producer;
//     producer.MakeDigits(digits,&dc);
//   }

// Author: Boris Polichtchouk

// --- ROOT system ---
#include "TClonesArray.h"

// --- IlcRoot header files ---
#include "IlcPVECRawDigiProducer.h"
#include "IlcPVECRawFitterv0.h"
#include "IlcPVECGeometry.h"
#include "IlcPVECDigit.h"
#include "IlcPVECCalibData.h"
#include "IlcPVECPulseGenerator.h"
#include "IlcCaloRawStreamV3.h"
#include "IlcLog.h"

ClassImp(IlcPVECRawDigiProducer)

IlcPVECCalibData * IlcPVECRawDigiProducer::fgCalibData  = 0 ; 

//--------------------------------------------------------------------------------------
IlcPVECRawDigiProducer::IlcPVECRawDigiProducer():
  TObject(),
  fEmcMinE(0.),
  fCpvMinE(0.),
  fSampleQualityCut(1.),
  fSampleToSec(0.),
  fEmcCrystals(0),
  fGeom(0),
  fPulseGenerator(0),
  fRawReader(0),
  fRawStream(0),
  fADCValuesLG(0),
  fADCValuesHG(0)
{
  // Default constructor

  fGeom=IlcPVECGeometry::GetInstance() ;
  if(!fGeom) fGeom = IlcPVECGeometry::GetInstance("ORKA");

  fEmcCrystals=fGeom->GetNCristalsInModule()*fGeom->GetNModules() ;
  fPulseGenerator = new IlcPVECPulseGenerator();
  GetCalibrationParameters() ; 

}
//--------------------------------------------------------------------------------------
IlcPVECRawDigiProducer::IlcPVECRawDigiProducer(IlcRawReader *rawReader,
					       IlcAltroMapping **mapping):
  TObject(),
  fEmcMinE(0.),
  fCpvMinE(0.),
  fSampleQualityCut(1.),
  fSampleToSec(0.),
  fEmcCrystals(0),
  fGeom(0),
  fPulseGenerator(0),
  fRawReader(rawReader),
  fRawStream(0),
  fADCValuesLG(0),
  fADCValuesHG(0)
{
  // Default constructor

  fGeom=IlcPVECGeometry::GetInstance() ;
  if(!fGeom) fGeom = IlcPVECGeometry::GetInstance("ORKA");

  fEmcCrystals=fGeom->GetNCristalsInModule()*fGeom->GetNModules() ;
  fPulseGenerator = new IlcPVECPulseGenerator();
  GetCalibrationParameters() ; 

  fRawStream = new IlcCaloRawStreamV3(rawReader,"PVEC",mapping);

}
//--------------------------------------------------------------------------------------
IlcPVECRawDigiProducer::IlcPVECRawDigiProducer(const IlcPVECRawDigiProducer &dp):
  TObject(),
  fEmcMinE(0.),
  fCpvMinE(0.),
  fSampleQualityCut(1.),
  fSampleToSec(0.),
  fEmcCrystals(0),
  fGeom(0),
  fPulseGenerator(0),
  fRawReader(0),
  fRawStream(0),
  fADCValuesLG(0),
  fADCValuesHG(0)

{                                                          
  // Copy constructor

  fEmcMinE = dp.fEmcMinE ;
  fCpvMinE = dp.fCpvMinE ;
  fSampleQualityCut = dp.fSampleQualityCut;
  fSampleToSec = dp.fSampleToSec ;
  fEmcCrystals = dp.fEmcCrystals ;
  fPulseGenerator = new IlcPVECPulseGenerator();
  fGeom = dp.fGeom ;
}
//--------------------------------------------------------------------------------------
IlcPVECRawDigiProducer& IlcPVECRawDigiProducer::operator= (const IlcPVECRawDigiProducer &dp)
{
  // Assign operator

  if(&dp == this) return *this;

  fEmcMinE = dp.fEmcMinE ;
  fCpvMinE = dp.fCpvMinE ;
  fSampleQualityCut = dp.fSampleQualityCut ;
  fSampleToSec = dp.fSampleToSec ;
  fEmcCrystals = dp.fEmcCrystals ;
  fGeom = dp.fGeom ;
  if(fPulseGenerator) delete fPulseGenerator ;
  fPulseGenerator = new IlcPVECPulseGenerator();
  return  *this;
} 
//--------------------------------------------------------------------------------------
IlcPVECRawDigiProducer::~IlcPVECRawDigiProducer()
{
  // Destructor
  if(fPulseGenerator) delete fPulseGenerator ;
  fPulseGenerator=0 ;
  delete fRawStream;
  delete [] fADCValuesLG;
  delete [] fADCValuesHG;
}
//--------------------------------------------------------------------------------------
void IlcPVECRawDigiProducer::MakeDigits(TClonesArray *digits, IlcPVECRawFitterv0* fitter) 
{
  // Create a temporary array of LG digits and then make digits from raw data

  TClonesArray *tmpLG = new TClonesArray("IlcPVECDigit",10000) ;
  MakeDigits(digits, tmpLG, fitter);
  tmpLG->Delete();
  delete tmpLG;
}
//--------------------------------------------------------------------------------------
void IlcPVECRawDigiProducer::MakeDigits(TClonesArray *digits, TClonesArray *tmpDigLG, IlcPVECRawFitterv0* fitter) 
{
  //Makes the job.
  //TClonesArray *digits, *tmpDigLG and raw data fitter should be provided by calling function.

  digits->Clear();
 
  Int_t iDigit=0 ;
  Int_t relId[4], absId=-1, caloFlag=-1;
  
  const Double_t baseLine=1. ; //Minimal energy of digit in ADC ch. 

  //Calculate conversion coeff. from Sample time step to seconds
  //If OCDB contains negative or zero value - use one from RCU trailer
  //Negative value in OCDB is used only for simulation of raw digits
  if(fgCalibData->GetSampleTimeStep()>0.)
    fSampleToSec=fgCalibData->GetSampleTimeStep() ;
  else
    fSampleToSec=fRawStream->GetTSample() ;
  
  // Clear a temporary array for LowGain digits
  tmpDigLG->Clear();
  Int_t ilgDigit=0 ;

  //Let fitter subtract pedestals in case of ZS
  fitter->SetCalibData(fgCalibData) ;
  
  while (fRawStream->NextDDL()) {
    while (fRawStream->NextChannel()) {
      relId[0] = 5 - fRawStream->GetModule() ; // counts from 1 to 5
      relId[1] = 0;
      relId[2] = fRawStream->GetCellX()  + 1; // counts from 1 to 64
      relId[3] = fRawStream->GetCellZ()  + 1; // counts from 1 to 56
      caloFlag = fRawStream->GetCaloFlag();   // 0=LG, 1=HG, 2=TRU
      
      if(caloFlag!=0 && caloFlag!=1) continue; //TRU data!
      
      fitter->SetChannelGeo(relId[0],relId[2],relId[3],caloFlag);

      if(fitter->GetAmpOffset()==0 && fitter->GetAmpThreshold()==0) {
	short value = fRawStream->GetAltroCFG1();
	bool ZeroSuppressionEnabled = (value >> 15) & 0x1;
	if(ZeroSuppressionEnabled) {
	  short offset = (value >> 10) & 0xf;
	  short threshold = value & 0x3ff;
	  fitter->SubtractPedestals(kFALSE);
	  fitter->SetAmpOffset(offset);
	  fitter->SetAmpThreshold(threshold);
	}
      }
      
      fGeom->RelToAbsNumbering(relId, absId);
      
      fitter->SetNBunches(0);
      Int_t sigStart =0 ;
      Int_t sigLength=0 ;
      while (fRawStream->NextBunch()) { //Take the first in time bunch
	const UShort_t *sig = fRawStream->GetSignals();
	sigStart  = fRawStream->GetStartTimeBin();
	sigLength = fRawStream->GetBunchLength();
	fitter->Eval(sig,sigStart,sigLength);
	if      (caloFlag == IlcCaloRawStreamV3::kLowGain) {
	  delete [] fADCValuesLG;
	  fADCValuesLG = new Int_t[sigLength];
	  for (Int_t i=0; i<sigLength; i++)
	    fADCValuesLG[sigLength-i-1] = sig[i];
	}
	else if (caloFlag == IlcCaloRawStreamV3::kHighGain) {
	  delete [] fADCValuesHG;
	  fADCValuesHG = new Int_t[sigLength];
	  for (Int_t i=0; i<sigLength; i++)
	    fADCValuesHG[sigLength-i-1] = sig[i];
	}
      } // End of NextBunch()

      
      Double_t energy = fitter->GetEnergy() ; 
      Double_t time   = fitter->GetTime() ;
      if(energy<=baseLine) //in ADC channels
	continue ;

      //remove digits with bad shape. Fitter should calculate quality so that 
      //in default case quality [0,1], while larger values of quality mean somehow 
      //corrupted samples, 999 means obviously corrupted sample.
      //It is difficult to fit samples with overflow (even setting cut on overflow values)
      //because too few points are left to fit. So we do not evaluate samples with overflow

      if(fitter->GetSignalQuality() > fSampleQualityCut && !(fitter->IsOverflow()))
	continue ;
      
      energy = CalibrateE(energy,relId,!caloFlag) ;

      //convert time from sample bin units to s
      time*=fSampleToSec ;
//CalibrateT moved to Clusterizer
//      time = CalibrateT(time,relId,!caloFlag) ;
      // subtract RCU L1 phase (L1Phase is in seconds) w.r.t. L0:
      //Very strange behaviour of electronics, but cross-checkes several times...
      if( fRawStream->GetL1Phase()<55.*1.e-9 ) //for phase=0,25,50
        time -= fRawStream->GetL1Phase();
      else //for phase 75
        time += 25.*1.e-9 ;
      
      if(energy <= 0.) 
	continue;
      
      if (caloFlag == IlcCaloRawStreamV3::kLowGain) {
	new((*tmpDigLG)[ilgDigit]) IlcPVECDigit(-1,absId,(Float_t)energy,(Float_t)time);
	if (sigLength>0 && fADCValuesLG!=0)
	  static_cast<IlcPVECDigit*>(tmpDigLG->At(ilgDigit))->SetALTROSamplesLG(sigLength,fADCValuesLG);
	ilgDigit++ ; 
      }
      else if (caloFlag == IlcCaloRawStreamV3::kHighGain) {
	if(fitter->IsOverflow()) //Keep this digit to replace it by Low Gain later.
	  //If there is no LogGain it wil be removed by cut on Min E
	  new((*digits)[iDigit]) IlcPVECDigit(-1,absId,-1.f,(Float_t)time);
	else
	  new((*digits)[iDigit]) IlcPVECDigit(-1,absId,(Float_t)energy,(Float_t)time);
	if (sigLength>0 && fADCValuesHG!=0)
	  static_cast<IlcPVECDigit*>(digits->At(iDigit))->SetALTROSamplesHG(sigLength,fADCValuesHG);
	iDigit++;
      }
    } // End of NextChannel()

    //Now scan created LG and HG digits and keep only those which appeared in both lists 
    //replace energy of HighGain digits only if there is overflow
    //negative energy (overflow)
    digits->Sort() ;
    tmpDigLG->Sort() ;
    Int_t iLG = 0;
    Int_t nLG1 = tmpDigLG->GetEntriesFast()-1 ;
    
    for(Int_t iDig=0 ; iDig < digits->GetEntriesFast() ; iDig++) { 
      IlcPVECDigit * digHG = dynamic_cast<IlcPVECDigit*>(digits->At(iDig)) ;
      if (!digHG) continue;
      IlcPVECDigit * digLG = dynamic_cast<IlcPVECDigit*>(tmpDigLG->At(iLG)) ;
      while(digLG && iLG<nLG1 && digHG->GetId()> digLG->GetId()){
	iLG++ ;
	digLG = dynamic_cast<IlcPVECDigit*>(tmpDigLG->At(iLG)) ;
      }
      absId=digHG->GetId() ;
      fGeom->AbsToRelNumbering(absId,relId) ;
 
      if(digLG && digHG->GetId() == digLG->GetId()){ //we found pair
	if(digHG->GetEnergy()<0.){ //This is overflow in HG
	  digHG->SetTime(digLG->GetTime()) ;
	  digHG->SetEnergy(digLG->GetEnergy()) ;
	}
      }
      else{ //no pair - remove
	if(digHG->GetEnergy()<0.) //no pair, in saturation
	  digits->RemoveAt(iDig) ;                                                          
      }
    }
  } // End of NextDDL()

  CleanDigits(digits) ;
  
}
//____________________________________________________________________________
Double_t IlcPVECRawDigiProducer::CalibrateE(Double_t amp, Int_t* relId, Bool_t isLowGain)
{
  // Convert EMC LG amplitude to HG (multipli by ~16)
  // Calibration parameters are taken from calibration data base 
  if(fgCalibData){ 
    Int_t   module = relId[0];  
    Int_t   column = relId[3];
    Int_t   row    = relId[2];
    if(relId[1]==0) { // this is EMC 
      if(isLowGain){
        amp*= fgCalibData->GetHighLowRatioEmc(module,column,row);
      }
      return amp ;         
    }         
  }          
  return 0;        
}
//____________________________________________________________________________
Double_t IlcPVECRawDigiProducer::CalibrateT(Double_t time, Int_t * relId, Bool_t /* isLowGain */)
{
  //Calibrate time
  if(fgCalibData){
    Int_t   module = relId[0];
    Int_t   column = relId[3];
    Int_t   row    = relId[2];
    if(relId[1]==0) { // this is EMC
      time += fgCalibData->GetTimeShiftEmc(module,column,row);                   
      return time ;             
    }
  }
 
  return -999.;
}
//____________________________________________________________________________
void IlcPVECRawDigiProducer::CleanDigits(TClonesArray * digits)
{
  // remove digits with amplitudes below threshold.
  // remove digits in bad channels
  // sort digits with icreasing AbsId
  
  //remove digits in bad map and below threshold
  Bool_t isBadMap = 0 ;
  if(fgCalibData->GetNumOfEmcBadChannels()){
    isBadMap=1 ;
  }
  
  for(Int_t i=0; i<digits->GetEntriesFast(); i++){
    IlcPVECDigit * digit = static_cast<IlcPVECDigit*>(digits->At(i)) ;
    if(!digit)
      continue  ;
    if ( (IsInEMC(digit) && digit->GetEnergy() < fEmcMinE) ||
	 (IsInCPV(digit) && digit->GetEnergy() < fCpvMinE) ){
      digits->RemoveAt(i) ;
      continue ;
    }
    if(isBadMap){ //check bad map now
      Int_t relid[4] ;
      fGeom->AbsToRelNumbering(digit->GetId(), relid) ; 
      if(fgCalibData->IsBadChannelEmc(relid[0],relid[3],relid[2])){
	digits->RemoveAt(i) ;
      }
    }
  }

  //Compress, sort and set indexes
  digits->Compress() ;
//  digits->Sort(); already sorted earlier
  for (Int_t i = 0 ; i < digits->GetEntriesFast() ; i++) { 
    IlcPVECDigit *digit = static_cast<IlcPVECDigit*>( digits->At(i) ) ; 
    digit->SetIndexInList(i) ;     
  }
}
//____________________________________________________________________________
Bool_t IlcPVECRawDigiProducer::IsInEMC(IlcPVECDigit * digit) const
{
  // Tells if (true) or not (false) the digit is in a PVEC-EMC module
  return digit->GetId() <= fEmcCrystals ;

}

//____________________________________________________________________________
Bool_t IlcPVECRawDigiProducer::IsInCPV(IlcPVECDigit * digit) const
{
  // Tells if (true) or not (false) the digit is in a PVEC-CPV module
  return digit->GetId() > fEmcCrystals ;
}
//____________________________________________________________________________
void IlcPVECRawDigiProducer::GetCalibrationParameters() 
{
  // Set calibration parameters:
  // if calibration database exists, they are read from database,
  // otherwise, reconstruction stops in the constructor of IlcPVECCalibData
  //
  // It is a user responsilibity to open CDB before reconstruction, for example: 
  // IlcCDBStorage* storage = IlcCDBManager::Instance()->GetStorage("local://CalibDB");

  if (!fgCalibData){
    fgCalibData = new IlcPVECCalibData(-1); //use IlcCDBManager's run number
  }
  if (fgCalibData->GetCalibDataEmc() == 0)
    IlcFatal("Calibration parameters for PVEC EMC not found. Stop reconstruction.\n");
  if (fgCalibData->GetCalibDataCpv() == 0)
    IlcFatal("Calibration parameters for PVEC CPV not found. Stop reconstruction.\n");
}
